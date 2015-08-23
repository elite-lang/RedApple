/**
 * Project Untitled
 */


#ifndef _INTNODE_H
#define _INTNODE_H

#include "ValueNode.h"


class IntNode: public ValueNode {
public: 
    IntNode(long v) {
        value = v;
    }

    long value;

    void generateCode() {

    }
};

#endif //_INTNODE_H
