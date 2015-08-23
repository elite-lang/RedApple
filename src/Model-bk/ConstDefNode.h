/**
 * Project Untitled
 */

#include<string>
using namespace std;

#ifndef _CONSTDEFNODE_H
#define _CONSTDEFNODE_H

#include "StatementNode.h"


class ConstDefNode: public StatementNode {
public: 
    string type_name;

    void generateCode() {

    }
};

#endif //_CONSTDEFNODE_H
