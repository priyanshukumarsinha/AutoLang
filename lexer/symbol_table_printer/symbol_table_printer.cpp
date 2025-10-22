#include <bits/stdc++.h>
#include "../lexer.h"
#include "../../parser/parser.h"
#include "../../parser/ast.h"
#include "../../parser/astPrinter/astPrinter.h"

using namespace std;

// -----------------------------------------------------
// Function: printTokens
// -----------------------------------------------------
void printTokens(const std::string& input) {
    Lexer lexer(input);
    Token token = lexer.getNextToken();

    std::cout << "ID\t\t" 
              << "TokenType\t\t" 
              << "Line[Col]\t\t" 
              << "Symbol\t\t" << std::endl;

    int idx = 0;
    while (token.type != TokenType::EOF_TOKEN) {
        std::cout << idx << "\t\t"
                  << tokenTypeToString(token.type) << "\t\t"
                  << token.line << "[" << token.col << "]\t\t\t";

        // Print literal or lexeme
        if (std::holds_alternative<int>(token.value))
            std::cout << std::get<int>(token.value);
        else if (std::holds_alternative<float>(token.value))
            std::cout << std::get<float>(token.value);
        else if (std::holds_alternative<bool>(token.value))
            std::cout << std::boolalpha << std::get<bool>(token.value);
        else
            std::cout << token.lexeme;

        std::cout << std::endl;

        token = lexer.getNextToken();
        idx++;
    }

    // Print lexical errors, if any
    const auto& errors = lexer.getErrors();
    if (!errors.empty()) {
        std::cout << "\nLexical Errors:\n";
        for (const auto& e : errors)
            std::cout << e << "\n";
    }
}