#include "parser.h"
#include <sstream>
#include "../lexer/lexer.h"
#include <unordered_set>
#include <iostream>

Parser::Parser(Lexer &lexer) : lexer(lexer) {
    currentToken = lexer.getNextToken();
}

void Parser::raiseError(const std::string &message){
    std::ostringstream oss;
    oss << "Line " << currentToken.line << ", Col " << currentToken.col << "; " << message;
    errors.push_back(oss.str());
}

// getErrors api
const std::vector<std::string>& Parser::getErrors(){
    return errors;
}

void Parser::advance(){
    currentToken = lexer.getNextToken();
}

void Parser::expect(TokenType type){
    if(currentToken.type == type){
        advance();
    }
    else {
        raiseError("Expected token " + tokenTypeToString(type) +
                   ", but got " + tokenTypeToString(currentToken.type));
    }
}

const std::unordered_set<TokenType> dataTypes = {
    TokenType::INT_TYPE,
    TokenType::FLOAT_TYPE,
    TokenType::BOOL_TYPE
};

std::unique_ptr<VarDeclNode> Parser::parseVarDecl(){
    // datatype already checked
    TokenType type = currentToken.type;

    // next we get identifier
    advance();
    if(currentToken.type != TokenType::IDENTIFIER){
        raiseError("Expected identifier in declaration");
        return nullptr;
    }
    std::string name = currentToken.lexeme;
    int nameLine = currentToken.line;
    int nameCol = currentToken.col;
    // one last thing to check here is semicolon
    advance();
    expect(TokenType::SEMICOLON);
    // even if there is no semicolon it parses it, throwing an error

    auto decl = std::make_unique<VarDeclNode> ();
    decl->identifier = name;
    decl->type = type;
    decl->line = nameLine;
    decl->col = nameCol;

    return decl;
}

const std::unordered_set<TokenType> literalTypes = {
    TokenType::INT_LITERAL,
    TokenType::FLOAT_LITERAL,
    TokenType::BOOL_LITERAL
};

std::unique_ptr<FactorNode> Parser::parseFactor(){
    // could be IDENTIFIER | literal | "(" expression ")" 
    if(currentToken.type == TokenType::IDENTIFIER){
        auto node = std::make_unique<IdentifierNode> ();
        node->identifier = currentToken.lexeme;
        node->col = currentToken.col;
        node->line = currentToken.line;
        advance();
        return node;
    }
    else if(literalTypes.count(currentToken.type)){
        auto node = std::make_unique<LiteralNode> ();
        node->literalType = currentToken.type;
        node->literalValue = currentToken.value;
        node->line = currentToken.line;
        node->col = currentToken.col;
        advance();
        return node;
    }
    else if(currentToken.type == TokenType::LPARABRACE){
        advance(); // consume "("
        auto expr = parseExpression();
        expect(TokenType::RPARABRACE);

        auto node = std::make_unique<ParenExpressionNode> ();
        node->expression = std::move(expr);
        return node;
    }
    else{
        raiseError("Unexpected token in factor: " + currentToken.lexeme);
        advance();
        return nullptr;
    }
}

std::unique_ptr<TermNode> Parser::parseTerm(){
    auto factor = parseFactor();
    auto term = std::make_unique<TermNode>();
    term->factor = std::move(factor);
    return term;
}

std::unique_ptr<ExpressionNode> Parser::parseExpression(){
    // first we get left term
    auto leftTerm = parseTerm();
    if(!leftTerm){
        // recover gracefully: create an empty expression (or return nullptr)
        raiseError("Expected expression term");
        return nullptr;
    }
    auto expr = std::make_unique<ExpressionNode> ();
    expr->left = std::move(leftTerm);
    // next we get operator : PLUS, MINUS
    TokenType op = TokenType::TOKEN_UNKNOWN;
    expr->right = nullptr;

    // while we have + or -
    while(currentToken.type == TokenType::SYM_PLUS || currentToken.type == TokenType::SYM_MINUS){
        TokenType op = currentToken.type;
        // int opLine = currentToken.line;
        // int opCol  = currentToken.col;

        // consume op
        advance();
        auto rightTerm = parseTerm();

        // if no rightTerm that means its a literal or identifier
        // which means op = Unknown and rightTerm = nullptr
        if(!rightTerm){
            // syntax error: operator without an operand
            raiseError("Expected term after '+' or '-'");
            // best effort: stop parsing expression and return what we have
            break;
        }

        // create a new expression node where current expr becomes the left (wrap)
        auto newExpr = std::make_unique<ExpressionNode>();
        // left of newExpr is a TermNode that wraps the previous expression
        // i.e expression processed till now
        auto wrapperFactor  = std::make_unique<ParenExpressionNode> ();
        wrapperFactor->expression = std::move(expr);

        auto wrapperTerm = std::make_unique<TermNode> ();
        wrapperTerm->factor = std::move(wrapperFactor);

        newExpr->left = std::move(wrapperTerm);
        newExpr->op = op;
        
        // now for right part
        auto rightWrapperTerm = std::move(rightTerm);
        newExpr->right = std::move(rightWrapperTerm);

        expr = std::move(newExpr);
    }    

    return expr;
}

