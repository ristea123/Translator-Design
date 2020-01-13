#pragma once
#ifndef ASTNODE_H
#define ASTNODE_H
#include "Token.h"
#include "string"
using namespace std;
class ASTNode
{
public:
    int index;
    TokenClass value;
    ASTNode** children;
    ASTNode* parent;
    int nrChildren = 0;
    static int globalIndex;
    bool isLeaf;
    void addChild(TokenClass value)
    {
        children[nrChildren] = new ASTNode(value, this);
        nrChildren++;
        globalIndex++;
        children[nrChildren - 1]->index = globalIndex;
    }
    ASTNode(TokenClass value, ASTNode* parent)
    {
        this->value = value;
        this->parent = parent;
        children = new ASTNode*[100];
        isLeaf = false;
    }
    ASTNode() {};
};
void printTree(ASTNode *root);
#endif
