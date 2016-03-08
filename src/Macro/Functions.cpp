/*
* @Author: sxf
* @Date:   2015-10-26 14:00:25
* @Last Modified by:   sxf
* @Last Modified time: 2015-12-27 16:30:41
*/

#include "CodeGenContext.h"
#include "StringNode.h"
#include "MetaModel/StructModel.h"
#include "MetaModel/FunctionModel.h"
#include "nodes.h"
#include "idtable.h"
#include <iostream>

using namespace std;

static LValue function_macro(CodeGenContext* context, Node* node) {
	// 第二个参数, 函数名
	node = node->getNext();
	std::string function_name = node->getStr();
	cout << function_name << endl;
	shared_ptr<FunctionModel> fm = context->getFunctionModel(function_name);
	fm->genMetaCode(context);
	LValue F = fm->getFunction(context);

	// 第三个参数, 参数表
	Node* args_node = node = node->getNext();

	// 第四个参数, 代码块
	node = node->getNext();
	if (node->getChild() == NULL) {
		return F; // 仅仅是函数声明
	}
	context->st->push();
	context->getLLCG()->CreateBasicBlock(F); // 创建新的Block

	// 特殊处理参数表, 这个地方特别坑，你必须给每个函数的参数
	// 手动AllocaInst开空间，再用StoreInst存一遍才行，否则一Load就报错
	// context->MacroMake(args_node->getChild());
	if (args_node->getChild() != NULL) {
		context->MacroMake(args_node);
		context->getLLCG()->FunctionBodyBegin(F, fm->name_list);
	}
	context->MacroMake(node);
	context->st->pop();
	// 处理块结尾
	context->getLLCG()->FunctionBodyEnd();
	return F;
}

static LValue set_macro(CodeGenContext* context, Node* node) {
	// 参数一 类型
	LValue t;
	if (node->isTypeNode()) {
		TypeNode* tn = (TypeNode*) node;
		t = tn->typeGen(context);
	} else {
		printf("错误的节点类型\n");
		exit(1);
	}

	// 参数二 变量名
	Node* var_node = node->getNext();
	string var_name = var_node->getStr();

	// 参数三 初始化值
	// 这里有个问题 初始化为常数时,Store会出问题
	node = var_node->getNext();
	if (node == NULL) {
		LValue addr = context->getLLCG()->DefVar(t, var_name);
		context->DefVar(var_name, addr);
		return addr;
	}
	LValue init_expr = node->codeGen(context);
	if (init_expr == NULL) {
		cerr << "变量的初始化无效: " << var_name << endl;
		exit(1);
	}
	LValue addr;
	if (context->st->getLevel() != 0)
		addr = context->getLLCG()->DefVar(t, var_name, init_expr);
	else
		addr = context->getLLCG()->DefGlobalVar(t, var_name, init_expr);
	context->DefVar(var_name, addr);
	return addr;
}

static LValue select_macro(CodeGenContext* context, Node* node) {
	bool save_bool = false;
	if (context->isSave()) {
		save_bool = true;
		context->setIsSave(false);
	}

	LValue value = node->codeGen(context);

	std::vector<LValue> args;
	for (Node* p = node->getNext(); p != NULL; p = p->getNext()) {
		LValue v = p->codeGen(context);
		if (v != NULL) {
			args.push_back(v);
		}
	}

	LValue ptr = context->getLLCG()->Select(value, args);
	if (save_bool) return ptr;
	return context->getLLCG()->Load(ptr);
}

static LValue call_macro(CodeGenContext* context, Node* node) {
	// 参数一 函数名
	LValue func = context->getFunction(node);
	if (func == NULL) {
		cerr << "找不到函数的定义: ";
		cerr << node->getStr() << endl;
		exit(1);
	}

	// 其余参数 要传入的参数
	std::vector<LValue> args;
	for (Node* p = node->getNext(); p != NULL; p = p->getNext()) {
		LValue v = p->codeGen(context);
		if (v != NULL)
			args.push_back(v);
	}

	return context->getLLCG()->Call(func, args);
}

