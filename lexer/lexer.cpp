#include "lexer.h"
#include <unordered_map>
#include <vector>

Lexer::Lexer(const std::string &src){
    input = src;
    pos = 0;
    line = 1;
    col = 1;
}

// static keyword map : singleton
const std::unordered_map<std::string, TokenType> Lexer::keywords = {
    {"control", TokenType::CONTROL},
    {"set", TokenType::SET},
    {"if", TokenType::IF},
    {"int", TokenType::INT_TYPE},
    {"float", TokenType::FLOAT_TYPE},
    {"bool", TokenType::BOOL_TYPE},
    {"true", TokenType::BOOL_LITERAL},
    {"false", TokenType::BOOL_LITERAL}
};

// getNextToken api
Token Lexer::getNextToken(){
    Token t;
    return t;
}

// getErrors api
const std::vector<std::string>& Lexer::getErrors(){
    return errors;
}