/**
 * Project Untitled
 */


#ifndef _IDNODE_H
#define _IDNODE_H

#include "ExpressionNode.h"


class IDNode: public ExpressionNode {
public: 
    IDNode(string* node) {
        idname = node;
    }

    void generateCode() {

    }
private:
    string* idname;
};

#endif //_IDNODE_H
