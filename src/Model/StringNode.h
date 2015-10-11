/* 
* @Author: sxf
* @Date:   2015-09-22 22:00:32
* @Last Modified by:   sxf
* @Last Modified time: 2015-10-10 21:24:32
*/

#ifndef STRING_NODE_H
#define STRING_NODE_H

#include "Node.h"
#include <string>

using namespace std;

class StringNode : public Node
{
public:
	StringNode(const char* _str){
		this->str = _str;
	}

	StringNode(char _str){
		this->str = _str;
	}

	std::string& getStr() { return str; }

	virtual std::string getType();
protected:
	virtual void printSelf();
	
private:
	string str;
};



#endif // STRING_NODE_H
