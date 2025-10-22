#include<bits/stdc++.h>
#include "lexer/lexer.h"
#include "parser/parser.h"
#include "parser/ast.h"
#include <string>

#include "parser/astPrinter/astPrinter.h"

int main(int argc, char*argv[]){
    if(argc!=2){
        std::cerr << "Usage: " << argv[0] << " <filename>\n";
        return 1;
    }

    std::string filename = argv[1];
    std::ifstream file(filename);
    if(!file){
        std::cerr << "ERROR :: FILE NOT FOUND :: " << filename << std::endl;
        return 1;
    }
    std::stringstream buffer;
    buffer << file.rdbuf(); // read entire file
    std::string input = buffer.str();


    // Lexer lexer(input);
    // Token token = lexer.getNextToken();
    // std::cout << "ID\t\t" << "TokenType\t\t" << "Line[Col]\t\t" << "Symbol\t\t" << std::endl;
    // int idx = 0;
    // while(token.type != TokenType::EOF_TOKEN){
    //     std::cout << idx << "\t\t" 
    //             << tokenTypeToString(token.type) << "\t\t" 
    //             << token.line << "[" << token.col << "]\t\t\t";

    //     // Print literal value if exists
    //     if (std::holds_alternative<int>(token.value))
    //         std::cout << std::get<int>(token.value);
    //     else if (std::holds_alternative<float>(token.value))
    //         std::cout << std::get<float>(token.value);
    //     else if (std::holds_alternative<bool>(token.value))
    //         std::cout << std::boolalpha << std::get<bool>(token.value);
    //     else
    //         // if must have a lexeme
    //         std::cout << token.lexeme;

    //     std::cout << std::endl;

    //     token = lexer.getNextToken();
    //     idx++;
    // }

    // // Print errors if any
    // const auto& errors = lexer.getErrors();
    // if (!errors.empty()) {
    //     std::cout << "\nLexical Errors:\n";
    //     for (const auto& e : errors) std::cout << e << "\n";
    // }


   try {
        Lexer lexer(input);
        Parser parser(lexer);
        auto program = parser.parseProgram();

        if (!parser.getErrors().empty()) {
            std::cout << "Errors:\n";
            for (const auto& err : parser.getErrors())
                std::cout << err << "\n";
        } else {
            // gets points to the ProgramNode
            printProgram(program.get());
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
    }

    return 0;
}