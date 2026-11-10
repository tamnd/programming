---
tag: NEW-interpreter-pattern
kind: pattern
slug: interpreter-pattern
name: Interpreter pattern
aliases: [Interpreter, tree-walking interpreter, AST interpreter]
area: software-design/behavioral-patterns
level: core
summary: >-
  Represent each rule of a small language as a class, build a tree of those objects for a sentence, and evaluate the sentence by asking the root of the tree to interpret itself.
family: behavioral-design
polarity: positive
intent: "Given a language, define a representation for its grammar along with an interpreter that uses the representation to interpret sentences in the language."
also_known_as: []
history:
  year: 1994
  people: [Erich Gamma, Richard Helm, Ralph Johnson, John Vlissides]
  note: Design Patterns (1994) described the pattern with regular expressions as the motivating example and Boolean expressions as the sample code.
see_also: [composite-pattern, visitor-pattern, flyweight-pattern, iterator-pattern, sum-types, recursion, code-injection]
sources:
  - book: isbn:9780201633610
    title: Gamma, Helm, Johnson and Vlissides, Design Patterns, Elements of Reusable Object-Oriented Software, Addison-Wesley, 1994
    locator: Chapter 5, Interpreter (Intent, Motivation, Applicability, Participants, Consequences, Implementation, Sample Code, Related Patterns)
  - url: https://craftinginterpreters.com/representing-code.html#working-with-trees
    title: Robert Nystrom, Crafting Interpreters, Chapter 5, Representing Code
    locator: 5.3 Working with Trees; 5.3.1 The Expression Problem; 5.3.2 The Visitor pattern
  - url: http://homepages.inf.ed.ac.uk/wadler/papers/expression/expression.txt
    title: Philip Wadler, The Expression Problem, message to the java-genericity mailing list, 12 November 1998
    locator: first paragraph
  - url: https://docs.python.org/3.14/library/functions.html#eval
    title: The Python Standard Library, Built-in Functions, Python 3.14
    locator: eval, Warning
  - url: https://docs.python.org/3.14/library/ast.html#ast.literal_eval
    title: The Python Standard Library, ast, Abstract syntax trees, Python 3.14
    locator: ast.literal_eval
  - url: https://docs.python.org/3.14/library/ast.html#module-ast
    title: The Python Standard Library, ast, Abstract syntax trees, Python 3.14
    locator: module introduction; ast.parse
  - url: https://docs.python.org/3.14/library/exceptions.html#RecursionError
    title: The Python Standard Library, Built-in Exceptions, Python 3.14
    locator: RecursionError
  - url: https://openjdk.org/jeps/441
    title: "JEP 441: Pattern Matching for switch"
    locator: Summary; Exhaustiveness of switch expressions and statements
  - url: https://www.typescriptlang.org/docs/handbook/2/narrowing.html#exhaustiveness-checking
    title: TypeScript Handbook, Narrowing
    locator: Discriminated unions; Exhaustiveness checking
  - url: https://doc.rust-lang.org/book/ch06-02-match.html#matches-are-exhaustive
    title: The Rust Programming Language, 6.2 The match Control Flow Construct
    locator: Matches Are Exhaustive
  - url: https://github.com/openjdk/jdk/blob/jdk-25%2B36/src/java.base/share/classes/java/util/regex/Pattern.java#L3711-L3748
    title: OpenJDK jdk-25+36 source, java/util/regex/Pattern.java
    locator: comment on the node classes and class Node, lines 3711 to 3748
  - url: https://github.com/golang/go/blob/go1.23.0/src/text/template/exec.go#L259-L295
    title: Go go1.23.0 source, src/text/template/exec.go
    locator: func (s *state) walk, lines 259 to 295
---

## Intent

Given a language, define a representation for its grammar along with an interpreter that uses the representation to interpret sentences in the language.[^1]

The language is small and belongs to the program: a filter that users type, a pricing rule, a query over a log, a template. Each rule of its grammar becomes a class. A sentence becomes a tree of objects of those classes, and the program evaluates the sentence by asking the root of the tree to interpret itself.

## Problem

A shop lets staff write discount rules such as `qty * (price + 2)` into a settings page. The program must compute the rule for each order. The rules change every week, and the people who write them are not programmers.

