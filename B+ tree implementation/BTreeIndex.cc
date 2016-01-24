/*
 * Copyright (C) 2008 by The Regents of the University of California
 * Redistribution of this file is permitted under the terms of the GNU
 * Public License (GPL).
 *
 * @author Junghoo "John" Cho <cho AT cs.ucla.edu>
 * @date 3/24/2008
 */
 
#include "BTreeIndex.h"
#include "BTreeNode.h"
#include <iostream>
using namespace std;

/*
 * BTreeIndex constructor
 */
BTreeIndex::BTreeIndex()
{
    rootPid = INITIAL;
    treeHeight = LOCATION_ROOT;
}

/*
 * Open the index file in read or write mode.
 * Under 'w' mode, the index file should be created if it does not exist.
 * @param indexname[IN] the name of the index file
 * @param mode[IN] 'r' for read, 'w' for write
 * @return error code. 0 if no error
 */
RC BTreeIndex::open(const string& indexname, char mode)
{   
	char buffer[PageFile::PAGE_SIZE]; //buffer to read index into

	if(pf.open(indexname, mode) < 0) //failed to open
		return -1;
		
	if(pf.endPid() == 0){ //our first page or not?
		rootPid = -1;
		treeHeight = 0;	
		close();
		return open(indexname,mode);
	}
	else{
		if(pf.read(0,buffer) < 0)//failed to read
			return -1;
		treeHeight = *(int *)buffer;
    	rootPid = *(PageId *)(buffer + sizeof(int));

	}
	
	
	//success
	return 0;	

}

/*
 * Close the index file.
 * @return error code. 0 if no error
 */
RC BTreeIndex::close()
{
	char buffer[PageFile::PAGE_SIZE]; //buffer to save index into
	*(int *)buffer = treeHeight;
    *(PageId *)(buffer+sizeof(int)) = rootPid;
	if(pf.write(0,buffer) < 0) //failed to write
		return -1;
	return pf.close(); //return any errors while trying to close pagefile

}

/*
 * Insert (key, RecordId) pair to the index.
 * @param key[IN] the key for the value inserted into the index
 * @param rid[IN] the Retemp_readcordId for the record being inserted into the index
 * @return error code. 0 if no error
 */
RC BTreeIndex::insert(int key, const RecordId& rid)
{
   
    RC rc;
    // initialize empty B+ tree and insert
    if (treeHeight == 0 ){
		BTLeafNode leaf;
		leaf.initialBuffer();
		leaf.insert(key,rid);
		rootPid = pf.endPid();
		if ( rootPid == 0 ){
			rootPid++;
		}
		rc = leaf.write(rootPid, pf);
		treeHeight = 1;
		return rc;
    }
    else {
		int returnKey;
		PageId returnPid;
		int stage = 1;
	//recursive insert, update B+ tree 
	//split leaf nodes and push up to parent 
		recursiveInsert(stage, key, returnPid, returnKey, rid, rootPid);	
    }
}

RC BTreeIndex::recursiveInsert(int stage, int key, PageId& returnPid, int& returnKey, const RecordId& rid, PageId currPid)
{

    RC rc;
    //start from non-leaf node
    if(stage != treeHeight){
	
	BTNonLeafNode nleaf;
	PageId childPid,splitPid;
	int splitKey;
	NonLeafEntry left_entry3;

	nleaf.initializeRoot(INITIAL,INITIAL,INITIAL);
	//get a non-leaf node
	nleaf.read(currPid,pf);    
	
	//find child node use recursive algorithm
	//end up with stage = treeheight
	nleaf.locateChildPtr(key,childPid);
	rc = recursiveInsert(stage+1,key,splitPid,splitKey,rid,childPid);
	
	//after split leaf node , push up first entry of new leaf node to parent 
	if( rc == RC_NODE_FULL){
	    //if parent node is not full, insert to it and
	    //write to pagefile and return
	    rc = nleaf.insert(splitKey,splitPid); 
	    if(rc == 0){
	    	return nleaf.write(currPid,pf);
	    }
	    //if parent node is full, split and insert
	    else{
			BTNonLeafNode sibling;
			int midKey;
		//initiate sibling node 
		//split node and insert
			sibling.initializeRoot(INITIAL,INITIAL,INITIAL);
			nleaf.insertAndSplit(splitKey, splitPid, sibling, midKey);
	   
		//get sibling node's pid
		//wirte sibling node to pagefile  
			PageId sibPid = pf.endPid();
			sibling.write(sibPid, pf);
		
		//update old non-leaf node's pointer 
			nleaf.write(currPid, pf);
			if(stage == 1){
		    	rc = popedupRoot(currPid, sibPid, midKey);
			}
			returnPid = sibPid;
			returnKey = midKey;
			return rc;	
	    
	    }
	}
		else   return rc;
    }

    //if arrived leaf node whrere pid=currPid
    else
    {
		BTLeafNode leaf;
		leaf.initialBuffer();
		leaf.read(currPid, pf);
		rc = leaf.insert(key, rid);
   
	//if this leaf node is not full,write to pagefile and return RC_NODE_FULL
		if(rc == 0){
			return leaf.write(currPid,pf);
		}
	
	//if leaf node is full, split and insert and
	// push up the first entry to parent 
		else{
	    	BTLeafNode sibling;
	    	int key_sibling;
	    //initiate sibling node 
	    //split node and insert 
	    	sibling.initialBuffer();
	    	leaf.insertAndSplit(key, rid, sibling, key_sibling);
	    
	    //get sibling node's pid
	    //wirte sibling node to pagefile  
	    	PageId sibPid = pf.endPid();
	    	sibling.write(sibPid, pf);
	    
	    //update old leaf node's pointer 
	    //point to sibling node where pid=sibPid
	    	leaf.setNextNodePtr(sibPid);
	    	leaf.write(currPid, pf);
	    
	    //return Pid and Key of first entry in sibling
	    //copy to parent node
	    	returnPid = sibPid;
	    	returnKey = key_sibling;  
	    
	    //firt push up of a one height B+ tree with full leaf node 
	    //push up to parent and treeheight++
	    	if(stage == 1){
	    		rc = popedupRoot(currPid, sibPid, key_sibling);
	    	}
	    //return RC_NODE_FULL when leaf full and exits non-leaf node
	    	return rc;
		} 
    }
}

