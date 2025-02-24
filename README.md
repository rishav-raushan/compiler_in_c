# SimpleLang Compiler - Project Documentation

## Introduction
The **SimpleLang Compiler** is a custom-built compiler in **C** that translates a high-level language (**SimpleLang**) into **assembly code for an 8-bit CPU**. It consists of a **lexer, parser (AST generation), and code generator**, ensuring proper translation from **SimpleLang to low-level assembly**.

## Features
- **Lexical Analysis (Lexer)** – Tokenizes SimpleLang source code.
- **Parsing (AST Generation)** – Builds an Abstract Syntax Tree (AST).
- **Assembly Code Generation** – Translates AST to 8-bit CPU assembly.
- **Handles variable declarations, assignments, arithmetic operations, and conditionals.**

## Project Structure
```
SimpleLang-Compiler/
│── src/
│   ├── simplelang_compiler.c   # Single file containing Lexer, Parser, Code Generator
│── examples/
│   ├── input.txt               # Example SimpleLang programs
│── output/
│   ├── tokens.txt              # Tokenized output
│   ├── ast_output.txt          # Abstract Syntax Tree (AST) output
│   ├── output.asm              # Generated assembly code
│── README.md                   # Project documentation
│── Makefile                     # Compilation script
```

## 1. SimpleLang: High-Level Language Design
SimpleLang supports:
- **Variable Declarations** (e.g., `int a;`)
- **Assignments** (e.g., `a = 10;`)
- **Arithmetic Operations** (e.g., `c = a + b;`)
- **Conditional Statements** (e.g., `if (c == 30) { c = c + 1; } else { c = c - 1; }`)

### Example SimpleLang Code:
```c
int a;
int b;
int c;
a = 10;
b = 20;
c = a + b;
if (c == 30) {
    c = c + 1;
} else {
    c = c - 1;
}
```

## 2. Lexer Implementation
The **lexer** scans the input and tokenizes keywords, identifiers, operators, and literals.

### Sample Token Output (`tokens.txt`):
```
Token: int
Token: a
Token: =
Token: 10
Token: ;
Token: if
Token: (
Token: c
Token: ==
Token: 30
Token: )
Token: {
Token: c
Token: =
Token: c
Token: +
Token: 1
Token: ;
Token: }
Token: else
Token: {
Token: c
Token: =
Token: c
Token: -
Token: 1
Token: ;
Token: }
```

## 3. Parser Implementation (AST Generation)
The **parser** converts tokens into an **Abstract Syntax Tree (AST)**.

### Sample AST Output (`ast_output.txt`):
```
AST Node: int a;
AST Node: int b;
AST Node: int c;
AST Node: a = 10;
AST Node: b = 20;
AST Node: c = a + b;
AST Node: if (c == 30) {
    AST Node: c = c + 1;
} else {
    AST Node: c = c - 1;
}
```

## 4. Code Generation
The **code generator** traverses the AST and translates it into **assembly instructions** for an 8-bit CPU.

### Sample Assembly Output (`output.asm`):
```
ldi A 10       ; Load 10 into register A
mov M A 100    ; Store A (10) in memory address 100 (variable a)
ldi A 20       ; Load 20 into register A
mov M A 101    ; Store A (20) in memory address 101 (variable b)
mov A M 100    ; Load value of a (10) from memory into register A
add M 101      ; Add value of b (20) from memory to A -> A = 10 + 20 = 30
mov M A 102    ; Store result (30) in memory address 102 (variable c)
cmp M 102 30   ; Compare c with 30
jne else_block ; If c != 30, jump to 'else'
inc A          ; If c == 30, increment A (A = 31)
mov M A 102    ; Store updated value of c (31) back to memory
else_block:
ldi A -1       ; Load -1 into register A (else condition)
add M 102      ; c = c - 1
mov M A 102    ; Store updated c value
end:
hlt            ; Halt execution
```

## 5. Compilation & Execution
### **Compile the Project**
```bash
gcc -o compiler simplelang_compiler.c
```
### **Run the Compiler**
```bash
./compiler
```
### **View Generated Outputs**
```bash
cat tokens.txt        # Tokenized output
cat ast_output.txt    # AST representation
cat output.asm        # Assembly code
```

## Conclusion
The **SimpleLang Compiler** is a **custom-built system software** that compiles high-level code into **8-bit assembly language**. This project strengthened my skills in **compilers, AST traversal, system-level programming, and low-level architecture**.

### Future Enhancements
- Add support for loops (`while`, `for`).
- Implement function definitions and calls.
- Improve error handling and debugging tools.
- Extend to support multiple CPU architectures.

---
**Author:** Rishav Raushan  
**Technologies Used:** C, Compiler Design, Assembly Code Generation
