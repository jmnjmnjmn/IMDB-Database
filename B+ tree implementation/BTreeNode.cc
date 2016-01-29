#include "BTreeNode.h"
#include <iostream>
using namespace std;

/*
 * Read the content of the node from the page pid in the PageFile pf.
 * @param pid[IN] the PageId to read
 * @param pf[IN] PageFile to read from
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTLeafNode::read(PageId pid, const PageFile& pf)
{ 
       RC rc = pf.read(pid, buffer);
       return rc; 
}
    
/*
 * Write the content of the node to the page pid in the PageFile pf.
 * @param pid[IN] the PageId to write to
 * @param pf[IN] PageFile to write to
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTLeafNode::write(PageId pid, PageFile& pf)
{ 
	RC rc = pf.write(pid, buffer);
	return rc; }

/*
 * Return the number of keys stored in the node.
 * @return the number of keys in the node
 */
int BTLeafNode::getKeyCount()
{ 
    int index = 0;
    int index_max = PageFile::PAGE_SIZE - sizeof(PageId);
    int key;
    RecordId rid;
    int keycount = 0;

    while (index < index_max){     
	     popBufferEntry(index,buffer,rid,key);
       //if not null keycount++
	     if( rid.pid == EMNULL ){break;}
	     keycount++;
	     index += SIZE_ENTRY;	
    }
    return keycount;
}


//use index to get an entry from buffer 
//int index[IN]
//char buffer[PAGE_SIZE][IN]
//RecordId rid[OUT]
//int key[OUT]
void BTLeafNode::popBufferEntry(int index, char* buffer, RecordId& rid, int& key)
{
    int intsize = sizeof(int);
    int* ptr = (int*) buffer;

    rid.pid = *(ptr+(index/intsize));
    rid.sid = *(ptr+(index/intsize)+1);
    key = *(ptr+(index/intsize)+2);

}

//add an entry into buffer 
//char buffer[PAGE_SIZE][OUT]
//RecordId rid[IN]
//int key[IN]
void BTLeafNode::pushBufferEntry(  RecordId rid, int key, char* buffer){

    int* ptr = (int*) buffer;

    *(ptr+2) = key;
    *(ptr+1) = rid.sid;
    *(ptr) = rid.pid;

    buffer = (char*) ptr;
}

/*
 * Insert a (key, rid) pair to the node.
 * @param key[IN] the key to insert
 * @param rid[IN] the RecordId to insert
 * @return 0 if successful. Return an error code if the node is full.
 */
RC BTLeafNode::insert(int key, const RecordId& rid)
{  
    int eid;   
    int keycount = getKeyCount();
    if (keycount == MAX_ENTRY_NUM){return RC_NODE_FULL;    }
   
	//if not find,insert to end of node
	if(locate(key,eid) != 0){eid=keycount;};
	int index = keycount * SIZE_ENTRY - 1;
    
	//shift entry where key >= searchkey
	while (index >= eid * SIZE_ENTRY){
	    buffer[index+SIZE_ENTRY]=buffer[index];
	    index--;
	}
    
	//insert key 
	char tmpbuf[SIZE_ENTRY];
	pushBufferEntry(rid, key, tmpbuf);
	index =  eid * SIZE_ENTRY;
	while (index < (eid+1) * SIZE_ENTRY){
	    buffer[index]=tmpbuf[index-eid * SIZE_ENTRY];
	    index++;
	}

    return 0; 

}

/*
 * Insert the (key, rid) pair to the node
 * and split the node half and half with sibling.
 * The first key of the sibling node is returned in siblingKey.
 * @param key[IN] the key to insert.
 * @param rid[IN] the RecordId to insert.
 * @param sibling[IN] the sibling node to split with. This node MUST be EMPTY when this function is called.
 * @param siblingKey[OUT] the first key in the sibling node after split.
 * @return 0 if successful. Return an error code if there is an error.
 */

