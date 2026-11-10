---
tag: NEW-abstract-syntax-tree
kind: concept
slug: abstract-syntax-tree
name: Abstract syntax tree
aliases: [AST, syntax tree, abstract syntax]
area: language-implementation/asts-and-irs
level: core
summary: >-
  An abstract syntax tree holds the structure of a program as a tree of operators, operands and statements. It drops the tokens that only guide the parser, such as parentheses.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1962
  people: [John McCarthy]
  note: McCarthy described an abstract syntax for programming languages that is independent of the notation used to write the program.
see_also: [recursive-descent-parsing, tree-walking-interpreter, bytecode, sum-types, composite-pattern, visitor-pattern, interpreter-pattern, recursion]
same_as:
  wikidata: Q127380
  wikipedia: Abstract_syntax_tree
sources:
  - url: https://craftinginterpreters.com/representing-code.html#implementing-syntax-trees
    title: Robert Nystrom, Crafting Interpreters, chapter 5, Representing Code
    locator: 5.2 Implementing Syntax Trees (parse tree and AST)
  - url: http://jmc.stanford.edu/articles/towards/towards.pdf
    title: John McCarthy, Towards a Mathematical Science of Computation, Proceedings of IFIP Congress 62 (retyped copy, 1996)
    locator: section 12, Abstract Syntax of Programming Languages
  - url: http://jmc.stanford.edu/articles/towards.html
    title: John McCarthy's papers, Towards a Mathematical Science of Computation
    locator: the description of the paper (given at the congress IFIP-62)
  - url: https://docs.python.org/3.14/library/ast.html#module-ast
    title: The Python Standard Library, ast, Python 3.14
    locator: introduction of the module ast
  - url: https://docs.python.org/3.14/library/ast.html#ast.parse
    title: The Python Standard Library, ast, Python 3.14
    locator: ast.parse
  - url: https://docs.python.org/3.14/library/ast.html#ast.dump
    title: The Python Standard Library, ast, Python 3.14
    locator: ast.dump
  - url: https://docs.python.org/3.14/library/ast.html#abstract-grammar
    title: The Python Standard Library, ast, Python 3.14
    locator: Abstract Grammar (attributes lineno, col_offset, end_lineno, end_col_offset)
  - url: https://docs.python.org/3.14/library/ast.html#ast.NodeVisitor
    title: The Python Standard Library, ast, Python 3.14
    locator: ast.NodeVisitor
  - url: https://docs.python.org/3.14/library/ast.html#ast.walk
    title: The Python Standard Library, ast, Python 3.14
    locator: ast.walk
  - url: https://docs.python.org/3.14/library/ast.html#ast.unparse
    title: The Python Standard Library, ast, Python 3.14
    locator: ast.unparse
  - url: https://pkg.go.dev/go/parser#ParseExpr
    title: Go standard library, package go/parser
    locator: Overview and func ParseExpr
  - url: https://pkg.go.dev/go/ast#ParenExpr
    title: Go standard library, package go/ast
    locator: type ParenExpr
  - url: https://pkg.go.dev/go/ast#Inspect
    title: Go standard library, package go/ast
    locator: func Inspect
  - url: https://github.com/microsoft/TypeScript/wiki/Using-the-Compiler-API
    title: TypeScript wiki, Using the Compiler API
    locator: the caution at the top, and Traversing the AST with a little linter
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/jdk.compiler/com/sun/source/tree/package-summary.html
    title: Java SE 25 API, module jdk.compiler, package com.sun.source.tree
    locator: package description, BinaryTree, ParenthesizedTree, TreeVisitor
  - url: https://docs.rs/syn/latest/syn/
    title: syn crate documentation
    locator: crate overview, syn::Expr, the visit module
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/advanced-topics/expression-trees/
    title: C# documentation, Expression Trees
    locator: the introduction and Limitations
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.linq.expressions.expressionvisitor
    title: .NET API reference, ExpressionVisitor Class
    locator: class summary
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.linq.expressions.expression-1.compile
    title: .NET API reference, Expression<TDelegate>.Compile Method
    locator: Compile()
  - url: https://docs.python.org/3.14/library/ast.html#ast.NodeTransformer
    title: The Python Standard Library, ast, Python 3.14
    locator: ast.NodeTransformer
  - paper: doi:10.1145/367177.367199
    title: John McCarthy, Recursive Functions of Symbolic Expressions and Their Computation by Machine, Part I, Communications of the ACM 3(4), 1960, pages 184 to 195
    locator: section 3e, Representation of S-Functions by S-Expressions
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.management.automation.scriptblock.ast
    title: PowerShell SDK reference, ScriptBlock.Ast Property
    locator: Definition
---

## Idea

