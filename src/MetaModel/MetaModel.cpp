/* 
* @Author: sxf
* @Date:   2015-11-14 14:33:49
* @Last Modified by:   sxf
* @Last Modified time: 2015-11-16 16:59:42
*/

#include "MetaModel.h"
#include "CodeGenContext.h"

MetaModel::MetaModel(std::string name) {
	this->name = name;
}

MetaModel::~MetaModel() {

}

MetaModel* MetaModel::readJson(cJSON* J) {

}

MetaModel* MetaModel::readMetaCode(Module* M) {

}


Constant* MetaModel::geti8StrVal(Module& M, char const* str) {
    LLVMContext& ctx = M.getContext(); // 千万别用Global Context
    Constant* strConstant = ConstantDataArray::getString(ctx, str);
    GlobalVariable* GVStr =
        new GlobalVariable(M, strConstant->getType(), true,
                           GlobalValue::InternalLinkage, strConstant, "");
    Constant* zero = Constant::getNullValue(IntegerType::getInt32Ty(ctx));
    Constant* indices[] = {zero, zero};
    Constant* strVal = ConstantExpr::getGetElementPtr(GVStr, indices, true);
    return strVal;
}
