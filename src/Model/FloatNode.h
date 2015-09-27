/**
 * Project Untitled
 */


#ifndef _FLOATNODE_H
#define _FLOATNODE_H

#include "Node.h"
#include <string>
using namespace std;

class FloatNode: public Node {
public: 
	float value;

	FloatNode(string num);
};

#endif //_FLOATNODE_H