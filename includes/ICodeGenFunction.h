/* 
* @Author: sxf
* @Date:   2015-12-24 08:56:30
* @Last Modified by:   sxf
* @Last Modified time: 2015-12-24 08:58:15
*/

#ifndef I_CODE_GEN_FUNCTION_H
#define I_CODE_GEN_FUNCTION_H

#include "Model/Node.h"
#include "LLCG/llcg.h"
class CodeGenContext;

class ICodeGenFunction
{
public:
	virtual LValue call(CodeGenContext*, Node*) = 0;	
};


#endif // I_CODE_GEN_FUNCTION_H
