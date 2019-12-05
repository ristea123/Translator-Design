// LexicalAnalyzer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "pch.h"
#include "Parser.h"
#include "AST.h"

int main()
{
    char * a = new char[100];
    ifstream f("stefi.in");
    int size = 0;
    while (!f.eof())
    {
        a[size] = f.get();
        size++;
    }
    string code = a;
    code.resize(size);
    Parser parser;
    parser.parse(code, 0);
}