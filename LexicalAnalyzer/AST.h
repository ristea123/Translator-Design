#pragma once
#ifndef __AST_H__
#define __AST_H__

#include "ASTNode.h"
#include "vector"
class AST
{
private:
    ASTNode * currNode;
public:
    ASTNode * root;
    AST();
    void buildTree(vector<TokenClass> tokens);
    void printTree();
    void parseTree();
    void checkComparistonAndOppForTypeErr(ASTNode * node);
    void checkAssignmentTypeError(ASTNode * node);
    void buildSymbolTable();
};

#endif // !__AST_H__