RC BTLeafNode::insertAndSplit(int key, const RecordId& rid, 
                              BTLeafNode& sibling, int& siblingKey)
{ 
    
   int left_key;
   RecordId left_rid; 
   int null_key=EMNULL;
   RecordId null_rid={EMNULL,EMNULL};

   int index_middle = (int) ((MAX_ENTRY_NUM / 2) );
   int index = index_middle * SIZE_ENTRY;
  
   if(getKeyCount() < MAX_ENTRY_NUM){
      return RC_INVALID_FILE_FORMAT;
    }

   while(index < (MAX_ENTRY_NUM * SIZE_ENTRY)){
   
    //copy left node to sibling node
    popBufferEntry(index,buffer,left_rid,left_key);
    sibling.insert(left_key,left_rid);
    if(index == index_middle * SIZE_ENTRY ) {
      siblingKey=left_key;  
    }
   
    //clear left node 
    char tmpbuf[SIZE_ENTRY];
    pushBufferEntry(null_rid, null_key, tmpbuf);
    int i =  index;
    while (i < index + SIZE_ENTRY){
	     buffer[i]=tmpbuf[i-index];
	     i++;
    }       
    index += SIZE_ENTRY;
   }   
  
    sibling.setNextNodePtr(getNextNodePtr());
    if (key < siblingKey){
      insert(key, rid);
    }else{
      sibling.insert(key, rid);
    }
      return 0;    
}

/*
 * Find the entry whose key value is larger than or equal to searchKey
 * and output the eid (entry number) whose key value >= searchKey.
 * Remeber that all keys inside a B+tree node should be kept sorted.
 * @param searchKey[IN] the key to search for
 * @param eid[OUT] the entry number that contains a key larger than or equalty to searchKey
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTLeafNode::locate(int searchKey, int& eid)
{   
    int key;
    RecordId rid;
    int keycount = getKeyCount();
    int index = 0;

    while (index < keycount){
      popBufferEntry(index*SIZE_ENTRY,buffer,rid,key);
	    if(key >= searchKey){
	      eid=index;
	      return 0;   
      }
	    index++ ;
    }   
    
    return RC_NO_SUCH_RECORD; 
}

/*
 * Read the (key, rid) pair from the eid entry.
 * @param eid[IN] the entry number to read the (key, rid) pair from
 * @param key[OUT] the key from the entry
 * @param rid[OUT] the RecordId from the entry
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTLeafNode::readEntry(int eid, int& key, RecordId& rid)
{ 
    if (eid < 0){return RC_INVALID_CURSOR;}
    if (eid >= getKeyCount()){return RC_INVALID_CURSOR;}
     
    int index = eid * SIZE_ENTRY;
    popBufferEntry(index, buffer, rid, key);
    return 0; 

}

/*
 * Return the pid of the next slibling node.
 * @return the PageId of the next sibling node 
 */
PageId BTLeafNode::getNextNodePtr()
{ 
    int* ptr = (int*) buffer;
    return *(ptr+MAX_ENTRY_NUM*3) ;
}

/*
 * Set the pid of the next slibling node.
 * @param pid[IN] the PageId of the next sibling node 
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTLeafNode::setNextNodePtr(PageId pid)
{ 
    char* ptr_char = buffer;
    int* ptr = (int*) ptr_char;
    *(ptr+MAX_ENTRY_NUM*3) = pid;
    ptr_char = (char*) ptr;
   
    return 0;
}

//initialBuffer set EMNULL
void BTLeafNode::initialBuffer(){

  RecordId null_rid = {EMNULL, EMNULL};
  int null_key = EMNULL;

  char tempbuf[SIZE_ENTRY];

  //fill with empty entrances
  int index = 0;
  while (index < MAX_ENTRY_NUM){

    pushBufferEntry(null_rid, null_key, tempbuf);
    int t = 0;
    while (t < SIZE_ENTRY){
  	  buffer[index*SIZE_ENTRY+t] = tempbuf[t];
    	t++;
    }
    
    index++;
  }

  setNextNodePtr(EMNULL);//next node pointer is null
}



/*
 * Read the content of the node from the page pid in the PageFile pf.
 * @param pid[IN] the PageId to read
 * @param pf[IN] PageFile to read from
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTNonLeafNode::read(PageId pid, const PageFile& pf)
{ 
    RC rc = pf.read(pid, buffer);
    return rc;
}
    
/*
 * Write the content of the node to the page pid in the PageFile pf.
 * @param pid[IN] the PageId to write to
 * @param pf[IN] PageFile to write to
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTNonLeafNode::write(PageId pid, PageFile& pf)
{
    RC rc = pf.write(pid, buffer);
    return rc;
}

/*
 * Return the number of keys stored in the node.
 * @return the number of keys in the node
 */
