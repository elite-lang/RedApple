/**
 * Project Untitled
 */


#ifndef _VALUENODE_H
#define _VALUENODE_H

#include "ExpressionNode.h"


class ValueNode: public ExpressionNode {
public: 
	
	void getValue();
};

#endif //_VALUENODE_H