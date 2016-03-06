/*
* @Author: sxf
* @Date:   2015-09-23 22:57:41
* @Last Modified by:   sxf
* @Last Modified time: 2015-11-25 22:03:20
*/

#include "RedCodeGen.h"
#include "idtable.h"
#include <string>
#include <map>
#include <vector>
#include <set>
using namespace std;

#include "CodeGenContext.h"

RedCodeGen::RedCodeGen() {

}

RedCodeGen* RedCodeGen::Create() {
    return new RedCodeGen();
}

void RedCodeGen::Init() {
    if (this->context != NULL) {
        delete context;
        context = NULL;
    }
    this->context = new CodeGenContext();
    pm.LoadDefaultLists();
    this->context->setPassManager(&pm);
}

RedCodeGen::~RedCodeGen() {
    if (context != NULL) delete context;
}


void RedCodeGen::PreScan(Node* node) {
    pm.RunPassList("prescan", node, context);
}

void RedCodeGen::PreScan(std::set<Node*>& nodes) {
    pm.RunPassListWithSet("prescan", nodes, context);
}


void RedCodeGen::MakeMeta(const char* outfile_name, const char* module_name) {
    string out_name = outfile_name;
    string mod_name = module_name;
    context->getLLCG()->MakeMetaModule(out_name, mod_name);
}


void RedCodeGen::Make(Node* node, const char* outfile_name, const char* module_name) {
    string mod_name = module_name;
    context->st->push();  // 进入文件局部符号表
    context->Init();
    context->getLLCG()->BeginModule(mod_name);
    // 正式流程
    pm.RunPassList("main", node, context);
    string out_name = outfile_name;
    context->getLLCG()->VerifyAndWrite(out_name);
    context->st->pop();   // 离开该文件符号表
}
