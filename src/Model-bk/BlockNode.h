/**
 * Project Untitled
 */


#ifndef _BLOCKNODE_H
#define _BLOCKNODE_H

#include "StatementNode.h"


class BlockNode: public StatementNode {
public: 
    BlockNode() {

    }

    BlockNode(StatementNode* stat) {
        this->addChildren(stat);
    }

    void generateCode() {

    }
};

#endif //_BLOCKNODE_H
