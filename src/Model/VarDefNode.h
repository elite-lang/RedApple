/**
 * Project Untitled
 */


#ifndef _VARDEFNODE_H
#define _VARDEFNODE_H

#include "StatementNode.h"
#include "ExpressionNode.h"
#include <string>

using namespace std;

class VarDefNode: public StatementNode {
public: 
	string type_name;

	VarDefNode(string type_name, string var_name, ExpressionNode* exp = NULL);
};

#endif //_VARDEFNODE_H