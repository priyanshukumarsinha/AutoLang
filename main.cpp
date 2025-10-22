#include<bits/stdc++.h>
#include "lexer/lexer.h"
#include "parser/parser.h"
#include "parser/ast.h"

void printIndent(int level) {
    for (int i = 0; i < level; ++i)
        std::cout << "  ";
}

void printExpression(const ExpressionNode* expr, int level);
void printFactor(const FactorNode* factor, int level);
void printStatement(const StatementNode* stmt, int level);

// Print Factor
void printFactor(const FactorNode* factor, int level) {
    if (auto id = dynamic_cast<const IdentifierNode*>(factor)) {
        printIndent(level);
        std::cout << "Identifier: " << id->identifier << "\n";
    } 
    else if (auto lit = dynamic_cast<const LiteralNode*>(factor)) {
        printIndent(level);
        std::cout << "Literal (Type=" << static_cast<int>(lit->literalType) << "): ";

        if (std::holds_alternative<int>(lit->literalValue))
            std::cout << std::get<int>(lit->literalValue);
        else if (std::holds_alternative<float>(lit->literalValue))
            std::cout << std::get<float>(lit->literalValue);
        else if (std::holds_alternative<bool>(lit->literalValue))
            std::cout << (std::get<bool>(lit->literalValue) ? "true" : "false");

        std::cout << "\n";
    } 
    else if (auto paren = dynamic_cast<const ParenExpressionNode*>(factor)) {
        printIndent(level);
        std::cout << "ParenExpression:\n";
        printExpression(paren->expression.get(), level + 1);
    }
}

// Print Expression
void printExpression(const ExpressionNode* expr, int level) {
    if (!expr) return;

    printIndent(level);
    std::cout << "Expression:\n";
    printIndent(level + 1);
    std::cout << "Left:\n";
    printFactor(expr->left->factor.get(), level + 2);

    if (expr->op != TokenType::TOKEN_UNKNOWN) {
        printIndent(level + 1);
        std::cout << "Operator: " << tokenTypeToString(expr->op) << "\n";
        printIndent(level + 1);
        std::cout << "Right:\n";
        printFactor(expr->right->factor.get(), level + 2);
    }
}

// Print Statement
void printStatement(const StatementNode* stmt, int level) {
    if (auto decl = dynamic_cast<const VarDeclNode*>(stmt)) {
        printIndent(level);
        std::cout << "VarDecl: " << tokenTypeToString(decl->type)
                  << " " << decl->identifier << "\n";
    } 
    else if (auto assign = dynamic_cast<const AssignmentNode*>(stmt)) {
        printIndent(level);
        std::cout << "Assignment: set " << assign->identifier << "\n";
        printExpression(assign->expression.get(), level + 1);
    } 
    else if (auto ifNode = dynamic_cast<const IfNode*>(stmt)) {
        printIndent(level);
        std::cout << "If Statement:\n";

        printIndent(level + 1);
        std::cout << "Condition:\n";
        printExpression(ifNode->condition->left.get(), level + 2);
        printIndent(level + 2);
        std::cout << "Comparison Op: " << tokenTypeToString(ifNode->condition->comparisonOp) << "\n";
        printExpression(ifNode->condition->right.get(), level + 2);

        printIndent(level + 1);
        std::cout << "Body:\n";
        for (const auto& s : ifNode->statements)
            printStatement(s.get(), level + 2);
    }
}

// Print Control Block
void printControl(const ControlNode* control, int level) {
    printIndent(level);
    std::cout << "Control: " << control->name << "\n";
    for (const auto& stmt : control->statements)
        printStatement(stmt.get(), level + 1);
}

// Print Program
void printProgram(const ProgramNode* program) {
    std::cout << "Program:\n";
    for (const auto& control : program->controlBlocks)
        printControl(control.get(), 1);
}

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
            set speed (speed+10.5);
        }
    }
    )";


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
            printProgram(program.get());
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
    }

    return 0;
}