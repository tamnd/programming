---
tag: BCKNT2
kind: concept
slug: assignment
name: Assignment
aliases: [assignment, assignment statement, assignment expression, assignment operator, "=", ":=", reassignment, chained assignment, multiple assignment, parallel assignment, destructive assignment, augmented assignment]
area: data-and-values/variables-and-assignment
level: intro
summary: >-
  Assignment gives a variable a new value: the program computes the expression on the right and stores the result in the place on the left. In some languages an assignment is also an expression.
applies_to: core-languages
viz: optional
formal: none
see_also: [variable-concept, constant, initialization-concept, binding, value, immutability, statements-and-expressions]
same_as:
  wikidata: Q231158
  wikipedia: Assignment_(computer_science)
sources:
- url: https://en.wikipedia.org/wiki/Assignment_(computer_science)
  title: Assignment (computer science), Wikipedia
  locator: Introduction; Semantics; Single assignment; Value of an assignment; Chained assignment; Parallel assignment; Assignment versus equality
- url: https://docs.python.org/3/reference/simple_stmts.html
  title: Simple statements, The Python Language Reference
  locator: 7.2 Assignment statements; 7.2.1 Augmented assignment statements
- url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-15.html
  title: Chapter 15, Expressions, The Java Language Specification, Java SE 25
  locator: 15.26 Assignment Operators
- url: https://docs.python.org/3/reference/expressions.html
  title: Expressions, The Python Language Reference
  locator: 6.12 Assignment expressions
- url: https://go.dev/ref/spec
  title: The Go Programming Language Specification
  locator: Assignment statements
- url: https://mitp-content-server.mit.edu/books/content/sectbyfn/books_pres_0/6515/sicp.zip/full-text/book/book-Z-H-20.html
  title: Structure and Interpretation of Computer Programs, 2nd edition, section 3.1, Assignment and Local State
  locator: 3.1.1; 3.1.3
- url: https://gcc.gnu.org/onlinedocs/gcc/Warning-Options.html
  title: Options to Request or Suppress Warnings, GCC manual
  locator: -Wparentheses
---

## Idea

A program that counts, sums or keeps a state must change the value of a variable. Assignment does this. "In computer programming, an assignment statement sets and/or re-sets the value stored in the storage location(s) denoted by a variable name; in other words, it copies a value into the variable."[^1]

In Python, the statement `total = total + price` reads the old value of `total`, adds the price, and stores the sum in `total`:

```python run id=idea
total = 0
for price in [3, 4, 5]:
    total = total + price
    print(total)
total += 10
print(total)
```

```output for=idea
3
7
12
22
```

`total = total + price` is not an equation, which would be false. It is an order: compute the right side, then put the value in the left side.

## How it works

**Two steps.** "The expression is evaluated in the current state of the program. The variable is assigned the computed value, replacing the prior value of that variable."[^1] The left side must name a place that can change: a variable, an element of an array or a field of an object. "For an assignment operation, it is necessary that the value of the expression is well-defined (it is a valid rvalue) and that the variable represents a modifiable entity (it is a valid modifiable (non-const) lvalue)."[^1]

**Copy or rebind.** In C, Go, Java and C#, assignment copies the value into the box of the variable. For a reference type, the value is a reference, so the object is not copied. In Python, "Assignment statements are used to (re)bind names to values and to modify attributes or items of mutable objects"[^2]: `x = y` makes the name `x` refer to the object of `y`.

**Statement or expression.** "In some languages, the symbol used is regarded as an operator (meaning that the assignment statement as a whole returns a value). Other languages define assignment as a statement (meaning that it cannot be used in an expression)."[^1] In C, C++, Java, C# and JavaScript, `x = 5` is an expression. In Java, "the result of the assignment expression is the value of the variable after the assignment has occurred"[^3]. In Python and Go, assignment is a statement, and `if x = 5:` is a syntax error. For an assignment inside an expression, Python has the separate operator `:=`: "An assignment expression (sometimes also called a “named expression” or “walrus”) assigns an expression to an identifier, while also returning the value of the expression."[^4]

**Chained assignment.** "A statement like w = x = y = z is called a chained assignment in which the value of z is assigned to multiple variables w, x, and y."[^1] In C, the chain works because each assignment is an expression with a value, and the operator groups from the right. In Python, `a = b = 0` is one statement with two targets, and the statement "assigns the single resulting object to each of the target lists, from left to right"[^2].

**Parallel assignment.** Python, Go, JavaScript and others assign several variables in one statement: `a, b = 0, 1`. All the values on the right are computed before any variable changes, so "a, b := b, a swaps the values of a and b"[^1]. In Go, "The assignment proceeds in two phases. First, the operands of index expressions and pointer indirections (including implicit pointer indirections in selectors) on the left and the expressions on the right are all evaluated in the usual order. Second, the assignments are carried out in left-to-right order."[^5]

