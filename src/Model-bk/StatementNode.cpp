/**
 * Project Untitled
 */


#include "StatementNode.h"

/**
 * @param pNode
 */
void StatementNode::addChildren(StatementNode* pNode) {
    if (this->children == NULL) this->children = pNode;
    else this->children->addBrother(pNode);
}

/**
 * @param pNode
 */
void StatementNode::addBrother(StatementNode* pNode) {
    StatementNode* pNext = this->brother;
    while (pNext->brother != NULL) {
        pNext = pNext->brother;
    }
    pNext->brother = pNode;
}
