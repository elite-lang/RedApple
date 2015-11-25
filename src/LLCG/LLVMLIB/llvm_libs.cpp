/* 
* @Author: sxf
* @Date:   2015-11-24 10:21:35
* @Last Modified by:   sxf
* @Last Modified time: 2015-11-25 21:41:38
*/

#include "llcg_llvm.h"


static void register_printf(llvm::Module *module) {
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

static void register_functioncall(llvm::Module *module) {
    std::vector<llvm::Type*> arg_types;
    arg_types.push_back(llvm::Type::getInt8PtrTy(module->getContext()));

    llvm::FunctionType* func_type =
        llvm::FunctionType::get(
            llvm::Type::getVoidTy(module->getContext())->getPointerTo(), arg_types, true);

    llvm::Function *func = llvm::Function::Create(
                func_type, llvm::Function::ExternalLinkage,
                llvm::Twine("FunctionCall"),
                module
           );
    func->setCallingConv(llvm::CallingConv::C);
}


static void register_elite_meta_function(Module *module) {
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

static void register_elite_meta_list(Module *module) {
    std::vector<Type*> arg_types;
    arg_types.push_back(Type::getInt8PtrTy(module->getContext()));

    FunctionType* meta_type =
        FunctionType::get(
            Type::getVoidTy(module->getContext()), arg_types, true);

    Function *func = Function::Create(
                meta_type, Function::ExternalLinkage,
                Twine("elite_meta_list"),
                module
           );
    func->setCallingConv(CallingConv::C);
}


static void register_elite_meta_init(Module *module) {
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

static void register_malloc_array(Module *module) {
    std::vector<Type*> arg_types;
    arg_types.push_back(Type::getInt64Ty(module->getContext()));

    FunctionType* meta_type =
        FunctionType::get(
            Type::getVoidTy(module->getContext())->getPointerTo(), arg_types, true);

    Function *func = Function::Create(
                meta_type, Function::ExternalLinkage,
                Twine("malloc_array"),
                module
           );
    func->setCallingConv(CallingConv::C);
}

extern const LibFunc stdlibs[] = {
	register_printf,
	register_functioncall,
	register_malloc_array,
	NULL
};

extern const LibFunc metalibs[] = {
	register_elite_meta_function,
	register_elite_meta_list,
	register_elite_meta_init,
	NULL
};