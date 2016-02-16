/*
* @Author: sxf
* @Date:   2015-09-22 19:21:10
* @Last Modified by:   sxf
* @Last Modified time: 2015-12-14 17:03:53
*/

#ifndef NODE_H
#define NODE_H

#include <string>
#include "LLCG/lvalue.h"

class ElegantList;

enum NodeType
{
	node_t = 0, int_node_t, float_node_t, char_node_t, id_node_t, string_node_t, type_node_t
};

class CodeGenContext;

/**
 * @brief List的基本组成元素，表示一个代码中的基本节点
 * @details 编译器后端代码生成时，会构建Node节点串连起来的语法树，结构为左孩子
 *          右兄弟的二叉树，这样即可表示代码的基本结构
 *
 *          Node节点的类型一般有如下几种(char 暂未使用)： \n
 *          id 		- 标识符 	- 对应NodeType为：id_node_t \n
 *          int 	- 整型 		- 对应NodeType为：int_node_t \n
 *          float 	- 浮点型 	- 对应NodeType为：float_node_t \n
 *          string 	- 字符串 	- 对应NodeType为：string_node_t \n
 *          type 	- 类型 		- 对应NodeType为：type_node_t \n
 *          @see NodeType
 *
 *          一般语法解析器在连接这款后端代码生成器时，只需要控制生成Node构造的语法树即可，
 *          语法树的形状符合RedApple中的定义，即可正确编译为后端代码。
 */
class Node
{
public:
	// 构建列表部分

	/**
	 * @brief 添加一个孩子节点
	 * @details 为当前Node添加一个孩子节点, 将child指针指向它, 可以是链表的首节点
	 *
	 * @param n 一个孩子节点,或孩子链表的首节点
	 */
	void addChildren(Node* n);

	/**
	 * @brief 添加一个兄弟节点
	 * @details 为当前Node添加一个兄弟节点, 将自己链表的末尾next指针设置为传入指针
	 *          由于为了减少语法树的大小, 这里采用的是单链表, 所以插入速度可能有点慢,
	 *          后期优化时, 可以权衡内存占用和速度, 再进行选择
	 *
	 * @param n 添加的节点指针
	 */
	void addBrother (Node* n);

	/**
	 * @brief 判断当前节点是一个元素, 还是一个链表
	 * @details 判断当前节点是一个元素, 还是一个链表, 主要用于getList方法和makeList方法
	 *          由于构造语法树时,很多情况不知道要添加的是一个节点还是一个链表.
	 *          这个方法可以用来判断.\n
	 *          但一般情况, getList方法应该足够使用了.
	 * @return 是单一元素返回true, 是链表返回false
	 */
	bool isSingle();

	// 便捷的构造list的方法

	/**
	 * @brief C++语言版的接口, 用来方便地构造一个列表
	 * @details 用来方便地构造一个列表, make_list里不定参数如果传入空, 那么该位置会被跳过
	 *          使用时务必注意, num和后面不定参数的个数一定要相当
	 *
	 * @param num 不定参数的格式
	 * @param ... 每个是一个 Node* 指针
	 *
	 * @return 构造好的列表的首节点指针
	 */
	static Node* make_list(int num, ...);

	/**
	 * @brief 其他语言的接口, 用来方便地构造一个列表
	 * @details 用来方便地构造一个列表, 功能和 make_list 一样
	 *          @see make_list()
	 *
	 * @param num 参数的个数
	 * @param plist 一个指针数组
	 *
	 * @return 构造好的列表的首节点指针
	 */
	static Node* makeList(int num, Node* plist[]);

	/**
	 * @brief 如果传入节点是单一元素,则为其包裹一层Node节点
	 * @details 如果传入节点是单一元素,则为其包裹一层Node节点
	 *          如果传入的是链表,能够方便地将其作为一个新Node的子元素
	 *          	然后将新的Node节点返回, 这样就形成了一个完整的链表
	 *          如果是单一元素,那么该函数什么都不做,将传入节点返回
	 *
	 * @param node 要处理的节点
	 * @return 是链表则包围一层, 否则直接返回传入的节点
	 */
	static Node* getList(Node* node);

	// 全局构造析构

	/**
	 * @brief 构造一个节点, 并为其添加一个子元素链表
	 *
	 * @param n 子元素链表, 相当于对空节点添加了孩子
	 * @return 新构造的节点
	 */
	static Node* Create(Node* n);

	/**
	 * @brief 构造一个空节点
	 * @return 新构造的节点
	 */
	static Node* Create();

	/**
	 * @brief 释放一个节点资源
	 * @param p 这里是一个指针的引用, 会自动将指针置为NULL
	 */
	static void Free(Node*& p);

	/**
	 * @brief 递归释放节点资源
	 * @param p 要释放的节点, 这里会递归调用子节点
	 */
	static void FreeAll(Node*& p);

	// 拷贝函数
	virtual Node* copy();
	virtual Node* copyAll();
	virtual Node* copyChild();

	// 节点的替换
	void replaceNext(Node* node);
	void replaceChild(Node* node);
	void replaceChildFirst(Node* node);

	/**
	 * @brief 可用于调试的打印指令
	 * @param k 缩进层次, 首次传0或1皆可
	 */
	void print(int k);

	Node* getNext() { return next; }
	Node* getChild() { return child; }

	/**
	 * @brief 重要的代码生成函数
	 * @details 重要的代码生成函数, 对于不同的节点, 会有不同的代码生成策略,
	 *          这个函数是多态调用的, 在每个Node的子类中都有相关实现
	 *          Node的代码生成会按照表处理, 将第一个元素视为宏名
	 *          ID会自动去符号表中查找定义, 返回符号表中存储的变量
	 *          Type会自动去符号表中查找, 返回一个类型的定义
	 *          Int会自动构造常量数字
	 *          String会自动构造常量字符串
	 *
	 * @param context 代码生成上下文类
	 * @return 返回代码生成的结果指针LValue
	 */
	virtual LValue codeGen(CodeGenContext* context);

	// 如果是含有字符串的节点，则返回所含字符串，否则将报错
	/**
	 * @brief 获取当前节点内存放的字符串
	 * @details 只有含有字符串的节点该函数有效, 目前ID和String有效
	 *          如果是普通的节点, 则会报异常
	 * @return 返回字符串的引用
	 */
	virtual std::string& getStr();

	/**
	 * @brief 获取类型名
	 * @return 节点的名字
	 */
	std::string getTypeName();

	/**
	 * @brief 获取节点类型
	 * @return 节点类型的枚举
	 */
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

	static ElegantList el;
};


#endif // NODE_H
