/*
* @Author: sxf
* @Date:   2015-11-11 13:57:49
* @Last Modified by:   sxf
* @Last Modified time: 2015-12-14 16:56:32
*/


#ifndef CODE_GEN_H
#define CODE_GEN_H

#include <set>

class Node;


/**
 * @brief 代码生成器的接口类
 * @details 代码生成器的接口
 *          使用RedApple后端代码生成器时,核心功能可以通过该接口调用
 *          首先调用Init初始化该生成器, 之后对全部定义头文件进行Prescan
 *          接着对每一个要编译的目标文件调用Make,
 *          最后调用MakeMeta
 */
class CodeGen
{
public:
	virtual void Init() = 0;
	virtual void PreScan(Node* node) = 0;
	virtual void PreScan(std::set<Node*>& nodes) = 0;
	virtual void Make(Node* node, const char* outfile_name, const char* module_name = "") = 0;
	virtual void MakeMeta(const char* outfile_name, const char* module_name = "") = 0;
};

#endif // CODE_GEN_H
