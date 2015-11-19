/* 
* @Author: sxf
* @Date:   2015-11-17 18:33:42
* @Last Modified by:   sxf
* @Last Modified time: 2015-11-17 20:51:34
*/

#include "MacroTranslate.h"

Node* MacroTranslate::Marco(Node* node_template, Node* args) {
	// 第一个参数 宏参数表
	Node* node = node_template->getChild();
	for (Node* p = node; p != NULL; p = p->getNext(), args = args->getNext()) {
		string name = p->getStr();
		args_map[name] = args;
	}

	// 第二个参数 模板部分
	node_template = node_template->getNext();
	Node* copied_temp = node_template->copyChild();
	marcoReplace(copied_temp);
	return copied_temp;
}

void MacroTranslate::marcoReplace(Node* node) {
	if (node == NULL) return;
	for (Node* p = node; p != NULL; p = p->getNext()) {
		Node* n = p->getNext();
		if (n != NULL && n->isIDNode()) {
			Node* arg = findArg(n->getStr());
			if (arg != NULL) {
				p->replaceNext(arg->copyChild());
			}
		}
		n = p->getChild();
		if (n != NULL) {
			if (n->isIDNode()) {
				Node* arg = findArg(n->getStr());
				if (arg != NULL) {
					p->replaceChild(arg->copyChild());
					continue;
				}
			}
			marcoReplace(n);
		}
	}
}

Node* MacroTranslate::findArg(const string& name) {
	auto p = args_map.find(name);
	if (p == args_map.end()) return NULL;
	return p->second;
}
