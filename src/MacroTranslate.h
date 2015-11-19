/* 
* @Author: sxf
* @Date:   2015-11-17 17:11:42
* @Last Modified by:   sxf
* @Last Modified time: 2015-11-17 20:06:48
*/

#ifndef MACRO_TRANSLATE_H
#define MACRO_TRANSLATE_H

#include <string>
#include <map>
#include "Node.h"

using namespace std;

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
