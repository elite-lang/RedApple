/**
 * Project Untitled
 */


#ifndef _FLOATNODE_H
#define _FLOATNODE_H

#include "ValueNode.h"


class FloatNode: public ValueNode {
public: 
    FloatNode(double d) {
        value = d;
    }

    double value;

    void generateCode() {

    }
};

#endif //_FLOATNODE_H
