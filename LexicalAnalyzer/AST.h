#ifndef AST_H
#define AST_H
#include "string"

class ASTNode
{
public:
    string value;
    ASTNode** children;
    ASTNode* parent;
    int nrChildren = 0;
    void addChild(string value)
    {
        children[nrChildren] = new ASTNode(value, this);
        nrChildren++;
    }
    ASTNode(string value, ASTNode* parent)
    {
        this->value = value;
        this->parent = parent;
        children = new ASTNode*[100];
    }
    ASTNode() {};
};
#endif