static LValue for_macro(CodeGenContext* context, Node* node) {
	context->st->push();
	// 参数一 初始化
	LValue init_block = context->getLLCG()->GetNowBasicBlock();
	context->MacroMake(node);

	// 参数二 终止条件
	node = node->getNext();
	LValue end_block = context->getLLCG()->CreateBasicBlock();
	LValue condition = context->MacroMake(node);

	// 参数三 每次循环
	node = node->getNext();
	LValue do_block = context->getLLCG()->CreateBasicBlock();
	context->MacroMake(node);

	// 参数四 循环体
	node = node->getNext();
	LValue work_block = context->getLLCG()->CreateBasicBlock();

	LValue false_block = context->getLLCG()->CreateBasicBlock();

	// 生成break和continue标识
	context->st->insert("break", conditions_t, false_block);
	context->st->insert("continue", conditions_t, end_block);

	 // 为了在生成代码前先记录break时要跳出的位置, 必须先建好各个BasicBlock, 最后再设定跳转关系
	context->getLLCG()->SetNowBasicBlock(work_block);
	context->MacroMake(node);
	LValue work_end_block = context->getLLCG()->GetNowBasicBlock();
	context->getLLCG()->SetNowBasicBlock(false_block);
	// 生成for循环
	context->getLLCG()->For(condition, init_block, end_block, do_block, work_block, work_end_block, false_block);

	context->st->pop();
	return NULL;
}

static LValue while_macro(CodeGenContext* context, Node* node) {

	// 参数一 条件
	LValue father_block = context->getLLCG()->GetNowBasicBlock();
	LValue pd_block     = context->getLLCG()->CreateBasicBlock();
	LValue condition = context->MacroMake(node);

	// 参数二 循环体
	node = node->getNext();
	LValue true_block = context->getLLCG()->CreateBasicBlock();
	LValue false_block = context->getLLCG()->CreateBasicBlock();

	context->st->push();
	context->getLLCG()->SetNowBasicBlock(true_block);
	context->MacroMake(node);
	context->st->pop();
	LValue work_end_block = context->getLLCG()->GetNowBasicBlock();

	context->getLLCG()->SetNowBasicBlock(false_block);

	// 生成while循环
	context->getLLCG()->While(condition, father_block, pd_block, true_block, work_end_block, false_block);
	return NULL;
}

static LValue if_macro(CodeGenContext* context, Node* node) {

	// 参数一 条件
	LValue condition = context->MacroMake(node);
	LValue father_block = context->getLLCG()->GetNowBasicBlock();

	// 参数二 为真时, 跳转到的Label
	node = node->getNext();

	LValue true_block = context->getLLCG()->CreateBasicBlock();
	context->st->push();
	context->MacroMake(node);
	context->st->pop();
	LValue true_end_block = context->getLLCG()->GetNowBasicBlock();

	// 参数三 为假时, 跳转到的Label
	node = node->getNext();
	LValue false_block = context->getLLCG()->CreateBasicBlock();
	if (node != NULL) {
		context->st->push();
		context->MacroMake(node);
		context->st->pop();
	}
	LValue false_end_block = context->getLLCG()->GetNowBasicBlock();


	context->getLLCG()->If(condition, father_block, true_block,
		true_end_block, false_block, false_end_block, node != NULL);
	return NULL;
}


static LValue opt1_macro(CodeGenContext* context, Node* node) {
	std::string opt = node->getStr();
	node = node->getNext();
	if (node == NULL) return NULL;
	context->setIsSave(true); // 这两句设置的目前是为下面的节点解析时,返回指针而不是load后的值
	LValue ans = node->codeGen(context);
	context->setIsSave(false);
	return context->getLLCG()->Opt1(opt, ans);
}

