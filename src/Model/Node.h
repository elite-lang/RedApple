/* 
* @Author: sxf
* @Date:   2015-09-22 19:21:10
* @Last Modified by:   sxf
* @Last Modified time: 2015-10-29 09:43:49
*/

#ifndef NODE_H
#define NODE_H

#include <string>
#include "llvm/IR/Module.h"
using namespace llvm;

class CodeGenContext;
class Node
{
public:
	Node();
	Node(Node* n);
	~Node();

	void addChildren(Node* n);
	void addBrother (Node* n);
	
	static Node* make_list(int num, ...);

	void print(int k);
	Node* getNext() { return next; }
	Node* getChild() { return child; }
	virtual Value* codeGen(CodeGenContext* context); 
	virtual std::string getType();

	// 这里负责获取或设置当前节点的LLVM类型, 未知类型返回NULL
	virtual Type* getLLVMType();
	virtual void  setLLVMType(Type* t);

protected:
	virtual void printSelf();
	void init();

	Type* llvm_type;
	Node* next;
	Node* child;
};


#endif // NODE_H
