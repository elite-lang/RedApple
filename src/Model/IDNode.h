/**
 * Project Untitled
 */


#ifndef _IDNODE_H
#define _IDNODE_H

#include "Node.h"
#include <string>

using namespace std;

class IDNode: public Node {
public: 
	IDNode(const char* _value){
		this->value = _value;
	}
	IDNode(char _value){
		this->value = _value;
	}
	std::string& getStr() { return value; }
	virtual Value* codeGen(CodeGenContext* context); 
	virtual NodeType getType();
protected:
	virtual void printSelf();
private:
	string value;
};

#endif //_IDNODE_H