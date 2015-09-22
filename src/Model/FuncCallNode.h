/**
 * Project Untitled
 */


#ifndef _FUNCCALLNODE_H
#define _FUNCCALLNODE_H

#include "ExpressionNode.h"
#include "CallArgsNode.h"
#include <string>
#include <list>
 
using namespace std;

class FuncCallNode: public ExpressionNode {
public: 
	FuncCallNode(int funcname, ExpressionNode* e1, ExpressionNode* e2);
	FuncCallNode(int funcname, ExpressionNode* e1);
	FuncCallNode(string funcname, list<CallArgsNode*> *args);

	void getSymbolTable();
};

#endif //_FUNCCALLNODE_H