#include "pch.h"
#include "string"
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

static vector<string> types = { "int", "bool", "void" };
static vector<string> otherReservedWords = { "CIN", "COUT", "program", "IF" };
static vector<string> operators = { "+", "=", "-", "*", "/", "!", "ANDAND", "OROR", "EQEQ", "NOTEQ", "<", ">", "LESSEQ", "GREATEREQ", "-" };
static vector<vector<string>> reservedWords = { types, operators, otherReservedWords };

inline bool verifyIdentifier(string identifier)
{
    if (isalpha(identifier[0]) == false && identifier[0] != '_')
    {
        cout << "Identifier must start with letter or _\n";
        return false;
    }
    for (auto vecIt : reservedWords)
        for (auto it : vecIt)
            if (it == identifier)
            {
                cout << "Identifier can not be a reserved word \n";
                return false;
            }
    for (auto it : identifier)
    {
        if (isalnum(it) == 0 && it != '_')
        {
            cout << "All characters in identifier must be alphanumerical or _";
            return false;
        }
    }
}

inline void readWhiteSpaces(string code, int &position)
{
    while (code[position] == ' ' || code[position] == '\n' && position < code.length() - 1)
    {
        position++;
    }
}

inline string readUntilSpaceOrNewLine(string code, int &position)
{
    string currStr = "";
    while (code[position] != ' ' && code[position] != '\n' && position < code.length() - 1)
    {
        currStr += code[position];
        position++;
    }
    return currStr;
}