A first design calls the host language's `eval` on the text. This runs. It also runs any other code that the text contains. The Python documentation says that `eval` "executes arbitrary code" and that calling it with untrusted input "will lead to security vulnerabilities".[^4] A formula field becomes a door into the server. See [[code-injection]].

A second design writes the rules as `if` statements in the program. Each change of a rule is then a release of the program.

The forces are these. The sentences come from outside the program, so the program must accept only the language it defines. The program must evaluate the same sentence many times, with different values. And the program needs more than one operation on a sentence: evaluate it, show it, check that every variable is known.

## Solution

Write a grammar for the language. Make one class for each rule. A sentence is then a tree of instances, and every class has an operation `interpret` that takes a **context**, which holds the values of the variables. Design Patterns names five participants.[^1]

- **AbstractExpression** declares `interpret(context)`.
- **TerminalExpression** implements it for a symbol with no parts: a number, a variable name.
- **NonterminalExpression** implements it for a rule with parts, such as `Add(left, right)`. It interprets its parts and joins the results.
- **Context** holds the information that is global to the interpreter, such as the values of the variables.
- **Client** builds the tree for a sentence and calls `interpret`.

```text
  grammar                       classes

  expr   := term ('+' term)*    Expr (abstract): evaluate(env), show()
  term   := factor ('*' factor)*  |- Num    value          terminal
  factor := NUMBER                |- Var    name           terminal
          | NAME                  |- Add    left, right    nonterminal
          | '(' expr ')'          |- Mul    left, right    nonterminal

  sentence:  x * (y + 2)

  tree:          Mul
                /    \
             Var x    Add
                     /    \
                  Var y   Num 2
```

The tree is an **abstract syntax tree**. It keeps the structure of the sentence and drops the parentheses and the spaces. It is also an instance of [[composite-pattern]]: a nonterminal holds child expressions, and the client treats a leaf and a whole tree the same way.[^1] The evaluation is [[recursion]] over that structure. Each node evaluates its children, then itself.

The pattern does not say how to build the tree. Design Patterns says that it does not address parsing.[^1] The client can build the tree by hand, as the example files do, or use a parser. A recursive-descent parser has one function for each rule of the grammar, so its shape follows the grammar too:

```python run id=calc
import re
from dataclasses import dataclass


class LanguageError(Exception):
    pass


@dataclass(frozen=True)
class Num:
    value: int

    def evaluate(self, env):
        return self.value


@dataclass(frozen=True)
class Var:
    name: str

    def evaluate(self, env):
        if self.name not in env:
            raise LanguageError(f"unknown variable {self.name}")
        return env[self.name]


@dataclass(frozen=True)
class Add:
    left: object
    right: object

    def evaluate(self, env):
        return self.left.evaluate(env) + self.right.evaluate(env)


@dataclass(frozen=True)
class Mul:
    left: object
    right: object

    def evaluate(self, env):
        return self.left.evaluate(env) * self.right.evaluate(env)


TOKEN = re.compile(r"\s*(?:(\d+)|([a-z]+)|(.))")


def parse(text):
    tokens = [m.groups() for m in TOKEN.finditer(text) if m.group(0).strip()]
    pos = 0

    def peek():
        return tokens[pos][2] if pos < len(tokens) else None

    def expr():  # expr := term ('+' term)*
        nonlocal pos
        node = term()
        while peek() == "+":
            pos += 1
            node = Add(node, term())
        return node

    def term():  # term := factor ('*' factor)*
        nonlocal pos
        node = factor()
        while peek() == "*":
            pos += 1
            node = Mul(node, factor())
        return node

    def factor():  # factor := NUMBER | NAME | '(' expr ')'
        nonlocal pos
        if pos == len(tokens):
            raise LanguageError("unexpected end")
        number, name, symbol = tokens[pos]
        pos += 1
        if number:
            return Num(int(number))
        if name:
            return Var(name)
        if symbol == "(":
            node = expr()
            if peek() != ")":
                raise LanguageError("expected )")
            pos += 1
            return node
        raise LanguageError(f"unexpected {symbol}")

    tree = expr()
    if pos != len(tokens):
        raise LanguageError(f"unexpected {tokens[pos][2] or 'token'}")
    return tree


env = {"qty": 3, "price": 4}
for text in ["qty * (price + 2)", "qty * price + 2", "qty + disc", "qty * (2", "__import__('os')"]:
    try:
        print(f"{text!r}: {parse(text).evaluate(env)}")
    except LanguageError as err:
        print(f"{text!r}: error: {err}")

print(parse("qty * (price + 2)"))
```

