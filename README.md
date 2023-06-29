# Compiler for Simple Perl Based Language
## Description
Built this working compiler in C++  as a project for NJIT CS280 Programming Language Concepts class. 
The compiler is designed around a small Perl-Like (SPL) programming language defined by an EBNF.

## Lexical Analyzer

 - The lexical analyzer interprets an input file and breaks its contents
   down into tokens. It scans the source code character by character and
   groups them into tokens. The token types include keywords of the
   language, variable types, and operators.
- I used a Finite State Automaton (FSA) to identify the tokens

## Parser Interpreter
- parserInt.cpp is a combination of a recursive descendent parser and language interpreter that performs the task of analyzing the syntactic structure of the given SPL program and executing it. 
- val.cpp provides overloaded operator functions for the Value class
	- The Value class represents a value object in the source code that stores the value of constants, variables, and evaluated expressions.
	- val.cpp provides functionalities to perform operations on values of different types as defined by the requirements of SPL.
