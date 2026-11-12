---
tag: 4TQ4HF
kind: concept
slug: operator-precedence
name: Operator precedence
aliases: [operator precedence, precedence, order of operations, associativity, operator associativity, binding power, precedence climbing, Pratt parsing, operator-precedence parser, shunting yard, left-associative, right-associative, non-associative]
area: syntax-and-semantics/grammars-and-notation
level: core
summary: >-
  Operator precedence and associativity decide how a parser groups an expression without parentheses, such as 1 + 2 * 3. Languages agree on arithmetic but differ on bitwise and logical operators.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1957
  people: [John Backus, Edsger W. Dijkstra, Vaughan Pratt, Dennis Ritchie]
  note: The FORTRAN I compiler of 1957 grouped expressions by adding parentheses around each operator. Dijkstra's shunting yard algorithm and Pratt's top-down operator precedence parser of 1973 are the usual methods today.
see_also: [context-free-grammars, abstract-syntax-tree, recursive-descent-parsing, order-of-evaluation, short-circuit-evaluation, statements-and-expressions, tokens-and-lexing]
same_as:
  wikidata: Q845118
  wikipedia: Order_of_operations
sources:
- url: https://docs.python.org/3/reference/expressions.html
  title: Expressions, The Python Language Reference
  locator: 6.5 The power operator; 6.17 Operator precedence
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Operators/Operator_precedence
  title: Operator precedence, MDN Web Docs
  locator: Precedence and associativity
- url: https://go.dev/ref/spec
  title: The Go Programming Language Specification
  locator: Operator precedence
- url: https://docs.oracle.com/javase/tutorial/java/nutsandbolts/operators.html
  title: Operators, The Java Tutorials
  locator: Operator Precedence
- url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/operators/
  title: C# operators and expressions, C# language reference
  locator: Operator precedence
- url: https://doc.rust-lang.org/reference/expressions.html
  title: Expressions, The Rust Reference
  locator: Expression precedence
- url: https://en.cppreference.com/w/c/language/operator_precedence
  title: C Operator Precedence, cppreference.com
  locator: Notes
- url: https://en.cppreference.com/w/cpp/language/operator_precedence
  title: C++ Operator Precedence, cppreference.com
  locator: Notes
- url: https://www.nokia.com/bell-labs/about/dennis-m-ritchie/chist.html
  title: The Development of the C Language, Dennis M. Ritchie
  locator: Embryonic C
- url: https://en.wikipedia.org/wiki/Operator-precedence_parser
  title: Operator-precedence parser, Wikipedia
  locator: Full parenthesization; Pratt parsing
- url: https://en.wikipedia.org/wiki/Order_of_operations
  title: Order of operations, Wikipedia
  locator: Programming languages
---

## Idea

The text `1 + 2 * 3` has two possible trees: `(1 + 2) * 3` is 9 and `1 + (2 * 3)` is 7. A language picks one with two rules. *Precedence* says which operator binds more tightly: `*` binds more tightly than `+`, so the answer is 7. *Associativity* says how a chain of operators at the same level groups: `-` groups to the left, so `10 - 4 - 3` is `(10 - 4) - 3`, which is 3. Parentheses override both rules. Precedence is a rule of the grammar. It decides the shape of the [abstract syntax tree](/abstract-syntax-tree/), not the time at which each operand runs.

```python run id=idea
import re

# Binding power of each binary operator, and whether it groups to the right.
OPS = {"+": (10, False), "-": (10, False), "*": (20, False), "/": (20, False), "^": (30, True)}


def parse(tokens, min_bp=0):
    """Precedence climbing: read an operand, then take operators while they bind at least min_bp."""
    left = tokens.pop(0)
    if left == "(":
        left = parse(tokens)
        tokens.pop(0)  # the closing ")"
    while tokens and tokens[0] in OPS:
        bp, right_assoc = OPS[tokens[0]]
        if bp < min_bp:
            break
        op = tokens.pop(0)
        right = parse(tokens, bp if right_assoc else bp + 1)
        left = (op, left, right)
    return left


def show(t):
    return t if isinstance(t, str) else f"({show(t[1])} {t[0]} {show(t[2])})"


def value(t):
    if isinstance(t, str):
        return int(t)
    a, b = value(t[1]), value(t[2])
    return {"+": a + b, "-": a - b, "*": a * b, "/": a // b, "^": a**b}[t[0]]


for src in ["1 + 2 * 3", "(1 + 2) * 3", "10 - 4 - 3", "2 ^ 3 ^ 2", "8 / 4 / 2", "1 + 2 * 3 ^ 2 - 4"]:
    tree = parse(re.findall(r"\d+|[-+*/^()]", src))
    print(f"{src:<20} => {show(tree):<28} = {value(tree)}")

# If ^ grouped to the left, 2 ^ 3 ^ 2 would be (2 ^ 3) ^ 2 = 64.
OPS["^"] = (30, False)
print("left-grouping ^:", show(parse(re.findall(r"\d+|[-+*/^()]", "2 ^ 3 ^ 2"))))
```