RC BTreeIndex::popedupRoot(PageId leftPid, PageId rightPid, int key)
{
    BTNonLeafNode nleaf;
    nleaf.initializeRoot(leftPid,key,rightPid);
    rootPid = pf.endPid();
    RC rc = nleaf.write(rootPid, pf); 
    treeHeight++;
 
    return rc; 
}


/*
 * Find the leaf-node index entry whose key value is larger than or 
 * equal to searchKey, and output the location of the entry in IndexCursor.
 * IndexCursor is a "pointer" to a B+tree leaf-node entry consisting of
 * the PageId of the node and the SlotID of the index entry.
 * Note that, for range queries, we need to scan the B+tree leaf nodes.
 * For example, if the query is "key > 1000", we should scan the leaf
 * nodes starting with the key value 1000. For this reason,
 * it is better to return the location of the leaf node entry 
 * for a given searchKey, instead of returning the RecordId
 * associated with the searchKey directly.
 * Once the location of the index entry is identified and returned 
 * from this function, you should call readForward() to retrieve the
 * actual (key, rid) pair from the index.
 * @param key[IN] the key to find.
 * @param cursor[OUT] the cursor pointing to the first index entry
 *                    with the key value.
 * @return error code. 0 if no error.
 */
RC BTreeIndex::locate(int searchKey, IndexCursor& cursor)
{
    BTNonLeafNode nleaf;
    RC rc;
    nleaf.initializeRoot(INITIAL,INITIAL,INITIAL);
    PageId pid = rootPid;

  
    for(int stage =1 ; stage < treeHeight; stage++){
	//if B+ tree not exits, return 
		rc = nleaf.read(pid, pf);
		if(rc != 0) return rc;
	//update pid, go to child node until leaf node
		rc = nleaf.locateChildPtr(searchKey,pid);
    } 

    BTLeafNode leaf;
    int eid;
    leaf.initialBuffer();
    leaf.read(pid, pf);
    rc = leaf.locate(searchKey,eid);
        //if not found entry return error code 
    if(rc !=0 ) return rc;
    cursor.eid = eid;
    cursor.pid = pid;
    return 0;

}

/*
 * Read the (key, rid) pair at the location specified by the index cursor,
 * and move foward the cursor to the next entry.
 * @param cursor[IN/OUT] the cursor pointing to an leaf-node index entry in the b+tree
 * @param key[OUT] the key stored at the index cursor location.
 * @param rid[OUT] the RecordId stored at the index cursor location.
 * @return error code. 0 if no error
 */
RC BTreeIndex::readForward(IndexCursor& cursor, int& key, RecordId& rid)
{
    BTLeafNode leaf;
    int eid = cursor.eid ;
    PageId pid = cursor.pid;
    leaf.initialBuffer();
    RC rc = leaf.read(pid,pf);
    if(rc != 0) return rc;
    rc = leaf.readEntry(eid, key, rid);
    if(rc != 0) return rc;
    
    eid++;
    if (eid >= leaf.getKeyCount()){
		cursor.pid = leaf.getNextNodePtr();
		cursor.eid = 0;
		return 0;
    }else{
		cursor.eid = eid;
		return 0;    
    }
}
