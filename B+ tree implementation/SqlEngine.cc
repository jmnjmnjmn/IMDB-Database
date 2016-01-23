/**
 * Copyright (C) 2008 by The Regents of the University of California
 * Redistribution of this file is permitted under the terms of the GNU
 * Public License (GPL).
 *
 * @author Junghoo "John" Cho <cho AT cs.ucla.edu>
 * @date 3/24/2008
 */

#include <cstdio>
#include <iostream>
#include <fstream>
#include "Bruinbase.h"
#include "SqlEngine.h"
#include "BTreeIndex.h"

using namespace std;

// external functions and variables for load file and sql command parsing 
extern FILE* sqlin;
int sqlparse(void);


RC SqlEngine::run(FILE* commandline)
{
  fprintf(stdout, "Bruinbase> ");

   // set the command line input and start parsing user input
  sqlin = commandline;
  sqlparse();  // sqlparse() is defined in SqlParser.tab.c generated from
               // SqlParser.y by bison (bison is GNU equivalent of yacc)

  return 0;
}

RC SqlEngine::select(int attr, const string& table, const vector<SelCond>& cond)
{
  RecordFile rf;   // RecordFile containing the table
  RecordId   rid;  // record cursor for table scanning
  BTreeIndex b;
  IndexCursor cursor;

  RC     rc;
  int    key;     
  string value;
  int    count;
  int    diff;
  int getkey = 0;
  bool   equal = false;
  bool   upper = false;

  // open the table file
  if ((rc = rf.open(table + ".tbl", 'r')) < 0) {
    fprintf(stderr, "Error: table %s does not exist\n", table.c_str());
    return rc;
  }

  // scan the table file from the beginning
  rid.pid = rid.sid = 0;
  count = 0;
  
  if(b.open(table+".idx",'r') == 0){ 

    for(int i = 0; i < cond.size(); i++){ 
      if(cond[i].attr != 1) 
        continue;
      else if(cond[i].comp == SelCond::EQ){
        getkey = atoi(cond[i].value);
        break;
      }
      else if(cond[i].comp == SelCond::GE){
        if(getkey == 0){
          getkey = atoi(cond[i].value);
          continue;
        }
        int compare = atoi(cond[i].value);
        if(compare > getkey)
          getkey = compare;
        
      }
      else if(cond[i].comp == SelCond::GT){
        if(getkey == 0){
          getkey = atoi(cond[i].value);
          continue;
        }
        int compare = atoi(cond[i].value)+1;
        if(compare > getkey)
          getkey = compare;
      }
    }

    b.locate(getkey,cursor);
 
    while(!b.readForward(cursor, key, rid) && !equal && !upper){ 
      if (attr != 4) { 
        if ((rc = rf.read(rid, key, value)) < 0) {
          fprintf(stderr, "Error: while reading a tuple from table %s\n", table.c_str());
          goto exit_select;
        }
      }
      
      for (unsigned i = 0; i < cond.size(); i++) {
        // compute the difference between the tuple value and the condition value
        switch (cond[i].attr) {
        case 1:
        diff = key - atoi(cond[i].value);
        break;
        case 2:
        diff = strcmp(value.c_str(), cond[i].value);
        break;
        }
        // skip the tuple if any condition is not met
         switch (cond[i].comp) 
        {
          case SelCond::EQ:
            if (diff == 0) equal = true;
            else goto next_tuple1;
            break;
          case SelCond::NE:
            if (diff == 0) goto next_tuple1;
            break;
          case SelCond::GT:
            if (diff <= 0) goto next_tuple1;
            break;
          case SelCond::LT:
            if (diff >= 0) 
            {
              upper = true;
              goto next_tuple1;
            }
            break;
          case SelCond::GE:
            if (diff < 0) goto next_tuple1;
            break;
          case SelCond::LE:
            if (diff > 0) 
            {
              upper = true;
              goto next_tuple1;
            }
            break;
        }
      }

      // the condition is met for the tuple. 
      // increase matching tuple counter
      count++;

      // print the tuple 
      switch (attr) {
      case 1:  // SELECT key
        fprintf(stdout, "%d\n", key);
        break;
      case 2:  // SELECT value
        fprintf(stdout, "%s\n", value.c_str());
        break;
      case 3:  // SELECT *
        fprintf(stdout, "%d '%s'\n", key, value.c_str());
        break;
      }
      next_tuple1:;
      
    }
  }

  else
    while (rid < rf.endRid()) {
      // read the tuple
      if ((rc = rf.read(rid, key, value)) < 0) {
        fprintf(stderr, "Error: while reading a tuple from table %s\n", table.c_str());
        goto exit_select;
      }

      // check the conditions on the tuple
      for (unsigned i = 0; i < cond.size(); i++) {
        // compute the difference between the tuple value and the condition value
        switch (cond[i].attr) {
        case 1:
    diff = key - atoi(cond[i].value);
    break;
        case 2:
    diff = strcmp(value.c_str(), cond[i].value);
    break;
        }

        // skip the tuple if any condition is not met
        switch (cond[i].comp) {
        case SelCond::EQ:
    if (diff != 0) goto next_tuple;
    break;
        case SelCond::NE:
    if (diff == 0) goto next_tuple;
    break;
        case SelCond::GT:
    if (diff <= 0) goto next_tuple;
    break;
        case SelCond::LT:
    if (diff >= 0) goto next_tuple;
    break;
        case SelCond::GE:
    if (diff < 0) goto next_tuple;
    break;
        case SelCond::LE:
    if (diff > 0) goto next_tuple;
    break;
        }
      }

      // the condition is met for the tuple. 
      // increase matching tuple counter
      count++;

      // print the tuple 
      switch (attr) {
      case 1:  // SELECT key
        fprintf(stdout, "%d\n", key);
        break;
      case 2:  // SELECT value
        fprintf(stdout, "%s\n", value.c_str());
        break;
      case 3:  // SELECT *
        fprintf(stdout, "%d '%s'\n", key, value.c_str());
        break;
      }

      // move to the next tuple
      next_tuple:
      ++rid;
    }

    // print matching tuple count if "select count(*)"
    if (attr == 4) {
      fprintf(stdout, "%d\n", count);
    }
    rc = 0;

    // close the table file and return
    exit_select:
    b.close();
    rf.close();
    return rc;
}


