PC Report
=========
Tucker Evans
October 16, 2019

PC parses and (starts to) compile a subset of pascal into X86-64
assembly. 

Installation/Running
--------------------
PC can be installed into the local directory using `make`, this creates the
binary 'pc'. PC accepts a source file via stdin and will output assembly to
stdout (given it is compiled with the macro `GENERATE_CODE`). Errors are reported
on stderr; if the binary is compiled with the `DEBUG` macro then tokens, parse
trees, and scope information will also be printed to stderr.

Status
------
PC has a working frontend and an incomplete backend . PC currently can tokenize,
parse, and run semantic checks on source files. The code generation only works
with integer variables and is currently limited to:
- Output (write() calls)
- Expressions
  - Function calls
  - Addition/Subtraction
  - Multiplication/Division
  - Negation
  - Variable access (No Arrays, No Reals)
- Statements
  - Assignment
  - Procedure calls

Therefore the current To Do list for code generation is:
- Input via read() (emulating scanf c call)
- If statements
- While loops (statement)
- For loops (statement)
- Array Access
- Real variables

Other To Do's
- Provide src/output files via flags.
