/* 
* @Author: sxf
* @Date:   2015-11-19 11:22:12
* @Last Modified by:   sxf
* @Last Modified time: 2015-11-25 15:47:07
*/

#ifndef TYPE_NODE_H
#define TYPE_NODE_H

#include "Node.h"
#include <string>
using namespace std;

namespace llvm {
	class Type;
} // llvm

class TypeNode: public Node {
public: 
	static TypeNode* Create(const char* name, bool is_const = false, bool is_source = false);
	LValue typeGen(CodeGenContext* context); 
	void addDimension();
	bool isArray() {
		return dimension == 0;
	}

	virtual NodeType getType();
	virtual string& getStr();
	string& getTypeName();

	virtual Node* copy() {
		return new TypeNode(*this);
	}
protected:
	virtual void printSelf();
	string str;	
	string name;
	bool is_const = false;
	bool is_source = false; // 这个只在函数是结构体时有用，来设置真正的原型类型
	int dimension = 0;
	TypeNode(const char* name, bool is_const = false, bool is_source = false);
};



#endif // TYPE_NODE_H
