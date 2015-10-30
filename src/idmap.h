#include <map>

using namespace std;

#ifndef IDMAP_H
#define IDMAP_H

enum SymbolType
{
	var_t, type_t, struct_t, enum_t, delegate_t, function_t
};

struct id {
    int        level;
    SymbolType type;
    void*      data;
};

class IDMap
{
public:
    IDMap();
    ~IDMap();
    id* find(string& str) const;
    void insert(string& str, int level, SymbolType type, void* data);
private:
    map<string,id*> ID_map;
};

#endif // IDMAP_H
