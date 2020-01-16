#ifndef d
#define d
#include "string"
using namespace std;

class TokenClass
{
public:
    string value, tokenType, type;
    int line;
    bool isInitialized;
    TokenClass() { isInitialized = false; }
    TokenClass(string value, string tokenType, string type = "") : value(value), tokenType(tokenType), type(type) { isInitialized = false; };
    TokenClass& operator=(const TokenClass& other)
    {
        value = other.value;
        tokenType = other.tokenType;
        line = other.line;
        isInitialized = other.isInitialized;
        return *this;
    }
};

#endif // !1
