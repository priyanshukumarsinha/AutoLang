#ifndef AST_H
#define AST_H

#include <string>
#include <vector>
#include <memory>
#include <variant>
#include "../lexer/token.h"

// Base class for all AST Nodes
// it makes it easier to keep different types of Nodes 
// like for statement, statement := declaration | assignment | if_statement
// now we need to store any of these three
// one option is to use variant
// but we are going to implement Base Node option here
// This is like S (Start Symbol)
// S:= ProgramNode

struct ASTNode;
struct ProgramNode;
struct ControlNode;
struct StatementNode;
struct VarDeclNode;
struct AssignmentNode;
struct ExpressionNode;
struct TermNode;
struct FactorNode;
struct IdentifierNode;
struct LiteralNode;
struct ParenExpressionNode;
struct IfNode;
struct ConditionNode;

struct ASTNode{
    virtual ~ASTNode() = default;
    // somehow it helps in cleanup when deleting derived classes such as declaration, assignment, etc...
};

// ProgramNode:= { ControlNode }
struct ProgramNode : ASTNode{
    // there can be more than one controlnodes
    // hence vector, but yeah not vector of ControlNodes
    // Not vector<ControlNodes>
    // we need to store pointer of these 
    // that too unique_ptr, don't know why
    std::vector<std::unique_ptr<struct ControlNode>> controlBlocks;
};

// ControlNode:= "control" IDENTIFIER "{" { statement } "}"
struct ControlNode : ASTNode{
    // IDENTIFIER is name of the control block
    std::string name;
    
    // and a vector of all the statements
    std::vector<std::unique_ptr<struct StatementNode>> statements;
};

// StatementNode:= VarDeclNode | AssignmentNode | IfNode
// Now again we have a base class for all these derived classes
struct StatementNode : ASTNode{
    virtual ~StatementNode() = default;
};

// VarDeclNode:= TYPE IDENTIFIER ";"
struct VarDeclNode : StatementNode{
    TokenType type; // INT_TYPE, FLOAT_TYPE, BOOL_TYPE
    std::string identifier;
    int line = 0, col = 0;
};

// AssignmentNode := "set" IDENTIFIER expression ";"
struct AssignmentNode : StatementNode{
    std::string identifier;
    std::unique_ptr<struct ExpressionNode> expression;
    int line = 0, col = 0; // position of `set` or identifier
};

// ExpressionNode := TermNode op TermNode
// can be implemented using a binary tree
struct ExpressionNode : ASTNode{
    std::unique_ptr<struct TermNode> left;
    TokenType op;
    std::unique_ptr<struct TermNode> right;
};

// TermNode:= FactorNode
struct TermNode : ASTNode{
    std::unique_ptr<struct FactorNode> factor;
};

// FactorNode:= IDENTIFIER | literal | "(" expression ")" 
struct FactorNode {
    virtual ~FactorNode() = default;
};

struct IdentifierNode : FactorNode {
    std::string identifier;
    int line = 0, col = 0;
};

struct LiteralNode : FactorNode {
    std::variant<std::monostate, int, float, bool> literalValue;
    TokenType literalType = TokenType::EOF_TOKEN;
    int line = 0, col = 0;
};

struct ParenExpressionNode : FactorNode {
    std::unique_ptr<struct ExpressionNode> expression; // for parentheses
};

// IfNode := "if" ConditionNode "{" { statement } "}"
struct IfNode : StatementNode{
    std::unique_ptr<ConditionNode> condition;
    std::vector<std::unique_ptr<StatementNode>> statements;
};


// ConditionNode := ExpressionNode comparisionOp ExpressionNode
struct ConditionNode{
    std::unique_ptr<struct ExpressionNode> left;
    TokenType comparisonOp; // GREATER or EQUAL_EQUAL
    std::unique_ptr<struct ExpressionNode> right;
};


#endif // AST_H