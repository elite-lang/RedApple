/**
 * Project Untitled
 */


#ifndef _BYTENODE_H
#define _BYTENODE_H

#include "Node.h"


class ByteNode: public Node {
public: 
	char value;

	// virtual Value* codeGen(CodeGenContext* context); 
};

#endif //_BYTENODE_H