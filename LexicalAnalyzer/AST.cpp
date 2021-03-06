#include "pch.h"
#include "AST.h"
#include "iostream"
#include "Utilities.h"
#include "queue"
using namespace std;
int ASTNode::globalIndex = 0;

AST::AST()
{
    root = new ASTNode(TokenClass("program", "command"), NULL);
    currNode = root;
}

ASTNode* buildIfNode(vector<TokenClass>::iterator it, ASTNode *currNode)
{
    currNode->addChild(*it);
    currNode = currNode->children[currNode->nrChildren - 1];
    it += 2;
    auto it2 = it + 1;
    while (find(comparators.begin(), comparators.end(), (*it2).value) == comparators.end())
        it2++;
    for (auto it3 = it; it3 < it2; it3++)
    {
        currNode->addChild(*it3);
        if ((*it3).tokenType == "identifier" && (*it3).isInitialized == false)
        {
            string error = "Error : variable " + (*it3).value + " used without being initialized\n";
            errors.push_back(error);
        }
    }
    currNode->addChild(*it2);
    it2++;
    while ((*it2).value != ")")
    {
        currNode->addChild(*it2);
        it2++;
    }
    return currNode;
}

ASTNode* buildWhileNode(vector<TokenClass>::iterator it, ASTNode *currNode)
{
    currNode->addChild(*it);
    currNode = currNode->children[currNode->nrChildren - 1];
    it += 2;
    auto it2 = it + 1;
    while (find(comparators.begin(), comparators.end(), (*it2).value) == comparators.end())
        it2++;
    for (auto it3 = it; it3 < it2; it3++)
    {
        currNode->addChild(*it3);
    }
    currNode->addChild(*it2);
    it2++;
    while ((*it2).value != ")")
    {
        currNode->addChild(*it2);
        it2++;
    }
    return currNode;
}

ASTNode* buildBlockNode(ASTNode *currNode)
{
    currNode->addChild(TokenClass("block", "command"));
    currNode = currNode->children[currNode->nrChildren - 1];
    return currNode;
}

ASTNode* endBlockNode(ASTNode *currNode)
{
    currNode = currNode->parent;
    if (currNode->parent != NULL)
        currNode = currNode->parent;
    return currNode;
}

void buildStatementNode(vector<TokenClass>::iterator it, ASTNode *currNode)
{
    auto it2 = it - 1;
    currNode->addChild(TokenClass("statement", "attribution"));
    currNode = currNode->children[currNode->nrChildren - 1];
    while ((*it2).value != "{" && (*it2).value != ";" && (*it2).value != "}")
        it2--;
    it2++;
    while (it2 != it)
    {
        currNode->addChild(*it2);
        it2++;
    }
    currNode->addChild(*it);
    it2 += 1;
    while ((*it2).value != ";")
    {
        if ((*it2).tokenType == "type")
        {
            string error = "Error : Expected ; before identifier " + (*it2).value + " on line " + to_string((*it2).line) + "\n";
            errors.push_back(error);
        }
        currNode->addChild(*it2);
        it2++;
    }
}

void buildDeclarationNode(vector<TokenClass>::iterator it, ASTNode *currNode)
{
    currNode->addChild(TokenClass("statement", "declaration"));
    currNode = currNode->children[currNode->nrChildren - 1];
    while ((*it).value != ";" && (*it).value != "=")
    {
        currNode->addChild(*it);
        it++;
    }

}

void buildFunctionNode(vector<TokenClass>::iterator it, ASTNode *currNode)
{
    currNode->addChild(TokenClass("Statement", "function declaration"));
    currNode = currNode->children[currNode->nrChildren - 1];
    currNode->addChild(*it);
    it++;
    currNode->addChild(*it);
    it++;
}

void AST::buildTree(vector<TokenClass> tokens)
{
    for (auto it = tokens.begin(); it != tokens.end(); it++)
    {
        if ((*it).value == "IF")
        {
            currNode = buildIfNode(it, currNode);
        }
        if ((*it).value == "WHILE")
        {
            currNode = buildWhileNode(it, currNode);
        }
        if ((*it).value == "{")
        {
            currNode = buildBlockNode(currNode);
        }
        if ((*it).value == "}")
        {
            currNode = endBlockNode(currNode);
        }
        if ((*it).value == "=")
        {
            buildStatementNode(it, currNode);
        }
        if ((*it).tokenType == "type" && ((*(it + 2)).value != "("))
        {
            buildDeclarationNode(it, currNode);
        }
        if ((*it).tokenType == "type" && ((*(it + 2)).value == "("))
        {
            buildFunctionNode(it, currNode);
        }
    }
}

void AST::printTree()
{
    if (root == NULL)
        return;
    queue<ASTNode *> q;
    q.push(root);
    while (!q.empty())
    {
        int n = q.size();
        while (n > 0)
        {
            ASTNode * p = q.front();
            q.pop();
            cout << p->index << "/" << p->value.value;
            if (p->parent != NULL)
                cout << "/" << p->parent->index << "   ";
            if (p->nrChildren == 0 && p->isLeaf == false)
            {
                TokenClass* a = new TokenClass(p->value.tokenType, "");
                p->addChild(*a);
                p->children[0]->isLeaf = true;
            }
            for (int i = 0; i < p->nrChildren; i++)
                q.push(p->children[i]);
            n--;
        }
        cout << endl;
    }
}


