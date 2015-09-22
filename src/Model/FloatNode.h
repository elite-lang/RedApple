/**
 * Project Untitled
 */


#ifndef _FLOATNODE_H
#define _FLOATNODE_H

#include "ValueNode.h"
#include <string>
using namespace std;

class FloatNode: public ValueNode {
public: 
	float value;

	FloatNode(string num);
};

#endif //_FLOATNODE_H