std::unique_ptr<AssignmentNode> Parser::parseAssignment(){
    // "set" already checked
    advance();

    // now comes identifier
    if(currentToken.type != TokenType::IDENTIFIER){
        raiseError("Expected identifier in assignment");
        return nullptr;
    }

    std::string name = currentToken.lexeme;
    int nameLine = currentToken.line;
    int nameCol = currentToken.col;
    
    // next we check for expression
    advance();
    auto expr = parseExpression();

    expect(TokenType::SEMICOLON);
    
    auto assignment = std::make_unique<AssignmentNode>();
    assignment->expression = std::move(expr);;
    assignment->identifier = name;
    assignment->line = nameLine;
    assignment->col = nameCol;
    
    return assignment;
}

std::unique_ptr<ConditionNode> Parser::parseCondition(){
    auto leftExpr = parseExpression();

    std::unique_ptr<ExpressionNode> rightExp;

    if (currentToken.type != TokenType::SYM_GREATER && currentToken.type != TokenType::EQUAL_EQUAL) {
        raiseError("Expected comparison operator '>' or '=='");
        return nullptr;
    }

    TokenType op = currentToken.type;
    advance();

    auto rightExpr = parseExpression();

    auto cond = std::make_unique<ConditionNode>();
    cond->left = std::move(leftExpr);
    cond->comparisonOp = op;
    cond->right = std::move(rightExpr);
    return cond;
}

std::unique_ptr<IfNode> Parser::parseIfStatement(){
    // this means currentToken = IF
    advance();
    expect(TokenType::LPARABRACE);

    auto condition = parseCondition();
    expect(TokenType::RPARABRACE);
    expect(TokenType::LCURLYBRACE);

    auto ifnode = std::make_unique<IfNode>();
    ifnode->condition = std::move(condition);

    while(currentToken.type != TokenType::RCURLYBRACE && currentToken.type != TokenType::EOF_TOKEN){

        auto stmt = parseStatement();
        ifnode->statements.push_back(std::move(stmt));
    }


    expect(TokenType::RCURLYBRACE);

    return ifnode;
}

std::unique_ptr<StatementNode> Parser::parseStatement(){
    // VarDeclNode
    // expecting dataType

    
    if(dataTypes.count(currentToken.type)){
        return parseVarDecl();
    }
    
    // AssignmentNode
    // if "set" parse assignment
    else if(currentToken.type == TokenType::KW_TOKEN_SET){
        return parseAssignment();
    }

    // ifNode
    else if(currentToken.type == TokenType::KW_TOKEN_IF){
        return parseIfStatement();
    }
    else {
        raiseError("Unexpected token in statement: " + currentToken.lexeme);
        advance();
        return nullptr;
    }
}

std::unique_ptr<ControlNode> Parser::parseControlBlock(){
    
    // expecting "control"
    expect(TokenType::KW_CONTROL);
    
    // next is name (identifier)
    if (currentToken.type != TokenType::IDENTIFIER) {
        raiseError("Expected control block name");
        return nullptr;
    }
    
    // if name is there
    std::string name = currentToken.lexeme;
    advance();
    
    // next we expect "{"
    expect(TokenType::LCURLYBRACE);
    
    // next 
    auto control = std::make_unique<ControlNode>();

    control->name = name;

    // expecting statements until "}" i.e RCURLYBRACE or EOF
    while(currentToken.type != TokenType::RCURLYBRACE && currentToken.type!= TokenType::EOF_TOKEN){
        // parse statements
        auto stmt = parseStatement();
        if(stmt){
            control->statements.push_back(std::move(stmt));
        }
    }

    // next expect RCURLYBRACE
    expect(TokenType::RCURLYBRACE);

    // all done
    return control;
}


std::unique_ptr<ProgramNode> Parser::parseProgram(){
    auto program = std::make_unique<ProgramNode>();
    // since we have to return a unique_ptr we make it unique
    
    // parse all controlBlocks
    while(currentToken.type == TokenType::KW_CONTROL){
        // parseControlBlock now
        auto control = parseControlBlock();
        if(control){
            program->controlBlocks.push_back(std::move(control));
        }
    }
    
    if(currentToken.type != TokenType::EOF_TOKEN){
        raiseError("Unexpected token at the end of program");
    }

    return program;

}


