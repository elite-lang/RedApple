/* 
* @Author: sxf
* @Date:   2015-09-23 22:57:41
* @Last Modified by:   sxf
* @Last Modified time: 2015-09-24 20:21:03
*/

#include "CodeGen.h"

#include <string>
#include <map>
#include <vector>
using namespace std;

#include "llvm/IR/Constants.h"
#include "llvm/IR/Verifier.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/ManagedStatic.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/raw_ostream.h"
using namespace llvm;

