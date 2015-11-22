/* 
* @Author: sxf
* @Date:   2015-11-15 10:19:10
* @Last Modified by:   sxf
* @Last Modified time: 2015-11-22 14:40:24
*/

#include <string>
#include <map>
#include <vector>
#include <cstdio>
#include <malloc.h>
#include <cstdarg>
#include "dyncall.h"

using namespace std;

typedef void* (*FuncPtrTy)(...);

struct FunctionMeta {
	string name;
	string ret_type;
	vector<string> type_list;
	vector<string> name_list; 
	void* function_ptr;
};

struct StructMeta {
	string name;
	vector<string> type_list;
	vector<string> name_list; 
};

enum NormalType
{
	void_t, int_t, float_t, double_t, char_t, byte_t, ptr_t, unknown_t
};

class EliteMeta {
public:
	static void RegFunction(string& name, string& ret_type, 
		vector<string>& type_list, vector<string>& name_list, void* fptr);
	static void RegStruct(string& name,
		vector<string>& type_list, vector<string>& name_list);

	static FunctionMeta* getFunction(string& name);
	static StructMeta* getStruct(string& name);
	static NormalType getNormalType(string& name);
	static EliteMeta& getInstance();

	DCCallVM* vm = NULL;
private:
	map<string, FunctionMeta*> func_map;
	map<string, StructMeta*> struct_map;

	EliteMeta() { vm = dcNewCallVM(4096); }
	~EliteMeta() { dcFree(vm); }
};


// 核心反射接口
extern "C" {
	void elite_meta_function(const char* name, const char* ret_type, ...) {
		printf("RegFunction: %s\n", name);
		va_list argp;                   /* 定义保存函数参数的结构 */    
	    int argno = 0;                  /* 纪录参数个数 */    
	    const char *para;                     /* 存放取出的字符串参数 */    
	    vector<string> type_list;
	    vector<string> name_list;    
	    va_start( argp, ret_type );
	    para = va_arg( argp, const char*);  
	    while (para != NULL) {
	        ++argno;
	        if (argno & 1) type_list.push_back(para);
	        else       name_list.push_back(para);
	        para = va_arg( argp, char *);  
	    }    
	    void* fptr = va_arg( argp, void*);  
	    string sname = name;
	    string sret_type = ret_type;
	    EliteMeta::RegFunction(sname, sret_type, type_list, name_list, fptr);
	    va_end( argp );                                   /* 将argp置为NULL */    
	}

	void elite_meta_struct(const char* name, ...) {
		printf("RegStruct: %s\n", name);
		va_list argp;                   /* 定义保存函数参数的结构 */    
	    int argno = 0;                  /* 纪录参数个数 */    
	    const char *para;                     /* 存放取出的字符串参数 */    
	    vector<string> type_list;
	    vector<string> name_list;    
	    va_start( argp, name );
	    para = va_arg( argp, const char*);  
	    while (para != NULL) {
	        ++argno;
	        if (argno & 1) type_list.push_back(para);
	        else       name_list.push_back(para);
	        para = va_arg( argp, char *);  
	    }    
	    string sname = name;
	    EliteMeta::RegStruct(sname, type_list, name_list);
	    va_end( argp );                                   /* 将argp置为NULL */    
	}

	void* FunctionCall(const char* name, ...) {
		string sname = name;
		FunctionMeta* fm = EliteMeta::getFunction(sname);
		va_list argp;                   /* 定义保存函数参数的结构 */    
	    va_start( argp, name );
	    
	    // 设定调用栈
	    DCCallVM* vm = EliteMeta::getInstance().vm;
	    dcMode(vm, DC_CALL_C_DEFAULT);
		dcReset(vm);
		
		for (auto& s : fm->type_list) {
			NormalType t = EliteMeta::getNormalType(s);
			switch (t) {
				case int_t:     dcArgInt(vm, va_arg(argp, int));  break;
				case float_t :  dcArgFloat(vm, (float)va_arg(argp, double)); break; // 默认参数提升
				case double_t : dcArgDouble(vm, va_arg(argp, double)); break;
				case char_t :   dcArgShort(vm, (short)va_arg(argp, int)); break;
				case byte_t :   dcArgChar(vm, (char)va_arg(argp, int)); break;
				case ptr_t :    dcArgPointer(vm, va_arg(argp, void*)); break;
				default:
					printf("函数类型异常: %s 类型不能作为参数\n", s.c_str());
	    			va_end( argp );                                   /* 将argp置为NULL */    
					return NULL;
			}
		}
	    va_end( argp );                                   /* 将argp置为NULL */    
		void* ans;
		DCpointer fptr = (DCpointer)(fm->function_ptr);
		NormalType t = EliteMeta::getNormalType(fm->ret_type);
			switch (t) {
				case void_t :  ans = NULL;   dcCallVoid(vm, fptr); break;
				case int_t:    ans = (void*) dcCallInt(vm, fptr);  break;
				// case float_t : ans = (void*) dcCallFloat(vm, fptr); break;
				// case double_t :ans = (void*) dcCallDouble(vm, fptr); break;
				case char_t :  ans = (void*) dcCallShort(vm, fptr); break;
				case byte_t :  ans = (void*) dcCallChar(vm, fptr); break;
				case ptr_t :   ans = (void*) dcCallPointer(vm, fptr); break;
				default:
					printf("函数类型异常: %s 类型不能作为返回类型\n", fm->ret_type.c_str());
					return NULL;
			}
		return ans;
	}

	void* malloc_array(int size, ...) {
		va_list argp;                   /* 定义保存函数参数的结构 */    
	    va_start( argp, size );
	    std::vector<int> wd;
	    int v; int num = 1;
		do {
			v = va_arg(argp, int);
			if (v != 0) {
				wd.push_back(v);
				num *= v;
			}
		} while (v != 0);
	    va_end( argp );                                   /* 将argp置为NULL */    

		int full_size = wd.size()*sizeof(int) + size* num;
		int* ans = (int*) malloc(full_size);
		int* i = ans;
		for (auto p = wd.begin(); p != wd.end(); ++p, ++i) {
			*i = *p;
		}
		return (void*) i;
	}

}





