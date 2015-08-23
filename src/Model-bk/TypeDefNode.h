/**
 * Project Untitled
 */

#include<string>
using namespace std;

#ifndef _TYPEDEFNODE_H
#define _TYPEDEFNODE_H

#include "StatementNode.h"


class TypeDefNode: public StatementNode {
public: 
    string type_name;

    void generateCode() {

    }
};

#endif //_TYPEDEFNODE_H
