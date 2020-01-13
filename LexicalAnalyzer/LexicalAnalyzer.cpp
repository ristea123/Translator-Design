// ConsoleApplication7.cpp : This file contains the 'main' function. Program execution begins and ends there.


#include "pch.h"
#include <fstream>
#include "Parser.h"
#include "AST.h"
#include "Utilities.h"
int accolades = 0;
int parantheses = 0;

int main()
{
    char * a = new char[500];
    ifstream f("stefi.in");
    int size = 0;
    while (!f.eof())
    {
        a[size] = f.get();
        size++;
    }
    string code = a;
    code.resize(size);
    Parser prs;
    prs.parse(code);
    refactorTokens(prs.tokens);
    cout << "------Tokens---------S" << "\n";
    prs.printTokens();
    AST AST;
    AST.buildTree(prs.tokens);
    cout << "------Abstract syntax tree---------S" << "\n";
    AST.printTree();

    cout << "\n\n\n\n";
    cout << "symbol name " << "Type " << "Scope" << "\n";
    for (auto it = prs.tokens.begin(); it != prs.tokens.end(); it++)
    {
        if ((*it).value == "{")
            accolades++;
        if ((*it).value == "}")
            accolades--;
        if ((*it).value == "(")
            parantheses++;
        if ((*it).value == ")")
            parantheses--;
        if ((*it).tokenType == "identifier" && (*(it - 1)).tokenType == "type")
        {
            string scope;
            if (accolades > 0)
                scope = "local";
            else
                scope = "global";
            if (parantheses > 0)
            {
                scope = "function parameter";
            }
            symbolTableRow *row = new symbolTableRow((*it).value, (*it).type, scope);
            rows.push_back(new symbolTableRow((*it).value, (*it).type, scope));
        }
        if ((*it).tokenType == "function")
        {
            if ((*(it - 1)).tokenType == "type")
            {
                rows.push_back(new symbolTableRow((*it).value, "function, " + (*it).type, "global"));
            }
        }
    }
    for (auto it : rows)
    {
        cout << it->name << " " << it->type << " " << it->scope << "\n";
    }
}
