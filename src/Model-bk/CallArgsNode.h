#include "ExpressionNode.h"

#ifndef CALLARGSNODE_H
#define CALLARGSNODE_H

class CallArgsNode : public ExpressionNode
{
public:
    CallArgsNode() { }

    CallArgsNode(ExpressionNode* exp) {
        this->addBrother(exp);
    }

    void generateCode();
};

#endif // CALLARGSNODE_H
