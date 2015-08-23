/**
 * Project Untitled
 */


#ifndef _VARDEFNODE_H
#define _VARDEFNODE_H

#include "StatementNode.h"
#include <string>

using namespace std;

class VarDefNode: public StatementNode {
public: 
	string type_name;
};

#endif //_VARDEFNODE_H