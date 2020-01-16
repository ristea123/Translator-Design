#include "pch.h"
#include "Utilities.h"
vector<symbolTableRow*> rows;
vector<string> errors;
string determineTokenType(string TokenClass)
{
    int i = 0;
    for (i; i < 5; i++)
    {
        if (find(reservedWords[i].begin(), reservedWords[i].end(), TokenClass) != reservedWords[i].end())
            break;

    }
    return tokenTypes[i];
}

void refactorTokens(vector<TokenClass> &tokens)
{
    for (auto it = tokens.begin(); it != tokens.end(); it++)
    {
        if ((*it).tokenType == "identifier")
        {
            if ((*(it - 1)).tokenType == "type" && (*(it + 1)).value == "(")
            {
                (*it).tokenType = "function";
            }
            if ((*(it - 1)).tokenType == "type")
            {
                (*it).type = (*(it - 1)).value;
            }
            if ((*it).value[0] == '"')
                (*it).tokenType = "string literal";
            if (isdigit((*it).value[0]) != 0)
                (*it).tokenType = "integer literal";
        }
    }
}
