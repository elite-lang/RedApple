/**
 * Project Untitled
 */


#include "StatementNode.h"

/**
 * StatementNode implementation
 */


StatementNode::StatementNode(string name) {
	this->name = name;
}

StatementNode::~StatementNode() {

}


void StatementNode::putSymbolTable() {

}

/**
 * @return const string&
 */
const string& StatementNode::getName() {
    return name;
}

/**
 * @return int
 */
int StatementNode::getType() {
    return 0;
}