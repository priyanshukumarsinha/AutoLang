## ⚙️ **AutoLang Lexical Specification**

### **1. Token Categories**

Our lexer will recognize and categorize the following tokens:

| Category                     | Token Name                                          | Examples                       |
| ---------------------------- | --------------------------------------------------- | ------------------------------ |
| **Keywords**                 | `CONTROL`, `SET`, `IF`                              | `control`, `set`, `if`         |
| **Types**                    | `TYPE`                                              | `int`, `float`, `bool`         |
| **Operators**                | `SYM_PLUS`, `SYM_MINUS`, `GT`, `EQEQ`                       | `+`, `-`, `>`, `==`            |
| **Delimiters / Punctuation** | `LCURLYBRACE`, `RCURLYBRACE`, `LPAREN`, `RPAREN`, `SEMICOLON` | `{`, `}`, `(`, `)`, `;`        |
| **Literals**                 | `INT_LITERAL`, `FLOAT_LITERAL`, `BOOL_LITERAL`      | `100`, `45.6`, `true`          |
| **Identifiers**              | `IDENTIFIER`                                        | `speed`, `engine_temp`, `rpm1` |
| **Whitespace / Comments**    | —                                                   | ignored                        |

---

### **2. Regular Expressions (Regex Patterns)**

Below are the **token patterns** we directly use in a lexer implementation.

#### **Keywords**

```
"control"        → CONTROL
"set"            → SET
"if"             → IF
```

#### **Data Types**

```
"int"            → TYPE
"float"          → TYPE
"bool"           → TYPE
```

#### **Operators**

```
"=="             → EQEQ
">"              → GT
"\+"             → SYM_PLUS
"-"              → SYM_MINUS
```

> ⚠️ Order matters — check for `"=="` **before** `"="` or `">"` so multi-character operators are recognized correctly.

#### **Delimiters / Punctuation**

```
"\{"             → LCURLYBRACE
"\}"             → RCURLYBRACE
"\("             → LPAREN
"\)"             → RPAREN
";"              → SEMICOLON
```

#### **Literals**

```
[0-9]+\.[0-9]+          → FLOAT_LITERAL
[0-9]+                  → INT_LITERAL
"true" | "false"        → BOOL_LITERAL
```

#### **Identifiers**

```
[a-zA-Z_][a-zA-Z0-9_]*  → IDENTIFIER
```

> Note: If an identifier matches a keyword (`control`, `set`, etc.), it should be classified as a **keyword token**, not an identifier.

#### **Whitespace and Comments**

```
[ \t\r\n]+              → (ignore)
```

---

### **3. Example Tokenization**

Example AutoLang code:

```autolang
control speedControl {
    float speed;
    set speed 80.5;
    if (speed > 60.0) {
        set speed speed - 5.0;
    }
}
```

Lexical Output:

```
CONTROL
IDENTIFIER(speedControl)
LCURLYBRACE
TYPE(float)
IDENTIFIER(speed)
SEMICOLON
SET
IDENTIFIER(speed)
FLOAT_LITERAL(80.5)
SEMICOLON
IF
LPAREN
IDENTIFIER(speed)
GT
FLOAT_LITERAL(60.0)
RPAREN
LCURLYBRACE
SET
IDENTIFIER(speed)
IDENTIFIER(speed)
SYM_MINUS
FLOAT_LITERAL(5.0)
SEMICOLON
RCURLYBRACE
RCURLYBRACE
```

---

### **4. Implementation Tips**

* **Case sensitivity:** All keywords and types are lowercase.
* **Keyword resolution order:**
  When matching identifiers, always check if the string is a keyword or type before classifying it as `IDENTIFIER`.
* **No assignment (`=`):**
  Only `==` is valid; standalone `=` should throw a lexical error.
* **Optional Extension (Comments):**
  You can add single-line comments like this:

  ```
  "#".* → (ignore)
  ```

---

### **5. Token Summary Table**

| Token Type    | Lexeme Example         | Description                 |
| ------------- | ---------------------- | --------------------------- |
| CONTROL       | `control`              | Defines a control block     |
| SET           | `set`                  | Assigns value to a variable |
| IF            | `if`                   | Conditional statement       |
| TYPE          | `int`, `float`, `bool` | Data type                   |
| SYM_PLUS          | `+`                    | Addition operator           |
| SYM_MINUS         | `-`                    | Subtraction operator        |
| GT            | `>`                    | Greater than                |
| EQEQ          | `==`                   | Equality comparison         |
| IDENTIFIER    | `speed`                | Variable name               |
| INT_LITERAL   | `100`                  | Integer value               |
| FLOAT_LITERAL | `45.6`                 | Float value                 |
| BOOL_LITERAL  | `true` / `false`       | Boolean value               |
| LCURLYBRACE        | `{`                    | Block start                 |
| RCURLYBRACE        | `}`                    | Block end                   |
| LPAREN        | `(`                    | Condition start             |
| RPAREN        | `)`                    | Condition end               |
| SEMICOLON     | `;`                    | Statement terminator        |


