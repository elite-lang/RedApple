/**
 * Project Untitled
 */


#ifndef _MODULEDEFNODE_H
#define _MODULEDEFNODE_H

#include "StatementNode.h"
#include <list>

using namespace std;

class ModuleDefNode: public StatementNode {
public: 
	list<StatementNode*> *def_list;

	ModuleDefNode(string type, string name, list<StatementNode*> *def_list);
};

#endif //_MODULEDEFNODE_H