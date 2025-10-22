#include<bits/stdc++.h>
#include "lexer/lexer.h"
#include "parser/parser.h"
#include "parser/ast.h"
#include <string>

#include "parser/astPrinter/astPrinter.h"
#include "lexer/symbol_table_printer/symbol_table_printer.h"


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

    int choice = 0;
    do {
        std::cout << "\n==============================\n";
        std::cout << "        MENU OPTIONS\n";
        std::cout << "==============================\n";
        std::cout << "1. Print Tokens (Symbol Table)\n";
        std::cout << "2. Print AST Tree\n";
        std::cout << "3. Exit\n";
        std::cout << "==============================\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;
        switch (choice) {
            case 1:
                try {
                    printTokens(input);
                } catch (const std::exception& e) {
                    std::cerr << "Error: " << e.what() << "\n";
                }
                break;

            case 2:
                try {
                    Lexer lexer(input);
                    Parser parser(lexer);
                    auto program = parser.parseProgram();

                    if (!parser.getErrors().empty()) {
                        std::cout << "Errors:\n";
                        for (const auto& err : parser.getErrors())
                            std::cout << err << "\n";
                    } else {
                        printProgram(program.get());
                    }
                } catch (const std::exception& e) {
                    std::cerr << "Error: " << e.what() << "\n";
                }
                break;

            case 3:
                std::cout << "Exiting program.\n";
                break;

            default:
                std::cout << "Invalid choice. Try again.\n";
        }
    } while (choice != 3);

    return 0;
}