**Augmented assignment.** `x += 1` is a short form of `x = x + 1`. In Python, "Augmented assignment is the combination, in a single statement, of a binary operation and an assignment statement"[^2].

**Assignment changes the meaning of a program.** Without assignment, a name stands for one value, and an expression gives the same result each time. With assignment, the order of the steps matters. SICP: "Our programming language can no longer be interpreted in terms of the substitution model of procedure application"[^6]. "Referential transparency is violated when we include set! in our computer language."[^6]

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | native | the statement `x = e`, also `a, b = b, a` and `a = b = 0`; the expression `(x := e)` |
| JavaScript | native | the expression `x = e`, with destructuring `[a, b] = [b, a]` |
| TypeScript | native | as JavaScript, with a type check of the value |
| Java | native | the expression `x = e`, whose value is the new value of `x` |
| Kotlin | native | the statement `x = e`; an assignment is not an expression |
| C# | native | the expression `x = e`, and tuple assignment `(a, b) = (b, a)` |
| Go | native | the statement `x = e`, also `a, b = b, a`; `:=` declares and assigns |
| Rust | native | the expression `x = e` of type `()`, for a `let mut` variable; `(a, b) = (b, a)` |
| C | native | the expression `x = e`, whose value is the new value of `x` |
| C++ | native | as C, and `operator=` for class types |
| Bash | native | `name=value` with no spaces around `=`; `(( x = x + 1 ))` for arithmetic |
| PowerShell | native | `$x = e`, and `$a, $b = $b, $a` |
| SQL | native | `UPDATE ... SET column = expression` changes stored rows; procedural extensions have `SET` for variables |
| Regex | n/a | a pattern has no variables to assign |
| HTML | n/a | HTML is markup, with no assignment |
| CSS | idiom | a declaration such as `color: red` sets a property; the cascade picks one value from many declarations |

**Go.** Both phases matter when the left side reads a variable that the same statement changes: in `i, x[i] = 1, 2`, Go computes the index `i` in the first phase, so the statement sets `x[0]`.

## Variations and related ideas

- **Initialization.** The first value of a variable, given in the declaration, is its [[initialization-concept]]. In C++, `int x = 5;` is an initialization and `x = 5;` is an assignment.
- **Single assignment.** "In functional programming, assignment is discouraged in favor of single assignment, more commonly known as initialization."[^1] A name gets one value and keeps it. See [[constant]].
- **Destructuring.** A tuple, an array or an object on the right can fill several names on the left: `a, b = point`. In the words of Wikipedia, "If the right-hand side of the assignment is a single variable (e.g. an array or structure), the feature is called unpacking"[^1] or destructuring assignment.
- **Notation.** The most common notation is `x = expr`, "originally Superplan 1949–51, popularized by Fortran 1957 and C"[^1]. The second is `x := expr`, "originally ALGOL 1958, popularised by Pascal"[^1].

## Pitfalls

- **`=` in place of `==`.** In C and C++, `if (x = 0)` assigns 0 to `x`, and the condition is always false. "Beginning programmers sometimes confuse assignment with the relational operator for equality, as "=" means equality in mathematics, and is used for assignment in many languages."[^1] GCC has the warning `-Wparentheses` for "an assignment in a context where a truth value is expected"[^7], and Python and Go reject the code. In JavaScript:

```javascript wrong id=assign-in-if
let level = 3;
if (level = 0) {
  console.log("zero");
} else {
  console.log("not zero");
}
console.log(level);
```

```output for=assign-in-if
not zero
0
```

The condition assigns 0, so the test is false, and `level` has lost its value.

- **The order of a parallel assignment.** Python and Go differ when a target uses a variable that the same statement changes:

```python wrong id=python-overlap
x = [0, 1]
i = 0
i, x[i] = 1, 2
print(x)
```

```output for=python-overlap
[0, 2]
```

Python assigns `i = 1` first, and then `x[i]` is `x[1]`. The Python reference warns that such overlaps occur "left-to-right, sometimes resulting in confusion"[^2]. Go computes the index before the assignments, so the same statement sets `x[0]`. Do not write such statements.
- **Aliases.** After `b = a` for a list or an object, `a` and `b` refer to one object. A change through `b` shows through `a`. Copy the object when a separate value is needed.
- **A chain with a mutable value.** In Python, `a = b = []` makes one list with two names, not two lists.

## Cost

An assignment of a simple value is one store to memory or a register, or nothing when the compiler keeps the value in a register. An assignment of a large struct in C, C++, Go or C# copies all its bytes. An assignment of a reference copies only the reference.

## History

"The use of equals for assignment dates back to Heinz Rutishauser's language Superplan, designed from 1949 to 1951, and was particularly popularized by Fortran"[^1]. Niklaus Wirth called it "A notorious example for a bad idea"[^1], because the sign `=` already meant a test for equality. ALGOL and Pascal used `:=`. "In C, the assignment operator is a single equals sign ("=") while the equality operator is a pair of equals signs ("==")."[^1]

## References
