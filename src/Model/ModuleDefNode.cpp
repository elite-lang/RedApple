/**
 * Project Untitled
 */


#include "ModuleDefNode.h"

/**
 * ModuleDefNode implementation
 */

ModuleDefNode::ModuleDefNode(string type, string name, list<StatementNode*> *def_list) 
	: StatementNode(name)
{
	this->def_list = def_list;
}