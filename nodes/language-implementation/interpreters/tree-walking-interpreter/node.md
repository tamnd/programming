---
tag: X3BBA0
kind: concept
slug: tree-walking-interpreter
name: Tree-walking interpreter
aliases: [tree-walk interpreter, AST-walking interpreter]
area: language-implementation/interpreters
level: core
summary: >-
  A tree-walking interpreter runs a program by visiting the nodes of its syntax tree. It evaluates the children of each node first, and keeps the values of variables in a chain of environments.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1960
  people: [John McCarthy]
  note: McCarthy defined LISP with a universal function apply and an evaluator eval that takes an expression and a list of pairs of names and values.
see_also: [abstract-syntax-tree, interpreter-pattern, bytecode, recursive-descent-parsing, visitor-pattern, sum-types, closures, garbage-collection]
sources:
  - paper: doi:10.1145/367177.367199
    title: John McCarthy, Recursive Functions of Symbolic Expressions and Their Computation by Machine, Part I, Communications of the ACM 3(4), 1960
    locator: section 1, Introduction (apply as an interpreter)
  - paper: doi:10.1145/367177.367199
    title: John McCarthy, Recursive Functions of Symbolic Expressions and Their Computation by Machine, Part I, Communications of the ACM 3(4), 1960
    locator: section 3f, The Universal S-Function apply (eval, assoc, pair)
  - url: https://craftinginterpreters.com/evaluating-expressions.html#evaluating-unary-expressions
    title: Robert Nystrom, Crafting Interpreters, chapter 7, Evaluating Expressions
    locator: 7.2.3 Evaluating unary expressions (post-order traversal)
  - url: https://craftinginterpreters.com/statements-and-state.html#environments
    title: Robert Nystrom, Crafting Interpreters, chapter 8, Statements and State
    locator: 8.3 Environments (bindings, undefined variables as runtime errors)
  - url: https://craftinginterpreters.com/statements-and-state.html#nesting-and-shadowing
    title: Robert Nystrom, Crafting Interpreters, chapter 8, Statements and State
    locator: 8.5.1 Nesting and shadowing (chained environments)
  - url: https://craftinginterpreters.com/evaluating-expressions.html#detecting-runtime-errors
    title: Robert Nystrom, Crafting Interpreters, chapter 7, Evaluating Expressions
    locator: 7.3.1 Detecting runtime errors (unwinding the recursive calls)
  - url: https://craftinginterpreters.com/resolving-and-binding.html#semantic-analysis
    title: Robert Nystrom, Crafting Interpreters, chapter 11, Resolving and Binding
    locator: 11.2 Semantic Analysis (a variable resolved each time it is evaluated)
  - url: https://craftinginterpreters.com/resolving-and-binding.html#interpreting-resolved-variables
    title: Robert Nystrom, Crafting Interpreters, chapter 11, Resolving and Binding
    locator: 11.4 Interpreting Resolved Variables (the number of environments to the declaration)
  - url: https://craftinginterpreters.com/chunks-of-bytecode.html
    title: Robert Nystrom, Crafting Interpreters, chapter 14, Chunks of Bytecode
    locator: chapter introduction (jlox and an equivalent C program on the same Fibonacci program)
  - url: https://craftinginterpreters.com/chunks-of-bytecode.html#why-not-walk-the-ast
    title: Robert Nystrom, Crafting Interpreters, chapter 14, Chunks of Bytecode
    locator: 14.1.1 Why not walk the AST?
  - url: https://craftinginterpreters.com/chunks-of-bytecode.html#what-is-bytecode
    title: Robert Nystrom, Crafting Interpreters, chapter 14, Chunks of Bytecode
    locator: 14.1.3 What is bytecode?
  - url: https://craftinginterpreters.com/evaluating-expressions.html#evaluating-expressions
    title: Robert Nystrom, Crafting Interpreters, chapter 7, Evaluating Expressions
    locator: 7.2 Evaluating Expressions (the Interpreter class as a visitor)
  - url: https://docs.python.org/3.14/library/exceptions.html#RecursionError
    title: The Python Standard Library, Built-in Exceptions, Python 3.14
    locator: RecursionError
