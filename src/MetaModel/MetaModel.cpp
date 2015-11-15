/* 
* @Author: sxf
* @Date:   2015-11-14 14:33:49
* @Last Modified by:   sxf
* @Last Modified time: 2015-11-15 15:44:24
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


