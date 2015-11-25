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

id* IDMap::find(string& str) const {
    auto p = ID_map.find(str);
    if (p != ID_map.end())
        return (*p).second;
    return NULL;
}

map<string,id*>& IDMap::getAll() {
    return ID_map;
}

void IDMap::insert(string& str, int level, SymbolType type, LValue data) {
	id* i = new id();
	i->level = level;
	i->type = type;
	i->data = data;
    ID_map[str] = i;
}
