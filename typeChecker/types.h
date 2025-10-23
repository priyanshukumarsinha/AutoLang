#ifndef TYPECHECKER_TYPES_H
#define TYPECHECKER_TYPES_H

enum class TypeTag{
    TYPE_INT,
    TYPE_FLOAT,
    TYPE_BOOL,
    TYPE_ERROR
};

inline const char* typeTagToString(TypeTag t){
    switch(t){
        case TypeTag::TYPE_INT: return "int";
        case TypeTag::TYPE_FLOAT: return "float";
        case TypeTag::TYPE_BOOL: return "int";
        default: return "error";
    }
}

#endif // TYPECHECKER_TYPES_H