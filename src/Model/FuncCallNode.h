/**
 * Project Untitled
 */


#ifndef _FUNCCALLNODE_H
#define _FUNCCALLNODE_H

#include "ExpressionNode.h"


class FuncCallNode: public ExpressionNode {
public: 
	
	void getSymbolTable();
};

#endif //_FUNCCALLNODE_H