RC SqlEngine::load(const string& table, const string& loadfile, bool index)
{
  
  // set up tablename
  string tablename = table + ".tbl"; 
  RecordFile NEW_table;
  NEW_table.open(tablename, 'w');     // open and set to write mode


  BTreeIndex b;
  if(index){ //if we need to create an index
    b.open(table + ".idx", 'w');//if opening fails
  }
  IndexCursor cursor;//pid , eid
  int keyout;
  RecordId ridout;
  
  // set up reading file
  ifstream t_file;
  t_file.open(loadfile.c_str());    // Open the file

  SqlEngine parser; 
  string  t_line;   
  int   linekey;    
  string  lineval;    
  RecordId recordid; 

  if(t_file.is_open())
  {
    while(t_file.good())
    {
      // get line from file
      getline(t_file, t_line);
      //parse the line that was gained
      if(parser.parseLoadLine(t_line, linekey, lineval) == 0){// add the line in the table
        NEW_table.append(linekey, lineval, recordid);
        if (index){
          b.insert(linekey, recordid);
        }
      }
    }
  }
   
  if(index)
    b.close();
 
  t_file.close();

  NEW_table.close();

  return 0;
}


RC SqlEngine::parseLoadLine(const string& line, int& key, string& value)
{
    const char *s;
    char        c;
    string::size_type loc;
    
    // ignore beginning white spaces
    c = *(s = line.c_str());
    while (c == ' ' || c == '\t') { c = *++s; }

    // get the integer key value
    key = atoi(s);

    // look for comma
    s = strchr(s, ',');
    if (s == NULL) { return RC_INVALID_FILE_FORMAT; }

    // ignore white spaces
    do { c = *++s; } while (c == ' ' || c == '\t');
    
    // if there is nothing left, set the value to empty string
    if (c == 0) { 
        value.erase();
        return 0;
    }

    // is the value field delimited by ' or "?
    if (c == '\'' || c == '"') {
        s++;
    } else {
        c = '\n';
    }

    // get the value string
    value.assign(s);
    loc = value.find(c, 0);
    if (loc != string::npos) { value.erase(loc); }

    return 0;
}