A program is a string of characters. A compiler or an interpreter cannot work on the string directly. It needs to know which operator applies to which operands, which statements are inside a loop, and which name a declaration binds. An abstract syntax tree (AST) holds this structure. Each node is one construct of the language: a number, a name, a binary operation, a call, an assignment. The children of a node are its parts.

The tree is abstract because it keeps the structure and drops the notation. The source `1 + 2 * (3 - x)` has parentheses. They tell the parser to group `3 - x`, but after the parser has built the tree, the grouping is in the shape of the tree. A parse tree keeps every rule of the grammar as a node. An AST leaves out the productions that later phases do not need.[^1] Python's `ast` module shows the tree that the CPython compiler works on.

```python run id=idea
import ast

tree = ast.parse("1 + 2 * (3 - x)", mode="eval")
print(ast.dump(tree, indent=2))
```

```output for=idea
Expression(
  body=BinOp(
    left=Constant(value=1),
    op=Add(),
    right=BinOp(
      left=Constant(value=2),
      op=Mult(),
      right=BinOp(
        left=Constant(value=3),
        op=Sub(),
        right=Name(id='x', ctx=Load())))))
```

The tree has no node for the parentheses. The multiplication is below the addition because `*` binds tighter than `+`. The subtraction is below the multiplication because the parentheses said so. The precedence rules and the parentheses have done their work, and the shape of the tree is the result.

McCarthy gave the idea its name. His abstract syntax is "analytic rather than synthetic". It asks of a term whether it is a sum and what its addend and augend are. It does not care whether the source wrote the sum as `a + b`, as `+ab` or as `(PLUS A B)`.[^2][^3]

## How it works

### From characters to tokens to a tree

A front end works in two steps. The lexer turns characters into tokens: numbers, names, operators and punctuation. The parser reads the tokens and builds the tree. Each rule of the parser returns the node for the text that it matched. A rule for a parenthesized expression reads `(`, calls the rule for an expression, reads `)` and returns the inner node unchanged. That is where the parentheses disappear. The page [[recursive-descent-parsing]] builds such a parser.

The token stream of the same source still has everything that the programmer typed, including the parentheses.

```python run id=tokens
import io
import tokenize

source = "1 + 2 * (3 - x)"
for tok in tokenize.generate_tokens(io.StringIO(source).readline):
    if tok.type in (tokenize.NEWLINE, tokenize.ENDMARKER):
        continue
    print(tokenize.tok_name[tok.type], repr(tok.string))
```

```output for=tokens
NUMBER '1'
OP '+'
NUMBER '2'
OP '*'
OP '('
NUMBER '3'
OP '-'
NAME 'x'
OP ')'
```

Nine tokens become seven nodes: four leaves and three operations. The tree is smaller than the text, and each node has a known kind and known children.

### The node kinds form a sum type

The kinds of node are fixed by the language. An expression is a constant, or a name, or a binary operation with two child expressions, and so on. This is a [[sum-types|sum type]] that is recursive. Python's documentation gives the whole set as an abstract grammar in ASDL. For example, it has the line `BinOp(expr left, operator op, expr right)`.[^7] In a language with sum types, each kind is one variant. In an object-oriented language, each kind is a class that implements a common interface, which is the [[composite-pattern]]. The example files show both forms.

A user-defined AST in Python fits in a few dataclasses. A `match` statement selects the case for each kind of node (see [[python/match-statement]]).

```python run id=own-ast
from dataclasses import dataclass


@dataclass
class Num:
    value: int


@dataclass
class Var:
    name: str


@dataclass
class Bin:
    op: str
    left: "Num | Var | Bin"
    right: "Num | Var | Bin"


def show(node, depth=0):
    pad = "  " * depth
    match node:
        case Num(value):
            print(f"{pad}Num {value}")
        case Var(name):
            print(f"{pad}Var {name}")
        case Bin(op, left, right):
            print(f"{pad}Bin {op}")
            show(left, depth + 1)
            show(right, depth + 1)


show(Bin("+", Num(1), Bin("*", Num(2), Bin("-", Num(3), Var("x")))))
```

```output for=own-ast
Bin +
  Num 1
  Bin *
    Num 2
    Bin -
      Num 3
      Var x
```

### Positions

A tree that has lost the source text must still report errors at the right place. So each node keeps its position. In Python, expression and statement nodes have the attributes `lineno`, `col_offset`, `end_lineno` and `end_col_offset`.[^7] A type checker or a linter uses them to point at the source.

### Walking the tree

