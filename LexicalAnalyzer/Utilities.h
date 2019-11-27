#include "pch.h"
#include "string"
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

struct Node
{
    string name;
    vector<Node> children;
};

class ASTClass
{
public:
    Node root;
};

static ASTClass AST;


inline void readWhiteSpaces(string code, int &position)
{
    while (code[position] == ' ' || code[position] == '\n' && position < code.length() - 1)
    {
        position++;
    }
}

inline string readUntilSpace(string code, int &position)
{
    string currStr = "";
    while (code[position] != ' ' && position < code.length() - 1)
    {
        currStr += code[position];
        position++;
    }
    return currStr;
}