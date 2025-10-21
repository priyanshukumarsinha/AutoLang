# 🧩 **AutoLang Parser Documentation**

## **1. Introduction**

The **Parser** for *AutoLang* is responsible for converting a linear stream of tokens (produced by the Lexical Analyzer) into a structured **Abstract Syntax Tree (AST)** that represents the syntactic structure of an AutoLang program.

It validates the **grammar rules** of AutoLang to ensure that the program is syntactically correct before further semantic analysis or type checking.

The Parser follows a **recursive descent parsing** approach, as the grammar of AutoLang is simple, unambiguous, and well-suited for a top-down parser.

---

## **2. Overview of AutoLang Grammar**

The parser is based on the following simplified **context-free grammar** (without the assignment operator `=`):

```
program          → { control_block }
control_block    → "control" IDENTIFIER "{" { statement } "}"
statement        → declaration | assignment | if_statement

declaration      → TYPE IDENTIFIER ";"
assignment       → "set" IDENTIFIER expression ";"
if_statement     → "if" "(" condition ")" "{" { statement } "}"

condition        → expression comparison_op expression
expression       → term { ("+" | "-") term }
term             → factor
factor           → IDENTIFIER | literal | "(" expression ")"

comparison_op    → ">" | "=="
literal          → INT_LITERAL | FLOAT_LITERAL | BOOL_LITERAL
```

This grammar ensures that all AutoLang programs are constructed using **typed variable declarations**, **set-based assignments**, and **conditional control structures**.

---

## **3. Parser Design**

### **3.1 Parsing Technique**

The parser uses a **recursive descent** approach — a **top-down parsing** method where each grammar rule is implemented as a separate function.

For example:

* `parseProgram()` corresponds to `<program>`
* `parseControlBlock()` corresponds to `<control_block>`
* `parseExpression()` corresponds to `<expression>`

Each function consumes tokens from the input stream using a `currentToken` pointer and recursively calls other functions to build the syntax tree.

---

## **4. Parser Architecture**

### **4.1 Components**

| Component         | Description                                 |
| ----------------- | ------------------------------------------- |
| **Token Stream**  | Input tokens from the Lexer                 |
| **Parser Class**  | Main driver class that processes tokens     |
| **AST Nodes**     | Data structures representing the parse tree |
| **Error Handler** | Reports syntax errors with line numbers     |

---

## **5. Parser Workflow**

1. **Input:** A list of tokens from the Lexer
2. **Process:**

   * Validate the syntax according to AutoLang grammar
   * Build AST nodes for each construct (`control`, `set`, `if`, etc.)
3. **Output:** A structured **Abstract Syntax Tree (AST)**

---

## **6. AST Structure Overview**

The parser produces an AST with the following node types:

| AST Node           | Description                                    | Example                      |
| ------------------ | ---------------------------------------------- | ---------------------------- |
| `ProgramNode`      | Root node containing multiple control blocks   | Entire program               |
| `ControlBlockNode` | Represents `control <id> { ... }`              | `control engineCtrl { ... }` |
| `DeclarationNode`  | Represents variable declarations               | `float speed;`               |
| `AssignmentNode`   | Represents assignments using `set`             | `set speed 80.5;`            |
| `IfNode`           | Represents conditional blocks                  | `if (speed > 50) { ... }`    |
| `BinaryOpNode`     | Represents arithmetic or comparison operations | `speed + 10`                 |
| `LiteralNode`      | Represents a constant value                    | `80.5`, `true`               |
| `IdentifierNode`   | Represents a variable reference                | `speed`                      |

---

## **7. Example Parser Flow**

### **Input (Tokens):**

```
CONTROL IDENTIFIER(speedControl) LCURLYBRACE
TYPE(float) IDENTIFIER(speed) SEMICOLON
SET IDENTIFIER(speed) FLOAT_LITERAL(80.5) SEMICOLON
IF LPAREN IDENTIFIER(speed) GT FLOAT_LITERAL(60.0) RPAREN
LCURLYBRACE SET IDENTIFIER(speed) IDENTIFIER(speed) SYM_MINUS FLOAT_LITERAL(5.0) SEMICOLON RCURLYBRACE
RCURLYBRACE
```

### **Parser Construction:**

```
ProgramNode
 └── ControlBlockNode ("speedControl")
      ├── DeclarationNode (float speed)
      ├── AssignmentNode (speed = 80.5)
      └── IfNode
          ├── Condition: (speed > 60.0)
          └── Body:
              └── AssignmentNode (speed = speed - 5.0)
```

---

## **8. Error Handling**

The parser performs **syntax validation** and reports:

* **Unexpected tokens** (e.g., missing `;`, `{`, `}`)
* **Unmatched braces or parentheses**
* **Invalid statement structure**

Example Error Messages:

```
Line 6: Expected ';' after declaration
Line 9: Unexpected token ')' in expression
Line 11: Missing '}' to close control block
```

Error handling ensures early detection of syntax issues before semantic analysis.

---

## **9. Parser Function Summary**

| Function              | Purpose                                                 |
| --------------------- | ------------------------------------------------------- |
| `parseProgram()`      | Entry point; parses entire file                         |
| `parseControlBlock()` | Parses `control` structure                              |
| `parseStatement()`    | Distinguishes between declaration, assignment, and `if` |
| `parseDeclaration()`  | Parses type + identifier                                |
| `parseAssignment()`   | Parses `set` statement                                  |
| `parseIfStatement()`  | Parses `if (cond) { ... }`                              |
| `parseExpression()`   | Handles arithmetic (+, -)                               |
| `parseCondition()`    | Handles `>` and `==`                                    |
| `parseFactor()`       | Parses identifiers, literals, and grouped expressions   |

---

## **10. Example Input and Output**

### **Input:**

```autolang
control engineControl {
    float speed;
    int rpm;
    set speed 80.5;
    if (speed > 60.0) {
        set speed speed - 5.0;
    }
}
```

### **Output (AST):**

```
Program
 └── ControlBlock(engineControl)
      ├── Declaration(float speed)
      ├── Declaration(int rpm)
      ├── Assignment(speed, 80.5)
      └── If
          ├── Condition: (speed > 60.0)
          └── Body:
              └── Assignment(speed, (speed - 5.0))
```

---

## **11. Parser Constraints**

* Only supports **`int`**, **`float`**, and **`bool`** types.
* Each statement must end with a **semicolon** (`;`).
* `set` assignments must always specify a valid variable and expression.
* Nested `if` statements and multiple control blocks are allowed.
* `=` operator is **not supported** — only `==` for comparison.

---


## **12. Conclusion**

The AutoLang Parser forms the **syntactic foundation** of the compiler pipeline.
It ensures that every program conforms to the defined grammar rules and produces a well-structured **AST**.
This AST then serves as the input for the **Type Checker** and **Code Generation** phases, ensuring syntactic correctness before runtime safety validation.


