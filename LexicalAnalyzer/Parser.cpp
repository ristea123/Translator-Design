#include "pch.h"
#include "Parser.h"

static bool isError = true;

bool Parser::parseArrayField(string code, int &position)
{
    string number = "";

    readWhiteSpaces(code, position);
    position++;
    while (code[position] != ']' && position <= code.length() - 1)
    {
        if (isdigit(code[position]) == false)
        {
            cout << "expected numerical expression after [\n";
            return false;
        }
        else
            number.push_back(code[position]);
        position++;
    }
    if (position == code.length() - 1)
    {
        cout << "Expected ; at the end of line\n";
        return false;
    }
    else
    {
        cout << "Token : [ - Symbol\n";
        readWhiteSpaces(code, position);
        if (code[position] != ']')
        {
            cout << "Expected ] after numerical expression";
            return false;
        }
        else
        {
            position++;
            cout << "Token : ] - Symbol\n";
        }
    }
    readWhiteSpaces(code, position);
    return true;
}

bool Parser::parseIdentifier(string code, int &position)
{
    readWhiteSpaces(code, position);
    string identifier;
    while (isalnum(code[position]) != 0 || code[position] == '_')
    {
        identifier.push_back(code[position]);
        position++;
    }
    if (verifyIdentifier(identifier) == false)
        return false;
    cout << "Token - " << identifier << " - identifier\n";
    readWhiteSpaces(code, position);
    return true;
}

bool Parser::parseVarDecl(string code, int &position)
{
    readWhiteSpaces(code, position);
    string type = readUntilSpaceOrNewLine(code, position);
    if (find(types.begin(), types.end(), type) == types.end())
    {
        cout << "Expected a type after program declaration\n";
        return false;
    }
    else
    {
        readWhiteSpaces(code, position);
        cout << "Token - " << type << " - type\n";
        if (parseIdentifier(code, position) == false)
            return false;

        readWhiteSpaces(code, position);

        if (code[position] == '(')
        {
            isError = false;
            return false;
        }

        if (code[position] == '[')
        {
            if (parseArrayField(code, position) == false)
                return false;
        }
        if (code[position] != ';')
        {
            cout << "expected ; after ]\n";
            return false;
        }
        else
            cout << "Token : ; - Symbol\n";
    }
    return true;
}

bool Parser::parseFunctionParameters(string code, int &position)
{
    readWhiteSpaces(code, position);
    string type = readUntilSpaceOrNewLine(code, position);
    if (find(types.begin(), types.end(), type) == types.end())
    {
        cout << "Expected a type after program declaration\n";
        return false;
    }
    else
    {
        readWhiteSpaces(code, position);
        cout << "Token - " << type << " - type\n";
        if (parseIdentifier(code, position) == false)
            return false;

        readWhiteSpaces(code, position);
        if (code[position] == '[')
        {
            if (parseArrayField(code, position) == false)
                return false;
        }
        if (code[position] == ',')
        {
            position++;
            if (parseFunctionParameters(code, position) == false)
            {
                return false;
            }
        }
        else
        {
            if (code[position] == ')')
            {
                cout << "Token : ) - Symbol\n";
                position++;
                return true;
            }
            else
            {
                cout << "expected )";
                return false;
            }
        }
    }
    return true;
}

bool Parser :: parseIf(string code, int & position)
{
    position++;
    readWhiteSpaces(code, position);
    string ifStmt = "";
    while (code[position] != ')')
    {
        ifStmt += code[position];
        position++;
    }
    vector<string> words;
    size_t current, previous = 0;
    current = ifStmt.find(" ");
    string expr1 = "";
    string expr2 = "";
    while (current != string::npos)
    {
        words.push_back(ifStmt.substr(previous, current - previous));
        previous = current + 1;
        current = ifStmt.find(" ", previous);
    }
    words.push_back(ifStmt.substr(previous, current - previous));
    string foundOper = "";
    for (auto oper : operators)
    {
        auto pos = find(words.begin(), words.end(), oper);
        if (pos != words.end())
        {
            for(auto it = words.begin(); it < pos; it++)
            {
                expr1 += *it;
            }
            for(auto it = pos + 1; it != words.end(); it++)
            {
                expr2 += *it;
            }
            foundOper = *pos;
            break;
        }
    }
    currNode->addChild(expr1);
    if (expr2 != "")
    {
        currNode->addChild(foundOper);
        currNode->addChild(expr2);
    }
    return true;
}

bool Parser::parseBlock(string code, int &position)
{
    position++;
    readWhiteSpaces(code, position);
    string word = "";
    while (true)
    {
        word += code[position];
        position++;
        if (word == "IF")
        {
            currNode->addChild("IF");
            currNode = currNode->children[currNode->nrChildren - 1];
            parseIf(code, position);
            break;
        }
    }
    return true;
}

bool Parser::parseProgram(string code, int position)
{
    string word = readUntilSpaceOrNewLine(code, position);
    if (word == "program")
    {
        cout << "Token - " << "program - " << " keyword\n";
        root = new ASTNode("program", NULL);
        root->addChild("block");
        currNode = root->children[0];
        readWhiteSpaces(code, position);
        while (parseVarDecl(code, position) == true)
        {
            position++;
        }
        if (isError == false)
        {
            position++;
            cout << "Token : ( - Symbol\n";
            if (parseFunctionParameters(code, position) == false)
                return false;
            else
            {
                readWhiteSpaces(code, position);
                if (code[position] == '{')
                {
                    cout << "Token : { - Symbol\n";
                    parseBlock(code, position);
                }
            }
        }
    }
    else
        return false;
}

bool Parser::parse(string code, int position)
{
    readWhiteSpaces(code, position);
    if (position == code.length() - 1)
        return true;
    else
        return parseProgram(code, position);
    return false;
}
