#include <deque>
#include "idmap.h"
using namespace std;

#ifndef IDTABLE_H
#define IDTABLE_H

class IDTable
{
public:
    IDTable();
    id* find(string& str) const;
    void insert(string& str,SymbolType type, void* data);
    void push();
    void pop();
    int getLevel();
private:
    deque<IDMap> ID_stack;
};

#endif // IDTABLE_H
