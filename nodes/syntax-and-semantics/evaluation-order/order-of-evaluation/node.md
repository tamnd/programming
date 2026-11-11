---
tag: SWPCV3
kind: concept
slug: order-of-evaluation
name: Order of evaluation
aliases: [evaluation order, sequence points, sequenced before, unsequenced, operand order]
area: syntax-and-semantics/evaluation-order
level: core
summary: >-
  The order of evaluation decides when each operand, argument and side effect of an expression happens. Some languages fix it from left to right, and C and C++ leave most of it open.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1960
  people: [Peter Naur]
  note: ALGOL 60 said that the operations in an expression are generally done from left to right.
see_also: [undefined-behavior, lexical-scope, closures, integer-overflow, data-races]
same_as:
  wikidata: Q2041271
  wikipedia: Sequence_point
sources:
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Operators/Operator_precedence
  title: Operator precedence, MDN Web Docs
  locator: Precedence and order of evaluation
- url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/language-specification/expressions
  title: C# language specification, Expressions
  locator: 12.4.1 Operators, General
- url: https://docs.python.org/3.14/reference/expressions.html#evaluation-order
  title: The Python Language Reference, Expressions, Python 3.14
  locator: 6.16 Evaluation order
- url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-15.html#jls-15.7
  title: The Java Language Specification, Java SE 25 Edition
  locator: 15.7 Evaluation Order
- url: https://tc39.es/ecma262/
  title: ECMAScript Language Specification
  locator: 13.15.4 EvaluateStringOrNumericBinaryExpression
- url: https://doc.rust-lang.org/reference/expressions.html#evaluation-order-of-operands
  title: The Rust Reference, Expressions
  locator: Evaluation order of operands
- url: https://go.dev/ref/spec#Order_of_evaluation
  title: The Go Programming Language Specification
  locator: Order of evaluation
- url: https://go.dev/ref/spec#Assignment_statements
  title: The Go Programming Language Specification
  locator: Assignment statements
- url: https://www.open-std.org/jtc1/sc22/wg14/www/docs/n1570.pdf
  title: ISO/IEC 9899:201x Committee Draft N1570
  locator: 6.5 Expressions, paragraphs 2 and 3
- url: https://www.open-std.org/jtc1/sc22/wg14/www/docs/n1570.pdf
  title: ISO/IEC 9899:201x Committee Draft N1570
  locator: 6.5.2.2 Function calls, paragraphs 10 and 12
- url: https://gcc.gnu.org/onlinedocs/gcc/Warning-Options.html
  title: GCC Manual, Options to Request or Suppress Warnings
  locator: -Wsequence-point
- url: https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0145r3.pdf
  title: Refining Expression Evaluation Order for Idiomatic C++, P0145R3, G. Dos Reis, H. Sutter, J. Caves, 2016
  locator: 1 Introduction, 7 Implementation experience report
- url: https://en.cppreference.com/w/cpp/language/eval_order
  title: Order of evaluation, cppreference.com
  locator: Rules, Undefined behavior
- url: https://www.postgresql.org/docs/current/sql-expressions.html#SYNTAX-EXPRESS-EVAL
  title: PostgreSQL Documentation, Value Expressions
  locator: 4.2.14 Expression Evaluation Rules
- url: https://docs.python.org/3.14/reference/expressions.html#dictionary-displays
  title: The Python Language Reference, Expressions, Python 3.14
  locator: 6.2.7 Dictionary displays
- url: https://doc.rust-lang.org/reference/expressions/operator-expr.html#compound-assignment-expressions
  title: The Rust Reference, Operator expressions
  locator: Compound assignment expressions
- url: https://www.masswerk.at/algol60/report.htm
  title: Revised Report on the Algorithmic Language ALGOL 60, P. Naur (ed.), Communications of the ACM 6(1), 1963
  locator: 3.3.5 Precedence of operators
---

## Idea

