#ifndef d
#define d
#include "string"
using namespace std;

class TokenClass
{
public:
    string value, tokenType, type;
    int line;
    TokenClass() {}
    TokenClass(string value, string tokenType, string type = "") : value(value), tokenType(tokenType), type(type) {};
    TokenClass& operator=(const TokenClass& other)
    {
        value = other.value;
        tokenType = other.tokenType;
        line = other.line;
        return *this;
    }
};

#endif // !1
