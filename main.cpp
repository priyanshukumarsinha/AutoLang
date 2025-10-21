#include<bits/stdc++.h>
#include "lexer/lexer.h"

int main(){
    std::string input = R"(
    control speedControl {
        float speed;
        int rpm;
        bool brakePressed;
        set speed 80.5;
        set rpm 3000;
        set brakePressed true;
        if(brakePressed == true){
            set speed speed-10.5;
        }
    )";


    Lexer lexer(input);
    Token token = lexer.getNextToken();
    std::cout << "ID\t\t" << "TokenType\t\t" << "Line[Col]\t\t" << "Symbol\t\t" << std::endl;
    int idx = 0;
    while(token.type != TokenType::EOF_TOKEN){
        std::cout << idx << "\t" 
                << tokenTypeToString(token.type) << "\t" 
                << token.line << "[" << token.col << "]\t";

        // Print literal value if exists
        if (std::holds_alternative<int>(token.value))
            std::cout << std::get<int>(token.value);
        else if (std::holds_alternative<float>(token.value))
            std::cout << std::get<float>(token.value);
        else if (std::holds_alternative<bool>(token.value))
            std::cout << std::boolalpha << std::get<bool>(token.value);
        else
            std::cout << "-";

        std::cout << std::endl;

        token = lexer.getNextToken();
        idx++;
    }

    // Print errors if any
    const auto& errors = lexer.getErrors();
    if (!errors.empty()) {
        std::cout << "\nLexical Errors:\n";
        for (const auto& e : errors) std::cout << e << "\n";
    }

    return 0;
}