---
tag: G225DW
kind: concept
slug: operator
name: Operator
aliases: [operator, operand, unary operator, binary operator, ternary operator, conditional operator, infix, prefix, postfix, arity, fixity, operator overloading, user-defined operator, compound assignment]
area: syntax-and-semantics/expressions-and-statements
level: core
summary: >-
  An operator is a symbol or a keyword, such as + or sizeof, that makes an expression from its operands. It is usually a short name for a function, with its own syntax and rules for grouping.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1956
  people: [John Backus]
  note: The 1956 FORTRAN manual let a program write arithmetic with the symbols +, -, *, / and **, and gave a hierarchy of operations so that the compiler could find the grouping of an expression without full parentheses.
see_also: [operator-precedence, short-circuit-evaluation, statements-and-expressions, integer-representation, higher-order-functions]
same_as:
  wikidata: Q1206110
  wikipedia: Operator_(computer_programming)
sources:
- url: https://en.wikipedia.org/wiki/Operator_(computer_programming)
  title: Operator (computer programming), Wikipedia
  locator: Introduction; Overloading; Customization
- url: https://docs.python.org/3/library/operator.html
  title: operator, Standard operators as functions, Python documentation
  locator: Introduction
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Operators
  title: Expressions and operators, MDN Web Docs
  locator: Unary operators; Binary logical operators
- url: https://docs.python.org/3/reference/expressions.html
  title: Expressions, The Python Language Reference
  locator: 6.5 The power operator; 6.10 Comparisons
- url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-15.html
  title: The Java Language Specification, Java SE 25 Edition, Chapter 15
  locator: 15.17.2 Division Operator /; 15.26.2 Compound Assignment Operators
- url: https://en.cppreference.com/w/c/language/sizeof.html
  title: sizeof operator, C reference, cppreference.com
  locator: Explanation
- url: https://go.dev/ref/spec
  title: The Go Programming Language Specification
  locator: Operators; Arithmetic operators; IncDec statements
- url: https://docs.python.org/3/reference/datamodel.html
  title: Data model, The Python Language Reference
  locator: 3.3.8 Emulating numeric types
- url: https://en.cppreference.com/w/cpp/language/operators.html
  title: Operator overloading, C++ reference, cppreference.com
  locator: Restrictions
- url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/operators/operator-overloading
  title: Operator overloading, C# language reference
  locator: Introduction
- url: https://doc.rust-lang.org/std/ops/index.html
  title: Module std::ops, The Rust Standard Library
  locator: Module description
- url: https://archive.computerhistory.org/resources/text/Fortran/102649787.05.01.acc.pdf
  title: The FORTRAN Automatic Coding System for the IBM 704 EDPM, Programmer's Reference Manual, IBM, 1956
  locator: Chapter 3, Expressions
---

## Idea

Mathematics writes `3 + 4` and not `add(3, 4)`, and programming languages keep this short form. An operator is a symbol such as `+`, `<` or `&&`, or a keyword such as `sizeof` or `typeof`, that makes an expression from one or more operands. Most operators are short names for functions, and some languages let a type give its own meaning to them. Others do things that a function cannot do, such as not evaluating an operand.

```python run id=idea
import operator


class Money:
    def __init__(self, cents):
        self.cents = cents

    def __add__(self, other):  # a + b calls a.__add__(b)
        return Money(self.cents + other.cents)

    def __repr__(self):
        return f"Money({self.cents})"


# An operator is a short name for a function.
print(3 + 4, operator.add(3, 4))
print(Money(150) + Money(275))

# Arity: unary minus takes one operand, binary minus takes two.
print(-5, 10 - 5)

# Precedence and associativity decide the grouping.
print(-2 ** 2, (-2) ** 2)    # ** binds tighter than unary minus
print(2 ** 3 ** 2)           # ** groups from the right: 2 ** 9

# Some operators do not evaluate all their operands.
print(0 and 1 / 0)           # and stops at the first false operand
```

```output for=idea
7 7
Money(425)
-5 5
-4 4
512
0
```

`3 + 4` and `operator.add(3, 4)` give the same value, because the operator is a short name for the function. `Money(150) + Money(275)` works because the class defines `__add__`. The minus sign is two operators: unary minus has one operand and binary minus has two. `-2 ** 2` is `-4`, because `**` binds tighter than the unary minus on its left, and `2 ** 3 ** 2` is `2 ** 9`, because `**` groups from the right. `0 and 1 / 0` gives `0` without an error, because `and` does not evaluate its right operand when the left one is false.