int BTNonLeafNode::getKeyCount()
{

    int index = 0;
    int index_max = PageFile::PAGE_SIZE - sizeof(PageId);;
    int keycount = 0;
    NonLeafEntry entry;
    while (index < index_max){     
	    popBufferEntry(index,buffer,entry);
	//if not null keycount++
	    if( entry.key == EMNULL ){break;}
	    keycount++;
	    index += SIZE_ENTRY;	
    }
    //test
    return keycount;
 }

//use index to get an entry from buffer 
//int index[IN]
//char buffer[PAGE_SIZE][IN]
//NonLeafEntry entry[OUT]

void BTNonLeafNode::popBufferEntry(int index, char* buffer, NonLeafEntry& entry)
{
    int intsize = sizeof(int);
    int* ptr = (int*) buffer;

    entry.key = *(ptr+(index/intsize)+1);
    entry.pid = *(ptr+(index/intsize));

}

void BTNonLeafNode::pushBufferEntry( PageId pid, int key, char* buffer){

    int* ptr = (int*) buffer;

    *(ptr) = key;
    *(ptr+1) = pid;

    buffer = (char*) ptr;
}

/*
 * Insert a (key, pid) pair to the node.
 * @param key[IN] the key to insert
 * @param pid[IN] the PageId to insert
 * @return 0 if successful. Return an error code if the node is full.
 */
RC BTNonLeafNode::insert(int key, PageId pid)
{   
    int eid;   
    int keycount = getKeyCount();
    if (keycount == MAX_ENTRY_NUM){return RC_NODE_FULL;}
    
    //if not find,insert to end of node
    if(locate(key,eid) != 0){eid=keycount;};
    int index = keycount * SIZE_ENTRY - 1 + sizeof(int);
    
    //shift entry where key >= searchkey
    while (index >= eid * SIZE_ENTRY + sizeof(int)){
	    buffer[index+SIZE_ENTRY]=buffer[index];
	    index--;
    }
    
    //insert key 
    char tmpbuf[SIZE_ENTRY];
    pushBufferEntry(pid, key, tmpbuf);
    index =  eid * SIZE_ENTRY + sizeof(int);
    while (index < (eid+1) * SIZE_ENTRY + sizeof(int)){
	    buffer[index]=tmpbuf[index - eid * SIZE_ENTRY - sizeof(int)];
	    index++;
    }

    return 0;    

}


RC BTNonLeafNode::locate(int searchKey, int& eid)
{   
    NonLeafEntry entry;
    int keycount=getKeyCount();
    int index = 0;

    while (index < keycount){
	    popBufferEntry(index*SIZE_ENTRY,buffer,entry);
	    if(entry.key >= searchKey){
	      eid=index;
	      return 0;   
	    }
	    index++ ;
    }     
    return RC_NO_SUCH_RECORD; 
}

