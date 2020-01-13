#include "pch.h"
#include "AST.h"
#include "iostream"
#include "Utilities.h"
#include "queue"
using namespace std;
int ASTNode::globalIndex = 0;

AST::AST()
{
    root = new ASTNode(TokenClass("program", "command"), NULL);
    currNode = root;
}

ASTNode* buildIfNode(vector<TokenClass>::iterator it, ASTNode *currNode)
{
    currNode->addChild(*it);
    currNode = currNode->children[currNode->nrChildren - 1];
    it += 2;
    auto it2 = it + 1;
    while (find(comparators.begin(), comparators.end(), (*it2).value) == comparators.end())
        it2++;
    for (auto it3 = it; it3 < it2; it3++)
    {
        currNode->addChild(*it3);
    }
    currNode->addChild(*it2);
    it2++;
    while ((*it2).value != ")")
    {
        currNode->addChild(*it2);
        it2++;
    }
    return currNode;
}

ASTNode* buildWhileNode(vector<TokenClass>::iterator it, ASTNode *currNode)
{
    currNode->addChild(*it);
    currNode = currNode->children[currNode->nrChildren - 1];
    it += 2;
    auto it2 = it + 1;
    while (find(comparators.begin(), comparators.end(), (*it2).value) == comparators.end())
        it2++;
    for (auto it3 = it; it3 < it2; it3++)
    {
        currNode->addChild(*it3);
    }
    currNode->addChild(*it2);
    it2++;
    while ((*it2).value != ")")
    {
        currNode->addChild(*it2);
        it2++;
    }
    return currNode;
}

ASTNode* buildBlockNode(ASTNode *currNode)
{
    currNode->addChild(TokenClass("block", "command"));
    currNode = currNode->children[currNode->nrChildren - 1];
    return currNode;
}

ASTNode* endBlockNode(ASTNode *currNode)
{
    currNode = currNode->parent;
    if (currNode->parent != NULL)
        currNode = currNode->parent;
    return currNode;
}

void buildStatementNode(vector<TokenClass>::iterator it, ASTNode *currNode)
{
    auto it2 = it - 1;
    currNode->addChild(TokenClass("statement", "attribution"));
    currNode = currNode->children[currNode->nrChildren - 1];
    while ((*it2).value != "{" && (*it2).value != ";" && (*it2).value != "}")
        it2--;
    it2++;
    while (it2 != it)
    {
        currNode->addChild(*it2);
        it2++;
    }
    currNode->addChild(*it);
    it2 += 1;
    while ((*it2).value != ";")
    {
        currNode->addChild(*it2);
        it2++;
    }
}

void AST::buildTree(vector<TokenClass> tokens)
{
    for (auto it = tokens.begin(); it != tokens.end(); it++)
    {
        if ((*it).value == "IF")
        {
            currNode = buildIfNode(it, currNode);
        }
        if ((*it).value == "WHILE")
        {
            currNode = buildWhileNode(it, currNode);
        }
        if ((*it).value == "{")
        {
            currNode = buildBlockNode(currNode);
        }
        if ((*it).value == "}")
        {
            currNode = endBlockNode(currNode);
        }
        if ((*it).value == "=")
        {
            buildStatementNode(it, currNode);
        }
    }
}

void AST::printTree()
{
    if (root == NULL)
        return;
    queue<ASTNode *> q;
    q.push(root);
    while (!q.empty())
    {
        int n = q.size();
        while (n > 0)
        {
            ASTNode * p = q.front();
            q.pop();
            cout << p->index << "/" << p->value.value;
            if (p->parent != NULL)
                cout << "/" << p->parent->index << "   ";
            if (p->nrChildren == 0 && p->isLeaf == false)
            {
                TokenClass* a = new TokenClass(p->value.tokenType, "");
                p->addChild(*a);
                p->children[0]->isLeaf = true;
            }
            for (int i = 0; i < p->nrChildren; i++)
                q.push(p->children[i]);
            n--;
        }
        cout << endl;
    }
}