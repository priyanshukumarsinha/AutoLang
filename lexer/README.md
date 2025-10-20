## **AutoLang Formal Grammar (EBNF)**

### **1. Program Structure**

```
<program>       ::= { <control_block> }
<control_block> ::= "control" <identifier> "{" { <statement> } "}"
```

---

### **2. Statements**

```
<statement> ::= <declaration> 
              | <assignment> 
              | <if_statement>
```

---

### **3. Declarations**

```
<declaration> ::= <type> <identifier> ";"
<type>        ::= "int" | "float" | "bool"
```

**Examples:**

```
int rpm;
float speed;
bool engineOn;
```

---

### **4. Assignments**

```
<assignment> ::= "set" <identifier> <expression> ";"
```

> 💡 Note: `set` directly assigns the result of `<expression>` to the variable.
> There is **no `=` operator** in the language.

**Examples:**

```
set speed 80.5;
set rpm rpm + 500;
set brakePressed true;
```

---

### **5. Conditional Statements**

```
<if_statement> ::= "if" "(" <condition> ")" "{" { <statement> } "}"
```

---

### **6. Expressions and Conditions**

```
<condition> ::= <expression> <comparison_op> <expression>
<expression> ::= <term> { <add_op> <term> }
<term>       ::= <factor>
<factor>     ::= <identifier> | <literal> | "(" <expression> ")"

<add_op>        ::= "+" | "-"
<comparison_op> ::= ">" | "=="
```

**Examples:**

```
speed + 10
rpm > 3000
(speed - 5.0) > 50
```

---

### **7. Identifiers and Literals**

```
<identifier> ::= <letter> { <letter> | <digit> | "_" }
<literal>    ::= <int_literal> | <float_literal> | <bool_literal>

<int_literal>   ::= <digit> { <digit> }
<float_literal> ::= <digit> { <digit> } "." <digit> { <digit> }
<bool_literal>  ::= "true" | "false"

<letter> ::= "A" | "B" | ... | "Z" | "a" | "b" | ... | "z"
<digit>  ::= "0" | "1" | ... | "9"
```

---

### **8. Keywords and Operators Summary**

| Category       | Tokens                  |
| -------------- | ----------------------- |
| **Keywords**   | `control`, `set`, `if`  |
| **Types**      | `int`, `float`, `bool`  |
| **Operators**  | `+`, `-`, `>`, `==`     |
| **Delimiters** | `{`, `}`, `(`, `)`, `;` |
| **Booleans**   | `true`, `false`         |

---

### **9. Example Program (Valid According to Updated Grammar)**

```autolang
control speedControl {
    float speed;
    int rpm;
    bool brakePressed;

    set speed 80.5;
    set rpm 3000;
    set brakePressed true;

    if (brakePressed == true) {
        set speed speed - 10.5;
    }
}
```

---

### **10. Notes for Implementation**

* **No assignment operator (`=`):**
  All assignments are performed using the `set` keyword followed directly by the target variable and the expression.

* **Comparison only via `>` and `==`.**

* **Whitespace** separates tokens but is otherwise ignored.

* **All statements** end with a semicolon (`;`).

* **Case-sensitive**: `Set speed 10;` is invalid; must be lowercase `set`.