An expression such as `f() + g() * h()` has three calls. Precedence says that the product is computed before the sum. It does not say which call runs first. The order of evaluation is the rule that decides when each operand, each argument and each side effect happens. When the operands have no side effects, the order cannot be seen and does not matter. When they print, write to a variable or read a variable that another operand changes, the order decides the result.

Languages take one of three positions. Some fix the order, usually from left to right. Some leave it unspecified, so that the compiler can choose. Some leave it unspecified and also make some programs undefined if they depend on it. Python is in the first group, and the three calls run from left to right even though `*` binds tighter than `+`:

```python run id=idea
def f(name, value):
    print("evaluate", name)
    return value


print(f("a", 1) + f("b", 2) * f("c", 3))
```

```output for=idea
evaluate a
evaluate b
evaluate c
7
```

The same expression in C can call the three functions in any of the six orders. A program that is correct on one compiler can print a different result on another compiler, or at another optimization level, with no change to the source.

## How it works

**Precedence is not order.** Precedence and associativity decide the shape of the expression tree: which operator gets which operands. The order of evaluation decides the order in which the program walks that tree. MDN states the difference for JavaScript: precedence decides the grouping of operators, "but not the order of evaluation of operands", and the operands are always evaluated from left to right.[^1] The C# specification gives the same rule with an example. In `F(i) + G(i++) * H(i)`, `F` gets the old value of `i`, `G` gets the old value, and `H` gets the new value, which "is separate from and unrelated to operator precedence".[^2] A tree has many valid walks. A language with a fixed order chooses one of them. A language without one lets the compiler choose any walk that computes each operand before the operator that uses it.

**A fixed order.** Python states: "Python evaluates expressions from left to right. Notice that while evaluating an assignment, the right-hand side is evaluated before the left-hand side."[^3] Java guarantees that the operands "appear to be evaluated in a specific evaluation order, namely, from left to right". The left operand of a binary operator is fully evaluated before any part of the right operand, and each argument of a call is fully evaluated before any argument to its right.[^4] In a compound assignment such as `a += e`, Java saves the value of `a` before it evaluates `e`. ECMAScript describes each binary operator as steps: evaluate the left operand, get its value, then evaluate the right operand.[^5] The Rust Reference states that expressions with many operands "are evaluated left to right as written in the source code".[^6]

A fixed order is a rule about what the program can observe. The word "appear" in the Java text is important. The compiler can still reorder, combine or remove operations if no program can see the difference. It cannot move a call before another call, because a call can have side effects.

**Partial orders.** Go fixes only part of the order. Function calls, method calls, receive operations and the logical operators are evaluated in lexical left-to-right order. The order of other events, such as reading a variable, compared to those calls is not specified. The specification gives the example `x := []int{a, f()}`, where `x` "may be [1, 2] or [2, 2]" if `f` changes `a`.[^7] An assignment in Go has two phases. First, the index expressions and pointer indirections on the left and all the expressions on the right are evaluated. Second, the assignments are done from left to right.[^8] So `a, b = b, a` swaps two variables, and `i, x[i] = 1, 2` sets `x[0]`, not `x[1]`.

**Sequencing in C.** C has no general order. The standard says: "Except as specified later, side effects and value computations of subexpressions are unsequenced."[^9] The specified cases are few. `&&`, `||`, `?:` and the comma operator evaluate their first operand completely before the rest. A function call has a sequence point after the evaluation of the function designator and the arguments, and before the call. The arguments themselves are evaluated in no fixed order, and the standard gives an example in which four functions "may be called in any order".[^10] GCC describes the result as a partial order: if two functions are called in one expression with no sequence point between them, the order of the calls is not specified.[^11]

C goes further than an unspecified order. "If a side effect on a scalar object is unsequenced relative to either a different side effect on the same scalar object or a value computation using the value of the same scalar object, the behavior is undefined."[^9] So `i = i++ + 1` and `a[i] = i++` are not programs with two possible results. They have no defined meaning, and the compiler can assume that they never run. This rule is one source of [undefined behavior](/undefined-behavior/).

