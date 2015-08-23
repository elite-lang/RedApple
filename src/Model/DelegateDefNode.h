/**
 * Project Untitled
 */


#ifndef _DELEGATEDEFNODE_H
#define _DELEGATEDEFNODE_H

#include "StatementNode.h"
#include <list>

using namespace std;

class DelegateDefNode: public StatementNode {
public: 
	string ret_type;
	list<string> args_type;
};

#endif //_DELEGATEDEFNODE_H