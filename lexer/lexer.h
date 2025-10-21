#ifndef LEXER_H
#define LEXER_H
#include "token.h"
#include<string>
#include<vector>
#include<unordered_map>

class Lexer{
private:
    std::string input;
    size_t pos;
    int line, col;
    
    std::vector<std::string>errors;

    static const std::unordered_map<std::string, TokenType> keywords;

    char peek(int k); // k is a lookahead
    char advance();
    void skipWhiteSpaceorComments();

    Token lexIdentifier();
    Token lexNumber();
    // Token lexBoolLiteral(); // already taken care in lexidentifier

    // utility functions
    bool isAlpha(char c);
    bool isDigit(char c);
    bool isAlnum(char c);

    void reportError(const std::string& message);

public:
    Lexer(const std::string& src);
    Token getNextToken();
    const std::vector<std::string>& getErrors();
};


#endif // LEXER_H