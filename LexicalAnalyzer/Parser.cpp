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
bool Parser::parseExpression(ASTNode *&currNode, string expression)
{
    vector<string> words;
    size_t current, previous = 0;
    current = expression.find(" ");
    string expr1 = "";
    string expr2 = "";
    string word;
    bool is1Expression = true;

    while (current != string::npos)
    {
        word = expression.substr(previous, current - previous);
        if (find(operators.begin(), operators.end(), word) != operators.end())
        {
            for (auto it : words)
                expr1 += it;
            previous = current + 1;
            while (expression[previous] == ' ')
                previous++;
            expr2 = expression.substr(previous, expression.length() - previous + 1);
            is1Expression = false;
            break;
        }
        else
        {
            words.push_back(word);
            previous = current + 1;
            current = expression.find(" ", previous);
        }
    }
    if(is1Expression == true)
        expr1 += expression.substr(previous, current - previous);

    currNode->addChild(expr1);
    currNode->children[currNode->nrChildren - 1]->addChild("atom");
    if (expr2 != "")
    {
        currNode->addChild(word);
        currNode->addChild(expr2);
        currNode = currNode->children[currNode->nrChildren - 1];
        currNode->addChild("atom");
        parseExpression(currNode, expr2);
    }
    currNode = currNode->parent;
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
    currNode->addChild("statement");
    currNode = currNode->children[currNode->nrChildren - 1];
    parseExpression(currNode, ifStmt);
    printTree(root);
    position++;
    readWhiteSpaces(code, position);
    if (code[position] != '{')
        cout << "Expected { after if statement";
    else
        parseBlock(code, position);
    return true;
}

bool Parser::parseBlock(string code, int &position)
{
    currNode->addChild("block");
    currNode = currNode->children[0];
    position++;
    readWhiteSpaces(code, position);
    string expr = "";
    while (position < code.length() - 1)
    {
        expr += code[position];
        position++;
        if (code[position] == ' ' && find(otherReservedWords.begin(), otherReservedWords.end(), expr) == otherReservedWords.end())
        {
            while (code[position] != ';')
            {
                expr += code[position];
                position++;
            }
            parseExpression(currNode, expr);
            printTree(root);
        }
        if (expr == "IF")
        {
            currNode->addChild("IF");
            currNode = currNode->children[currNode->nrChildren - 1];
            parseIf(code, position);
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
        currNode = root;
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
