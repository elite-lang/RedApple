#include "ExpressionNode.h"
#include "VarDefNode.h"

#ifndef FUNCARGSNODE_H
#define FUNCARGSNODE_H

class FuncArgsNode : public ExpressionNode
{
public:
    FuncArgsNode(VarDefNode* vardef ) {
        this->addChildren(vardef);
    }

    void generateCode() {

    }
private:
    VarDefNode* var_def;
};

#endif // FUNCARGSNODE_H
