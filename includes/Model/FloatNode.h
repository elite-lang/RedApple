/**
 * Project Untitled
 */


#ifndef _FLOATNODE_H
#define _FLOATNODE_H

#include "Node.h"
#include <string>
using namespace std;

class FloatNode: public Node {
public: 
	static FloatNode* Create(const char* num);
	virtual Value* codeGen(CodeGenContext* context); 
	virtual NodeType getType();
	float getFloat();
protected:
	float value;
	FloatNode(const char* num);
};

#endif //_FLOATNODE_H