/**
 * Project Untitled
 */


#ifndef _FUNCDEFNODE_H
#define _FUNCDEFNODE_H

#include "StatementNode.h"
#include "BlockNode.h"
#include "FuncArgsNode.h"
#include "IDNode.h"

class FuncDefNode: public StatementNode {
public: 
    FuncDefNode(IDNode* returntype, IDNode* funcname, FuncArgsNode* args, BlockNode* funcblock ) {
        this->addChildren(returntype);
        this->addChildren(funcname);
        this->addChildren(args);
        this->addChildren(funcblock);

        func_name = funcname->name;
        return_name = returntype->name;
        this->args  = args;
        this->block = funcblock;
    }


    void generateCode() {

    }
private:
    string* func_name;
    string* return_name;
    FuncArgsNode* args;
    BlockNode* block;
};

#endif //_FUNCDEFNODE_H
