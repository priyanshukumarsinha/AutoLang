#ifndef TOKEN_H
#define TOKEN_H

#include<string>
#include <variant>

enum class TokenType{
    // keywords
    KW_CONTROL, KW_TOKEN_SET, KW_TOKEN_IF,

    // Data types
    INT_TYPE, FLOAT_TYPE, BOOL_TYPE,

    // OPERATORS
    SYM_PLUS, SYM_MINUS, SYM_GREATER, EQUAL_EQUAL,

    // IDENTIFIERS AND LITERALS
    IDENTIFIER, INT_LITERAL, FLOAT_LITERAL, BOOL_LITERAL,

    // SYMBOLS
    LCURLYBRACE, RCURLYBRACE, LPARABRACE, RPARABRACE, SEMICOLON,

    // MISC
    EOF_TOKEN, TOKEN_UNKNOWN
};

struct Token{
    TokenType type;
    int line;
    int col;
    std::string lexeme;

    // with hasValue we won't get
    // bool hasValue; // true for literals only
    // union {
    //     int intVal;
    //     float floatVal;
    //     bool boolVal;
    // };

    std::variant<std::monostate, int, float, bool> value;

    Token(){
        type = TokenType::TOKEN_UNKNOWN;
        lexeme = "";
        line = 0;
        col = 0;
        value = std::monostate{};
    }

    Token(TokenType t, std::string lex, int ln, int c){
        type = t;
        lexeme = lex;
        line = ln;
        col = c;
        // hasValue = false;
        value = std::monostate{};
    }

    // now if we give value, it can either be int, float or bool
    // that means we have to make 3 similar constructors
    // OR ... we can just use templates

    template <typename T>
    Token(TokenType t, std::string lex, int ln, int c, T val){
        type = t;
        lexeme = lex;
        line = ln;
        col = c;
        // hasValue = false;
        value = val;
    }

};

inline std::string tokenTypeToString(TokenType type) {
    switch (type) {
        case TokenType::KW_CONTROL: return "KW_CONTROL";
        case TokenType::KW_TOKEN_SET: return "KW_TOKEN_SET";
        case TokenType::KW_TOKEN_IF: return "KW_TOKEN_IF";
        case TokenType::INT_TYPE: return "INT_TYPE";
        case TokenType::FLOAT_TYPE: return "FLOAT_TYPE";
        case TokenType::BOOL_TYPE: return "BOOL_TYPE";
        case TokenType::SYM_PLUS: return "SYM_PLUS";
        case TokenType::SYM_MINUS: return "SYM_MINUS";
        case TokenType::SYM_GREATER: return "SYM_GREATER";
        case TokenType::EQUAL_EQUAL: return "EQUAL_EQUAL";
        case TokenType::IDENTIFIER: return "IDENTIFIER";
        case TokenType::INT_LITERAL: return "INT_LITERAL";
        case TokenType::FLOAT_LITERAL: return "FLOAT_LITERAL";
        case TokenType::BOOL_LITERAL: return "BOOL_LITERAL";
        case TokenType::LCURLYBRACE: return "LCURLYBRACE";
        case TokenType::RCURLYBRACE: return "RCURLYBRACE";
        case TokenType::LPARABRACE: return "LPARABRACE";
        case TokenType::RPARABRACE: return "RPARABRACE";
        case TokenType::SEMICOLON: return "SEMICOLON";
        case TokenType::EOF_TOKEN: return "EOF_TOKEN";
        default: return "TOKEN_UNKNOWN";
    }
}

#endif