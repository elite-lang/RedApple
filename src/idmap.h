#include <map>
using namespace std;

#ifndef IDMAP_H
#define IDMAP_H

#include "LLCG/lvalue.h"

enum SymbolType
{
	var_t, type_t, struct_t, enum_t, delegate_t, function_t, macro_t
};

struct id {
    int        level;
    SymbolType type;
    LValue     data;
};


/**
 * @brief 一款简单的栈式符号表，代表栈的一层
 * @details 简单的栈式符号表，负责一层局部符号，用Map构建，而存储的内容则是void*，使用时必须强制类型转换
 */
class IDMap
{
public:
    IDMap();
    ~IDMap();

    /**
     * @brief 查找一个符号，若不存在返回空
     * 
     * @param str 符号名
     * @return 返回对应的id结构体指针，未找到返回NULL
     */
    id* find(string& str) const;

    /**
     * @brief 返回当前表中全部元素
     */
    map<string,id*>& getAll();

    /**
     * @brief 插入一个符号
     * 
     * @param str 名字
     * @param level 当前层次
     * @param type 枚举类型
     * @param data 数据指针
     */
    void insert(string& str, int level, SymbolType type, LValue data);
private:
    map<string,id*> ID_map;
};

#endif // IDMAP_H