```output for=idea
1 + 2 * 3            => (1 + (2 * 3))                = 7
(1 + 2) * 3          => ((1 + 2) * 3)                = 9
10 - 4 - 3           => ((10 - 4) - 3)               = 3
2 ^ 3 ^ 2            => (2 ^ (3 ^ 2))                = 512
8 / 4 / 2            => ((8 / 4) / 2)                = 1
1 + 2 * 3 ^ 2 - 4    => ((1 + (2 * (3 ^ 2))) - 4)    = 15
left-grouping ^: ((2 ^ 3) ^ 2)
```

The parser gives each operator a binding power. After it reads an operand, it takes the next operator only if that operator binds at least as tightly as the current level, and it parses the right side at a higher level. For a left-associative operator, the right side starts one level higher, so an equal operator stops it. For a right-associative operator such as `^`, the right side starts at the same level, so `2 ^ 3 ^ 2` is `2 ^ (3 ^ 2)`.

## How it works

**A table of levels.** Each language documents a table of operators from the tightest to the loosest. Python's table lists the operators "from highest precedence (most binding) to lowest precedence (least binding)"[^1], and MDN says that "Operators with higher precedence become the operands of operators with lower precedence"[^2]. Most languages put postfix operators and calls first, then unary operators, then multiplication, addition, shifts, comparisons, bitwise operators, logical operators, the conditional operator and assignment.

**Associativity.** Most binary operators group to the left, so `8 / 4 / 2` is 1. Assignment groups to the right, so `a = b = 0` sets both. Exponentiation groups to the right in Python, JavaScript and mathematics: MDN gives `4 ** 3 ** 2` as `4 ** (3 ** 2)`[^2]. The conditional operator groups to the right in C-like languages, so a chain of `? :` reads as an if-else chain. Some operators do not associate: Rust marks the comparison operators "Require parentheses", so `a < b < c` is a compile error[^6]. Python gives the same text a third meaning: `a < b < c` is a chain that means `a < b and b < c`.

**Unary operators.** In Python, "The power operator binds more tightly than unary operators on its left; it binds less tightly than unary operators on its right"[^1], so `-2 ** 2` is −4 and `2 ** -1` is 0.5. JavaScript does not choose: `-2 ** 2` is a syntax error, and the program must write `(-2) ** 2` or `-(2 ** 2)`. In Rust, a method call binds more tightly than unary minus, so `-2i32.pow(2)` is −4.

**Where languages differ.** Languages agree on arithmetic but not on the other operators:

- *Bitwise and comparison.* In C, C++, Java, C# and JavaScript, `==` binds more tightly than `&`, so `x & 1 == 0` means `x & (1 == 0)`. In Go, `&` is at the level of `*`, and Go has only "five precedence levels for binary operators"[^3], so `x&1 == 0` means `(x & 1) == 0`. Python and Rust also put `&` above `==`[^1][^6].
- *Shift and addition.* In C, Java and Rust, `1 << 2 + 1` is `1 << 3`, which is 8[^4]. In Go, `<<` is at the level of `*`, so the same text is `(1 << 2) + 1`, which is 5[^3].
- *Increment.* In C, `*p++` increments the pointer. In Go, `++` is a statement, so "statement *p++ is the same as (*p)++"[^3].
- *Null coalescing.* C# puts `??` below `||`, and the C# reference says that parentheses "change the order of evaluation imposed by operator precedence and associativity"[^5]. JavaScript forbids `??` next to `||` or `&&` without parentheses.
- *Conditional and assignment.* In C++, "the conditional operator has the same precedence as assignment operators", so `flag ? a : b = 9` means `flag ? a : (b = 9)`; in C, the same text is an error[^7].

**Grammar, not evaluation.** The C and C++ standards have no precedence table: the levels "are derived from the grammar", and cppreference notes that "Precedence and associativity are compile-time concepts and are independent from order of evaluation"[^8]. In `f() + g() * h()`, the multiplication groups first, but the language decides separately which call runs first. See [order of evaluation](/order-of-evaluation/).

