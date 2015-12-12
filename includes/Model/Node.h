/* 
* @Author: sxf
* @Date:   2015-09-22 19:21:10
* @Last Modified by:   sxf
* @Last Modified time: 2015-12-10 17:36:01
*/

#ifndef NODE_H
#define NODE_H

#include <string>
#include "LLCG/lvalue.h"

enum NodeType
{
	node_t = 0, int_node_t, float_node_t, char_node_t, id_node_t, string_node_t, type_node_t
};

class CodeGenContext;
class Node
{
public:
	// 构建列表部分
	void addChildren(Node* n);
	void addBrother (Node* n);
	bool isSingle();

	// 便捷的构造list的方法
	static Node* make_list(int num, ...);
	static Node* makeList(int num, Node* plist[]);
	static Node* getList(Node* node);
	
	// 全局构造析构
	static Node* Create(Node* n);
	static Node* Create();
	static void Free(Node*& p);
	static void FreeAll(Node*& p);

	// 拷贝函数
	virtual Node* copy();
	virtual Node* copyAll();
	virtual Node* copyChild();

	// 节点的替换
	void replaceNext(Node* node);
	void replaceChild(Node* node);
	void replaceChildFirst(Node* node);

	void print(int k);
	Node* getNext() { return next; }
	Node* getChild() { return child; }
	virtual LValue codeGen(CodeGenContext* context); 

	// 如果是含有字符串的节点，则返回所含字符串，否则将报错
	virtual std::string& getStr();

	// 类型相关
	std::string getTypeName();
	virtual NodeType getType();

	bool isNode();
	bool isIntNode();
	bool isFloatNode();
	bool isIDNode();
	bool isStringNode();
	bool isCharNode();
	bool isTypeNode();


protected:
	Node();
	Node(Node* n);
	virtual ~Node();

	virtual void printSelf();
	void init();

	Node* next;
	Node* child;
};


#endif // NODE_H