---

## Idea

A parser turns the text of a program into an [[abstract-syntax-tree]]. A tree-walking interpreter runs the program directly from that tree. It has one case for each kind of node. To evaluate `a * b`, it evaluates `a`, evaluates `b`, and multiplies the two values. To run a `while` loop, it evaluates the condition and runs the body until the condition is false. The interpreter is a recursive function over the tree, and the language it is written in, the host language, does the real work.

Variables need a place to live. The interpreter keeps them in an environment, a map from names to values.[^4] A block opens a new environment that points to the environment around it. The interpreter looks up a name in the innermost environment first and then follows the parent links outward.[^5]

The method is the shortest path from a parser to a running language. The structure at run time is the structure of the syntax, so there is no second representation to design or debug.[^10] The price is speed: each step of the program costs a dynamic dispatch on a node and a chase of pointers through the heap.[^10]

## How it works

### Expressions are evaluated bottom-up

A node can compute its value only when it has the values of its children. The interpreter therefore evaluates the operands of a node before it applies the operator. This order is a post-order traversal of the tree.[^3] The recursion of the evaluator follows the nesting of the expression, so it is a direct case of [[recursion]] over a [[sum-types|sum type]].

### Statements change the state

An expression gives a value. A statement has an effect: it binds a variable, changes one, or prints. The interpreter has a second function for statements, and the two functions call each other. Control flow in the guest language becomes control flow in the host: the guest `while` is a host `while` around a call that runs the body.

### Environments form a chain

LISP's `eval[e; a]` already worked this way in 1960. Its second argument is a list of pairs of names and values. A variable is looked up with `assoc`, and a call of a lambda adds the pairs of parameters and arguments to the front of the list with `pair`.[^2] Crafting Interpreters uses one hash map for each scope, and each map has a reference to the enclosing one. A lookup walks from the innermost map outward until it finds the name.[^5] Shadowing falls out of this order. An inner `let x` hides the outer `x` because the walk finds the inner one first.

### The whole interpreter

The program below computes 5 factorial with a loop. It builds the tree by hand, so that the page shows only the interpreter. The [[recursive-descent-parsing]] page shows how a parser builds such a tree. The interpreter counts the nodes that it visits, the variable lookups, and the parent links that the lookups follow.

```python run id=interpreter
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
    left: object
    right: object


@dataclass
class Let:
    name: str
    init: object


@dataclass
class Assign:
    name: str
    value: object


@dataclass
class Block:
    body: list


@dataclass
class While:
    cond: object
    body: object


@dataclass
class Print:
    value: object


class Env:
    def __init__(self, parent=None):
        self.vars = {}
        self.parent = parent


class Interpreter:
    def __init__(self):
        self.nodes = self.lookups = self.links = 0

    def find(self, env, name):
        self.lookups += 1
        while env is not None:
            if name in env.vars:
                return env
            self.links += 1
            env = env.parent
        raise NameError(f"undefined variable {name!r}")

    def evaluate(self, node, env):
        self.nodes += 1
        match node:
            case Num(value):
                return value
            case Var(name):
                return self.find(env, name).vars[name]
            case Bin(op, left, right):
                a = self.evaluate(left, env)
                b = self.evaluate(right, env)
                match op:
                    case "+":
                        return a + b
                    case "-":
                        return a - b
                    case "*":
                        return a * b
                    case "<":
                        return 1 if a < b else 0
        raise TypeError(f"unknown expression {node!r}")

    def execute(self, node, env):
        self.nodes += 1
        match node:
            case Let(name, init):
                env.vars[name] = self.evaluate(init, env)
            case Assign(name, value):
                result = self.evaluate(value, env)
                self.find(env, name).vars[name] = result
            case Block(body):
                inner = Env(env)
                for stmt in body:
                    self.execute(stmt, inner)
            case While(cond, body):
                while self.evaluate(cond, env) != 0:
                    self.execute(body, env)
            case Print(value):
                print(self.evaluate(value, env))
            case _:
                raise TypeError(f"unknown statement {node!r}")


# let n = 5; let acc = 1;
# while (0 < n) { let step = n; acc = acc * step; n = n - 1; }
# print acc;
program = Block([
    Let("n", Num(5)),
    Let("acc", Num(1)),
    While(Bin("<", Num(0), Var("n")), Block([
        Let("step", Var("n")),
        Assign("acc", Bin("*", Var("acc"), Var("step"))),
        Assign("n", Bin("-", Var("n"), Num(1))),
    ])),
    Print(Var("acc")),
])

interp = Interpreter()
interp.execute(program, Env())
print(f"nodes visited: {interp.nodes}")
print(f"variable lookups: {interp.lookups}, parent links followed: {interp.links}")
```

