/* 
* @Author: sxf
* @Date:   2015-11-17 17:11:42
* @Last Modified by:   sxf
* @Last Modified time: 2015-12-14 17:09:06
*/

#ifndef MACRO_TRANSLATE_H
#define MACRO_TRANSLATE_H

#include <string>
#include <map>
#include "Node.h"

using namespace std;

/**
 * @brief 宏翻译器, 可以支持用户自定义宏
 * @details 宏翻译器, 应该说是宏展开器, 能够根据用户的定义, 
 *          将宏转换为真正的形式, 支持递归
 */
class MacroTranslate
{
public:
	Node* Marco(Node* node_template, Node* args);
private:
	void marcoReplace(Node* node);
	Node* findArg(const string& name);
	map< string, Node* > args_map;
};



#endif // MACRO_TRANSLATE_H
