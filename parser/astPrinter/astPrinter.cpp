#include <iostream>
#include <variant>
#include <string>
#include <memory>
#include "astPrinter.h"
#include "../../lexer/token.h"

void printBranch(int level){
    for(int i=0; i<level; i++){
        std::cout << "|\t";
    }
    std::cout << "|-";
}

void printIdentifier(const IdentifierNode * ident){
    std::cout << "identifier : " << ident->identifier << "\n";
}

void printLiteral(const LiteralNode* literal){
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

void printFactor(const FactorNode* factor, int level){
    printBranch(level);
    std::cout << "factor : ";
    if(auto ident = dynamic_cast<const IdentifierNode*> (factor)){
        printIdentifier(ident);
    }
    else if(auto literal = dynamic_cast<const LiteralNode*> (factor)){
        printLiteral(literal);
    }
    else{
        auto paren = dynamic_cast<const ParenExpressionNode*> (factor);
        std::cout << "\n";
        printExpression(paren->expression.get(), level+1);
    }
}


void printTerm(const TermNode * term, const std::string& msg, int level){
    printBranch(level);
    std::cout << msg <<"Term\n";
    printFactor(term->factor.get(), level+1);
}

void printExpression(const ExpressionNode * expression,int level){
    printBranch(level);

    std::cout << "expression\n";
    // print left term
    printTerm(expression->left.get(), "Left", level+1);

    if(expression->right.get()){
        printBranch(level+1);
        std::cout << "op : " << tokenTypeToString(expression->op);
        std::cout << "\n";
        printTerm(expression->right.get(), "Right", level+1);
    }

}

void printCondition(const ConditionNode * condition, int level){
    // print left term
    printExpression(condition->left.get(), level);
    printBranch(level);
    std::cout << "op : " << tokenTypeToString(condition->comparisonOp);
    std::cout << "\n";

    printExpression(condition->right.get(), level);
}

void printValDeclNode(const VarDeclNode* decl, int level){
    std::cout << "varDeclNode\n";
    printBranch(level);
    std::cout << "type : " << tokenTypeToString(decl->type) << "\n";
    printBranch(level);
    std::cout << "identifier : " << decl->identifier << "\n";
}

void printAssignmentNode(const AssignmentNode* assign, int level){
std::cout << "assignmentNode (set) \n";
    printBranch(level);
    std::cout << "identifier : " << assign->identifier << "\n";

    // print expression
    // std::cout << "expression : " << assign->expression.get() << "\n";
    printExpression(assign->expression.get(), level); 
}

void printIfNode(const IfNode* ifNode, int level){
    std::cout << "ifNode \n";
        printBranch(level);

        // print condition
        std::cout << "condition\n";
        printCondition(ifNode->condition.get(), level+1);

        // print statements
        const auto& statements = ifNode->statements;
        for(const auto& statement : statements){
            printStatement(statement.get(), level);
        }
}

void printStatement(const StatementNode * statement, int level){
    printBranch(level);
    std::cout << "statement : ";
    level++;
    if(auto decl = dynamic_cast<const VarDeclNode*>(statement)){
        printValDeclNode(decl, level);
    }
    else if(auto assign = dynamic_cast<const AssignmentNode*>(statement)){
        printAssignmentNode(assign, level);
    }
    else{
        auto ifNode = dynamic_cast<const IfNode*>(statement);
        printIfNode(ifNode, level);
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
