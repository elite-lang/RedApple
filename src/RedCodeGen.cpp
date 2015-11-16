/* 
* @Author: sxf
* @Date:   2015-09-23 22:57:41
* @Last Modified by:   sxf
* @Last Modified time: 2015-11-16 15:56:44
*/

#include "RedCodeGen.h"
#include <string>
#include <map>
#include <vector>
using namespace std;

#include "CodeGenContext.h"

RedCodeGen::RedCodeGen() {

}

RedCodeGen::RedCodeGen(Node* node) {
    // 创建一个上下文类
    Init(node);
}

RedCodeGen* RedCodeGen::Create() {
    return new RedCodeGen();
}

RedCodeGen* RedCodeGen::Create(Node* node) {
    return new RedCodeGen(node);
}


void RedCodeGen::Init(Node* node) {
    if (this->context != NULL) {
        delete context;
        context = NULL;
    }
    this->context = new CodeGenContext(node); 
    Module* M = new Module("", *(context->getContext()));
    register_elite_meta_function(M);
    register_elite_meta_struct(M);
    register_elite_meta_init(M);
    this->context->setMetaModule(M);
}

RedCodeGen::~RedCodeGen() {
    if (context != NULL) delete context;
}



void RedCodeGen::PreScan(Node* node) {
    context->ScanOther(node);
}

void RedCodeGen::MakeMeta(const char* outfile_name, const char* module_name) {
    Module* M = context->getMetaModule();
    M->setModuleIdentifier(module_name);

    Function *F = M->getFunction("elite_meta_init");
    BasicBlock& bb = F->getEntryBlock(); // 仅有一个BasicBlock,所以这样写可以
    if (bb.getTerminator() == NULL) // 处理函数结尾返回
        ReturnInst::Create(*(context->getContext()), &bb);
    Type* function_ptr = F->getFunctionType()->getPointerTo(); // 获取函数指针类型

    vector<Type*> type_list;
    type_list.push_back(Type::getInt32Ty(M->getContext()));
    type_list.push_back(function_ptr);
    type_list.push_back(Type::getInt8PtrTy(M->getContext()));

    vector<Constant*> init_list;
    init_list.push_back(ConstantInt::get(Type::getInt32Ty(M->getContext()), 65535));
    init_list.push_back(ConstantExpr::getBitCast(F, function_ptr));
    init_list.push_back(Constant::getNullValue(Type::getInt8PtrTy(M->getContext())));

    StructType* st = StructType::create(type_list, "");
    ArrayType* at = ArrayType::get(st, 1);
    vector<Constant*> array_init_list;
    array_init_list.push_back(ConstantStruct::get(st, init_list));
    Constant* init_var = ConstantArray::get(at, array_init_list);

    GlobalVariable* gv = new GlobalVariable(
        *M, at, false, GlobalValue::AppendingLinkage, init_var, "llvm.global_ctors"
    );

    VerifyAndWrite(M, outfile_name);
}


void RedCodeGen::Make(Node* node, const char* outfile_name, const char* module_name) {
	// Create some module to put our function into it.
	std::unique_ptr<Module> M(new Module(module_name, *(context->getContext())));

	context->setModule(M.get());
    // register_malloc(M.get());
    register_printf(M.get());

    // 正式流程
    context->Init();
	context->MacroMake(node);

    VerifyAndWrite(M.get(), outfile_name);
}

void RedCodeGen::VerifyAndWrite(llvm::Module* M, const char* outfile_name) {
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
    WriteBitcodeToFile(M, *out);
    out->flush(); delete out;
}

void RedCodeGen::register_printf(llvm::Module *module) {
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

void RedCodeGen::register_malloc(llvm::Module *module) {
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

void RedCodeGen::register_echo(CodeGenContext* context, llvm::Function* printfFn)
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

void RedCodeGen::register_elite_meta_function(Module *module) {
    std::vector<Type*> arg_types;
    arg_types.push_back(Type::getInt8PtrTy(module->getContext()));
    arg_types.push_back(Type::getInt8PtrTy(module->getContext()));

    FunctionType* meta_type =
        FunctionType::get(
            Type::getVoidTy(module->getContext()), arg_types, true);

    Function *func = Function::Create(
                meta_type, Function::ExternalLinkage,
                Twine("elite_meta_function"),
                module
           );
    func->setCallingConv(CallingConv::C);
}

void RedCodeGen::register_elite_meta_struct(Module *module) {
    std::vector<Type*> arg_types;
    arg_types.push_back(Type::getInt8PtrTy(module->getContext()));

    FunctionType* meta_type =
        FunctionType::get(
            Type::getVoidTy(module->getContext()), arg_types, true);

    Function *func = Function::Create(
                meta_type, Function::ExternalLinkage,
                Twine("elite_meta_struct"),
                module
           );
    func->setCallingConv(CallingConv::C);
}


void RedCodeGen::register_elite_meta_init(Module *module) {
    std::vector<Type*> arg_types;

    FunctionType* meta_type =
        FunctionType::get(
            Type::getVoidTy(module->getContext()), arg_types, false);

    Function *func = Function::Create(
                meta_type, Function::InternalLinkage,
                Twine("elite_meta_init"),
                module
           );
    func->setCallingConv(CallingConv::C);

    BasicBlock::Create(module->getContext(), "entry", func);
}