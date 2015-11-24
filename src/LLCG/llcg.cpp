/* 
* @Author: sxf
* @Date:   2015-11-23 21:43:28
* @Last Modified by:   sxf
* @Last Modified time: 2015-11-24 11:01:39
*/

#include "llcg.h"
#include "llcg_llvm.h"

llcg* llcg::CreateLLVM() {
	return new llcg_llvm();
}