void EliteMeta::RegFunction(string& name, string& ret_type, 
	vector<string>& type_list, vector<string>& name_list, void* fptr) {
	FunctionMeta* fm = new FunctionMeta();
	fm->name = name;
	fm->ret_type = ret_type;
	fm->type_list = type_list;
	fm->name_list = name_list;
	fm->function_ptr = fptr;
	getInstance().func_map[name] = fm;
}

void EliteMeta::RegStruct(string& name,
	vector<string>& type_list, vector<string>& name_list) {
	StructMeta* sm = new StructMeta();
	sm->name = name;
	sm->type_list = type_list;
	sm->name_list = name_list;
	getInstance().struct_map[name] = sm;
}

FunctionMeta* EliteMeta::getFunction(string& name) {
	auto& map = getInstance().func_map;
	auto p = map.find(name);
	if (p == map.end()) return NULL;
	else return p->second;
}

StructMeta* EliteMeta::getStruct(string& name) {
	auto& map = getInstance().struct_map;
	auto p = map.find(name);
	if (p == map.end()) return NULL;
	else return p->second;
}


EliteMeta& EliteMeta::getInstance() {
	static EliteMeta meta;
	return meta;
}


NormalType EliteMeta::getNormalType(string& name) {
	if (getStruct(name) != NULL) return ptr_t;
	if (name == "void") return void_t;
	if (name == "int") return int_t;
	if (name == "float") return float_t;
	if (name == "double") return double_t;
	if (name == "char") return char_t;
	if (name == "byte") return byte_t;
	return unknown_t;
}
