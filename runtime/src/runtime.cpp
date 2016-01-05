/*
* @Author: sxf
* @Date:   2015-11-15 10:19:10
* @Last Modified by:   sxf
* @Last Modified time: 2016-01-01 18:55:41
*/

#include <string>
#include <map>
#include <vector>
#include <cstdio>
#include <cstdlib>
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
	void_normal_t,
	int_normal_t,
	float_normal_t,
	double_normal_t,
	char_normal_t,
	byte_normal_t,
	ptr_normal_t,
	unknown_normal_t
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
	void elite_meta_function(const char* name, const char** list, void* fptr) {
		// printf("RegFunction: %s\n", name);
	    int argno = 0;                  /* 纪录参数个数 */
	    vector<string> type_list;
	    vector<string> name_list;
	    string sret_type = list[0];
	    const char **para = list+1;                     /* 存放取出的字符串参数 */
	    while (*para != NULL) {
	        ++argno;
	        if (argno & 1) type_list.push_back(*para);
	        else       name_list.push_back(*para);
	        // printf("%s ", *para);
	        ++para;
	    }
	    string sname = name;
	    EliteMeta::RegFunction(sname, sret_type, type_list, name_list, fptr);
	}

	void elite_meta_list(const char** list) {
		string meta_type = list[0];
		if (meta_type == "struct") {
			string meta_name = list[1];
			// printf("RegStruct: %s\n", meta_name.c_str());
		    int argno = 0;                  /* 纪录参数个数 */
		    const char** para = list+2;        /* 存放取出的字符串参数 */
		    vector<string> type_list;
		    vector<string> name_list;
		    while (*para != NULL) {
		        ++argno;
		        if (argno & 1) type_list.push_back(*para);
		        else       name_list.push_back(*para);
		        // printf("%s ", *para);
		        ++para;
		    }
		    EliteMeta::RegStruct(meta_name, type_list, name_list);
		}
	}

	void* FunctionCall(const char* name, ...) {
		string sname = name;
		FunctionMeta* fm = EliteMeta::getFunction(sname);
		if (fm == NULL) {
			printf("运行时异常, 反射调用的函数未找到: %s\n", name);
			return NULL;
		}
		va_list argp;                   /* 定义保存函数参数的结构 */
	    va_start( argp, name );

	    // 设定调用栈
	    DCCallVM* vm = EliteMeta::getInstance().vm;
	    dcMode(vm, DC_CALL_C_DEFAULT);
		dcReset(vm);

		for (auto& s : fm->type_list) {
			NormalType t = EliteMeta::getNormalType(s);
			switch (t) {
				case int_normal_t:    dcArgInt(vm, va_arg(argp, int));  break;
				case float_normal_t:  dcArgFloat(vm, (float)va_arg(argp, double)); break; // 默认参数提升
				case double_normal_t: dcArgDouble(vm, va_arg(argp, double)); break;
				case char_normal_t:   dcArgShort(vm, (short)va_arg(argp, int)); break;
				case byte_normal_t:   dcArgChar(vm, (char)va_arg(argp, int)); break;
				case ptr_normal_t:    dcArgPointer(vm, va_arg(argp, void*)); break;
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
				case void_normal_t:  ans = NULL;   dcCallVoid(vm, fptr); break;
				case int_normal_t:    ans = (void*)dcCallInt(vm, fptr);  break;
				// case float_normal_t : ans = (void*) dcCallFloat(vm, fptr); break;
				// case double_normal_t :ans = (void*) dcCallDouble(vm, fptr); break;
				case char_normal_t:  ans = (void*)dcCallShort(vm, fptr); break;
				case byte_normal_t:  ans = (void*)dcCallChar(vm, fptr); break;
				case ptr_normal_t:   ans = (void*)dcCallPointer(vm, fptr); break;
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

	void free_array(void* ptr, int v) {
		if (ptr == NULL) return;
		void* real_ptr = ptr - v*sizeof(int);
		free(ptr);
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
	if (getStruct(name) != NULL) return ptr_normal_t;
	if (name == "void") return void_normal_t;
	if (name == "int") return int_normal_t;
	if (name == "float") return float_normal_t;
	if (name == "double") return double_normal_t;
	if (name == "char") return char_normal_t;
	if (name == "byte") return byte_normal_t;
	return unknown_normal_t;
}
