/* 
* @Author: sxf
* @Date:   2015-09-22 19:21:40
* @Last Modified by:   sxf
* @Last Modified time: 2015-11-19 19:43:22
*/

#include "Node.h"
#include <stdarg.h>
#include <stdio.h>
#include "nodes.h"
#include <iostream>

void Node::init() {
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

void Node::Free(Node*& p) {
	if (p != NULL) {
		delete p;
		p = NULL;
	}
}

void Node::FreeAll(Node*& p) {
	if (p != NULL) {
		Node::FreeAll(p->next);
		Node::FreeAll(p->child);	
		delete p;
		p = NULL;
	}
}

Node* Node::copy() {
	return new Node(*this);
}

Node* Node::copyAll() {
	Node* n = copy();
	if (next != NULL) n->next = next->copyAll();
	if (child != NULL) n->child = child->copyAll();
	return n;
}

Node* Node::copyChild() {
	Node* n = copy();
	n->next = NULL;
	if (child != NULL) n->child = child->copyAll();
	return n;
}


void Node::replaceNext(Node* node) {
	Node* n = this->next;
	if (n == NULL) { printf("替换失败\n"); return; }
	next = node;
	node->next = n->next;
	Node::FreeAll(n->child);
	Node::Free(n);
}

void Node::replaceChild(Node* node) {
	Node* n = this->child;
	if (n == NULL) { printf("替换失败\n"); return; }
	child = node;
	Node::FreeAll(n);
}

void Node::replaceChildFirst(Node* node) {
	Node* n = this->child;
	if (n == NULL) { printf("替换失败\n"); return; }
	child = node;
	node->next = n->next;
	Node::FreeAll(n->child);
	Node::Free(n);
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

bool Node::isTypeNode() {
	return getType() == type_node_t;
}


std::string Node::getTypeName() {
	switch (getType()) {
		case node_t: return "Node";
		case int_node_t: return "IntNode";
		case string_node_t: return "StringNode";
		case id_node_t: return "IDNode";
		case char_node_t: return "CharNode";
		case float_node_t: return "FloatNode";
		case type_node_t: return "TypeNode";
	}
}

std::string& Node::getStr() {
	std::cerr << "getStr() - 获取字符串错误, 该类型不正确：" << getTypeName() << std::endl;
	exit(1);
}