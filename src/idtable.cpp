#include "idtable.h"

IDTable::IDTable()
{
    ID_stack.push_back(IDMap());
}

const id* IDTable::find(string idname) const {
    for (auto p = ID_stack.rbegin(); p != ID_stack.rend(); ++p) {
        const IDMap& imap = *p;
        const id* pid = imap.find(idname);
        if (pid != NULL) return pid;
    }
    return NULL;
}

void IDTable::insert(string str,id* pid) {
    IDMap& imap = ID_stack.back();
    imap.insert(str,pid);
}

void IDTable::push() {
    ID_stack.push_back(IDMap());
}

void IDTable::pop() {
    ID_stack.pop_back();
}

int IDTable::getLevel() {
    return ID_stack.size()-1;
}
