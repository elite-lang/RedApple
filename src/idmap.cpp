#include "idmap.h"

IDMap::IDMap()
{
}

IDMap::~IDMap()
{
    for (auto p : ID_map) {
        id* pid = p.second;
        delete pid;
    }
}

const id* IDMap::find(string& str) const {
    auto p = ID_map.find(str);
    if (p != ID_map.end())
        return (*p).second;
    return NULL;
}

void IDMap::insert(string str,id* pid) {
    ID_map.insert(make_pair(str,pid));
}
