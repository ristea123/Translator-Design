// LexicalAnalyzer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

vector<string> types = { "int", "bool", "void" };
vector<string> reservedWords = { "CIN" };

void readWhiteSpaces(string code, int &position)
{
    while (code[position] == ' ' || code[position] == '\n' && position < code.length() - 1)
    {
        position++;
    }
}

bool checkForComment(string code, int position)
{
    if (code[position] == '#')
        return true;
    if (position < code.length() - 2)
        if (code[position] == '/' && code[position + 1] == '/')
            return true;

    return false;
}

string readWord(string code, int &position)
{
    string token;
    while (checkForComment(code, position) == false && isalpha(code[position]) && position != code.length() - 1)
    {
        token.push_back(code[position]);
        position++;
    }
    return token;
}

void readSpacesAndComments(string code, int &position)
{
    while (position < code.length() - 2)
    {
        readWhiteSpaces(code, position);
        if (code[position] == '#' || (code[position] == '/' && code[position + 1] == '/'))
            while (code[position] != '\n')
            {
                if (position != code.length() - 1)
                    position++;
                else
                    break;
            }
        if (code[position] != '#' && (code[position] != '/' || code[position + 1] != '/') && code[position] != ' ' && code[position] != '\n')
            break;
        position++;
    }
} 

bool readType(string code, int &position)
{
    string currToken;
    readSpacesAndComments(code, position);
    currToken = readWord(code, position);

    if (currToken == "")
    {
        cout << "Expected a type\n";
        return false;
    }

    auto it = find(types.begin(), types.end(), currToken);
    if (it != types.end())
    {
        cout << "Token : " << currToken << "  - Type\n";
        return true;
    }
    else
    {
        cout << "Type does not exist\n";
        return false;
    }
}

bool verifyIdentifier(string param)
{
    if (param == "")
    {
        cout << "Expected identifier after type\n";
        return false;
    }
    auto it = param.begin();
    if (isalpha(*it) == 0 && *it != '_')
    {
        cout << "Identifier must begin with alphabet character or _\n";
        return false;
    }
    it++;
    while (it != param.end())
    {
        if (isalpha(*it) == 0 && (*it >= '0' && *it <= '9') && *it != '_')
        {
            cout << "Identifier must contain only alphanumeric and _\n";
            return false;
        }
        it++;
    }
    if (find(types.begin(), types.end(), param) != types.end())
    {
        cout << "Identifier cannot be a reserved word\n";
        return false;
    }
    return true;
}

bool readIdentifier(string code, int &position)
{
    string currToken;
    readSpacesAndComments(code, position);
    currToken = readWord(code, position);
    if (currToken == "")
    {
        cout << "expected identifier after type";
        return false;
    }
    if (verifyIdentifier(currToken) == true)
    {
        cout << "Token : " << currToken << " - identifier\n";
        return true;
    }
    else
    {
        return false;
    }

}

bool readType_Id(string code, int &position)
{
    if (readType(code, position) == false)
        return false;
    if (readIdentifier(code, position) == false)
        return false;
    return true;
}

bool readArrSize(string code, int &position)
{
    cout << "Token : [ - Symbol\n";
    position++;
    string currentToken = "";
    while (code[position] != ']' && position < code.length() - 1)
    {
        if (code[position] >= '0' && code[position] <= '9')
        {
            currentToken.push_back(code[position]);
            position++;
        }
        else
            if (code[position] != ']')
            {
                cout << "Expected ]\n";
                return false;
            }
    }
    if (code[position] != ']')
    {
        cout << "Expected ']'\n";
        return false;
    }
    if (currentToken == "")
    {
        cout << "expected integral literal after [";
        return false;
    }
    cout << "Token : " << currentToken << " - Integral literal\n";
    cout << "Token : ] - Symbol\n";
    position++;
    return true;
}

void readVariables(string code, int &position)
{
    while (1) // make more clear
    {
        readSpacesAndComments(code, position);
        if (position >= code.length() - 1)
            break;
        if (readType_Id(code, position) == false)
            return;
        readSpacesAndComments(code, position);
        if (code[position] == '[')
        {
            if (readArrSize(code, position) == false)
                return;
        }
        else
        {
            if (code[position] == '(')
                break;
        }
        readSpacesAndComments(code, position);
        if (code[position] != ';')
        {
            cout << "Expected ; after declaration";
            return;
        }
        else
        {
            position++;
            cout << "Token : ; - Symbol\n";
        }
    }
}

bool readParameteres(string code, int &position)
{
    while (1) // make more clear
    {
        readSpacesAndComments(code, position);
        if (position >= code.length() - 1)
            break;
        if (readType_Id(code, position) == false)
            return false;
        readSpacesAndComments(code, position);
        if (code[position] == '[')
        {
            if (readArrSize(code, position) == false)
                return false;
        }

        if (code[position] == ')')
            break;
        readSpacesAndComments(code, position);
        if (code[position] != ',')
        {
            cout << "Expected , after parameter";
            return false;
        }
        else
        {
            position++;
            cout << "Token : , - Symbol\n";
        }
    }
}

bool readFunctionCode(string code, int &position)
{
    position++;
    readWhiteSpaces(code, position);
    if (code[position] != '{')
    {
        cout << "Expected { after function declaration";
        return false;
    }
    else
    {
        cout << "Token : { - Identifier\n";
        position++;
        while (1)
        {
            readWhiteSpaces(code, position);
            string currToken = readWord(code, position);
            if (find(types.begin(), types.end(), currToken) != types.end())
            {
                position -= currToken.length();
                readType_Id(code, position);
                position++;
            }
            else
            {
                if (currToken == "cin")
                {
                    cout << "Token : " << currToken << " - Reserved word";
                    break;
                }
            }
        }
    }
    return true;
}

void parse(string code)
{
    int position = 0;
    readVariables(code, position);
    if (code[position] == '(')
    {
        position++;
        readSpacesAndComments(code, position);
        if (position >= code.length() - 1)
        {
            cout << "Expected ) after (";
        }
        else
        {
            if (code[position] == ')')
            {
                cout << "Token : ) - Symbol\n";
                if (readFunctionCode(code, position) == false)
                    return;
            }
            else
            {
                if (readParameteres(code, position) == false)
                    return;
                if (code[position] == ')')
                {
                    cout << "Token : ) - Symbol\n";
                    if (readFunctionCode(code, position) == false)
                        return;
                }
            }
        }
    }

}

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
    string b = a;
    b.resize(size);
    parse(b);
}