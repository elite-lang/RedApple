/* 
* @Author: sxf
* @Date:   2015-09-22 19:21:10
* @Last Modified by:   sxf
* @Last Modified time: 2015-11-11 19:48:24
*/

#ifndef NODE_H
#define NODE_H

#include <string>

namespace llvm {
	class Value;
	class Type;
} // llvm

using namespace llvm;

enum NodeType
{
	node_t = 0, int_node_t, float_node_t, char_node_t, id_node_t, string_node_t
};

class CodeGenContext;
class Node
{
public:
	// 构建列表部分
	void addChildren(Node* n);
	void addBrother (Node* n);
	bool isSingle();
	static Node* make_list(int num, ...);
	static Node* getList(Node* node);
	static Node* Create(Node* n);
	static Node* Create();

	void print(int k);
	Node* getNext() { return next; }
	Node* getChild() { return child; }
	virtual Value* codeGen(CodeGenContext* context); 

	// 这里负责获取或设置当前节点的LLVM类型, 未知类型返回NULL
	virtual Type* getLLVMType();
	virtual void  setLLVMType(Type* t);

	// 如果是含有字符串的节点，则返回所含字符串，否则将报错
	std::string& getStr();

	// 类型相关
	std::string getTypeName();
	virtual NodeType getType();
	bool isNode();
	bool isIntNode();
	bool isFloatNode();
	bool isIDNode();
	bool isStringNode();
	bool isCharNode();

protected:
	Node();
	Node(Node* n);
	~Node();

	virtual void printSelf();
	void init();

	Type* llvm_type;
	Node* next;
	Node* child;
};


#endif // NODE_H
