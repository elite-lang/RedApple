/* 
* @Author: sxf
* @Date:   2015-11-25 15:02:32
* @Last Modified by:   sxf
* @Last Modified time: 2015-11-25 21:56:53
*/

#include "MacroModel.h"

MacroModel::MacroModel(std::string& name, Node* node) : MetaModel(name) {
	this->node = node;
}

MacroModel::~MacroModel() {

}

	
void MacroModel::insertToST(CodeGenContext* context) {

}

void MacroModel::genCode(CodeGenContext* context) {

}

cJSON* MacroModel::genJson() {

}

void MacroModel::genMetaCode(CodeGenContext* context) {

}

MetaType MacroModel::getMetaType() {
	return macro_meta_t;
}
