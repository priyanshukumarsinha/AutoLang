#include<bits/stdc++.h>
#include "lexer/lexer.h"
#include "parser/parser.h"
#include "parser/ast.h"
#include <string>

void printExpression(const ExpressionNode * expression, int level);

void printBranch(int level){
    for(int i=0; i<level; i++){
        std::cout << "|\t";
    }
    std::cout << "|-";
}

void printIdentifier(const IdentifierNode * ident){
    std::cout << "identifier : " << ident->identifier << "\n";
}


void printFactor(const FactorNode* factor, int level){
    printBranch(level);
    std::cout << "factor : ";
    if(auto ident = dynamic_cast<const IdentifierNode*> (factor)){
        printIdentifier(ident);
    }
    else if(auto literal = dynamic_cast<const LiteralNode*> (factor)){
        std::cout << "literal : ";
        std::cout << tokenTypeToString(literal->literalType) << " [";

        // for literalValue (int, float, bool)
        if (std::holds_alternative<int>(literal->literalValue))
            std::cout << std::get<int>(literal->literalValue);
        else if (std::holds_alternative<float>(literal->literalValue))
            std::cout << std::get<float>(literal->literalValue);
        else if (std::holds_alternative<bool>(literal->literalValue))
            std::cout << (std::get<bool>(literal->literalValue) ? "true" : "false");

        std::cout << "]\n";
    }
    else{
        auto paren = dynamic_cast<const ParenExpressionNode*> (factor);
        std::cout << "\n";
        printExpression(paren->expression.get(), level+1);
    }
}


void printTerm(const TermNode * term, const std::string& msg, int level){
    for(int i=0; i<level; i++){
        std::cout << "|\t";
    }
    std::cout << "|-";
    std::cout << msg <<"Term\n";
    printFactor(term->factor.get(), level+1);
}

void printExpression(const ExpressionNode * expression,int level){
    // std::cout << "|\t|\t|\n";
    // std::cout << "|\t|\t|-";
    for(int i=0; i<level; i++){
        std::cout << "|\t";
    }
    std::cout << "|-";

    std::cout << "expression\n";
    // print left term
    printTerm(expression->left.get(), "Left", level+1);

    if(expression->right.get()){
        for(int i=0; i<level+1; i++){
            std::cout << "|\t";
        }
        std::cout << "|-";
        std::cout << "op : " << tokenTypeToString(expression->op);
        std::cout << "\n";
        printTerm(expression->right.get(), "Right", level+1);
    }

}

void printCondition(const ConditionNode * condition, int level){
    // print left term
    printExpression(condition->left.get(), level);
    for(int i=0; i<level; i++){
            std::cout << "|\t";
        }
    std::cout << "|-";
    std::cout << "op : " << tokenTypeToString(condition->comparisonOp);
    std::cout << "\n";

    printExpression(condition->right.get(), level);
}

void printStatement(const StatementNode * statement, int level){
    for(int i=0; i<level; i++){
        std::cout << "|\t";
    }
    std::cout << "|-";

    std::cout << "statement : ";
    level++;
    if(auto decl = dynamic_cast<const VarDeclNode*>(statement)){
        std::cout << "varDeclNode\n";

        for(int i=0; i<level; i++){
           std::cout << "|\t";
        }
        std::cout << "|-";


        std::cout << "type : " << tokenTypeToString(decl->type) << "\n";
        for(int i=0; i<level; i++){
           std::cout << "|\t";
        }
        std::cout << "|-";
        std::cout << "identifier : " << decl->identifier << "\n";

    }
    else if(auto assign = dynamic_cast<const AssignmentNode*>(statement)){
        std::cout << "assignmentNode (set) \n";
        for(int i=0; i<level; i++){
           std::cout << "|\t";
        }
        std::cout << "|-";
        std::cout << "identifier : " << assign->identifier << "\n";

        // print expression
        // std::cout << "expression : " << assign->expression.get() << "\n";
        printExpression(assign->expression.get(), level);        
    }
    else{
        auto ifNode = dynamic_cast<const IfNode*>(statement);
        std::cout << "ifNode \n";
        for(int i=0; i<level; i++){
           std::cout << "|\t";
        }
        std::cout << "|-";
        // print condition
        std::cout << "condition\n";
        printCondition(ifNode->condition.get(), level+1);

        // print statements
        const auto& statements = ifNode->statements;
        for(const auto& statement : statements){
            printStatement(statement.get(), level);
        }
        std::cout << "\n"; //temporary

    }
}

void printControlBlock(const ControlNode * controlBlock, int level){
    std::cout << "|\n";
    std::cout << "|- ";
    std::cout << "controlBlock : " << controlBlock->name << "\n";
    const auto& statements = controlBlock->statements;
    for(const auto& statement : statements){
        printStatement(statement.get(), level+1);
    }
}

void printProgram(const ProgramNode * program){
    std::cout << "Program\n";
    int level = 0;
    // printControlBlocks from 
    // std::vector<std::unique_ptr<struct ControlNode>> controlBlocks;
    const auto& controlBlocks = program->controlBlocks;
    for(const auto& controlBlock : controlBlocks){
        printControlBlock(controlBlock.get(), level);
    }
}

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