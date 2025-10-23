#include "lexer.h"
#include <unordered_map>
#include <vector>
#include <cctype>
#include <iostream>
#include <sstream>

Lexer::Lexer(const std::string &src){
    input = src;
    pos = 0;
    line = 1;
    col = 1;
}

// static keyword map : singleton
const std::unordered_map<std::string, TokenType> Lexer::keywords = {
    {"control", TokenType::KW_CONTROL},
    {"set", TokenType::KW_TOKEN_SET},
    {"if", TokenType::KW_TOKEN_IF},
    {"int", TokenType::INT_TYPE},
    {"float", TokenType::FLOAT_TYPE},
    {"bool", TokenType::BOOL_TYPE},
    {"true", TokenType::BOOL_LITERAL},
    {"false", TokenType::BOOL_LITERAL}
};

char Lexer::peek(int k){
    if(pos+k >= input.size()) return '\0';
    return input[pos+k];
}

char Lexer::advance(){
    if(pos>=input.size()) return '\0';
    char ch = input[pos++];
    if(ch == '\n'){
        line++;
        col = 1;
    }
    else{
        col++;
    }
    
    return ch;
    // else nowhere to advance, all inputs consumed already
}

void Lexer::skipWhiteSpaceorComments(){
    while(pos<input.size()){
        char c=peek(0);

        if(isspace(c)){
            // ' ','\n','\t', etc ...
            advance();
            continue;
        }

        else if(c == '#'){
            // then it must be a comment
            while(pos<input.size() && peek(0) != '\n'){
                advance();
            }
            continue;
        }

        // if not space or comments then look for other things
        break;
    }
}

bool Lexer::isAlpha(char c){
    return std::isalpha(c) || c=='_';
}

bool Lexer::isAlnum(char c){
    return std::isalnum(c) || c=='_';
}

Token Lexer::lexIdentifier(){
    int startCol = col;
    std::string value;
    while(isAlnum(peek(0))){
        value += advance();
    }

    auto it = keywords.find(value);
    if(it != keywords.end()){
        // that means its a keyword then
        TokenType type = it->second;
        if(type == TokenType::BOOL_LITERAL){
            // since bool literals are also keywords
            // i.e true and false
            return Token(type, value, line, startCol, value == "true");
        }
        return Token(type, value, line, startCol);
    }
    return Token(TokenType::IDENTIFIER, value, line, startCol);
}

bool Lexer::isDigit(char ch){
    return std::isdigit(ch);
}

Token Lexer::lexNumber(){
    int startCol = col;
    std::string value; // we store it initially in string form
    // later we will use stoi() or stof() to convert them back to numbers
    bool isFloat = 0;
    while(isDigit(peek(0)) || peek(0) == '.'){
        if(peek(0) == '.') isFloat = 1;
        value+=advance();
    }

    // what if someone give malformed number like 123.12.123;
    try{
        if(isFloat){
            return Token(TokenType::FLOAT_LITERAL, value, line, startCol, stof(value));
        }
        else{
            return Token(TokenType::INT_LITERAL, value, line, startCol, stoi(value));
        }
    }
    catch (...){
        reportError("Invalid numeric literal " + value);
        return Token(TokenType::TOKEN_UNKNOWN, "", line, startCol);
    }
}

// getNextToken api
Token Lexer::getNextToken(){
    skipWhiteSpaceorComments();

    char ch = peek(0);
    if(ch == '\0'){
        // this means we reached EOF
        return Token(TokenType::EOF_TOKEN, "", line, col);
    }

    // look for identifiers or keywords
    if(isAlpha(ch)) return lexIdentifier();

    if(isDigit(ch)) return lexNumber();

    // lastly we need to process all these symbols 
    //  { } ( ) ; == > + -
    int startCol = col;
    switch(ch){
        case '{': advance(); return Token(TokenType::LCURLYBRACE, std::string(1,ch), line, startCol);
        case '}': advance(); return Token(TokenType::RCURLYBRACE, std::string(1,ch), line, startCol);
        case '(': advance(); return Token(TokenType::LPARABRACE, std::string(1,ch), line, startCol);
        case ')': advance(); return Token(TokenType::RPARABRACE, std::string(1,ch), line, startCol);
        case ';': advance(); return Token(TokenType::SEMICOLON, std::string(1,ch), line, startCol);
        case '>': advance(); return Token(TokenType::SYM_GREATER, std::string(1,ch), line, startCol);
        case '+': advance(); return Token(TokenType::SYM_PLUS, std::string(1,ch), line, startCol);
        case '-': advance(); return Token(TokenType::SYM_MINUS, std::string(1,ch), line, startCol);
        case '=': 
            advance();
            if(peek(0) == '='){
                advance(); 
                return Token(TokenType::EQUAL_EQUAL, "==", line, startCol);
            }
            reportError("Unexpected character: =");
            return Token(TokenType::TOKEN_UNKNOWN, "=", line, startCol);
        default: break;
    }

    // For unknown tokens
    advance();
    reportError(std::string("Unknown character: ") + ch);
    return Token(TokenType::TOKEN_UNKNOWN, std::string(1, ch), line, col);
}

// for reporting errors
void Lexer::reportError(const std::string &message){
    std::ostringstream oss;
    oss << "Line " << line << ", Col " << col << "; " << message;
    errors.push_back(oss.str());
}

// getErrors api
const std::vector<std::string>& Lexer::getErrors(){
    return errors;
}