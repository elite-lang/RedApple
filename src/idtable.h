#include <deque>
#include "idmap.h"
using namespace std;

#ifndef IDTABLE_H
#define IDTABLE_H


/**
 * @brief 简易栈式符号表，需要在进入函数后手动压栈等操作
 */
class IDTable
{
public:
    IDTable();

    /**
     * @brief 查找元素是否在符号表中
     * @details 查找元素是否在当前符号表中，若当前符号栈不只一层，还好向上层符号表进行查找
     * 
     * @param str 要查找的符号名
     * @return 若找到，返回id结构体的指针，无需释放内存。未找到返回NULL
     */
    id* find(string& str) const;

    /**
     * @brief 返回一层的全部元素
     * 
     * @param level 返回一层的全部元素，level是传入的层级
     * @return 返回的元素map
     */
    map<string,id*>& getAll(int level);

    /**
     * @brief 插入一个元素
     * 
     * @param str 要插入的名称
     * @param type 类型的枚举
     * @param data 数据指针
     */
    void insert(string& str,SymbolType type, LValue data);

    /**
     * @brief 压栈，一般在进入新的符号区域，例如函数体中
     */
    void push();

    /**
     * @brief 弹栈，在离开一个符号区域时需要手动调用
     */
    void pop();

    /**
     * @brief 获取当前层级
     */
    int getLevel();

private:
    deque<IDMap> ID_stack;
};

#endif // IDTABLE_H
