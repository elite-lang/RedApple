/**
 * Project Untitled
 */


#include "ASTNode.h"

/**
 * ASTNode implementation
 * 
 * ASTNode 抽象语法树节点
 * ====================
 * 
 * name表示节点的名字  
 * type表示节点的类型
 * 
 * 而语法的具体内容，保存在各个子类上
 * 
 * 本语法树抽象节点，可以认为是一种抽象的S表达式
 */


/**
 * @return string&
 */
string* ASTNode::getName() {
    return name;
}

/**
 * @return int
 */
int ASTNode::getType() {
    return type;
}

