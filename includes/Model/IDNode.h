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
	static IDNode* Create(const char* _value);
	static IDNode* Create(char _value);
	std::string& getStr() { return value; }
	virtual LValue codeGen(CodeGenContext* context); 
	virtual NodeType getType();
	virtual Node* copy() {
		return new IDNode(*this);
	}
protected:
	IDNode(const char* _value);
	IDNode(char _value);
	virtual void printSelf();
private:
	string value;
};

#endif //_IDNODE_H