**Overloaded operators.** An overloaded operator keeps the precedence of the built-in one. If a C++ library uses `^` for power, `x ^ 3 * 2` still means `x ^ (3 * 2)`, because `^` binds less tightly than `*`. C++ output with `<<` keeps the precedence of the shift operator.

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | native | precedence table; `**` groups to the right; comparisons chain |
| JavaScript | native | precedence table; `**` groups to the right; `-2 ** 2` and `a ?? b \|\| c` are syntax errors |
| TypeScript | native | as JavaScript |
| Java | native | the precedence of the C family; `?:` and `=` group to the right |
| Kotlin | native | precedence table; named infix functions bind more tightly than comparisons |
| C# | native | the precedence of the C family; `??` and `?:` group to the right |
| Go | native | five levels of binary operators; `&` and `<<` at the level of `*` |
| Rust | native | precedence table; comparisons and ranges need parentheses |
| C | native | precedence derived from the grammar; `==` binds more tightly than `&` |
| C++ | native | as C; `?:` at the level of assignment; overloaded operators keep their level |
| Bash | native | C precedence in `$(( ))`; `&&` and `\|\|` of the same level between commands |
| PowerShell | native | precedence table; comparison operators such as `-eq` below arithmetic |
| SQL | native | `NOT` above `AND` above `OR`; arithmetic above comparison |
| Regex | native | repetition binds more tightly than concatenation, and concatenation more tightly than `\|` |
| HTML | n/a | no operators |
| CSS | native | `calc()` with `*` and `/` above `+` and `-` |

Every general-purpose language in the table defines precedence and associativity, so the verdict is native. The arithmetic levels are the same everywhere, and the differences are in the bitwise operators, shifts, unary minus with exponentiation, comparisons and the null and conditional operators. Regular expressions and CSS `calc()` also have precedence, so their cells are native too.

The example files show the cases where languages differ. Python shows `-2 ** 2` and a chained comparison. JavaScript shows the mixes that are syntax errors. Java shows that string concatenation groups to the left. C# shows `??` with `+`. Go shows its five levels, and Rust shows method calls, `as` and the `?` operator. C shows `x & 1 == 0` and `*p++`, with the warnings of the compiler, and C++ shows an overloaded `^` and the conditional operator with assignment.

## Variations and related ideas

- **No precedence.** APL evaluates from right to left, and Smalltalk evaluates binary messages from left to right, so `2 + 3 * 4` is 20 in Smalltalk[^11]. Lisp writes prefix lists such as `(+ 1 (* 2 3))`, and Forth and PostScript use postfix, so no rule is needed.
- **User-defined levels.** Haskell declares the level and the associativity of an operator with `infixl`, `infixr` and `infix`. Swift has precedence groups. Kotlin gives all named infix functions one level.
- **Parsing methods.** A grammar can have one rule per level, as in [recursive descent](/recursive-descent-parsing/). Precedence climbing, Pratt parsing and the shunting yard algorithm read a table of levels instead[^10]. See [context-free grammars](/context-free-grammars/).
- **Spreadsheets.** In Microsoft Excel, unary minus binds more tightly than `^`, so `-3^2` is 9[^11].

## Pitfalls

- **`x & mask == 0` in C-like languages.** This tests `x & (mask == 0)`. Write `(x & mask) == 0`. Compilers warn with `-Wparentheses`.
- **Shifts mixed with arithmetic.** `1 << n + 1` is `1 << (n + 1)` in C and `(1 << n) + 1` in Go. Use parentheses.
- **Unary minus and exponentiation.** `-2 ** 2` is −4 in Python, an error in JavaScript and 4 in Excel. Use parentheses.
- **String concatenation.** `"total: " + a + b` in Java or JavaScript joins the digits of `a` and `b` instead of adding them. Write `"total: " + (a + b)`.
- **Chained comparisons.** `a < b < c` is a chain in Python, a compile error in Rust, and `(a < b) < c` in C and JavaScript, which compares 0 or 1 with `c`.
- **Precedence is not order.** Parentheses change the grouping, not the order in which operands run. In C, `(i++) + (i++)` is still undefined behavior.
- **Overloaded operators.** A library that uses `^` for power or `<<` for output keeps the built-in levels, so mixes with `+` or `==` may group in a way that the reader does not expect.

## Cost

Precedence has no cost at run time: the parser builds the tree once, and the compiled code is the same as with explicit parentheses. A precedence-climbing or Pratt parser takes O(*n*) time for *n* tokens, with recursion as deep as the number of levels or the nesting of parentheses. A grammar with one rule per level does the same work with more function calls, which matters only in parsers that run very often.

## History

The FORTRAN I compiler of 1957 grouped expressions by inserting parentheses: it replaced `+` with `))+((` and `*` with `)*(` and added parentheses at the ends, so that an ordinary parser could read the result[^10]. Edsger W. Dijkstra's shunting yard algorithm converts infix expressions with a stack, and Vaughan Pratt described top-down operator precedence parsing in a 1973 paper[^10].

C inherited a level for `&` and `|` from B, where they also served as Boolean operators in conditions. When Dennis Ritchie split off `&&` and `||`, he kept the level of `&` relative to `==` to make the conversion of B code easy, and he later wrote that "Their tardy introduction explains an infelicity of C's precedence rules"[^9]. Java, C#, JavaScript and C++ copied the C table, and Go and Rust changed the levels of the bitwise operators[^3][^6].

## References
