/**
 * Project Untitled
 */


#ifndef _CHARNODE_H
#define _CHARNODE_H

#include "Node.h"


class CharNode: public Node {
public: 
	char value;

	CharNode(char* v) {
		value = *v;
	}

	// virtual Value* codeGen(CodeGenContext* context); 
};

#endif //_CHARNODE_H