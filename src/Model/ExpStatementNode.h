/* 
* @Author: sxf
* @Date:   2015-08-24 23:52:22
* @Last Modified by:   sxf
* @Last Modified time: 2015-08-24 23:54:37
*/

#ifndef EXP_STATEMENT_NODE_H
#define EXP_STATEMENT_NODE_H

#include "StatementNode.h"
#include "ExpressionNode.h"

class ExpStatementNode : public StatementNode
{
public:
	ExpStatementNode(ExpressionNode*);
	
};


#endif // EXP_STATEMENT_NODE_H
