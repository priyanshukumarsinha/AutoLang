#ifndef AST_PRINTER_H
#define AST_PRINTER_H

#include <iostream>
#include <string>
#include <variant>
#include <memory>
#include "../ast.h"   // assuming all node types are defined here
#include "../../lexer/token.h" // for tokenTypeToString()

// Function declarations
void printBranch(int level);

void printIdentifier(const struct IdentifierNode* ident);
void printLiteral(const struct LiteralNode* literal);

void printFactor(const struct FactorNode* factor, int level);
void printTerm(const struct TermNode* term, const std::string& msg, int level);
void printExpression(const struct ExpressionNode* expression, int level);
void printCondition(const struct ConditionNode* condition, int level);

void printValDeclNode(const struct VarDeclNode* decl, int level);
void printAssignmentNode(const struct AssignmentNode* assign, int level);
void printIfNode(const struct IfNode* ifNode, int level);

void printStatement(const struct StatementNode* statement, int level);
void printControlBlock(const struct ControlNode* controlBlock, int level);
void printProgram(const struct ProgramNode* program);

#endif // AST_PRINTER_H
