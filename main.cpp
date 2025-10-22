#include<bits/stdc++.h>
#include "lexer/lexer.h"
#include "parser/parser.h"
#include "parser/ast.h"
#include <string>

#include "parser/astPrinter/astPrinter.h"
#include "lexer/symbol_table_printer/symbol_table_printer.h"

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <filename> <-s|-p>\n";
        return 1;
    }

    std::string filename = argv[1];
    std::string flag = argv[2];

    std::ifstream file(filename);
    if (!file) {
        std::cerr << "ERROR :: FILE NOT FOUND :: " << filename << std::endl;
        return 1;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string input = buffer.str();

    if (flag == "-s") {
        // Print Tokens / Symbol Table
        try {
            printTokens(input);
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << "\n";
        }
    } 
    else if (flag == "-p") {
        // Print AST Tree
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
    } 
    else {
        std::cerr << "ERROR :: Invalid option. Use -s for symbol table or -p for parse tree.\n";
        return 1;
    }

    return 0;
}
