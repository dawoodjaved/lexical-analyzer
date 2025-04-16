# Compiler Design Exercises

## 1. Introduction & Overview

Compiler design involves translating a high-level language (like C) into a lower-level representation (assembly or machine code). Each exercise in this report showcases a specific compiler phase: from lexical analysis (tokenizing source text) and syntax analysis (parsing tokens) to code generation and optimizations. By completing these tasks, we develop a holistic understanding of how modern compilers operate internally.

---

## 2. Exercise 1: Manual Lexical Analyzer in C

### 2.1 Conceptual Background
A lexical analyzer (or lexer) transforms raw input text into a sequence of tokens (identifiers, keywords, numbers, operators, punctuation).  
Key tasks:
- Ignore whitespace and comments
- Distinguish keywords (e.g., `int`, `float`) from identifiers (e.g., `myVariable`)

### 2.2 Approach & Implementation
- Implemented a C program `manual_lexer.c`
- Reads characters from stdin or file
- Detects patterns and classifies tokens
- Matches keywords using a lookup array
- Skips whitespace (`' '`, `'\t'`, `'\n'`)

### 2.3 Sample Testing & Observations
> **Input:**  
> Sample C-style expression  
> **Output:**  
> Token stream identifying keywords, identifiers, and symbols

---

## 3. Exercise 2: Lexical Analyzer Using Lex/Flex

### 3.1 Conceptual Background
Lex/Flex generates lexical analyzers from regular expressions and actions, offering a more efficient and less error-prone approach than manual methods.

### 3.2 Approach & Implementation
- Created `lexer.l` with pattern-action pairs
- On regex match, associated C code block is executed

### 3.3 Sample Testing
- Compiled using `flex lexer.l && gcc lex.yy.c -o lexer -lfl`
- Used same test input as Exercise 1
- Output matched manual lexer, confirming correctness

---

## 4. Exercise 3: YACC Specifications for Syntactic Categories

### 4.1 (a) Recognize a Valid Arithmetic Expression
- YACC grammar used to parse expressions  
> **Example:**  
> Input: `3+5*2` → Valid  
> Input: `3+*4` → Invalid

### 4.2 (b) Recognize a Valid Variable
- Valid variable: starts with a letter, followed by letters or digits
- Lex: `[a-zA-Z][a-zA-Z0-9]*`
- YACC: `S -> ID { printf("Valid variable.\n"); }`

### 4.3 (c) Implementation of a Calculator (Lex & YACC)
- Recognized digits via Lex, returning `NUMBER`
- Evaluated expressions via YACC rules  
> Input: `3+5*2` → Output: `Result = 13`

### 4.4 (d) Convert BNF to YACC & Generate AST
- Created AST using:
  ```c
  AST* createNode(char* token, AST* left, AST* right);

