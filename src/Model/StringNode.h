/* 
* @Author: sxf
* @Date:   2015-09-22 22:00:32
* @Last Modified by:   sxf
* @Last Modified time: 2015-11-01 18:48:56
*/

#ifndef STRING_NODE_H
#define STRING_NODE_H

#include "Node.h"
#include <string>

using namespace std;

class StringNode : public Node
{
public:
	StringNode(const char* _value){
		this->value = _value;
	}

	StringNode(char _value){
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



#endif // STRING_NODE_H
