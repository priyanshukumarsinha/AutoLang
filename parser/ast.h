#ifndef AST_H
#define AST_H

#include<string>

// Base Node
// Every other node will be a derived class of ASTNode
struct ASTNode{
    // the virtual destructor helps delete derived classes safely
    virtual ~ASTNode() = default;
};

// ProgramNode := {controlNode}
struct ProgramNode : ASTNode{
    std::vector<std::unique_ptr<ControlNode>> controlBlocks;
};

// control Engine {
//     set speed 100;
//     if (speed > 50) { ... }
// }
// ControlNode := "control" <name> {statements}
struct ControlNode : ASTNode{
    std::string name; // <name>
    std::vector<std::unique_ptr<ASTNode>> statements; // {statements}
};

using StatementVariant = std::variant<
    std::unique_ptr<VarDeclNode>,
    std::unique_ptr<SetNode>,
    std::unique_ptr<IfNode>,
>;

// Statements := VarDeclNode | setNode | ifNode | ExpressionNode
struct StatementNode : ASTNode{
    StatementVariant statement;  // only one active at a time
};

// VarDeclNode := type name;
struct VarDeclNode : ASTNode{
    TokenType type; // datatype : INT, FLOAT, BOOL
    std::string name; // variable
};

// SetNode := set <identifier> <expression>;
struct SetNode : ASTNode{
    std::string name; // <identifier>
    std::unique_ptr<ASTNode> expr; // ExpressionNode
};

// IfNode := if (ComparisionExpression) { statements }
struct IfNode : ASTNode{
    std::unique_ptr<ASTNode> condition;
    std::vector<std::unique_ptr<ASTNode>> statements;
};

// Expression := <term> {operator} <term>
struct ExpressionNode : ASTNode {
    std::string value;
    std::string op;
    std::unique_ptr<ExpressionNode> left;
    std::unique_ptr<ExpressionNode> right;
};

#endif // AST_H