## How it works

**Operator and function.** "An operator is a programming language construct that provides functionality that may not be possible to define as a user-defined function (e.g. sizeof in C) or has syntax different from that of a function (e.g. infix addition as in a+b)"[^1]. In many languages, most operators map to functions. Python says that "The operator module exports a set of efficient functions corresponding to the intrinsic operators of Python. For example, operator.add(x, y) is equivalent to the expression x+y"[^2].

**Arity and fixity.** The arity of an operator is the number of its operands. "A unary operation is an operation with only one operand"[^3], such as `-x`, `!x` or `typeof x`. A binary operator has two operands, such as `a + b`. The conditional operator `c ? a : b` is ternary, and it is the only ternary operator in C, Java and JavaScript. The fixity is the position of the operator: prefix (`-x`), infix (`a + b`) or postfix (`x++`).

**Precedence and associativity.** An expression with many operators needs rules for its grouping. Precedence says which operator binds tighter, so `a + b * c` is `a + (b * c)`. Associativity says how operators of the same level group, so `a - b - c` is `(a - b) - c`. In Python, "The power operator binds more tightly than unary operators on its left; it binds less tightly than unary operators on its right"[^4]. See [operator precedence](/operator-precedence/).

**The type of the operands.** The same symbol can do different things for different types. Wikipedia gives an example: "in Java the + operator sums numbers or concatenates strings"[^1]. The rules for numbers also differ. In Java, "Integer division rounds toward 0"[^5], and C, C# and Go do the same. Python `//` rounds toward minus infinity, so `-7 // 2` is `-4`. Before an operation, C converts the operands to a common type, so `-1 < 1u` compares two unsigned values and is false. See [integer representation](/integer-representation/).

**Operators that are not functions.** Some operators do things that a call cannot do:

- `&&`, `||`, `and`, `or` and `??` do not evaluate the right operand when the left one decides the result. See [short-circuit evaluation](/short-circuit-evaluation/).
- In C, the operand of `sizeof` "is not evaluated" unless it is a variable-length array, so `sizeof n++` does not change `n`[^6].
- An assignment operator changes its left operand. In Java, "A compound assignment expression of the form E1 op= E2 is equivalent to E1 = (T) ((E1) op (E2)), where T is the type of E1, except that E1 is evaluated only once"[^5], so `b += 10` on a `byte` casts the result back to `byte`.
- In Go, `++` and `--` are statements and not expressions[^7], so `y := x++` does not compile.

**Overloading.** "Some languages allow a language-defined operator to be overridden with user-defined behavior and some allow for user-defined operator symbols"[^1]. Each language has its own mechanism:

- Python calls a special method. "To evaluate the expression x + y, where x is an instance of a class that has an __add__() method, type(x).__add__(x, y) is called", and the reflected methods such as `__radd__` handle the case where the right operand defines the operation[^8].
- C++ defines functions named `operator+` and similar. Some operators cannot be overloaded: "The operators :: (scope resolution), . (member access), .* (member access through pointer to member), and ?: (ternary conditional) cannot be overloaded. New operators such as **, <>, or &| cannot be created"[^9].
- C# uses the `operator` keyword: "You can overload a predefined C# operator in a user-defined type"[^10].
- Rust uses traits. "Only operators backed by traits can be overloaded. For example, the addition operator (+) can be overloaded through the Add trait"[^11].
- Java and Go have no user-defined overloading. Go gives a defined type the operators of its underlying type, so `Money(150) + Money(275)` works for `type Money int64`[^7].

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | native | special methods such as `__add__`; chained comparisons; `operator` module |
| JavaScript | native | fixed set; `+` converts its operands; `??`, `?.`, `typeof` |
| TypeScript | native | as JavaScript; type operators such as `keyof` and `typeof` in types |
| Java | native | fixed set; `+` on strings; `>>>`; `instanceof` with a pattern |
| Kotlin | native | `operator fun plus` and similar; conventions map operators to functions |
| C# | native | `operator` declarations; conversion operators; `??=`, `?.` |
| Go | native | fixed set; no overloading; `&^`, `<-`; `++` is a statement |
| Rust | native | traits in `std::ops`; `?`; `..` and `..=` |
| C | native | fixed set; `sizeof`; the comma operator; the usual arithmetic conversions |
| C++ | native | `operator` functions; `<=>`; function objects; user-defined literals |
| Bash | native | arithmetic in `$(( ))`; test operators such as `-eq` and `-f` |
| PowerShell | native | `-eq`, `-like`, `-match` and other named operators; `+` on arrays and strings |
| SQL | native | arithmetic, comparison, `LIKE`, `IN`, `BETWEEN`, `IS NULL`, `\|\|` |
| Regex | idiom | quantifiers `*`, `+`, `?` and alternation `\|` act as operators on patterns |
| HTML | n/a | no expressions |
| CSS | idiom | `+`, `-`, `*` and `/` inside `calc()`; combinators such as `>` and `+` in selectors |

