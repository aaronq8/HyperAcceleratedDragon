# HyperAcceleratedDragon 🐉

An educational C compiler implementation in modern C++

## Completed tasks

- Lexical analysis using a maximal-munch lexer
- Recursive-descent parsing
- AST generation

## TODO
- Line numbers in Syntax errors
- x86 codegen 
- Microbenchmarking
- ....

## Current AST Structure
```
program = Program(function_definition)
function_definition = Function(identifier name, statement body)
statement = Return(exp)
exp = Constant(int)
```
## Current Formal Grammar
```
<program> ::= <function>
<function> ::= "int" <identifier> "(" "void" ")" "{" <statement> "}"
<statement> ::= "return" <exp> ";"
<exp> ::= <int>
<identifier> ::= ? An identifier token ?
<int> ::= ? A constant token ?
```
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
LEXER took 56 us
----------AST----------
Program
  Function
    Identifier(main)
    Statement
      Return
        Exp
          Constant(20)
---------------------------
AST took 31 us
Wrote assembly to return_2.s
compiler driver returned : 0
```
