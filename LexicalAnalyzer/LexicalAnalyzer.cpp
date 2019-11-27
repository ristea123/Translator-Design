// LexicalAnalyzer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

vector<string> types = { "int", "bool", "void" };
vector<string> reservedWords = { "CIN", "COUT" };
vector<string> identifiers = {};
vector<string> operators = {"+", "-", "*", "/", "!", "ANDAND", "OROR", "EQEQ", "NOTEQ", "<", ">", "LESSEQ", "GREATEREQ", "-"};

string code;
int position;

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
    code = a;
    code.resize(size);
}