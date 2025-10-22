#ifndef PARSER_H
#define PARSER_H

#include "lexer/lexer.h"
#include "ast.h"

class Parser{
private:
    Lexer &lexer;
    Token currentToken;
    std::vector<std::string>errors;

    void raiseError(const std::string& msg);

    std::unique_ptr<ControlNode> parseControlBlock();
    std::unique_ptr<StatementNode> parseStatement();
    std::unique_ptr<VarDeclNode> parseVarDecl();
    std::unique_ptr<AssignmentNode> parseAssignment();
    std::unique_ptr<IfNode> parseIfStatement();
    std::unique_ptr<ExpressionNode> parseExpression();
    std::unique_ptr<TermNode>parseTerm();
    std::unique_ptr<FactorNode>parseFactor();
    std::unique_ptr<ConditionNode>parseCondition();

    void expect(TokenType tok);
    void advance();
public:
    Parser(Lexer &lexer);
    std::unique_ptr<ProgramNode> parseProgram();
    const std::vector<std::string> & getErrors();
};

#endif // PARSER_H