**C++17.** C++ had the C rules until 2017. In 2016, Gabriel Dos Reis, Herb Sutter and Jonathan Caves proposed a fixed order for some operators. Their paper asks what `m[0] = m.size()` stores in a map, and notes that `f(i++, i)` and `v[i] = i++` were undefined.[^12] C++17 adopted most of the proposal. In `E1[E2]`, `E1.E2`, `E1 << E2` and `E1 >> E2`, the left operand is sequenced first. In `E1 = E2` and in compound assignment, the right operand is sequenced first. The function name is sequenced before the arguments, but the arguments are still indeterminately sequenced, so each one is evaluated completely but in an order that the compiler chooses. cppreference sums up the general rule: "There is no concept of left-to-right or right-to-left evaluation in C++."[^13]

**Why leave it open.** An open order lets a compiler pick the walk that uses the fewest registers, or evaluate the arguments of a call in the order that the calling convention pushes them. GCC on x86-64 often evaluates the arguments of a call from right to left, and Clang from left to right. A fixed order removes a class of bugs and makes a program mean the same thing on each compiler. Java and C# were designed with a fixed order. C++17 added one for the cases where idiomatic code, such as chained `<<` and chained method calls, broke most often.

**Declarative languages.** SQL describes a result, not steps. PostgreSQL states: "The order of evaluation of subexpressions is not defined. In particular, the inputs of an operator or function are not necessarily evaluated left-to-right or in any other fixed order." The planner can rearrange `AND` and `OR` terms in `WHERE` and `HAVING` by the laws of Boolean algebra, so `x <> 0 AND y / x > 1.5` does not protect the division. A `CASE` expression is the documented way to force an order.[^14]

## In languages

| Language | Verdict | Order |
|---|---|---|
| Python | native | left to right, but the right side of an assignment first |
| JavaScript | native | left to right |
| TypeScript | native | left to right, as JavaScript |
| Java | native | left to right, left operand saved first |
| Kotlin | native | left to right |
| C# | native | left to right |
| Go | native | calls and logical operators left to right, other reads unspecified |
| Rust | native | left to right, but the right side first in primitive compound assignment |
| C | absent | unspecified, undefined for unsequenced changes |
| C++ | absent | unspecified, with fixed rules for some operators in C++17 |
| Bash | native | commands and words left to right |
| PowerShell | native | left to right |
| SQL | absent | not defined, use `CASE` to force an order |
| Regex, HTML, CSS | n/a | no side effects to order |

**Python** evaluates from left to right, with the right-hand side of an assignment before the target. A dictionary display evaluates each key before its value. A dictionary comprehension does the same in Python 3.8 and later versions.[^15]

**JavaScript** and **TypeScript** evaluate operands, array elements and arguments from left to right. A compound assignment such as `total += await f()` reads `total` before the `await`, which loses updates when two async calls interleave.

**Java** and **C#** evaluate from left to right, and save the value of the left operand of a compound assignment before they evaluate the right operand. So `a += (a = 3)` with `a` equal to 9 gives 12 in both languages. **Kotlin** compiles to the same JVM model and also evaluates from left to right.

**Go** orders calls, receives and the logical operators from left to right, but not plain reads of variables compared to those calls. The gc compiler reads `a` after the call in `[]int{a, f()}`.

**Rust** evaluates from left to right with one exception. For a compound assignment where both operands are primitive types, "the right hand side is evaluated first, the left hand side is evaluated next". For other types, `+=` is a call to a trait method and the left side comes first. The Reference warns that code must not depend on the difference.[^16]

**C** leaves the order unspecified and makes unsequenced changes to one object undefined. GCC warns with `-Wsequence-point` and Clang with `-Wunsequenced`. Both warnings are in `-Wall`.