```output
'qty * (price + 2)': 18
'qty * price + 2': 14
'qty + disc': error: unknown variable disc
'qty * (2': error: expected )
"__import__('os')": error: unexpected _
Mul(left=Var(name='qty'), right=Add(left=Var(name='price'), right=Num(value=2)))
```

The last input is not in the grammar, so the parser rejects it, and nothing runs. The language can only do what its classes do. That is the security property that `eval` cannot give.

The same text with `eval` runs whatever it contains:

```python wrong id=eval-formula
env = {"qty": 3, "price": 4}
rule = "print('this ran on the server') or qty * price"
print(eval(rule, {"__builtins__": {"print": print}}, env))
```

```output
this ran on the server
12
```

The documentation also says that removing `__builtins__` from the globals "is not a security mechanism".[^4] The block above gives `print` back on purpose, to make the call visible. `ast.literal_eval` is no answer here either: it accepts only literals, not operators or names.[^5]

### The parser of the host language as the front end

Python exposes its own parser through the `ast` module, which returns a tree of node objects.[^6] A program can use that parser and still define its own language, if its interpreter accepts only some node types. Every other node is an error:

```python run id=python-ast
import ast


def evaluate(node, env):
    match node:
        case ast.Expression(body=body):
            return evaluate(body, env)
        case ast.Constant(value=int() as value):
            return value
        case ast.Name(id=name) if name in env:
            return env[name]
        case ast.BinOp(left=left, op=ast.Add(), right=right):
            return evaluate(left, env) + evaluate(right, env)
        case ast.BinOp(left=left, op=ast.Mult(), right=right):
            return evaluate(left, env) * evaluate(right, env)
        case _:
            raise ValueError(f"not allowed: {type(node).__name__}")


env = {"qty": 3, "price": 4}
for text in ["qty * (price + 2)", "qty ** 2", "__import__('os')"]:
    try:
        print(f"{text}: {evaluate(ast.parse(text, mode='eval'), env)}")
    except ValueError as err:
        print(f"{text}: {err}")
```

```output
qty * (price + 2): 18
qty ** 2: not allowed: BinOp
__import__('os'): not allowed: Call
```

Here the classes of the tree come from the library, and the operations are one function with a `match` statement (see [[python/match-statement]]). The grammar of the language is now the set of `case` clauses.

## Example

Each example file builds the tree for `x * (y + 2)` by hand, as the client. It evaluates the tree with two contexts, shows it as text, and evaluates `z + 1`, where `z` has no value. The files use the two forms of the pattern.

| Form | Languages | One more operation | One more node type |
|---|---|---|---|
| one class for each rule, one method for each operation | Python, JavaScript, Go, C# | a method in every class | one class |
| one type for each rule, one function for each operation | TypeScript, Java, Rust, C++, C | one function | a case in every function |

The second form uses a closed set of node types: a discriminated union in TypeScript, a sealed interface with records in Java, an `enum` in Rust, a `std::variant` in C++, and a tagged union in C. In TypeScript, Java and Rust, the compiler reports a `switch` or `match` that misses a node type.[^8][^9][^10]

## Consequences

Design Patterns lists four consequences.[^1]

- **The grammar is straightforward to change and to extend.** The rules are classes, so inheritance can change or extend them, and a rule that one sentence uses is the same class for every sentence.
- **The grammar is straightforward to implement.** The classes for the nodes have similar code, and each one is small.
- **Complex grammars are hard to maintain.** One class for each rule means hundreds of classes for a large grammar.
- **Other ways to interpret expressions** are possible, because the tree is data. A pretty-printer or a type checker is a second walk over the same tree.

The cost of the fourth point depends on the form. In the class form, each operation is a method in every class, so one more operation touches every class. In the function form, each node type is a case in every function, so one more node type touches every function. Wadler named this trade-off the expression problem: add both cases and functions to a data type without recompiling existing code, and keep static type safety.[^3] Crafting Interpreters calls the class form, with an abstract `interpret()` on each class, "literally" the Interpreter pattern, and says that it works for tiny projects but scales poorly, because the parser, the interpreter and later passes all work on the same tree classes.[^2]

