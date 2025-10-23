#include "typechecker.h"
#include <sstream>

#include "../lexer/lexer.h"
#include <iostream>

TypeChecker::TypeChecker(){
    // nothing to initialize
}

const std::vector<std::string> & TypeChecker::getErrors(){
    return errors;
}

void TypeChecker::reportError(int line, int col, const std::string& msg){
    std::ostringstream oss;

    // mostly line will always be >0 but just in case it isn't 
    if(line > 0)
        oss << "Line " << line << ", Col " << col << ": " << msg;
    else{
        oss << msg;
    }

    errors.push_back(oss.str());
}

void TypeChecker::clearSymbolTable(){
    symtab.clear();
}

// Now its time to implement the inferExpression part
TypeTag TypeChecker::inferExpression(const ExpressionNode* expression){
    // 
}

void TypeChecker::checkAssignment(const AssignmentNode* assign){
    // std::cout << "Its assignment \n";
    // while assignment 
    // we need to check if the assignment is declared
    // something like a lookup
    // if its not already declared we can't assign value
    if(!assign) return;
    std::string identifier = assign->identifier;
    auto it = symtab.find(identifier);
    if(it==symtab.end()){
        // means the variable is not declared
        // throw error
        reportError(assign->line, assign->col, "Undeclared variable " + identifier + " in assignment");
        return;
    }

    // else lets assign value
    // i.e put value in symtab
    TypeTag varType = it->second;

    // infer RHS
    // infer means check which datatype is RHS
    // or which TypeTag it represents
    // on the RHS we must be having an expression, acc to the grammar
    // AssignmentNode := "set" IDENTIFIER expression ";"
    // so we infer the expression

    // check exprType
    TypeTag exprType = TypeTag::TYPE_ERROR; // initially

    // now sometimes it might happen that the user doesn't provides the expression itself;
    // in this case its something like
    // a = ;
    // this should give an error
    // in this case assign->expression = nullptr
    if(assign->expression){
        // we will get the exprType from inferExpression()
        // exprType = inferExpression(assign->expression.get());
    }
    else{
        // reportError
        reportError(assign->line, assign->col, "Empty expression in assignment to '" + identifier + "'");
        return;
    }

    // Now since we have the type of exprType i.e int, float, bool or error
    // we check if its error
    if(exprType == TypeTag::TYPE_ERROR){
        // error is already pushed in inferExpression, so no repeating needed here
        return;
    }

    // now lets check if our varType and exprType match
    // we are allowing int->float widening 
    // so even if its float we can assign int values in the variable
    // BUT not vice versa
    // if varType is int we don't want to put float values in it, since then it will loose some of its values
    // int a = 1.5; is invalid in most languages, hence this
    
    // but not if int and boolean
    // so first we check 
    if(varType == exprType){
        // then its good
        return;
    }

    // widening
    else if(varType == TypeTag::TYPE_FLOAT && exprType == TypeTag::TYPE_FLOAT){
        // then also good
        return;
    }

    // BUT all other cases are a mismatch
    // hence error 
    reportError(assign->line, assign->col, 
        "Type mismatch in assignment to '" + identifier + "' : expected " +  typeTagToString(varType) + " but found " + typeTagToString(exprType)
    );

}

void TypeChecker::checkIf(const IfNode* ifnode){
    std::cout << "Its ifNode \n";
}
void TypeChecker::checkVarDecl(const VarDeclNode* decl){
    // while declaring variable
    // we need to check if it has been declared earlier (in symtab)
    // if declared earlier => reportError
    // else add to symtab

    if(!decl) return;

    // we need to check if the variable name already exists
    std::string identifier = decl->identifier;
    // check in symtab
    if(symtab.find(identifier) != symtab.end()){
        // that means the identifier already exists
        // hence reportError
        reportError(decl->line, decl->col, "Variable " + identifier + " already declared in this scope");
        return;
    }

    // if not present -> lets declare 
    // i.e add to symtab
    // first map token to TypeTag
    TypeTag tag = TypeTag::TYPE_ERROR; // initially
    // since we don't want it to be null
    // other option is to make a TYPE_UNKNOWN for now

    switch(decl->type){
        case TokenType::INT_TYPE: tag = TypeTag::TYPE_INT; break;
        case TokenType::FLOAT_TYPE: tag = TypeTag::TYPE_FLOAT; break;
        case TokenType::BOOL_TYPE: tag = TypeTag::TYPE_BOOL; break;
        default:
            reportError(decl->line, decl->col, "Unkown type in declaration for '" + identifier + "'");
    }

    // and that's how its done; welcome
    symtab[identifier] = tag;
}


void TypeChecker::checkStatement(const StatementNode* statement){
    // checks each statement
    // now statement can be of 3 types
    // VarDecl, Assignment and ifNode
    if(!statement) return;

    if(auto decl = dynamic_cast<const VarDeclNode*>(statement)){
        // if variable declaration
        checkVarDecl(decl);
    }
    else if(auto assign = dynamic_cast<const AssignmentNode*>(statement)){
        // if Assignment 
        checkAssignment(assign);
    }
    else if(auto ifnode = dynamic_cast<const IfNode*>(statement)){
        // if ifNode
        checkIf(ifnode);
    }
    else{
        // unknown statement type - shouldn't happen generally 
        // but lets consider
        reportError(0,0, "Unknown statement node encountered in typechecker");
    }
}

void TypeChecker::checkControlBlock(const ControlNode* control){
    // checks all statements in control block
    // each control block has its own symbol table
    // so whenever we checkControlBlock first we clear the symbol table
    if(!control) return;
    clearSymbolTable();

    // next we iterate statements; and check for declarations
    // declarations fill symtab (symbol table)
    
    for(const auto &statement : control->statements){
        checkStatement(statement.get());
    }
}

bool TypeChecker::checkProgram(const ProgramNode * program){
    errors.clear();
    if(!program){
        reportError(0,0,"Null AST passed to TypeChecker");
        return false;
    }

    
    // For each controlBlock use a fresh symbol table (scoped per control)
    // std::cout << program->controlBlocks.size();

    for(const auto& controlBlock:program->controlBlocks){
        checkControlBlock(controlBlock.get());
    }

    // if no error , then return true
    //  if error.empty() return true
    // else if there are erros return false
    return errors.empty();
}
