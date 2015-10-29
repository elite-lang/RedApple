#include <map>

using namespace std;

#ifndef IDMAP_H
#define IDMAP_H

struct id {
    int   level;
    int   type;
    void* data;
};

class IDMap
{
public:
    IDMap();
    ~IDMap();
    id* find(string& str) const;
    void insert(string& str, int level, int type, void* data);
private:
    map<string,id*> ID_map;
};

#endif // IDMAP_H
