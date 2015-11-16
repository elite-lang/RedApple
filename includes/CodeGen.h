/* 
* @Author: sxf
* @Date:   2015-11-11 13:57:49
* @Last Modified by:   sxf
* @Last Modified time: 2015-11-16 11:21:16
*/


#ifndef CODE_GEN_H
#define CODE_GEN_H

class Node;

class CodeGen
{
public:
	virtual void Init(Node* node = NULL) = 0;
	virtual void PreScan(Node* node) = 0;
	virtual void Make(Node* node, const char* outfile_name, const char* module_name = "") = 0;
	virtual void MakeMeta(const char* outfile_name, const char* module_name = "") = 0;
};

#endif // CODE_GEN_H
