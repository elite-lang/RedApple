/* 
* @Author: sxf
* @Date:   2015-09-22 21:27:42
* @Last Modified by:   sxf
* @Last Modified time: 2015-09-22 21:52:14
*/


#ifndef SYMBOL_H
#define SYMBOL_H

#include <string>

using namespace std;

class Symbol : public Node
{
public:
	Symbol();
	~Symbol();
	
	string name;
};


#endif // SYMBOL_H
