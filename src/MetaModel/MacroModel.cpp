/*
* @Author: sxf
* @Date:   2015-11-25 15:02:32
* @Last Modified by:   sxf
* @Last Modified time: 2015-12-24 09:46:30
*/

#include "MetaModel/MacroModel.h"

MacroModel::MacroModel(const std::string& name, Node* node) : MetaModel(name) {
	this->node = node;
}

MacroModel::~MacroModel() {

}


void MacroModel::insertToST(CodeGenContext* context) {

}

void MacroModel::genCode(CodeGenContext* context) {

}


void MacroModel::genMetaCode(CodeGenContext* context) {

}

MetaType MacroModel::getMetaType() {
	return macro_meta_t;
}
