#pragma once
#ifndef __PARSER_H__
#define __PARSER_H__


#include "string"
#include "regex"
#include "iostream"
#include "vector"
#include "Utilities.h"
using namespace std;

class Parser
{
public:
    int nrLines = 1;
    std::vector<TokenClass> tokens;
    int nrTokens = 0;
    void printTokens()
    {
        for (auto it : tokens)
            cout << it.value << " " << it.tokenType << " " << it.type << " " << it.line << "\n";
    }
    void parse(string code)
    {
        string delims{ " \n" };
        size_t beg, pos = 0;
        while ((beg = code.find_first_not_of(delims, pos)) != std::string::npos)
        {
            pos = code.find_first_of(delims, beg + 1);
            string value = code.substr(beg, pos - beg);
            string type = determineTokenType(value);
            TokenClass t(value, type);
            t.line = nrLines;
            tokens.push_back(t);
            if (pos < code.length() && (code[pos] == '\n' || code[pos + 1] == '\n'))
                nrLines++;
            nrTokens++;
        }
    }
};

#endif // !__PARSER_H__