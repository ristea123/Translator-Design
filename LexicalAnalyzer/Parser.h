#include "Utilities.h"
#include "AST.h"

class Parser
{
    vector<string> identifiers;
    ASTNode *root;
    ASTNode *currNode;
public:
    bool parse(string code, int position);
    bool parseProgram(string code, int position);
    bool parseVarDecl(string code, int &position);
    bool parseIdentifier(string code, int &position);
    bool parseArrayField(string code, int &position);
    bool parseFunctionParameters(string code, int &position);
    bool parseBlock(string code, int &position);
    bool parseIf(string code, int &position);
    bool parseWhile(string code, int &position);
    bool parseExpression( ASTNode *&currNode, string expression);
    bool parseReturn(string code, int &position);
};