static LValue opt2_macro(CodeGenContext* context, Node* node) {
	std::string opt = node->getStr();

	Node* op1 = (node = node->getNext());
	if (node == NULL) return NULL;
	Node* op2 = (node = node->getNext());
	if (node == NULL) return NULL;

	if (opt == "=" || opt == "+=" || opt == "-=" || opt == "&=" || opt == "|=" || opt == "^=") {
		context->setIsSave(true); // 这两句设置的目前是为下面的节点解析时,返回指针而不是load后的值
		LValue ans1 = op1->codeGen(context);
		context->setIsSave(false);
		LValue ans2 = op2->codeGen(context);
		return context->getLLCG()->Assignment(opt, ans1, ans2);
	}

	if (opt == ".") {
		bool save_bool = false;
		if (context->isSave()) {
			save_bool = true;
			context->setIsSave(false);
		}

		LValue ans1 = op1->codeGen(context);
		string ans2 = op2->getStr();
		string struct_name = ans1->getTypeName();
		shared_ptr<StructModel> sm_ptr = context->getStructModel(struct_name);
		int n = sm_ptr->find(ans2);

		LValue ptr = context->getLLCG()->Dot(ans1, n);
		if (save_bool) return ptr;
		return context->getLLCG()->Load(ptr);
	}

	LValue ans1 = op1->codeGen(context);
	LValue ans2 = op2->codeGen(context);
	if (opt == "+"  || opt == "-" || opt == "*" || opt == "*" || opt == "/" || opt == "<<" ||
		opt == ">>" || opt == "&" || opt == "|" || opt == "^" || opt == "%") {
		return context->getLLCG()->Opt2(opt, ans1, ans2);
	}

	if (opt == "==" || opt == "!=" || opt == "<=" || opt == ">=" || opt == "<" || opt == ">") {
		return context->getLLCG()->Cmp(opt, ans1, ans2);
	}
	return NULL;
}

static LValue struct_macro(CodeGenContext* context, Node* node) {
	std::string struct_name = node->getStr();
	shared_ptr<StructModel> sm = context->getStructModel(struct_name);
	sm->genMetaCode(context);
	return NULL;
}

static LValue return_macro(CodeGenContext* context, Node* node) {
	if (node != NULL) {
		LValue v = node->codeGen(context);
		return context->getLLCG()->Return(v);
	}
	return context->getLLCG()->Return();
}

static LValue new_macro(CodeGenContext* context, Node* node) {
	TypeNode* tn = (TypeNode*) node;
	LValue t = context->FindSrcType(tn->getTypeName());

	// 第二个参数，构造函数表
	node = node->getNext();

	// 第三个参数，维度信息
	vector<LValue> args;
	node = node->getNext();
	if (node != NULL)
		for (Node* p = node->getChild(); p != NULL; p = p->getNext()) {
			args.push_back(p->codeGen(context));
		}
	if (args.size() == 0) {
		return context->getLLCG()->New(t,args);
	} else {
		return context->getLLCG()->NewArray(t, args);
	}
}

static LValue delete_macro(CodeGenContext* context, Node* node) {
	std::string str = node->getStr();

	return NULL;
}

static LValue delete_array_macro(CodeGenContext* context, Node* node) {
	return NULL;
}

static LValue break_macro(CodeGenContext* context, Node* node) {
	printf("break macro\n");
	id* d = context->st->find("break");
	if (d == NULL) {
		printf("错误的break语句使用环境\n");
		exit(1);
	}
	context->getLLCG()->Goto(d->data);
	return NULL;
}

static LValue continue_macro(CodeGenContext* context, Node* node) {
	printf("continue macro\n");
	id* d = context->st->find("continue");
	if (d == NULL) {
		printf("错误的continue语句使用环境\n");
		exit(1);
	}
	context->getLLCG()->Goto(d->data);
	return NULL;
}




/*
extern Node* parseFile(const char* path);

static LValue import_macro(CodeGenContext* context, Node* node) {
	string file_name = node->getStr();
	context->SaveMacros();
	context->ScanOther(parseFile(file_name.c_str()));
	context->RecoverMacros();
	return NULL;
}
*/
extern const FuncReg macro_funcs[] = {
	{"function", function_macro},
	{"struct",   struct_macro},
	{"set",      set_macro},
	{"call",     call_macro},
	{"select",   select_macro},
	{"opt1",     opt1_macro},
	{"opt2",     opt2_macro},
	{"for",      for_macro},
	{"while",    while_macro},
	{"if",       if_macro},
	{"return",   return_macro},
	{"new",      new_macro},
	{"delete",   delete_macro},
	{"delete[]", delete_array_macro},
	{"break",    break_macro},
	{"continue", continue_macro},
	//{"import",   import_macro}, // 实验型导入功能,最后应从库中删除
	{NULL, NULL}
};