/*
 * Insert the (key, pid) pair to the node
 * and split the node half and half with sibling.
 * The middle key after the split is returned in midKey.
 * @param key[IN] the key to insert
 * @param pid[IN] the PageId to insert
 * @param sibling[IN] the sibling node to split with. This node MUST be empty when this function is called.
 * @param midKey[OUT] the key in the middle after the split. This key should be inserted to the parent node.
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTNonLeafNode::insertAndSplit(int key, PageId pid, BTNonLeafNode& sibling, int& midKey)
{

  
   NonLeafEntry left_entry1,left_entry2,left_entry3;
   int null_key=EMNULL;
   PageId null_pid=EMNULL;

   int index_middle = (int) ((MAX_ENTRY_NUM / 2) );
   int index = index_middle * SIZE_ENTRY;
   //cout<<"nonleaf split key="<<key<<" pid="<<pid<<"mid="<<index_middle<<"getKeycount="<<getKeyCount()<<endl;  
   
   
   // popBufferEntry(0,buffer,left_entry);
    ////cout<<"index"<<index<<"left.key"<<left_entry.key<<" pid="<<left_entry.pid<<endl;

    //popBufferEntry(SIZE_ENTRY,buffer,left_entry);
    ////cout<<"index"<<index<<"left.key"<<left_entry.key<<" pid="<<left_entry.pid<<endl;

    //popBufferEntry(2*SIZE_ENTRY,buffer,left_entry);
    ////cout<<"index"<<index<<"left.key"<<left_entry.key<<" pid="<<left_entry.pid<<endl;

     //popBufferEntry(3*SIZE_ENTRY,buffer,left_entry);
    ////cout<<"index"<<index<<"left.key"<<left_entry.key<<" pid="<<left_entry.pid<<endl;

   if(getKeyCount() < MAX_ENTRY_NUM){return RC_INVALID_FILE_FORMAT;}

    //initialize sibling node
    //popBufferEntry(index,buffer,left_entry1);

    //popBufferEntry(index+SIZE_ENTRY,buffer,left_entry1);
    //popBufferEntry(index+2*SIZE_ENTRY,buffer,left_entry2);
    //sibling.initializeRoot(left_entry1.pid,left_entry1.key,left_entry2.pid);

   while(index < (MAX_ENTRY_NUM * SIZE_ENTRY)){
    
    popBufferEntry(index,buffer,left_entry1);
    popBufferEntry(index+SIZE_ENTRY,buffer,left_entry2);
    
    if(index == index_middle * SIZE_ENTRY ){
	    midKey=left_entry1.key;
	    sibling.initializeRoot(left_entry2.pid,null_key,null_pid);
    }else{
    //copy left node to sibling node
    //popBufferEntry(index,buffer,left_entry1);
	    sibling.insert(left_entry1.key,left_entry2.pid);
    }
    //clear left node 
	  char tmpbuf[SIZE_ENTRY];
	  pushBufferEntry(null_pid, null_key, tmpbuf);
	  int i =  index + sizeof(int);
	  while (i < index + SIZE_ENTRY + sizeof(int)){
	    buffer[i]=tmpbuf[i-index - sizeof(int)];
	    i++;
    }
      index += SIZE_ENTRY;
   }   
   
   if (key < midKey){
     insert(key, pid);
   }
   else{
      sibling.insert(key, pid);
   }



      return 0;   
}

/*
 * Given the searchKey, find the child-node pointer to follow and
 * output it in pid.
 * @param searchKey[IN] the searchKey that is being looked up.
 * @param pid[OUT] the pointer to the child node to follow.
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTNonLeafNode::locateChildPtr(int searchKey, PageId& pid)
{ 
    int key;
    NonLeafEntry entry;
    int keycount=getKeyCount();
    int index = 0;

    while (index < keycount){
	    popBufferEntry(index*SIZE_ENTRY,buffer,entry);
	    if(entry.key > searchKey){
	      pid=entry.pid;
	      return 0;   
	    }
	    index++ ;
    }    
    popBufferEntry(index*SIZE_ENTRY,buffer,entry);
    pid = entry.pid;  
    return 0; 
}

/*
 * Initialize the root node with (pid1, key, pid2).
 * @param pid1[IN] the first PageId to insert
 * @param key[IN] the key that should be inserted between the two PageIds
 * @param pid2[IN] the PageId to insert behind the key
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTNonLeafNode::initializeRoot(PageId pid1, int key, PageId pid2)
{
    char* ptr_char = buffer;
    int* ptr = (int*) ptr_char;
    int index = 0;
    while (index < MAX_ENTRY_NUM*2){
	    *(ptr+index) = EMNULL;
	    index++;
    }
    //ptr_char = (char*) ptr;
    //  NonLeafEntry entry1,entry2,entry3;
    //insert pid1, key , pid2
    *(ptr) = pid1;
    *(ptr+1) = key;
    *(ptr+2) = pid2;
  
   // popBufferEntry(0*SIZE_ENTRY,buffer,entry1);
   // popBufferEntry(1*SIZE_ENTRY,buffer,entry2);
   // popBufferEntry(2*SIZE_ENTRY,buffer,entry3);

    ////cout<<" entry1.pid"<<entry1.pid<<" entry1.key="<<entry1.key<<" entry2.pid"<<entry2.pid<<" entry2.key="<<entry2.key<<"entry3.pid="<<entry3.pid<<" entry3.key="<<entry3.key<<endl;

    return 0;    


}