Every later phase is a walk over the tree: a type checker, an optimizer, a code generator or an interpreter. A walk is a recursive function with one case for each kind of node. When the set of operations grows and the set of node kinds stays fixed, the walk is often written as a [[visitor-pattern|visitor]]. Python's `ast.NodeVisitor` calls a method `visit_<ClassName>` for each node that it visits, and `generic_visit` visits the children.[^8] `ast.walk` yields all nodes of a tree, but "in no specified order".[^9]

```python run id=walk
import ast


class Names(ast.NodeVisitor):
    def __init__(self):
        self.names = []

    def visit_Name(self, node):
        self.names.append((node.id, node.col_offset))


tree = ast.parse("total = price * count + tax")
finder = Names()
finder.visit(tree)
print(finder.names)
print(sum(1 for _ in ast.walk(tree)), "nodes")
```

```output for=walk
[('total', 0), ('price', 8), ('count', 16), ('tax', 24)]
14 nodes
```

An interpreter is a walk that computes a value for each node. The page [[tree-walking-interpreter]] builds one. A compiler is a walk that emits instructions for each node; the page [[bytecode]] shows it.

## In languages

Some standard libraries expose the language's own parser and its AST. In the other languages, a program that needs an AST defines one. The examples parse `1 + 2 * (3 - x)`, print the tree, walk it to count the nodes and collect the names, and evaluate it for `x = 1`.

| Language | Verdict | Form |
|---|---|---|
| Python | stdlib | the `ast` module: `ast.parse`, `ast.dump`, `ast.NodeVisitor`, `ast.NodeTransformer` |
| Go | stdlib | `go/parser` builds `go/ast` nodes; `ast.Inspect` walks them |
| C# | stdlib | expression trees in `System.Linq.Expressions`; the compiler builds one from a lambda |
| Java | idiom | a sealed interface and records; the JDK parser in `com.sun.source.tree` is a module of the JDK |
| Kotlin | idiom | a sealed interface and data classes, matched with `when` |
| TypeScript | idiom | a discriminated union; the compiler API exposes the TypeScript AST |
| JavaScript | idiom | plain objects with a `kind` field |
| Rust | idiom | an `enum` with `Box` children; the `syn` crate parses Rust itself |
| C++ | idiom | `std::variant` with `std::unique_ptr` children |
| C | idiom | a tagged struct with a union and pointers to children |
| SQL | n/a | queries have no access to a syntax tree |
| Bash | n/a | the shell exposes no syntax tree |
| PowerShell | stdlib | the `Ast` property of a script block[^22] |
| Regex | n/a | a pattern language, not a host for trees |
| HTML/CSS | n/a | the DOM is a tree of the document, not of a program |

Python's `ast.parse` takes a mode. `"exec"` parses a module, `"eval"` a single expression, and `"single"` one interactive statement.[^5] `ast.dump` prints a tree, and its `indent` argument puts each node on its own line.[^6] The documentation warns that the abstract syntax "might change with each Python release".[^4]

Go's `go/parser` produces the AST of a file or of a single expression. `parser.ParseExpr` is a convenience function for one expression, and with a syntax error it returns a partial tree with `ast.Bad*` nodes.[^11] Unlike Python's tree, Go's tree keeps parentheses: `ast.ParenExpr` "represents a parenthesized expression".[^12] `ast.Inspect` walks the tree in depth-first order and calls a function for each node.[^13]

The TypeScript compiler API exposes the trees that `tsc` builds. `ts.createSourceFile` parses a file, `ts.forEachChild` visits the children of a node, and `node.kind` holds a value of `ts.SyntaxKind`. The wiki warns that a later major version of TypeScript will have a different API.[^14] The examples do not use the API, because `typescript` is a package and not part of a standard library.

Java's compiler exposes its tree in the package `com.sun.source.tree` of the module `jdk.compiler`. The package "provides interfaces to represent source code as abstract syntax trees". It has `BinaryTree` for a binary expression, `ParenthesizedTree` for a parenthesized one, and `TreeVisitor`, "a visitor of trees, in the style of the visitor design pattern".[^15] Like Go, it keeps the parentheses.

Rust's standard library has no parser for Rust. The `syn` crate parses a stream of Rust tokens into a syntax tree and is "geared toward use in Rust procedural macros". `syn::Expr` is its expression type, and its `visit` module walks the tree.[^16]

C# builds expression trees in the compiler. When a lambda is assigned to an `Expression<TDelegate>`, the compiler emits code that builds the tree instead of a delegate.[^17] The tree can be walked with an `ExpressionVisitor`[^18] and turned into code with `Compile`.[^19] Only expression lambdas can be converted. Statement lambdas and `await` cannot.[^17]

## Variations and related ideas