**A tree walk is slow.** Each evaluation visits every node again and makes a call for each one. Design Patterns says that the pattern works well when efficiency is not a critical concern, and that efficient interpreters usually translate the tree into another form first.[^1] A rule that runs once per order is fine. An inner loop over millions of rows is a reason to compile the tree.

**Recursion has a depth limit.** Both `evaluate` and a recursive-descent parser use one stack frame for each level of the tree. A machine-generated sentence with a deep nesting can exceed the limit, and Python raises `RecursionError`.[^7]

```python fails id=deep expect=exception:RecursionError
class Num:
    def __init__(self, value):
        self.value = value

    def evaluate(self):
        return self.value


class Add:
    def __init__(self, left, right):
        self.left, self.right = left, right

    def evaluate(self):
        return self.left.evaluate() + self.right.evaluate()


tree = Num(0)
for _ in range(100_000):  # 0 + 1 + 1 + ... as a left-deep tree
    tree = Add(tree, Num(1))
print(tree.evaluate())
```

The fix keeps the work on an explicit stack in the heap. The walk pushes a node, and when the children of a node are done, it pops their values and joins them:

```python run id=deep-iterative
class Num:
    def __init__(self, value):
        self.value = value


class Add:
    def __init__(self, left, right):
        self.left, self.right = left, right


def evaluate(root):
    values, work = [], [(root, False)]
    while work:
        node, children_done = work.pop()
        if isinstance(node, Num):
            values.append(node.value)
        elif children_done:
            right, left = values.pop(), values.pop()
            values.append(left + right)
        else:
            work += [(node, True), (node.right, False), (node.left, False)]
    return values.pop()


tree = Num(0)
for _ in range(100_000):
    tree = Add(tree, Num(1))
print(evaluate(tree))
```

```output
100000
```

The cost is clarity. The operation is no longer one method on each class. A program that accepts sentences from outside should also limit their length or depth before it evaluates them.

## When not to use it

Do not invent a language when a data format is enough. A list of rules in JSON with fixed fields (a product, a minimum quantity, a percentage) needs no grammar and no tree.

Do not write an interpreter for a large grammar by hand. Design Patterns recommends tools such as parser generators for complex grammars.[^1] For a general-purpose scripting need, embed an existing language instead of growing a private one, one feature at a time.

Do not write your own interpreter for a language that already has one. Regular expressions, SQL and template languages come with a library. Use it.

Do not use `eval` of the host language as the interpreter of a language whose sentences come from users.[^4] The small interpreter on this page exists to avoid that.

## Variants and related patterns

**Composite.** The abstract syntax tree is a composite. Design Patterns says so in the related patterns of Interpreter.[^1] Interpreter adds a grammar and an operation that gives a meaning to the tree. See [[composite-pattern]].

**Visitor.** When the class form needs many operations, a visitor moves each operation into its own class, and the node classes get one `accept` method. Design Patterns suggests Visitor for this case, and Crafting Interpreters uses it for its tree-walking interpreter.[^1][^2] In a language with sum types and exhaustive pattern matching, one function with a `match` does the same job with less code. See [[visitor-pattern]] and [[sum-types]].

**Flyweight.** Terminal symbols, such as the variable `x`, can be shared. A sentence that names `x` ten times can use one `Var("x")` object.[^1] The objects of the Python example are frozen dataclasses, so sharing them is safe. See [[flyweight-pattern]].

**Iterator.** An iterator can traverse the tree, for example to list every variable that a sentence uses.[^1] See [[iterator-pattern]].

**Compile to closures.** A variant walks the tree once and returns a function for each node. `Add` returns `lambda env: left(env) + right(env)`. The program then calls the function many times without the dispatch on node types. See [[closures]].

## Known uses

- OpenJDK: [Pattern.java at jdk-25+36](https://github.com/openjdk/jdk/blob/jdk-25%2B36/src/java.base/share/classes/java/util/regex/Pattern.java#L3711-L3748) compiles a regular expression into a tree of node objects. Its comment says that each type of node "knows how to match its equivalent construct with the match() method". `Node` is the AbstractExpression, and the regular expressions are the example that Design Patterns uses to motivate the pattern.[^11][^1]
- Go: `walk` in [text/template/exec.go at go1.23.0](https://github.com/golang/go/blob/go1.23.0/src/text/template/exec.go#L259-L295) interprets the parse tree of a template. It is the function form: one `switch` on the node type, with a case for actions, `if`, `range`, text and the other nodes.[^12]

## References
