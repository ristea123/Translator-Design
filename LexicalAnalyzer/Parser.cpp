#include "pch.h"
#include "Parser.h"

bool Parser::parseProgram(string code, int position)
{
    string word = readUntilSpace(code, position);
    if (word == "program")
    {
        return true;
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
