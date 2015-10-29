/* 
* @Author: sxf
* @Date:   2015-09-22 19:21:40
* @Last Modified by:   sxf
* @Last Modified time: 2015-10-29 09:44:13
*/

#include "Node.h"
#include <stdarg.h>
#include <stdio.h>

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

std::string Node::getType() {
	return std::string("Node");
}

Node* Node::make_list(int num, ...) {
	va_list argp; Node* para = NULL;  
	Node* ans = NULL;  
	va_start( argp, num );    
    for (int i = 0; i < num; ++i) {    
        para = va_arg( argp, Node* );  
        if ( ans == NULL )    
        	ans = para;
        else ans->addBrother(para);
    }    
    va_end( argp );
    return ans;
}

Type* Node::getLLVMType() {
	return llvm_type;
}

void  Node::setLLVMType(Type* t) {
	llvm_type = t;
}
