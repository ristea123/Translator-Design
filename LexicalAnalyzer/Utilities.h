#pragma once
#include "pch.h"

#include "Token.h"
#include "string"
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

static vector<string> types = { "int", "bool", "void" };
static vector<string> separator = { ",", ";", "{", "(", ")", "}" };
static vector<string> commands = { "CIN", "COUT", "program", "IF", "RETURN", "WHILE" };
static vector<string> operators = { "+", "=", "-", "*", "/", "!", "-" };
static vector<string> comparators = { "ANDAND", "OROR", "EQEQ", "NOTEQ", "<", ">", "LESSEQ", "GREATEREQ" };
static vector<vector<string>> reservedWords = { types, commands, operators, comparators, separator };
static vector<string> tokenTypes = { "type", "command", "operator", "comparator","separator",  "identifier" };

class symbolTableRow
{
public:
    string name, type, scope;
    symbolTableRow(string name, string type, string scope) : name(name), type(type), scope(scope) {}
};


void checkFunctionParameters();

extern vector<symbolTableRow*> rows;
extern vector<string> errors;

string determineTokenType(string TokenClass);
void buildSymbolTable(vector<TokenClass> tokens);
void refactorTokens(vector<TokenClass> &tokens);