Every general-purpose language has operators, so the verdict is native. The languages differ in the set of operators, in the rules for the types of the operands, and in whether a program can give operators new meanings.

The example files show these differences. Python defines `+`, `*`, the reflected `__rmul__` and `@` for a class, uses the `operator` module and shows chained comparisons, floor division and `:=`. JavaScript shows how `+` converts its operands, `??` and `||`, `?.`, `typeof` and the syntax error for `-2 ** 2`. Java shows `+` on strings, integer division, `>>` and `>>>`, the cast in a compound assignment and `==` on objects. C# overloads `+`, unary `-`, `<` and a conversion to `decimal`. Go shows a defined type with the operators of `int64`, `&^`, `<-` and `++` as a statement. Rust implements `Add` and `Neg` and uses `?` and ranges. C shows `sizeof`, the comma operator, the bitwise operators and a comparison of `-1` with an unsigned value, and the compiler warns about `sizeof n++` on purpose. C++ uses a defaulted `<=>`, an `operator+`, an `operator<<` for output, a function object and a user-defined literal.

## Variations and related ideas

- **User-defined operator symbols.** Haskell, Scala, Swift and OCaml let a program define new operators from symbol characters, and Haskell and Swift let the program give them a precedence and an associativity.
- **Operators as values.** In Haskell, `(+)` is the function behind `+`, and `(+ 1)` is a section that adds 1. Python gives the same functions in the `operator` module[^2]. See [higher-order functions](/higher-order-functions/).
- **Named operators.** Some operators are words: `and`, `or`, `not`, `is` and `in` in Python, `instanceof` in Java, `sizeof` in C, and `-eq` and `-like` in PowerShell.
- **Chained comparisons.** In Python, "x < y <= z is equivalent to x < y and y <= z, except that y is evaluated only once"[^4]. In C and JavaScript, `1 < x < 5` compares a Boolean with 5.
- **Operators on collections.** APL, NumPy and MATLAB apply an operator to each element of an array, so `a + b` adds two arrays element by element.

## Pitfalls

- **Precedence that surprises.** `a & mask == 0` in C means `a & (mask == 0)`, and `-2 ** 2` in Python is `-4`. Use parentheses when the grouping is not obvious.
- **`=` and `==`.** `if (x = 0)` in C assigns and gives a false result. Java and C# reject it for an `int`. Many compilers warn about it.
- **`==` on objects.** In Java, `==` compares references, so two equal strings made at run time can be not equal. Use `equals`.
- **Conversions of the operands.** `"5" + 2` is `"52"` in JavaScript, and `-1 < 1u` is false in C. Convert the operands explicitly.
- **Division and remainder of negative numbers.** `-7 / 2` is `-3` and `-7 % 2` is `-1` in C, Java and Go, but `-7 // 2` is `-4` and `-7 % 2` is `1` in Python.
- **Overloading that breaks the rules.** An overloaded `+` that is not associative, or an `==` that does not agree with a hash, surprises the reader and breaks collections. An overloaded `&&` in C++ evaluates both operands.

## Cost

An operator on a built-in type usually compiles to one machine instruction, or to a few for division and for checks of overflow. An overloaded operator costs a function call, which the compiler can often inline in C++, C# and Rust. In Python, each operator is a call of a special method, and the interpreter also tries the reflected method when the first method returns `NotImplemented`.

## History

The 1956 FORTRAN manual let a program write arithmetic as a formula. "The characters +, -, *, and / denote addition, subtraction, multiplication, and division", and "The symbol ** denotes exponentiation"[^12]. It gave a "Hierarchy of Operations": exponentiation first, then multiplication and division, then addition and subtraction, and it grouped consecutive operations of one level from the left[^12]. It did not allow `A**B**C`[^12], and later languages chose a grouping for it.

Later languages added operators for logic, bits, strings and pointers, and C gave a fixed set with many levels of precedence. C++ let a class define the meaning of most operators[^9], and C#, Python, Rust and Kotlin kept overloading with different mechanisms[^10][^8][^11]. Java and Go do not allow it.

## References
