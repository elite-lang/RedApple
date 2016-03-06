/*
* @Author: sxf
* @Date:   2015-11-14 14:33:49
* @Last Modified by:   sxf
* @Last Modified time: 2015-12-24 09:46:33
*/

#include "MetaModel/MetaModel.h"
#include "CodeGenContext.h"

MetaModel::MetaModel(std::string name) {
	this->name = name;
}

MetaModel::~MetaModel() {

}

MetaModel* MetaModel::readJson() {
	return NULL;
}

MetaModel* MetaModel::readMetaCode() {
	return NULL;
}
