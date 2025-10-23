#ifndef TYPECHECKER_H
#define TYPECHECKER_H

#include<unordered_map>
#include<string>
#include<vector>

#include "types.h"
#include "../parser/ast.h"

class TypeChecker{
    private:
    // first we need a symbol table which stores a stack of all scopes
    // this way when we check variables in a scope we check first in the local scope
    // then we move forward scope by scope from back side to check for variables 
    // and if not foud in any variable we throw an error
    // but here we are just implementing global scope and no lexical scoping to keep it simple
    std::unordered_map<std::string, TypeTag> symtab;

    // accumulated error list
    std::vector<std::string> errors;

    // helpers
    void reportError(int line, int col, const std::string& msg);
    void clearSymbolTable();

    // AST visitors / checkers
    void checkControlBlock(const ControlNode* control);
    void checkStatement(const StatementNode* statement);
    void checkVarDecl(const VarDeclNode* decl);
    void checkAssignment(const AssignmentNode* assign);
    void checkIf(const IfNode* node);

    // inference : returns inferred type or TYPE_ERROR
    TypeTag inferExpression(const ExpressionNode* expr);
    TypeTag inferTerm(const TermNode* term);
    TypeTag inferFactor(const FactorNode* factor);

    // helpers for binary ops
    bool isNumeric(TypeTag t);
    TypeTag numericWiden(TypeTag a, TypeTag b); // if int + float => float

    public:
    TypeChecker();

    // Entry point
    // return true if no semantic errors
    bool checkProgram(const ProgramNode* program);
    const std::vector<std::string> & getErrors();
};

#endif // TYPECHECKER_H
