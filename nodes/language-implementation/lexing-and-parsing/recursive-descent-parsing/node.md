---
tag: PQR252
kind: concept
slug: recursive-descent-parsing
name: Recursive descent parsing
aliases: [recursive descent parser, top-down parsing, Pratt parsing, precedence climbing, top down operator precedence]
area: language-implementation/lexing-and-parsing
level: core
summary: >-
  A recursive descent parser has one function for each rule of the grammar. Each function reads its tokens and calls the functions of inner rules, so the call stack follows the nesting of the input.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1973
  people: [Vaughan R. Pratt]
  note: Pratt published top down operator precedence, a recursive descent parser driven by the binding powers of tokens.
see_also: [abstract-syntax-tree, tree-walking-interpreter, recursion, interpreter-pattern, sum-types]
same_as:
  wikidata: Q1323264
  wikipedia: Recursive_descent_parser
sources:
  - url: https://craftinginterpreters.com/parsing-expressions.html#recursive-descent-parsing
    title: Robert Nystrom, Crafting Interpreters, chapter 6, Parsing Expressions
    locator: 6.2 Recursive Descent Parsing
  - url: https://craftinginterpreters.com/parsing-expressions.html#ambiguity-and-the-parsing-game
    title: Robert Nystrom, Crafting Interpreters, chapter 6, Parsing Expressions
    locator: 6.1 Ambiguity and the Parsing Game (precedence, associativity, left recursion)
  - paper: doi:10.1145/512927.512931
    title: Vaughan R. Pratt, Top Down Operator Precedence, Proceedings of the 1st ACM SIGACT-SIGPLAN Symposium on Principles of Programming Languages (POPL '73), 1973, pages 41 to 51 (full text at https://tdop.github.io/)
    locator: section 2.2, Conventions for Linearizing Trees (Floyd's operator precedence, binding powers, decrementing the right binding powers)
  - paper: doi:10.1145/512927.512931
    title: Vaughan R. Pratt, Top Down Operator Precedence, Proceedings of the 1st ACM SIGACT-SIGPLAN Symposium on Principles of Programming Languages (POPL '73), 1973, pages 41 to 51 (full text at https://tdop.github.io/)
    locator: section 3, Implementation (rbp, lbp, nud, led, recursive descent)
  - url: https://docs.python.org/3.14/reference/expressions.html#the-power-operator
    title: The Python Language Reference, Expressions, Python 3.14
    locator: 6.5 The power operator
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Operators/Exponentiation#description
    title: MDN Web Docs, Exponentiation (**)
    locator: Description
  - url: https://peps.python.org/pep-0617/#background-on-peg-parsers
    title: PEP 617, New PEG parser for CPython
    locator: Background on PEG parsers (recursive descent, packrat parsing)
  - url: https://peps.python.org/pep-0617/#left-recursion
    title: PEP 617, New PEG parser for CPython
    locator: Left recursion
  - url: https://craftinginterpreters.com/parsing-expressions.html#panic-mode-error-recovery
    title: Robert Nystrom, Crafting Interpreters, chapter 6, Parsing Expressions
    locator: 6.3.1 Panic mode error recovery
  - url: https://peps.python.org/pep-0617/#lack-of-left-recursion
    title: PEP 617, New PEG parser for CPython
    locator: Lack of left recursion (the definition of a left-recursive rule)
---

## Idea

A parser reads a sequence of tokens and finds its structure. A recursive descent parser does this with ordinary functions. There is one function for each rule of the grammar. The function for a rule reads the tokens that the rule matches. Where the rule refers to another rule, the function calls the function of that rule. Where a rule refers to itself, through parentheses for example, the function calls itself. The call stack of the parser then has the same shape as the nesting of the input.

The method is called top-down because it starts from the outermost rule and works down to the leaves. GCC, V8 and Roslyn use it.[^1] It needs no generator tool. The programmer writes each error message, and the code follows the grammar closely enough that a reader can check one against the other.

The grammar on this page covers integer arithmetic. Each line is one rule, and each rule becomes one function.

```text
expr  = term  { ("+" | "-") term }
term  = unary { ("*" | "/") unary }
unary = "-" unary | power
power = atom [ "^" unary ]
atom  = NUMBER | "(" expr ")"
```

`{ ... }` means zero or more times, `[ ... ]` means optional, and `|` separates choices. The order of the rules gives the precedence: `+` and `-` bind loosest, `^` binds tightest. The form of the rules gives the associativity, as the next section shows.

## How it works

### Tokens first

A lexer, also called a tokenizer, turns the characters into tokens. A parser that works on tokens does not have to skip spaces or read the digits of a number. Each token keeps its position, so that an error message can point at the source.

### One function per rule

The parser keeps an index into the token list. `peek` looks at the next token without using it, and `take` uses it. One token of lookahead is enough for this grammar: the next token always tells the parser which choice to take. Each function returns the text of its rule with full parentheses and the value of the rule. A real front end returns an [[abstract-syntax-tree]] node instead.

```python run id=parser
def tokenize(src):
    tokens, i = [], 0
    while i < len(src):
        c = src[i]
        if c.isspace():
            i += 1
        elif c.isdigit():
            j = i
            while j < len(src) and src[j].isdigit():
                j += 1
            tokens.append(("num", src[i:j], i))
            i = j
        elif c in "+-*/^()":
            tokens.append((c, c, i))
            i += 1
        else:
            raise SyntaxError(f"unexpected character {c!r} at column {i + 1}")
    tokens.append(("end", "", len(src)))
    return tokens


def combine(op, left, right):
    (ltext, a), (rtext, b) = left, right
    match op:
        case "+":
            value = a + b
        case "-":
            value = a - b
        case "*":
            value = a * b
        case "/":
            value = a // b
        case "^":
            value = a**b
    return f"({ltext} {op} {rtext})", value


class Parser:
    def __init__(self, src):
        self.tokens = tokenize(src)
        self.i = 0

    def peek(self):
        return self.tokens[self.i][0]

    def take(self):
        self.i += 1
        return self.tokens[self.i - 1]

    def fail(self, expected):
        column = self.tokens[self.i][2] + 1
        raise SyntaxError(f"expected {expected} at column {column}")

    def parse(self):
        result = self.expr()
        if self.peek() != "end":
            self.fail("an operator")
        return result

    def expr(self):  # expr = term { ("+" | "-") term }
        left = self.term()
        while self.peek() in ("+", "-"):
            op = self.take()[0]
            left = combine(op, left, self.term())
        return left

    def term(self):  # term = unary { ("*" | "/") unary }
        left = self.unary()
        while self.peek() in ("*", "/"):
            op = self.take()[0]
            left = combine(op, left, self.unary())
        return left

    def unary(self):  # unary = "-" unary | power
        if self.peek() == "-":
            self.take()
            text, value = self.unary()
            return f"(-{text})", -value
        return self.power()

    def power(self):  # power = atom [ "^" unary ]
        base = self.atom()
        if self.peek() == "^":
            self.take()
            return combine("^", base, self.unary())
        return base

    def atom(self):  # atom = NUMBER | "(" expr ")"
        kind, text, _ = self.tokens[self.i]
        if kind == "num":
            self.take()
            return text, int(text)
        if kind == "(":
            self.take()
            inner = self.expr()
            if self.peek() != ")":
                self.fail("')'")
            self.take()
            return inner
        self.fail("a number or '('")


for src in ["1 + 2 * 3", "1 - 2 - 3", "2 ^ 3 ^ 2", "-2 ^ 2", "(1 + 2) * 3", "1 + * 2", "(1 + 2", "1 $ 2"]:
    try:
        text, value = Parser(src).parse()
        print(f"{src} -> {text} = {value}")
    except SyntaxError as e:
        print(f"{src} -> error: {e}")
```

```output for=parser
1 + 2 * 3 -> (1 + (2 * 3)) = 7
1 - 2 - 3 -> ((1 - 2) - 3) = -4
2 ^ 3 ^ 2 -> (2 ^ (3 ^ 2)) = 512
-2 ^ 2 -> (-(2 ^ 2)) = -4
(1 + 2) * 3 -> ((1 + 2) * 3) = 9
1 + * 2 -> error: expected a number or '(' at column 5
(1 + 2 -> error: expected ')' at column 7
1 $ 2 -> error: unexpected character '$' at column 3
```

### Precedence comes from the layers

`expr` calls `term` for each operand, and `term` calls `unary`. So a `*` is always consumed inside a `term`, before `expr` sees the next `+`. In `1 + 2 * 3`, the multiplication is complete before the addition can use its result. A grammar with one layer per level of precedence is called stratified.[^2] Parentheses work because `atom` calls `expr` again from the bottom layer. The text inside them is parsed as a whole expression and returned as one operand.

### Associativity comes from loops and recursion

`1 - 2 - 3` must mean `(1 - 2) - 3`. The loop in `expr` gives that: it combines the first two operands, then uses the result as the left operand of the next `-`. A loop makes an operator left-associative.

`2 ^ 3 ^ 2` must mean `2 ^ (3 ^ 2)`. The function `power` does not loop. After `^`, it calls `unary`, which can reach `power` again, so the right operand takes the rest of the chain. Recursion on the right makes an operator right-associative.

`-2 ^ 2` is `-(2 ^ 2)`, because `unary` handles the minus and then calls `power` for the operand. Python's power operator has the same rule: it "binds more tightly than unary operators on its left", and `-1**2` is `-1`.[^5]

### The call stack follows the input

Each call of a rule function is a frame on the stack (see [[recursion]]). The trace shows the calls for `1 - 2 * 3`. The indentation is the depth of the call.

```python run id=trace
tokens = ["1", "-", "2", "*", "3", "end"]
pos = 0
depth = 0


def traced(fn):
    def wrapper():
        global depth
        print("  " * depth + fn.__name__ + " at " + tokens[pos])
        depth += 1
        result = fn()
        depth -= 1
        return result

    return wrapper


@traced
def expr():
    global pos
    value = term()
    while tokens[pos] in ("+", "-"):
        op = tokens[pos]
        pos += 1
        right = term()
        value = value + right if op == "+" else value - right
    return value


@traced
def term():
    global pos
    value = atom()
    while tokens[pos] == "*":
        pos += 1
        value *= atom()
    return value


@traced
def atom():
    global pos
    pos += 1
    return int(tokens[pos - 1])


print(expr())
```

```output for=trace
expr at 1
  term at 1
    atom at 1
  term at 2
    atom at 2
    atom at 3
-5
```

Each number is read by `atom` at the bottom of a chain of calls. The first number needs a call at every level. The deepest point of the stack matches the deepest nesting of the input. The `*` is handled inside the second `term` call, which is why `2 * 3` is one operand of `-`.

### Pratt parsing

A stratified grammar needs one function per level of precedence, and every operand passes through all the levels. A language with fifteen levels of precedence pays for fifteen calls before it reaches a number. Pratt's method, top down operator precedence, uses one function and a table of numbers instead.[^4]

Pratt builds on Floyd's operator precedence. In that scheme, a number is associated with each argument position of an operator. These numbers "are sometimes called binding powers", and an operand goes to the operator whose argument position has the higher number.[^3] In Pratt's parser, each token has a left binding power, `lbp`. The parser function takes a right binding power, `rbp`, the strength with which the caller holds on to the operand. The parser keeps extending the left operand while the next token binds more tightly: "To return to q0 we require rbp < lbp."[^4]

A token can play two roles. Its nud (null denotation) is its meaning with no expression on its left. Its led (left denotation) is its meaning with an expression on its left. In Pratt's words, "the nud of - denotes unary minus, and its led, binary minus".[^4] For right associativity, Pratt decrements by 1 the right binding powers of the class.[^3] Here `^` has a left binding power of 30 and calls the parser for its right operand with 29, so a second `^` (30 > 29) joins the right operand.

```python run id=pratt
import re

LBP = {"+": 10, "-": 10, "*": 20, "/": 20, "^": 30}
RIGHT_ASSOC = {"^"}
UNARY_MINUS_BP = 25


def combine(op, left, right):
    (ltext, a), (rtext, b) = left, right
    match op:
        case "+":
            value = a + b
        case "-":
            value = a - b
        case "*":
            value = a * b
        case "/":
            value = a // b
        case "^":
            value = a**b
    return f"({ltext} {op} {rtext})", value


def parse(src):
    tokens = re.findall(r"\d+|[-+*/^()]", src) + ["end"]
    pos = 0

    def expression(rbp):
        nonlocal pos
        token = tokens[pos]
        pos += 1
        # nud: the meaning of a token with nothing on its left
        if token.isdigit():
            left = (token, int(token))
        elif token == "-":
            text, value = expression(UNARY_MINUS_BP)
            left = (f"(-{text})", -value)
        elif token == "(":
            left = expression(0)
            pos += 1  # skip ")"
        else:
            raise SyntaxError(f"unexpected {token!r}")
        # led: extend the left operand while the next operator binds tighter
        while LBP.get(tokens[pos], 0) > rbp:
            op = tokens[pos]
            pos += 1
            right_bp = LBP[op] - 1 if op in RIGHT_ASSOC else LBP[op]
            left = combine(op, left, expression(right_bp))
        return left

    return expression(0)


for src in ["1 + 2 * 3", "1 - 2 - 3", "2 ^ 3 ^ 2", "-2 ^ 2", "(1 + 2) * 3", "8 / 4 / 2"]:
    text, value = parse(src)
    print(f"{src} -> {text} = {value}")
```

```output for=pratt
1 + 2 * 3 -> (1 + (2 * 3)) = 7
1 - 2 - 3 -> ((1 - 2) - 3) = -4
2 ^ 3 ^ 2 -> (2 ^ (3 ^ 2)) = 512
-2 ^ 2 -> (-(2 ^ 2)) = -4
(1 + 2) * 3 -> ((1 + 2) * 3) = 9
8 / 4 / 2 -> ((8 / 4) / 2) = 1
```

The results agree with the stratified parser. The precedence is now data: a table of numbers, not a chain of functions. To add an operator, the programmer adds a row to the table.

## In languages

A recursive descent parser needs functions that can call themselves, a way to return a result and a way to report an error. Every general-purpose language in tier 1 has them, so the verdict is idiom everywhere. The examples implement the grammar of this page with the same inputs and print the same lines.

| Language | Verdict | Form |
|---|---|---|
| Python | idiom | methods on a parser class; `SyntaxError` for errors |
| JavaScript | idiom | closures over the token index; `SyntaxError` for errors |
| TypeScript | idiom | as JavaScript, with a type for the result |
| Java | idiom | methods on a parser class; an unchecked exception for errors |
| Kotlin | idiom | methods on a parser class; a `fail` function of type `Nothing` throws the error |
| C# | idiom | methods on a parser class; a custom exception for errors |
| Go | idiom | methods on a parser struct; errors returned by `panic` and `recover` inside the parser |
| Rust | idiom | methods that return `Result`; `?` passes errors up the recursion |
| C++ | idiom | member functions; an exception for errors |
| C | idiom | functions over a global cursor; `setjmp` and `longjmp` for errors |
| PowerShell | idiom | functions and a script-scope index (no example: pwsh is not installed) |
| Bash | idiom | functions and global variables; possible but slow and hard to read |
| SQL | n/a | a query language with no general recursion over a token list |
| Regex | absent | a regular expression cannot match nested parentheses |
| HTML/CSS | n/a | no functions |

The languages differ in how an error leaves the recursion. An exception unwinds all the frames at once. Rust and Go return the error from each level, Rust with `?` and Go with an explicit check, or with `panic` and `recover` at the top. C has no exceptions; the C example jumps back to the caller with `longjmp`.

The languages also differ on `-2 ^ 2`. Python gives `**` higher precedence than unary minus on its left.[^5] JavaScript makes `-2 ** 2` a `SyntaxError`, so the programmer must write the parentheses.[^6] The JavaScript example shows the error.

CPython's own parser is generated from a grammar. It is a PEG parser, which PEP 617 describes as usually "constructed as a recursive descent parser in which every rule in the grammar corresponds to a function".[^7]

## Variations and related ideas

- **Pratt parsing and precedence climbing.** One loop and a table of binding powers replace one function per level (see How it works).[^4] A parser can use functions per rule for statements and a Pratt loop for expressions.
- **Backtracking and PEG.** A parser that cannot decide with one token of lookahead can try one choice and go back to try another. A PEG parser tries its choices in order. Without help, this backtracking can take exponential time. Packrat parsing avoids that by memoizing the result of each rule at each position, at the cost of memory.[^7]
- **Left recursion in PEG.** CPython's PEG parser accepts left-recursive rules. It uses a technique based on the memoization cache, so that `rule: rule 'a'` does not recurse forever.[^8]
- **Error recovery.** A parser that stops at the first error reports one problem per run. Panic mode recovery skips tokens until a point where the grammar can restart, such as the end of a statement, and then continues to find more errors.[^9]
- **Bottom-up parsers.** LR parsers start from the leaves and combine them into larger and larger structures.[^1]
- **Parser combinators.** A library builds parsers from functions that combine smaller parsers. The result is still recursive descent, written with higher-order functions (see [[closures]]).

## Pitfalls

### Left recursion

The natural way to write a left-associative operator in a grammar is `expr = expr "-" term | term`. A rule is left-recursive if it can derive a form that starts with itself.[^10] The function for such a rule calls itself before it reads a token. The second call is in the same state as the first, so the recursion never ends "until the parser hits a stack overflow and dies".[^2]

```python fails expect=exception:RecursionError id=left-recursion
tokens = ["1", "-", "2", "-", "3", "end"]
pos = 0


def expr():  # expr = expr "-" term | term
    global pos
    left = expr()
    if tokens[pos] == "-":
        pos += 1
        return left - term()
    return left


def term():
    global pos
    pos += 1
    return int(tokens[pos - 1])


print(expr())
```

The fix is the loop: `expr = term { "-" term }`. The loop consumes a token before it goes around again, so each step makes progress, and it keeps the left associativity.

```python run id=left-recursion-fix
tokens = ["1", "-", "2", "-", "3", "end"]
pos = 0


def expr():  # expr = term { "-" term }
    global pos
    left = term()
    while tokens[pos] == "-":
        pos += 1
        left = left - term()
    return left


def term():
    global pos
    pos += 1
    return int(tokens[pos - 1])


print(expr())
```

```output for=left-recursion-fix
-4
```

### Right recursion in place of a loop

Another way to remove the left recursion is to move the recursive call to the right: `expr = term [ "-" expr ]`. The parser now ends, but it parses `1 - 2 - 3` as `1 - (2 - 3)`. The answer is wrong, and no error tells the programmer.

```python wrong id=right-recursion
tokens = ["1", "-", "2", "-", "3", "end"]
pos = 0


def expr():  # expr = term [ "-" expr ]
    global pos
    left = term()
    if tokens[pos] == "-":
        pos += 1
        return left - expr()
    return left


def term():
    global pos
    pos += 1
    return int(tokens[pos - 1])


print(expr())
```

```output for=right-recursion
2
```

The result is 2, not -4. Right recursion is correct only for operators that are right-associative, such as `^`. Use a loop for the left-associative ones.

### Deep nesting overflows the stack

Each `(` adds frames for every rule from `expr` down to `atom`. Input with thousands of nested parentheses overflows the stack of a recursive descent parser. The input may come from a user or an attacker. CPython sets a limit and reports a syntax error.

```python run id=nesting
try:
    eval("(" * 300 + "1" + ")" * 300)
except SyntaxError as e:
    print("SyntaxError:", e.msg)
```

```output for=nesting
SyntaxError: too many nested parentheses
```

A parser of untrusted input should count its depth in the function for parentheses and report an error when the count passes a limit. A clear error is better than a crash of the process.

## Cost

- **Time.** With one token of lookahead and no backtracking, each token is read once, and the parser runs in time proportional to the length of the input. Backtracking can make the time exponential unless the parser memoizes, as packrat parsers do.[^7]
- **Calls per operand.** In a stratified grammar, an operand passes through every level between the current rule and `atom`. The trace above shows six calls for three numbers. Pratt parsing makes one call per operand and one table lookup per operator.[^4]
- **Stack.** The stack depth is proportional to the nesting depth of the input, times the number of levels between `expr` and `atom`.
- **Code.** A recursive descent parser is a few lines per rule. A change to the grammar is a change to the matching function.

## History

- **1963.** Robert W. Floyd suggested operator precedence. Numbers associated with argument positions, called binding powers, decide which operator takes an operand. Floyd showed that ALGOL 60 could be handled in this way.[^3]
- **1973.** Vaughan Pratt published "Top Down Operator Precedence" at the first POPL. He made Floyd's method work top-down, "implementing the stack by means of recursion, a technique known as recursive descent", and attached the nud and led code to the tokens.[^4]
- **2020.** PEP 617, created on 24 March 2020, proposed a PEG parser for CPython. The parser supports left-recursive rules through its memoization cache.[^8]

## References
