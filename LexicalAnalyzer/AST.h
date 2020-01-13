#pragma once
#ifndef __AST_H__
#define __AST_H__

#include "ASTNode.h"
#include "vector"
class AST
{
private:
    ASTNode * root;
    ASTNode * currNode;
public:
    AST();
    void buildTree(vector<TokenClass> tokens);
    void printTree();
};

#endif // !__AST_H__