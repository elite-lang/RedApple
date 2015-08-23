#include <deque>
#include "idmap.h"
using namespace std;

#ifndef IDTABLE_H
#define IDTABLE_H

class IDTable
{
public:
    IDTable();
    const id* find(string) const;
    void insert(string,id*);
    void push();
    void pop();
    int getLevel();
private:
    deque<IDMap> ID_stack;
};

#endif // IDTABLE_H
