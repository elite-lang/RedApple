/**
 * Project Untitled
 */


#ifndef _STATEMENTNODE_H
#define _STATEMENTNODE_H

#include "ASTNode.h"

class StatementNode: public ASTNode {
public: 
    /**
     * @param pNode
     */
    void addChildren(StatementNode* pNode);

    /**
     * @param pNode
     */
    void addBrother(StatementNode* pNode);

private:
    StatementNode* children;
    StatementNode* brother;
};

#endif //_STATEMENTNODE_H
