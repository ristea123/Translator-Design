#include "pch.h"
#include "AST.h"
#include "iostream"
void printTree(ASTNode * root)
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
            cout << p->value << " |";
            for (int i = 0; i < p->nrChildren; i++)
                q.push(p->children[i]);
            n--;
        }
        cout << endl;
    }
}