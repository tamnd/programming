---
tag: 7D2HC4
kind: feature
owner: python
slug: the-ast-module
name: The ast module
aliases: [ast, ast.parse, ast.unparse, ast.dump, ast.walk, NodeVisitor, NodeTransformer, fix_missing_locations, get_source_segment, Python abstract syntax tree]
area: languages/python/metaprogramming
level: advanced
summary: >-
  The ast module parses Python source into a tree of nodes. A program can read the tree with a NodeVisitor, change it with a NodeTransformer, turn it back into source with unparse, or compile it.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/library/ast.html", locator: "ast — Abstract syntax trees"}
proposals: []
see_also: [abstract-syntax-tree, python/eval-and-exec, python/bytecode-and-dis, python/code-and-frame-objects, python/the-python-grammar]
sources:
  - url: https://docs.python.org/3.15/library/ast.html
    title: ast — Abstract syntax trees, Python 3.15 documentation
    locator: Node classes; Helpers; ast.parse(); ast.NodeVisitor; ast.NodeTransformer
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/builtins/functions.html#compile
    title: Built-in Functions, Python 3.15 documentation
    locator: compile()
    accessed: 2026-11-16
---

## Synopsis

```python run id=parse
import ast

tree = ast.parse("1 + 2 * 3", mode="eval")
print(ast.dump(tree))
print(ast.unparse(tree))
print(eval(compile(tree, "<ast>", "eval")))
```

```output for=parse
Expression(body=BinOp(left=Constant(value=1), op=Add(), right=BinOp(left=Constant(value=2), op=Mult(), right=Constant(value=3))))
1 + 2 * 3
7
```

## Semantics

**r1.** "The ast module helps Python applications to process trees of the Python abstract syntax grammar. The abstract syntax itself might change with each Python release".[^1]

**r2.** There is "one class defined for each constructor on the right-hand side" of the grammar, and "Each concrete class has an attribute _fields which gives the names of all child nodes."[^1] For example, "ast.BinOp instances have an attribute left of type ast.expr."[^1] Example: [`parse`](#parse).

**r3.** `ast.parse` parses "the source into an AST node". `compile` takes "a normal string, a byte string, or an AST object", so a tree can become a code object.[^2] `ast.unparse` generates "a string with code that would produce an equivalent ast.AST object if parsed back".[^1]

**r4.** A `NodeVisitor` "walks the abstract syntax tree and calls a visitor function for every node found." Its `visit` method "calls the method called self.visit_classname where classname is the name of the node class, or generic_visit() if that method doesn’t exist."[^1] Example: [`visitor`](#visitor).

**r5.** A `NodeTransformer` uses "the return value of the visitor methods to replace or remove the old node."[^1] Example: [`transformer`](#transformer).

**r6.** Statement and expression nodes have `lineno`, `col_offset`, `end_lineno` and `end_col_offset`. "the compiler expects lineno and col_offset attributes for every node that supports them", and `fix_missing_locations` sets them "to the values of the parent node."[^1] Example: [`segment`](#segment).

**r7.** `ast.walk` yields "all descendant nodes in the tree starting at node (including node itself), in no specified order."[^1] Example: [`walk`](#walk).

## Examples

```python run id=visitor
import ast

source = """
import json
from os import path

def load(p):
    return json.loads(path.join(p))
"""

class Imports(ast.NodeVisitor):
    def __init__(self):
        self.names = []

    def visit_Import(self, node):
        self.names += [a.name for a in node.names]

    def visit_ImportFrom(self, node):
        self.names.append(node.module)

v = Imports()
v.visit(ast.parse(source))
print(v.names)
```

```output for=visitor
['json', 'os']
```

The visitor finds the imports without running the code (rule r4). Linters and other code checkers work in this way.

```python run id=transformer
import ast

class RewriteName(ast.NodeTransformer):
    def visit_Name(self, node):
        return ast.Subscript(
            value=ast.Name(id="data", ctx=ast.Load()),
            slice=ast.Constant(value=node.id),
            ctx=node.ctx,
        )

tree = ast.parse("price * qty", mode="eval")
tree = ast.fix_missing_locations(RewriteName().visit(tree))
print(ast.unparse(tree))
code = compile(tree, "<rewritten>", "eval")
print(eval(code, {"data": {"price": 3, "qty": 4}}))
```

```output for=transformer
data['price'] * data['qty']
12
```

Each name becomes a lookup in `data` (rule r5). The new nodes have no positions, so `fix_missing_locations` adds them before `compile` (rule r6).

```python run id=walk
import ast
from collections import Counter

tree = ast.parse("x = [i * i for i in range(10) if i % 2]")
counts = Counter(type(n).__name__ for n in ast.walk(tree))
print(counts["Name"], counts["BinOp"], counts["comprehension"])
```

```output for=walk
6 2 1
```

`walk` gives every node, so a count does not need a visitor class (rule r7).

```python run id=segment
import ast

source = "total = price * (1 + rate)"
node = ast.parse(source).body[0].value
print(type(node).__name__, node.col_offset, node.end_col_offset)
print(ast.get_source_segment(source, node))
```

```output for=segment
BinOp 8 26
price * (1 + rate)
```

The offsets point into the source, and `get_source_segment` gives the text of the node (rule r6).

## Pitfalls

```python fails id=parse-is-not-compile expect=exception:SyntaxError
import ast

tree = ast.parse("return 42")
print(type(tree.body[0]).__name__)
compile(tree, "<ast>", "exec")
```

"successfully parsing source code into an AST object doesn’t guarantee that the source code provided is valid Python code".[^1] `ast.parse` "won’t do any scoping checks, which the compilation step does."[^1]

```python fails id=no-locations expect=exception:TypeError
import ast

tree = ast.Expression(body=ast.BinOp(ast.Constant(2), ast.Add(), ast.Constant(3)))
compile(tree, "<ast>", "eval")
```

Nodes made by hand have no line numbers, so `compile` fails. Call `ast.fix_missing_locations(tree)` first (rule r6).

```python wrong id=no-generic-visit
import ast

source = """
def outer():
    def inner():
        pass
"""

class Funcs(ast.NodeVisitor):
    def visit_FunctionDef(self, node):
        print(node.name)

Funcs().visit(ast.parse(source))
```

```output for=no-generic-visit
outer
```

"child nodes of nodes that have a custom visitor method won’t be visited unless the visitor calls generic_visit() or visits them itself."[^1] Add `self.generic_visit(node)` at the end of `visit_FunctionDef` to find `inner` too.

## Comparison

The tree is a value, so a program can change code before it runs. Python has no macro system, and the tree comes from a string, not from code in the same file. See [[abstract-syntax-tree]] for the general idea, [[python/eval-and-exec]] for running the result, and [[python/bytecode-and-dis]] for the next step after the tree.
