# HyperAcceleratedDragon 🐉

An educational C compiler implementation in modern C++

## Completed tasks

- Lexical analysis using a maximal-munch lexer
- Recursive-descent parsing
- AST generation
- x64 codegen 

## TODO
- x64 Code emission
- fix AST program node ownership
- Add grammar class and make parser read from grammar rules
- Line numbers in Syntax errors
- Microbenchmarking
- ....

## AST and x64_AST Structure Documentation

### Abstract Syntax Tree (AST)

The AST is defined in [include/ast.hpp](include/ast.hpp) and represents the intermediate representation of the input C source code after parsing. It uses a visitor pattern with a common base class `ast_node`.

#### AST Node Hierarchy

```
ast_node (abstract base)
├── ast_token_node (terminal symbols - tokens)
│   ├── const_token_node - Represents integer constants
│   └── identifier_token_node - Represents identifiers
├── exp_node - Expression node containing a constant
├── ret_node - Return statement containing an expression
├── stmt_node - Statement containing a return node
├── func_node - Function definition with identifier and body
└── program_node - Root node containing a function
```

#### AST Grammar (ASDL)

```
program = Program(function_definition)
function_definition = Function(identifier name, statement body)
statement = Return(exp)
exp = Constant(int)
```

#### Grammar Rules

```
<program> ::= <function>
<function> ::= "int" <identifier> "(" "void" ")" "{" <statement> "}"
<statement> ::= "return" <exp> ";"
<exp> ::= <int>
<identifier> ::= ? An identifier token ?
<int> ::= ? A constant token ?
```

#### Key Components

- **Terminal Nodes** (`*_token_node`): Map directly to lexical tokens (constants, identifiers)
- **Non-Terminal Nodes** (`*_node`): Represent grammar rules and contain other nodes
- **Visitor Pattern**: All nodes implement `print(std::ostream &os, int indent)` for tree traversal
- **Memory Management**: Uses `std::unique_ptr` for automatic resource management

### x64 Assembly AST (x64_AST)

The x64_AST is defined in [include/x64_ast.hpp](include/x64_ast.hpp) and represents x86-64 assembly code as an intermediate step before code generation. It's built by translating the source AST using the `x64_translater` class.

#### x64_AST Node Hierarchy

```
x64_ast_node (abstract base)
├── x64_identifier_node - Function names
├── x64_operand_node (abstract operand)
│   ├── x64_imm_node - Immediate values (constants)
│   └── x64_reg_node - Register operands
├── x64_inst_node (abstract instruction)
│   ├── x64_mov_inst_node - MOV instruction (src, dest)
│   └── x64_ret_inst_node - RET instruction
├── x64_func_node - Function with name and instructions
└── x64_program_node - Root node containing a function
```

#### x64_AST Grammar (ASDL)

```
program = Program(function_definition)
function_definition = Function(identifier name, instruction* instructions)
instruction = Mov(operand src, operand dst) | Ret
operand = Imm(int) | Register
```

#### Key Components

- **Operands**: Abstract `x64_operand_node` with concrete implementations for immediates and registers
- **Instructions**: Abstract `x64_inst_node` with MOV and RET implementations
- **Translater**: `x64_translater` class performs the conversion from source AST to x64_AST
- **Code Generation Ready**: x64_AST is structured to facilitate straightforward assembly code generation

### Translation Pipeline

The compilation pipeline transforms source code through multiple representations:

```
Source Code (.c)
    ↓
Lexer → Tokens
    ↓
Parser → AST (defined in include/ast.hpp)
    ↓
x64 Translater → x64_AST (defined in include/x64_ast.hpp)
    ↓
Code Generator → Assembly Code (.s)
```

#### Translation Details

- **Tokens** flow through [src/maximal_munch_lexer.cpp](src/maximal_munch_lexer.cpp)
- **AST Parsing** happens in [src/ast.cpp](src/ast.cpp) with parser defined in [include/ast.hpp](include/ast.hpp)
- **x64 Translation** is implemented in [src/x64_ast.cpp](src/x64_ast.cpp)
- **Overall Compilation** is orchestrated by [src/had_compiler.cpp](src/had_compiler.cpp)

## Building
### Deps
- C++20 compiler (GCC/CLANG)
- Boost (program_options)
### Compile

```
sh scripts/build.sh
```
## Example output
Source code
```
int main(void){
    return 020;
}
```
Output
```
aaroncp@aaroncp-ROG-Strix-G634JZ-G634JZ:~/Desktop/workspace/HyperAcceleratedDragon$ ./build/HyperAcceleratedDragon sample_c_files/return_2.c 
file_path : sample_c_files/return_2.c
file name : return_2
--------tokens----------
Token{type=KEYWORD_INT, lexeme="int"}
Token{type=IDENTIFIER, lexeme="main"}
Token{type=PARANTHESIS_OPEN, lexeme="("}
Token{type=KEYWORD_VOID, lexeme="void"}
Token{type=PARANTHESIS_CLOSED, lexeme=")"}
Token{type=BRACE_OPEN, lexeme="{"}
Token{type=KEYWORD_RETURN, lexeme="return"}
Token{type=CONSTANT, lexeme="020"}
Token{type=SEMICOLON, lexeme=";"}
Token{type=BRACE_CLOSE, lexeme="}"}
-----------------------
LEXER took 70 us
----------AST----------
Program
  Function
    Identifier(main)
    Statement
      Return
        Exp
          Constant(20)
---------------------------
--------------x64_AST------------
x64_program_node
  x64_func_node
    func_name:
      x64_identifier_node: main
    instructions:
      x64_mov_inst_node
        src:
          x64_imm_node: 20
        dest:
          x64_reg_node: EAX
      x64_ret_inst_node
----------------------------------
AST took 76 us
Wrote assembly to return_2.s
compiler driver returned : 0
```
