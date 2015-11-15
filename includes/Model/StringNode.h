/* 
* @Author: sxf
* @Date:   2015-09-22 22:00:32
* @Last Modified by:   sxf
* @Last Modified time: 2015-11-02 15:45:59
*/

#ifndef STRING_NODE_H
#define STRING_NODE_H

#include "Node.h"
#include <string>

using namespace std;

class StringNode : public Node
{
public:
	static StringNode* Create(const char* _value);
	static StringNode* Create(char _value);
	std::string& getStr() { return value; }
	virtual Value* codeGen(CodeGenContext* context); 
	virtual NodeType getType();
protected:
	virtual void printSelf();
	StringNode(const char* _value);
	StringNode(char _value);
private:
	string value;
};



#endif // STRING_NODE_H
