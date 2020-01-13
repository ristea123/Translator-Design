#ifndef d
#define d
#include "string"
using namespace std;

class TokenClass
{
public:
    string value, tokenType, type;
    TokenClass() {}
    TokenClass(string value, string tokenType, string type = "") : value(value), tokenType(tokenType), type(type) {};
    TokenClass& operator=(const TokenClass& other)
    {
        value = other.value;
        tokenType = other.tokenType;
        return *this;
    }
};

#endif // !1
