/* 
* @Author: sxf
* @Date:   2015-10-12 19:21:50
* @Last Modified by:   sxf
* @Last Modified time: 2015-10-31 21:09:13
*/

#include "CodeGenContext.h"
#include "nodes.h"

Value* Node::codeGen(CodeGenContext* context) {
	return context->MacroMake(this);
}

Value* IntNode::codeGen(CodeGenContext* context) {
    Type* t = Type::getInt64Ty(*(context->getContext()));
    setLLVMType(t);
	return ConstantInt::get(t, value);
}

Value* FloatNode::codeGen(CodeGenContext* context) {
    Type* t = Type::getFloatTy(*(context->getContext()));
    setLLVMType(t);
	return ConstantFP::get(t, value);
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
    Module* M = context->getModule();
    LLVMContext& ctx = M->getContext(); // 千万别用Global Context
    Constant* strConstant = ConstantDataArray::getString(ctx, value);
    Type* t = strConstant->getType();
    setLLVMType(t);
    GlobalVariable* GVStr = new GlobalVariable(*M, t, true,
                            GlobalValue::InternalLinkage, strConstant, "");
    Constant* zero = Constant::getNullValue(IntegerType::getInt32Ty(ctx));
    Constant* indices[] = {zero, zero};
    Constant* strVal = ConstantExpr::getGetElementPtr(GVStr, indices, true);
	return strVal;
}

Value* IDNode::codeGen(CodeGenContext* context) {
    BasicBlock* bb = context->getNowBlock();
    ValueSymbolTable* st = bb->getValueSymbolTable();
    Value* v = st->lookup(value);
    setLLVMType(v->getType());
    if (v->hasName() == false) {
        errs() << "undeclared variable " << value << "\n";
        return NULL;
    }
    if (context->isSave()) return v;
    // if (v->getType()->getPointerElementType()->isStructTy()) return v;
    Value* load = new LoadInst(v, "", false, bb);
    return load;
}