```output for=interpreter
120
nodes visited: 81
variable lookups: 37, parent links followed: 25
```

The tree has 22 nodes, but the interpreter visits 81, because it visits the condition and the body of the loop again on each pass. The loop body is a block, so each pass makes a fresh environment. `step` is local to that environment. `n` and `acc` live one level out, so each lookup of them from inside the body follows one parent link.

### Errors unwind the host stack

An error deep in the tree, such as an undefined variable, must stop the whole evaluation. The interpreter's state is the host call stack, so the natural tool is an exception of the host language. It unwinds every recursive call at once.[^6] The interpreter above raises `NameError` for an unknown name and `TypeError` for an unknown node.

## In languages

Every general-purpose language can host a tree-walking interpreter. The languages differ in how the evaluator selects the case for a node. The examples run the same program and print the same three lines.

| Language | Verdict | Form |
|---|---|---|
| Python | idiom | dataclasses for the nodes; `match` with class patterns in `evaluate` and `execute` |
| JavaScript | idiom | plain objects with a `kind` field; `switch` on the kind |
| TypeScript | idiom | a discriminated union; `switch` on the kind, checked for exhaustiveness |
| Java | idiom | a sealed interface and records; `switch` with record patterns |
| Kotlin | idiom | a sealed interface and data classes; `when` with `is` |
| C# | idiom | records; a `switch` expression with type patterns |
| Go | idiom | an interface for nodes; a type switch |
| Rust | idiom | an `enum` with `Box` children; `match`; environments in `Rc<RefCell<...>>` |
| C++ | idiom | `std::variant` nodes; `std::visit` or `std::get_if` |
| C | idiom | a tagged struct; `switch` on the tag; environments as linked lists |
| PowerShell | idiom | hashtables for nodes and a `switch` on the kind (no example: pwsh is not installed) |
| Bash | idiom | possible with associative arrays and recursion, but slow and hard to read |
| SQL | n/a | a query language, not a host for an evaluator |
| Regex | n/a | a pattern language, not a host for an evaluator |
| HTML/CSS | n/a | no recursion over data |

In the object-oriented form, each node class has its own method that interprets the node. This is the [[interpreter-pattern]]. Crafting Interpreters builds its first interpreter, jlox, in Java with the Visitor pattern instead. The evaluator is one class that implements the visitor interface, with one `visit` method for each class of expression.[^12] Languages with [[sum-types]] and pattern matching put the cases in one function instead, as the Python example does. See [[visitor-pattern]].

## Variations and related ideas

- **A resolver pass.** The interpreter above searches the chain of environments on every lookup, even though the answer for a given variable node never changes. A variable inside a loop that runs a thousand times is resolved a thousand times.[^7] A resolver walks the tree once before the run and records, for each variable node, how many environments lie between the use and the declaration. The interpreter then follows exactly that many parent links and does not search.[^8]
- **Closures.** A function value that keeps a pointer to the environment where it was created is a closure. The environment chain above is the data structure that makes [[closures]] work, and it is why environments live on the heap and not on the host stack.
- **Memory.** Each pass through a block allocates an environment. When a block ends, its environment becomes garbage unless a closure still points to it. An interpreter written in a host with [[garbage-collection]] gets this for free. An interpreter in C must free the environments itself.
- **Bytecode.** A [[bytecode]] interpreter compiles the tree once into a flat array of instructions and then runs a loop over the array. The [[bytecode]] page compiles the same factorial program and counts its instructions.

## Pitfalls

