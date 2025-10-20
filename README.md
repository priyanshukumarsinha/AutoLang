# **AutoLang: A Real-Time Automotive Control Language**

**Overview:**
AutoLang is a lightweight, domain-specific language (DSL) designed for **real-time automotive control systems**. It enables developers to write programs that interact with vehicle sensors, actuators, and control units in a deterministic and efficient manner. The language emphasizes **type safety**, **control logic**, and **simplicity**, ensuring reliable operation in embedded automotive environments.

---

### **1. Lexical Elements**

#### **1.1 Character Set**

AutoLang uses the standard ASCII character set:

* Letters: `A–Z`, `a–z`
* Digits: `0–9`
* Symbols: `+`, `-`, `>`, `==`, `;`, `(`, `)`, `{`, `}`, `_`
* Whitespace: space, tab, newline

---

#### **1.2 Tokens**

A program in AutoLang is composed of the following token types:

1. **Keywords**
   Reserved words with predefined meanings.

   ```
   control, set, if
   ```
2. **Data Types**

   ```
   int, float, bool
   ```
3. **Identifiers**
   User-defined names for variables or control blocks.

   * Must start with a letter (A–Z or a–z)
   * Can contain letters, digits, and underscores (`_`)
   * Examples: `speed`, `engine_temp`, `controlLoop1`
4. **Literals**

   * **Integer literals**: sequences of digits (e.g., `100`)
   * **Float literals**: digits with a decimal point (e.g., `45.7`)
   * **Boolean literals**: `true`, `false`
5. **Operators**

   * **Arithmetic:** `+`, `-`
   * **Comparison:** `>`, `==`
   * **Assignment:** `=`
6. **Delimiters and Punctuation**

   * Semicolon (`;`) terminates statements
   * Parentheses `(`, `)` for grouping or conditions
   * Braces `{`, `}` for control blocks

---

### **2. Syntax and Structure**

#### **2.1 Program Structure**

An AutoLang program consists of one or more `control` blocks, each defining a specific control logic unit.

**Example:**

```autolang
control speedControl {
    float speed;
    set speed 60.5;
    if (speed > 40.0) {
        set speed (speed - 5.0);
    }
}
```

---

#### **2.2 Variable Declarations**

Variables must be **declared with a type** before use.

**Syntax:**

```
<type> <identifier> ;
```

**Examples:**

```autolang
int rpm;
float speed;
bool engineOn;
```

---

#### **2.3 Assignment Statements**

The `set` keyword is used to assign values to variables.

**Syntax:**

```
set <identifier> <expression> ;
```

**Examples:**

```autolang
set speed 80.0;
set rpm 3000;
```

---

#### **2.4 Expressions**

Expressions can include:

* Literals
* Identifiers
* Arithmetic operators (`+`, `-`)
* Comparison operators (`>`, `==`)

**Examples:**

```autolang
speed + 10.0
rpm > 1000
```

---

#### **2.5 Conditional Statements**

`if` is used for conditional control flow. The condition must be a **boolean expression**.

**Syntax:**

```
if (<condition>) {
    <statements>
}
```

**Example:**

```autolang
if (speed > 100.0) {
    set speed (speed - 10.0);
}
```

---

### **3. Semantics**

* **Type Checking:**
  AutoLang enforces strict typing; arithmetic operations must involve compatible numeric types.

* **Real-Time Execution:**
  Control blocks (`control`) are designed to map to periodic or event-driven automotive control routines.

* **Determinism:**
  No dynamic memory allocation, recursion, or undefined behavior is allowed.

---

### **4. Example Program**

```autolang
control brakeControl {
    float speed;
    bool brakePressed;

    set speed 90.0;
    set brakePressed true;

    if (brakePressed == true) {
        set speed (speed - 10.0);
    }
}
```

---

### **5. Summary Table**

| Category        | Elements                            |
| --------------- | ----------------------------------- |
| **Types**       | `int`, `float`, `bool`              |
| **Keywords**    | `control`, `set`, `if`              |
| **Operators**   | `+`, `-`, `>`, `==`, `=`            |
| **Delimiters**  | `{`, `}`, `(`, `)`, `;`             |
| **Identifiers** | User-defined (letters, digits, `_`) |
| **Literals**    | Integers, floats, booleans          |


