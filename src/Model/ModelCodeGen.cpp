/* 
* @Author: sxf
* @Date:   2015-10-12 19:21:50
* @Last Modified by:   sxf
* @Last Modified time: 2015-11-25 21:58:03
*/

#include "CodeGenContext.h"
#include "nodes.h"

#include <iostream>
using namespace std;


LValue Node::codeGen(CodeGenContext* context) {
	return context->MacroMake(this);
}

LValue IntNode::codeGen(CodeGenContext* context) {
	return context->getLLCG()->ConstInt(value);
}

LValue FloatNode::codeGen(CodeGenContext* context) {
	return context->getLLCG()->ConstDouble(value);
}


LValue StringNode::codeGen(CodeGenContext* context) {
    return context->getLLCG()->ConstString(value);
}

LValue IDNode::codeGen(CodeGenContext* context) {
    LValue v = context->FindVar(value);
    if (v == NULL) {
        cerr << "未定义的变量: " << value << endl;
        return NULL;
    }
    if (context->isSave()) return v;
    return context->getLLCG()->Load(v);
}


