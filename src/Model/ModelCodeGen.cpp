/* 
* @Author: sxf
* @Date:   2015-10-12 19:21:50
* @Last Modified by:   sxf
* @Last Modified time: 2015-10-14 16:58:30
*/

#include "CodeGenContext.h"
#include "nodes.h"

#include <exception>

Value* Node::codeGen(CodeGenContext* context) {
	return context->MacroMake(this);
}

Value* IntNode::codeGen(CodeGenContext* context) {
	return ConstantInt::get(Type::getInt64Ty(*(context->getContext())), value);
}

Value* FloatNode::codeGen(CodeGenContext* context) {
	return ConstantFP::get(Type::getFloatTy(*(context->getContext())), value);
}

Constant* geti8StrVal(Module& M, char const* str, Twine const& name) {
    LLVMContext& ctx = M.getContext(); // 千万别用Global Context
    Constant* strConstant = ConstantDataArray::getString(ctx, str);
    GlobalVariable* GVStr =
        new GlobalVariable(M, strConstant->getType(), true,
                           GlobalValue::InternalLinkage, strConstant, name);
    Constant* zero = Constant::getNullValue(IntegerType::getInt32Ty(ctx));
    Constant* indices[] = {zero, zero};
    Constant* strVal = ConstantExpr::getGetElementPtr(GVStr, indices, true);
    return strVal;
}

Value* StringNode::codeGen(CodeGenContext* context) {
	return geti8StrVal(*(context->getModule()), value.c_str(), "");
}

Value* IDNode::codeGen(CodeGenContext* context) {
    BasicBlock* bb = context->getNowBlock();
    ValueSymbolTable* st = bb->getValueSymbolTable();
    Value* v = st->lookup(value);
    if (v->hasName() == false) {
        errs() << "undeclared variable " << value << "\n";
        return NULL;
    }
    Value* load = NULL;
    try {
        load = new LoadInst(v, "", false, bb);
    } catch (std::exception& e) {
        errs() << e.what() << '\n';
    }
    return load;
}


