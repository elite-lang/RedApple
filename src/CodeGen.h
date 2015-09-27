/* 
* @Author: sxf
* @Date:   2015-09-23 22:55:30
* @Last Modified by:   sxf
* @Last Modified time: 2015-09-23 23:07:17
*/

#ifndef CODE_GEN_H
#define CODE_GEN_H

class CodeGen
{
public:
	CodeGen(Node* node);
	~CodeGen();
	
	Value* Make();

private:
	Node* root;
};



#endif // CODE_GEN_H
