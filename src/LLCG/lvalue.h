/* 
* @Author: sxf
* @Date:   2015-11-23 10:54:27
* @Last Modified by:   sxf
* @Last Modified time: 2015-11-25 16:41:47
*/


#ifndef LVALUE_H
#define LVALUE_H

#include <memory>
using namespace std;

class lvalue;
typedef shared_ptr<lvalue> LValue;

/**
 * @brief 代替LLVM中Value的类，可供返回部分自定义数据，例如整数有无符号等信息
 */
class lvalue
{
public:

	virtual bool isStructType(){};

	// 当前为值时，获取当前的类型
	virtual LValue getType(){};

	// 当前为值或类型,都能获得对应的名称
	virtual string getTypeName(){};

	// 当前为类型时, 返回一个新的指针类型
	virtual LValue getPointerTo(){};
};



#endif // LVALUE_H
