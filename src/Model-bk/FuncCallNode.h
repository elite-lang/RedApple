/**
 * Project Untitled
 */


#ifndef _FUNCCALLNODE_H
#define _FUNCCALLNODE_H

#include "ExpressionNode.h"
#include "IDNode.h"
#include "ValueNode.h"
#include "CallArgsNode.h"

class FuncCallNode: public ExpressionNode {
public: 
    FuncCallNode(IDNode* idnode, CallArgsNode* args) {
        this->idnode = idnode;
        this->callargs = args;
    }

    FuncCallNode(string* oper, ExpressionNode* idnode, ExpressionNode* expr) {
        this->idnode = new IDNode(oper);
        this->callargs = new CallArgsNode();
        this->callargs->addChildren(idnode);
        this->callargs->addChildren(expr);
    }

    void generateCode() {

    }

private:
    IDNode* idnode;
    CallArgsNode* callargs;
};

#endif //_FUNCCALLNODE_H
