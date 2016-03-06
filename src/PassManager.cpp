
#include "PassManager.h"
#include "ICodeGenContext.h"


PassManager::PassManager () {

}

PassManager::~PassManager () {

}


void PassManager::NewPassList(const string& name, const vector<Pass*>& vec) {
    NewPassList(name, list<Pass*>(vec.begin(), vec.end()));
}


void PassManager::NewPassList(const string& name, const list<Pass*>& lst) {
    pass_lists[name] = lst;
}


list<Pass*>* PassManager::getPassList(const string& name) {
    auto idx = pass_lists.find(name);
    if (idx != pass_lists.end()) return &(idx->second);
    return NULL;
}


void PassManager::RunPassList(const string& name, Node* node, ICodeGenContext* ctx) {
    auto idx = pass_lists.find(name);
    if (idx != pass_lists.end()) {
        for (auto i : idx->second) {
            // 设置i为活动pass
            ctx->setNowPass(i);
            ctx->MacroMake(node);
        }
    }
}

void PassManager::RunPassListWithSet(const string& name, set<Node*>& nodes, ICodeGenContext* ctx) {
    auto idx = pass_lists.find(name);
    if (idx != pass_lists.end()) {
        for (auto i : idx->second) {
            // 设置i为活动pass
            ctx->setNowPass(i);
            for (auto node : nodes)
                ctx->MacroMake(node);
        }
    }
}


extern const FuncReg macro_funcs[];
extern const FuncReg macro_classes[];
extern const FuncReg macro_prescan[];
extern const FuncReg macro_pretype[];
extern const FuncReg macro_defmacro[];


void PassManager::LoadDefaultLists() {
    list<Pass*> prescan = { new Pass(macro_defmacro), new Pass(macro_prescan), new Pass(macro_pretype) };
    list<Pass*> main = { new Pass(macro_funcs, macro_classes) };
    NewPassList("prescan", prescan);
    NewPassList("main", main);
}
