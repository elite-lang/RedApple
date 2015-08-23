/**
 * Project Untitled
 */
#include<string>
#include "IDNode.h"

using namespace std;

#ifndef _VARDEFNODE_H
#define _VARDEFNODE_H

#include "StatementNode.h"


class VarDefNode: public StatementNode {
public:
    VarDefNode(IDNode* type_name, IDNode* idname) {
        this->type_name = type_name->name;
        this->name = idname->name;
    }

    VarDefNode(IDNode* type_name, IDNode* idname, ExpressionNode* init_exp) {
        this->type_name = type_name->name;
        this->name = idname->name;
        this->init_exp = init_exp;
    }

    void generateCode() {

    }

    string* type_name;
    ExpressionNode* init_exp;
};

#endif //_VARDEFNODE_H
