---
tag: 01E4VJ
kind: feature
owner: python
slug: more-control-flow-tools
name: More Control Flow Tools
aliases: [Python control flow, control flow statements in Python, compound statements, Python flow control overview]
area: languages/python/control-flow
level: intro
summary: >-
  Python controls the order of statements with if, while, for, break, continue, pass and match. Each clause of a compound statement has a header that ends with a colon and an indented suite.
grammar: "compound_stmt: if_stmt | while_stmt | for_stmt | try_stmt | with_stmt | match_stmt | funcdef | classdef | async_with_stmt | async_for_stmt | async_funcdef (Language Reference, 8)"
spec: {standard: null, url: "https://docs.python.org/3.15/reference/compound_stmts.html#compound-statements", locator: "8. Compound statements"}
see_also: [python/if-statements, python/while-statement, python/for-statements, python/break-and-continue-statements, python/else-clauses-on-loops, python/pass-statements, python/match-statement, python/looping-techniques]
sources:
  - url: https://docs.python.org/3.15/tutorial/controlflow.html#more-control-flow-tools
    title: The Python Tutorial, 4. More Control Flow Tools
    locator: 4. More Control Flow Tools, introduction
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/compound_stmts.html#compound-statements
    title: The Python Language Reference, Python 3.15
    locator: 8. Compound statements, introduction
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec#Statements
    title: The Go Programming Language Specification
    locator: Statements; Blocks
    accessed: 2026-11-16
---

## Synopsis

```python run id=tour
for n in range(1, 10):
    if n % 2 == 0:
        continue
    match n:
        case 1:
            print(n, "is one")
        case 9:
            print(n, "is the last")
            break
        case _:
            pass
else:
    print("not reached")
```

```output for=tour
1 is one
9 is the last
```

The program uses `for`, `if`, `continue`, `match`, `break`, `pass` and a loop `else`. The `break` ends the loop, so the `else` clause does not run.

## Semantics

**r1.** The tutorial chapter starts with these words: "As well as the while statement just introduced, Python uses a few more that we will encounter in this chapter."[^1] The chapter has sections on if, for, range(), break and continue, else clauses on loops, pass and match statements.[^1]

**r2.** "Compound statements contain (groups of) other statements; they affect or control the execution of those other statements in some way. In general, compound statements span multiple lines, although in simple incarnations a whole compound statement may be contained in one line."[^2] Example: [`one-line`](#one-line).

**r3.** "The if, while and for statements implement traditional control flow constructs."[^2]

**r4.** "A compound statement consists of one or more ‘clauses.’ A clause consists of a header and a ‘suite.’ The clause headers of a particular compound statement are all at the same indentation level. Each clause header begins with a uniquely identifying keyword and ends with a colon."[^2] Example: [`tour`](#tour).

**r5.** "A suite can be one or more semicolon-separated simple statements on the same line as the header, following the header’s colon, or it can be one or more indented statements on subsequent lines. Only the latter form of a suite can contain nested compound statements".[^2] Example: [`nested-one-line`](#nested-one-line).

## Examples

```python run id=one-line
x = 5
if x > 0: print("positive"); print("still the if suite")
while x > 3: x -= 1
print(x)
```

```output for=one-line
positive
still the if suite
3
```

The suite of each clause is on the same line as its header (rule r5).

## Pitfalls

A suite on the header line cannot contain a compound statement:

```python fails id=nested-one-line expect=exception:SyntaxError
for i in range(3): if i: print(i)
```

Put the inner statement on its own indented line.

In a suite on one line, all the statements belong to the clause. The reference gives this example: in `if x < y < z: print(x); print(y); print(z)`, "either all or none of the print() calls are executed."[^2]

## Comparison

Go statements also "control execution".[^3] In Go, "A block is a possibly empty sequence of declarations and statements within matching brace brackets."[^3] So Go uses braces where Python uses a clause header with a colon and an indented suite (rule r4). Go has if, for, switch, select, goto, break, continue, return, fallthrough and defer statements.[^3] Python has no goto, and it uses while and for where Go uses one for statement with three forms.
