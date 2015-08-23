/**
 * Project Untitled
 */


#ifndef _VALUENODE_H
#define _VALUENODE_H

#include "ExpressionNode.h"


class ValueNode: public ExpressionNode {
public: 
	
	void getValue();

    void generateCode() {

    }
};

#endif //_VALUENODE_H