### One flat environment for the whole program

An interpreter that stores every variable in one dictionary gives blocks no scope of their own. An inner `let x` overwrites the outer `x`, and the change is still visible after the block ends.

```python wrong id=flat-env
env = {}


def run(node):
    match node:
        case ("let", name, value):
            env[name] = value
        case ("block", *body):
            for stmt in body:
                run(stmt)
        case ("print", name):
            print(name, "=", env[name])


# let x = 1; { let x = 2; print x; } print x;
run(("block", ("let", "x", 1), ("block", ("let", "x", 2), ("print", "x")), ("print", "x")))
```

```output for=flat-env
x = 2
x = 2
```

The second line should print `x = 1`. The fix is a new environment for each block, with a link to the enclosing one, as in the main example.

```python run id=flat-env-fix
class Env:
    def __init__(self, parent=None):
        self.vars = {}
        self.parent = parent

    def get(self, name):
        env = self
        while name not in env.vars:
            env = env.parent
        return env.vars[name]


def run(node, env):
    match node:
        case ("let", name, value):
            env.vars[name] = value
        case ("block", *body):
            inner = Env(env)
            for stmt in body:
                run(stmt, inner)
        case ("print", name):
            print(name, "=", env.get(name))


run(("block", ("let", "x", 1), ("block", ("let", "x", 2), ("print", "x")), ("print", "x")), Env())
```

```output for=flat-env-fix
x = 2
x = 1
```

### Guest loops on the host stack

A `while` loop can be written as recursion: run the body once, then run the whole loop again. The code looks clean. Each pass, however, adds frames to the host stack, and the guest program now has a loop limit that has nothing to do with the guest language. Python raises `RecursionError` when the depth of the stack passes the recursion limit.[^13]

```python fails expect=exception:RecursionError id=loop-recursion
def evaluate(node, env):
    match node:
        case ("num", value):
            return value
        case ("var", name):
            return env[name]
        case ("lt", a, b):
            return evaluate(a, env) < evaluate(b, env)
        case ("sub", a, b):
            return evaluate(a, env) - evaluate(b, env)


def execute(node, env):
    match node:
        case ("set", name, expr):
            env[name] = evaluate(expr, env)
        case ("print", expr):
            print(evaluate(expr, env))
        case ("while", cond, body):
            if evaluate(cond, env):
                execute(body, env)
                execute(node, env)


env = {"n": 10_000}
loop = ("while", ("lt", ("num", 0), ("var", "n")), ("set", "n", ("sub", ("var", "n"), ("num", 1))))
execute(loop, env)
execute(("print", ("var", "n")), env)
```

The fix uses a host loop for the guest loop. The stack stays at a constant depth, whatever the number of passes.

```python run id=loop-recursion-fix
def evaluate(node, env):
    match node:
        case ("num", value):
            return value
        case ("var", name):
            return env[name]
        case ("lt", a, b):
            return evaluate(a, env) < evaluate(b, env)
        case ("sub", a, b):
            return evaluate(a, env) - evaluate(b, env)


def execute(node, env):
    match node:
        case ("set", name, expr):
            env[name] = evaluate(expr, env)
        case ("print", expr):
            print(evaluate(expr, env))
        case ("while", cond, body):
            while evaluate(cond, env):
                execute(body, env)


env = {"n": 10_000}
loop = ("while", ("lt", ("num", 0), ("var", "n")), ("set", "n", ("sub", ("var", "n"), ("num", 1))))
execute(loop, env)
execute(("print", ("var", "n")), env)
```

```output for=loop-recursion-fix
0
```

The same limit applies to guest recursion. If each guest call becomes a host call, a deep guest recursion overflows the host stack.

### Errors that wait for the code to run

A tree-walking interpreter looks up a name only when it evaluates the node. A typo in a branch that does not run is never reported. Crafting Interpreters makes this choice on purpose for Lox, because an error at the point of use, and not at the point of reference, lets mutually recursive functions refer to each other before both exist.[^4] The cost is that the program below runs and prints `0` with a misspelled name in it.