**C++** has the C rules plus the C++17 sequencing rules for subscripts, member access, shifts and assignment. The arguments of a call are still in an unspecified order.

**Bash** runs the commands of a list from left to right, and `&&` and `||` decide whether the next command runs. It expands the words of a command from left to right.

**SQL** has no defined order inside an expression. `CASE` and the short-circuit rules of a given database are the only tools to control it.

## Variations and related ideas

- **Short-circuit evaluation.** `&&`, `||` and the conditional operator evaluate the second operand only when it is needed. They are the only operators with a fixed order in C, and the order is part of their meaning.
- **Evaluation strategy.** Strict languages evaluate the arguments of a call before the call. Lazy languages, such as Haskell, evaluate an argument only when its value is needed, so the order of evaluation is decided by data dependencies and not by the text.
- **Sequence points and sequenced-before.** C89 described the order with sequence points. C11 and C++11 replaced them with the "sequenced before" relation, which can also describe the order between threads.
- **Reordering by the compiler and the hardware.** Even with a fixed order in the language, the compiler and the processor can reorder memory operations that another thread can see. The rules for that are part of the memory model of the language. See [data races](/data-races/).
- **Pure functions.** If no operand has a side effect, every order gives the same result. Code that uses pure functions does not depend on the order of evaluation. See [higher-order functions](/higher-order-functions/).

## Pitfalls

- **Precedence read as order.** `f() + g() * h()` does not call `g` and `h` before `f` in Python, Java or C#. Precedence only builds the tree.
- **A variable changed and read in one expression.** `i = i++`, `a[i] = i++` and `f(i, i++)` are undefined in C and were undefined in C++ before C++17. In Java, `count = count++` is defined but leaves `count` unchanged.
- **The order of arguments.** In C and C++, `printf("%d %d", next(), next())` can print the two values in either order. GCC and Clang choose differently. Assign each call to a variable first.
- **Reads that are not calls in Go.** `[]int{a, f()}` can read `a` before or after `f` changes it.
- **Guards in SQL.** `WHERE x <> 0 AND y / x > 1` can divide by zero, because the planner can evaluate the division first. Use `CASE`.
- **Compound assignment across an `await` or a yield point.** In JavaScript, `total += await f()` reads `total` before it waits. Another task can change `total` in the meantime, and its change is lost.
- **`a && b || c` in shell scripts.** If `b` fails, `c` runs too. It is not an if-then-else.
- **Default arguments.** Python evaluates a default value once, when the function is defined, and not at each call.

## Cost

A fixed order has a small cost in some cases. The compiler cannot evaluate the arguments in the order that suits the calling convention or the register allocator. In practice, most operands have no side effects that the compiler cannot see, and for those the compiler is free to reorder. The P0145 authors changed the Visual C++ compiler to force left-to-right evaluation of all call arguments, with no new optimizations. On the SPEC benchmarks, some entries ran slower and some ran faster, and the variation was between -4% and +4%. One test out of 26 in a large application code base failed, because the code depended on the old order.[^12]

An unspecified order has a cost for the programmer. Code that depends on the order works on one compiler and fails on another, and the failure appears only after a change of compiler, flags or platform.

## History

ALGOL 60 stated a general rule: "The sequence of operations within one expression is generally from left to right", with extra rules for precedence and parentheses.[^17] The text was about the order of operators, and it did not settle when the side effects of function designators happen. Later languages took different positions. C left the order unspecified so that compilers on many machines could generate efficient code, and the C standards of 1989 and later added sequence points and the rule that makes unsequenced changes undefined. Java, in 1996, and C# then chose a fixed left-to-right order. C++ adopted P0145 in C++17, after a paper of 2014 and its revision of 2016, and fixed the order for the operators where idiomatic code broke most often.[^12] Python had a left-to-right rule for expressions, and fixed the order of keys and values in dictionary comprehensions in Python 3.8.[^15]

## References
