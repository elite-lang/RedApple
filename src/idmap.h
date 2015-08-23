#include <map>

using namespace std;

#ifndef IDMAP_H
#define IDMAP_H

struct id {
    int level;
    int kind;
    int type;
    int lua_data;
    int address;
};


class IDMap
{
public:
    IDMap();
    ~IDMap();
    const id* find(string& str) const;
    void insert(string str,id* pid);
private:
    map<string,id*> ID_map;
};

#endif // IDMAP_H