- **Parse trees and concrete syntax trees.** A parse tree has one node for each grammar rule that matched, and it keeps every token.[^1] A tool that rewrites source code and must keep the comments and the layout needs such a tree. An AST is the wrong input for it (see Pitfalls).
- **Code as data.** In LISP, a program is an S-expression, the same list structure that the program works on. McCarthy defined the translation of each form of a function into an S-expression, so that a function can take a program as its argument.[^21] The AST and the surface syntax are almost the same thing.
- **Transforms.** An optimizer or a macro system rewrites one tree into another. Python's `ast.NodeTransformer` replaces each node with the value that its visit method returns.[^20] Rust procedural macros work on token streams and use `syn` to see them as trees.[^16]
- **Intermediate representations.** After the AST, many compilers lower the program to a flatter form: three-address code, a control-flow graph, or [[bytecode]]. A flat form is easier to optimize and to run, but it loses the nesting of the source.
- **Interpreters.** An AST can be run directly by a recursive evaluator. This is the [[interpreter-pattern]] at the scale of a whole language, and the [[tree-walking-interpreter]] page builds one.

## Pitfalls

### Walking in the wrong order

`ast.walk` visits the nodes in no specified order.[^9] In this run, the names of `a * b + c` come out level by level and not in source order.

```python wrong id=walk-order
import ast

tree = ast.parse("a * b + c", mode="eval")
print([n.id for n in ast.walk(tree) if isinstance(n, ast.Name)])
```

```output for=walk-order
['c', 'a', 'b']
```

`c` is closer to the root than `a` and `b`, so it comes first. A code generator or a pretty printer needs a depth-first walk in the order of the fields. A `NodeVisitor` gives that order.

```python run id=walk-order-fix
import ast


class Names(ast.NodeVisitor):
    def __init__(self):
        self.names = []

    def visit_Name(self, node):
        self.names.append(node.id)


finder = Names()
finder.visit(ast.parse("a * b + c", mode="eval"))
print(finder.names)
```

```output for=walk-order-fix
['a', 'b', 'c']
```

### A deep tree overflows a recursive walk

A sum of many terms is a deep tree. `+` is left-associative, so `1 + 1 + 1 + ...` with 3,000 terms nests 3,000 `BinOp` nodes on the left. A walk that calls itself for each child needs one frame per level (see [[recursion]]).

```python fails expect=exception:RecursionError id=deep
import ast

tree = ast.parse("1" + " + 1" * 3000, mode="eval")


def height(node):
    return 1 + max((height(c) for c in ast.iter_child_nodes(node)), default=0)


print(height(tree))
```

The fix is a walk with an explicit stack of nodes that are still to visit. The stack lives on the heap, so its size is limited only by memory. `ast.walk` also counts the nodes of the same tree without an error.

```python run id=deep-fix
import ast

tree = ast.parse("1" + " + 1" * 3000, mode="eval")

deepest = 0
stack = [(tree, 1)]
while stack:
    node, depth = stack.pop()
    deepest = max(deepest, depth)
    for child in ast.iter_child_nodes(node):
        stack.append((child, depth + 1))
print(deepest)
print(sum(1 for _ in ast.walk(tree)))
```

```output for=deep-fix
3002
9002
```

### Expecting the tree to keep the source

An AST does not keep comments, and Python's does not keep redundant parentheses. A tool that reads a file into an AST and writes it back with `ast.unparse` loses both.[^10]

```python wrong id=unparse
import ast

source = "x = (1 + 2)  # three\n"
print(ast.unparse(ast.parse(source)))
```

```output for=unparse
x = 1 + 2
```

The output is valid code with the same meaning, but the comment is gone. A formatter or a refactoring tool needs a concrete syntax tree, or it must edit the original text at the positions that the AST records.

## Cost

- **Memory.** Each node is a separate object with pointers to its children and four position fields. The deep example above has 9,002 nodes for 12,001 characters of source. In Python, each operator such as `Add()` is a node too.
- **Time.** A parser builds each node once, so building the tree is proportional to the number of tokens for the grammars on these pages. Each later phase walks the tree again.
- **Stack.** A recursive walk needs stack space in proportion to the height of the tree. Long chains of left-associative operators and deeply nested blocks make tall trees.
- **Locality.** A walk follows pointers from node to node. The nodes may be far apart in memory. A flat encoding such as [[bytecode]] avoids this, which is one reason that interpreters compile the AST before they run it.

## History

- **1960.** John McCarthy defined LISP. He gave a rule that translates each form of a LISP function into an S-expression, so that programs and data share one tree structure.[^21]
- **1962.** McCarthy presented "Towards a Mathematical Science of Computation" at IFIP Congress 62.[^3] Its section on abstract syntax describes a term by predicates such as `issum` and functions such as `addend` and `augend`, and it ignores the notation of the source.[^2]

## References
