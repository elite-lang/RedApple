/* 
* @Author: sxf
* @Date:   2015-09-22 19:21:40
* @Last Modified by:   sxf
* @Last Modified time: 2015-11-02 15:33:52
*/

#include "Node.h"
#include <stdarg.h>
#include <stdio.h>
#include "nodes.h"
#include <iostream>

void Node::init() {
	llvm_type = NULL;
	next = child = NULL; 
}

Node::Node() {
	init();
}

Node::Node(Node* n) {
	init();
	addChildren(n);
}

Node::~Node() {

}

Node* Node::Create(Node* n) {
	return new Node(n);
}

Node* Node::Create() {
	return new Node();
}


void Node::addChildren(Node* n) {
	if (child == NULL) {
		child = n;
	} else {
		child->addBrother(n);
	}
}

void Node::addBrother (Node* n) {
	Node* p = this;
	while (p->next != NULL) {
		p = p->next;
	}
	p->next = n;
}

void Node::print(int k) {
	for (int i = 0; i < k; ++i)
		printf("    ");
	printSelf();
	printf("\n");

	Node* p = child; int t = 0;
	while (p != NULL) {
		p->print(k+1);
		p = p->next;
		++t;
	}
	if (t >= 3) printf("\n");
}

void Node::printSelf() {
	printf("Node");
}

NodeType Node::getType() {
	return node_t;
}

bool Node::isSingle() {
	return next == NULL;
}

Node* Node::make_list(int num, ...) {
	va_list argp; Node* para = NULL;  
	Node* ans = NULL;  
	va_start( argp, num );    
    for (int i = 0; i < num; ++i) {    
        para = va_arg( argp, Node* );  
        if (!para->isSingle()) para = new Node(para);
        if ( ans == NULL )    
        	ans = para;
        else ans->addBrother(para);
    }    
    va_end( argp );
    return ans;
}

Node* Node::getList(Node* node) {
	if (!node->isSingle()) return new Node(node);
	return node;
}

Type* Node::getLLVMType() {
	return llvm_type;
}

void  Node::setLLVMType(Type* t) {
	llvm_type = t;
}

bool Node::isNode() {
	return getType() == node_t;
}

bool Node::isIntNode() {
	return getType() == int_node_t;
}

bool Node::isFloatNode() {
	return getType() == float_node_t;
}

bool Node::isIDNode() {
	return getType() == id_node_t;
}

bool Node::isStringNode() {
	return getType() == string_node_t;
}

bool Node::isCharNode() {
	return getType() == char_node_t;
}

std::string Node::getTypeName() {
	switch (getType()) {
		case node_t: return "Node";
		case int_node_t: return "IntNode";
		case string_node_t: return "StringNode";
		case id_node_t: return "IDNode";
		case char_node_t: return "CharNode";
		case float_node_t: return "FloatNode";
	}
}

std::string& Node::getStr() {
	if (this->isStringNode()) {
		StringNode* string_this = (StringNode*)this;
		return string_this->getStr();
	} 
	if (this->isIDNode()) {
		IDNode* string_this = (IDNode*)this;
		return string_this->getStr();
	} 
	std::cerr << "getStr() - 获取字符串错误, 该类型不正确：" << getTypeName() << std::endl;
	exit(1);
}