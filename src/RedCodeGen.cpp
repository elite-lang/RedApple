/* 
* @Author: sxf
* @Date:   2015-09-23 22:57:41
* @Last Modified by:   sxf
* @Last Modified time: 2015-11-25 22:03:20
*/

#include "RedCodeGen.h"
#include <string>
#include <map>
#include <vector>
using namespace std;

#include "CodeGenContext.h"

RedCodeGen::RedCodeGen() {

}

RedCodeGen::RedCodeGen(Node* node) {
    // 创建一个上下文类
    Init(node);
}

RedCodeGen* RedCodeGen::Create() {
    return new RedCodeGen();
}

RedCodeGen* RedCodeGen::Create(Node* node) {
    return new RedCodeGen(node);
}


void RedCodeGen::Init(Node* node) {
    if (this->context != NULL) {
        delete context;
        context = NULL;
    }
    this->context = new CodeGenContext(node); 
}

RedCodeGen::~RedCodeGen() {
    if (context != NULL) delete context;
}


void RedCodeGen::PreScan(Node* node) {
    context->ScanOther(node);
}


void RedCodeGen::MakeMeta(const char* outfile_name, const char* module_name) {
    string out_name = outfile_name;
    string mod_name = module_name;
    context->getLLCG()->MakeMetaModule(out_name, mod_name);
}


void RedCodeGen::Make(Node* node, const char* outfile_name, const char* module_name) {
    string mod_name = module_name;
    context->getLLCG()->BeginModule(mod_name);

    // 正式流程
    context->Init();
	context->MacroMakeAll(node);
    string out_name = outfile_name;
    context->getLLCG()->VerifyAndWrite(out_name);
}
