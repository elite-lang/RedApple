/**
 * Project Untitled
 */


#ifndef _FUNCCALLNODE_H
#define _FUNCCALLNODE_H

#include "ExpressionNode.h"
#include "CallArgsNode.h"
#include <string>

using namespace std;

class FuncCallNode: public ExpressionNode {
public: 
	FuncCallNode(string funcname, ExpressionNode* e1, ExpressionNode* e2);
	FuncCallNode(string funcname, ExpressionNode* e1);
	FuncCallNode(string funcname, CallArgsNode* args);

	void getSymbolTable();
};

#endif //_FUNCCALLNODE_H