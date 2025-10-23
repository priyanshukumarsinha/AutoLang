#include "typechecker.h"
#include <sstream>

#include "../lexer/lexer.h"

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

