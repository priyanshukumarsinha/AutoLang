#include<bits/stdc++.h>
#include "lexer.h"

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
    int idx =0;
    while(token.type != TokenType::EOF_TOKEN){
        std::cout << idx << "\t" << (tokenTypeToString(token.type)) << "\t" << token.line << "[" << token.col << "]\t" << token.lexeme<< std::endl;
        token = lexer.getNextToken();
    }


    return 0;
}