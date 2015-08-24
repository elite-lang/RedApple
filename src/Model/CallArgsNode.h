/* 
* @Author: sxf
* @Date:   2015-08-24 19:14:42
* @Last Modified by:   sxf
* @Last Modified time: 2015-08-24 19:19:22
*/

#ifndef CALL_ARGS_NODE_H
#define CALL_ARGS_NODE_H

#include "ExpressionNode.h"
#include <string>

using namespace std;

class CallArgsNode : public ExpressionNode
{
public:
	CallArgsNode(ExpressionNode* e);
	CallArgsNode(string name, ExpressionNode* e);
	
};


#endif // CALL_ARGS_NODE_H