void AST::buildSymbolTable()
{
    int nrAccolades = 0;
    if (root == NULL)
        return;
    queue<ASTNode *> q;
    q.push(root);
    while (!q.empty())
    {
        int n = q.size();
        while (n > 0)
        {
            string scope = "";
            ASTNode * p = q.front();
            q.pop();
            if (p->value.value == "{")
                nrAccolades++;
            if (p->value.value == "}")
                nrAccolades--;
            if (p->value.tokenType == "declaration" || p->value.tokenType == "function declaration")
            {
                if (nrAccolades == 0)
                    scope = "global";
                else
                    scope = "block local";
                if(p->nrChildren >= 3 && (p->children[2]->value.value == ")" || p->children[2]->value.value == ","))
                    scope = "function parameter";
                rows.push_back(new symbolTableRow(p->children[1]->value.value, p->children[0]->value.value, scope));
                if (p->value.tokenType == "function declaration")
                    rows.back()->type += "-function";
            }
            for (int i = 0; i < p->nrChildren; i++)
                q.push(p->children[i]);
            n--;
        }
        cout << endl;
    }
}

void AST::parseTree()
{
    if (root == NULL)
        return;
    queue<ASTNode *> q;
    q.push(root);
    while (!q.empty())
    {
        int n = q.size();
        while (n > 0)
        {
            ASTNode * p = q.front();
            q.pop();
            if (p->value.tokenType == "comparator")
            {
                checkComparistonAndOppForTypeErr(p->parent);
            }
            if(p->value.value == "=")
            {
                checkAssignmentTypeError(p->parent);
            }
            for (int i = 0; i < p->nrChildren; i++)
                q.push(p->children[i]);
            n--;
        }
        cout << endl;
    }
}

void AST::checkAssignmentTypeError(ASTNode * node)
{
    if (node->children[1]->value.value != "=" && (node->children[2]->value.value != "=" && node->children[0]->value.tokenType == "type"))
    {
        string error = "Error: cannot be more than 1 token before assignment operator on line" + to_string(node->children[1]->value.line) + "\n";
        errors.push_back(error);
        return;
    }
    bool found = false;
    for (auto it : rows)
        if (node->children[0]->value.value == it->name)
        {
            found = true;
            break;
        }
    if (found == false)
    {
        string error = "Error : identifier " + node->children[0]->value.value + " not found \n";
        errors.push_back(error);
        return;
    }
    string identifierType;
    for (auto it : rows)
        if (node->children[0]->value.value == it->name)
            identifierType = it->type;
    int i = 2;
    if (node->children[0]->value.tokenType == "type")
        i++;
    for (i; i < node->nrChildren; i++)
    {
        string currType = "";
        if (node->children[i]->value.tokenType == "string literal")
            currType = "string";
        else
            if (node->children[i]->value.tokenType == "integer literal")
                currType = "int";
            else
            {
                for (auto it : rows)
                    if (node->children[i]->value.value == it->name)
                    {
                        currType = it->type;
                        break;
                    }
            }
        if (currType != identifierType && currType != "")
        {
            if (node->children[0]->value.tokenType != "type")
            {
                string error = + "Error : Type  " + identifierType + node->children[0]->value.value + " identifier cannot be assigned " + currType + " " + " type on line " + to_string(node->children[0]->value.line) + "\n";
                errors.push_back(error);
            }
            else
            {
                string error = + "Error : Type " + identifierType + node->children[0]->value.value + " identifier cannot be initialized with " + currType + " type on line " + to_string(node->children[0]->value.line) + "\n";
                errors.push_back(error);
            }
            break;
        }

    }
    for (int i = 0; i < node->nrChildren; i++)
        if (node->children[i]->value.tokenType == "identifier")
        {
            node->children[i]->value.isInitialized = true;
            break;
        }
}

void AST::checkComparistonAndOppForTypeErr(ASTNode * node)
{
    vector<string> stmtTypes;

    for (int i = 0; i < node->nrChildren; i++)
    {
        string currType = "";
        if (node->children[i]->value.tokenType == "string literal")
            currType = "string";
        else
            if (node->children[i]->value.tokenType == "integer literal")
                currType = "int";
            else
            {
                for (auto it : rows)
                    if (node->children[i]->value.value == it->name)
                        currType = it->type;
            }
        if (currType != "")
        {
            stmtTypes.push_back(currType);
            if (stmtTypes.size() > 1 && stmtTypes[stmtTypes.size() - 1] != stmtTypes[stmtTypes.size() - 2])
                cout << "Error : types " << stmtTypes[stmtTypes.size() - 2] << " and " << stmtTypes[stmtTypes.size() - 1] << " cannot be compared and no operations between them can be made" << "on line " << node->value.line << "\n";
        }
    }
}

void AST::checkMultipleDeclarations()
{
    vector<string> identifiers;
    for(auto it : rows)
    {
        if (find(identifiers.begin(), identifiers.end(), it->name) != identifiers.end())
            cout << "Error : identifier " << it->name << " declared multiple times\n";
        else
            identifiers.push_back(it->name);
    }
}