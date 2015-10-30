/* 
* @Author: sxf
* @Date:   2015-09-23 22:57:41
* @Last Modified by:   sxf
* @Last Modified time: 2015-10-30 12:38:41
*/

#include "CodeGen.h"
#include <string>
#include <map>
#include <vector>
using namespace std;

#include "CodeGenContext.h"

CodeGen::CodeGen(Node* node) {
    // 创建一个上下文类
    
    context = new CodeGenContext(node); 
}

CodeGen::~CodeGen() {
    if (context != NULL) delete context;
}

void register_printf(llvm::Module *module);
void register_malloc(llvm::Module *module);
void register_echo(CodeGenContext* context, llvm::Function* printfFn);



void CodeGen::Make(const char* outfile_name) {
	InitializeNativeTarget();
	
    LLVMContext Context;
	// Create some module to put our function into it.
	std::unique_ptr<Module> M(new Module("main", Context));

	context->setModule(M.get());
	context->setContext(&Context);
    register_malloc(M.get());
    register_printf(M.get());

    // prescan流程, 负责优先解析全局函数和类型并存放于符号表
    context->PreInit();
    context->MakeBegin();
    printf("-- 预处理流程完成 --\n");

    // pretype流程，负责处理所有类型相关问题并更新符号表
    context->PreTypeInit();
    context->MakeBegin();
    printf("-- 类型化流程完成 --\n");

    // 正式流程初始化
    context->Init();
	
	// register_echo(context, M->getFunction("printf"));
	context->MakeBegin();

	// 校验问题, 这个函数需要一个输出流来打印错误信息
	if (verifyModule(*M, &errs())) {
		errs() << "构建LLVM字节码出错!\n";
		exit(1);
	}

    // 输出编译后的LLVM可读字节码
    outs() << "LLVM module:\n\n" << *M;
    outs() << "\n\n";
    outs().flush();

	// 输出二进制BitCode到.bc文件
	std::error_code ErrInfo;
	raw_ostream *out = new raw_fd_ostream(outfile_name, ErrInfo, sys::fs::F_None);
	WriteBitcodeToFile(M.get(), *out);
	out->flush(); delete out;
	llvm_shutdown();
}

void register_printf(llvm::Module *module) {
    std::vector<llvm::Type*> printf_arg_types;
    printf_arg_types.push_back(llvm::Type::getInt8PtrTy(module->getContext()));

    llvm::FunctionType* printf_type =
        llvm::FunctionType::get(
            llvm::Type::getInt32Ty(module->getContext()), printf_arg_types, true);

    llvm::Function *func = llvm::Function::Create(
                printf_type, llvm::Function::ExternalLinkage,
                llvm::Twine("printf"),
                module
           );
    func->setCallingConv(llvm::CallingConv::C);
}

void register_malloc(llvm::Module *module) {
    std::vector<llvm::Type*> arg_types;
    arg_types.push_back(llvm::Type::getInt64Ty(module->getContext()));

    llvm::FunctionType* printf_type =
        llvm::FunctionType::get(
            llvm::Type::getVoidTy(module->getContext())->getPointerTo(), arg_types, false);

    llvm::Function *func = llvm::Function::Create(
                printf_type, llvm::Function::ExternalLinkage,
                llvm::Twine("malloc"),
                module
           );
    func->setCallingConv(llvm::CallingConv::C);
}

void register_echo(CodeGenContext* context, llvm::Function* printfFn)
{
    std::vector<llvm::Type*> echo_arg_types;
    echo_arg_types.push_back(llvm::Type::getInt64Ty(*(context->getContext())));

    llvm::FunctionType* echo_type =
        llvm::FunctionType::get(
            llvm::Type::getVoidTy(*(context->getContext())), echo_arg_types, false);

    llvm::Function *func = llvm::Function::Create(
                echo_type, llvm::Function::InternalLinkage,
                llvm::Twine("echo"),
                context->getModule()
           );
    
    llvm::BasicBlock *bblock = context->createBlock(func);
    const char *constValue = "%d\n";
    llvm::Constant *format_const = llvm::ConstantDataArray::getString(*(context->getContext()), constValue);
    llvm::GlobalVariable *var =
        new llvm::GlobalVariable(
            *(context->getModule()), llvm::ArrayType::get(llvm::IntegerType::get(*(context->getContext()), 8), strlen(constValue)+1),
            true, llvm::GlobalValue::PrivateLinkage, format_const, ".str");
    llvm::Constant *zero =
        llvm::Constant::getNullValue(llvm::IntegerType::getInt32Ty(*(context->getContext())));

    std::vector<llvm::Constant*> indices;
    indices.push_back(zero);
    indices.push_back(zero);
    llvm::Constant *var_ref =
        llvm::ConstantExpr::getGetElementPtr(var, indices);

    std::vector<Value*> args;
    args.push_back(var_ref);

    Function::arg_iterator argsValues = func->arg_begin();
    Value* toPrint = argsValues++;
    toPrint->setName("toPrint");
    args.push_back(toPrint);

	CallInst::Create(printfFn, makeArrayRef(args), "", bblock);
	ReturnInst::Create(*(context->getContext()), bblock);
}