```python wrong id=late-error
def evaluate(node, env):
    match node:
        case ("num", value):
            return value
        case ("var", name):
            if name not in env:
                raise NameError(f"undefined variable {name!r}")
            return env[name]
        case ("lt", a, b):
            return evaluate(a, env) < evaluate(b, env)


def execute(node, env):
    match node:
        case ("let", name, expr):
            env[name] = evaluate(expr, env)
        case ("print", expr):
            print(evaluate(expr, env))
        case ("while", cond, body):
            while evaluate(cond, env):
                execute(body, env)
        case ("block", *body):
            for stmt in body:
                execute(stmt, env)


# let count = 0; while (count < 0) { print cuont; } print count;
program = ("block",
    ("let", "count", ("num", 0)),
    ("while", ("lt", ("var", "count"), ("num", 0)), ("print", ("var", "cuont"))),
    ("print", ("var", "count")),
)
execute(program, {})
```

```output for=late-error
0
```

A static pass over the whole tree finds the name before the program starts. The pass visits both branches of every choice and every loop body once, whether or not they would run.

```python run id=late-error-fix
def check(node, scope):
    """Walk every node once before the program runs, in all branches."""
    match node:
        case ("num", _):
            pass
        case ("var", name):
            if name not in scope:
                raise NameError(f"undefined variable {name!r}")
        case ("lt", a, b):
            check(a, scope)
            check(b, scope)
        case ("let", name, expr):
            check(expr, scope)
            scope.add(name)
        case ("print", expr):
            check(expr, scope)
        case ("while", cond, body):
            check(cond, scope)
            check(body, scope)
        case ("block", *body):
            for stmt in body:
                check(stmt, scope)


program = ("block",
    ("let", "count", ("num", 0)),
    ("while", ("lt", ("var", "count"), ("num", 0)), ("print", ("var", "cuont"))),
    ("print", ("var", "count")),
)
try:
    check(program, set())
    print("no errors")
except NameError as e:
    print(f"error before the run: {e}")
```

```output for=late-error-fix
error before the run: undefined variable 'cuont'
```

This check is a small version of the resolver pass. A full one must also handle functions that refer to names declared later.

## Cost

- **Dispatch on every node.** Each node visit is a call plus a branch on the type of the node. In the example, 22 nodes cost 81 visits. A loop multiplies the visits of its body by the number of passes.
- **Pointers and locality.** The tree is a web of separate objects on the heap. Each pointer takes 32 or 64 bits, each object carries a header, and the objects are scattered in memory. This is bad for the CPU caches, which load whole lines of adjacent bytes at a time.[^10]
- **Variable lookups.** A lookup through a chain of hash maps costs one hash lookup for each scope it passes. The example followed 25 parent links for 37 lookups. A resolver pass removes the search but still follows the links.[^8]
- **Measured gap.** Crafting Interpreters reports one measurement. A Fibonacci program took jlox, its tree-walking interpreter in Java, about 72 seconds on the author's laptop. An equivalent C program finished in half a second.[^9] The author's conclusion is that walking the AST is "fundamentally the wrong design" for a general-purpose language that must run fast.[^9]
- **Startup.** A tree-walker can start to run as soon as the parser finishes. It has no compile step.

### Tree-walker or bytecode

| | Tree-walking interpreter | Bytecode interpreter |
|---|---|---|
| Code at run time | the AST: separate nodes linked by pointers | a dense, linear array of instructions[^11] |
| Dispatch | a call and a type test for each node | a loop that runs one instruction at a time[^11] |
| Work before the run | none after parsing | a compiler pass over the tree |
| Effort to build | the least: the runtime structure is the syntax[^10] | a compiler and a virtual machine |

Bytecode keeps the portability of a tree-walker, because the virtual machine is itself a program in a portable language. It gives up some simplicity for speed, but it is not as fast as native code.[^11]

## History

- **1960.** John McCarthy published the definition of LISP in Communications of the ACM. He described the universal function `apply`, which "plays the theoretical role of a universal Turing machine and the practical role of an interpreter".[^1] `apply` calls `eval[e; a]`, a recursive function over the expression `e` with the environment `a` as a list of pairs of names and values.[^2]

## References
