---
tag: NEW-sum-types
kind: concept
slug: sum-types
name: Sum types
aliases: [tagged union, variant type, discriminated union, algebraic data type, variant record, sealed class hierarchy]
area: types/algebraic-data-types
level: core
summary: >-
  A value of a sum type is exactly one of a fixed set of cases, and each case carries its own data. A tag records the case, and the compiler can check that a match handles every case.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1980
  people: [R. M. Burstall, D. B. MacQueen, D. T. Sannella]
  note: HOPE declared a data type as a set of constructors, and its designers asked that a compiler check whether a case analysis over the constructors is exhaustive.
see_also: [interfaces, subtyping, null-references, parametric-polymorphism, exception-handling]
same_as:
  wikidata: Q7675229
  wikipedia: Tagged_union
sources:
  - paper: doi:10.1145/800087.802799
    title: R. M. Burstall, D. B. MacQueen and D. T. Sannella, HOPE, An experimental applicative language, LFP 1980
    locator: 2 Desirable programming language features, Exhaustive case analysis (pp. 136 and 137)
  - paper: doi:10.1145/800087.802799
    title: R. M. Burstall, D. B. MacQueen and D. T. Sannella, HOPE, An experimental applicative language, LFP 1980
    locator: 3.1 Data declarations (p. 137), the declarations of num and numtree, with ++ between constructors and # for the cartesian product
  - url: https://doc.rust-lang.org/book/ch06-01-defining-an-enum.html#enum-values
    title: The Rust Programming Language, 6.1 Defining an Enum
    locator: Enum Values (IpAddr with V4 and V6, and the Message enum, where each variant has different types and amounts of data), and The Option Enum and Its Advantages Over Null Values
  - url: https://doc.rust-lang.org/book/ch06-02-match.html#matches-are-exhaustive
    title: The Rust Programming Language, 6.2 The match Control Flow Construct
    locator: Matches Are Exhaustive
  - url: https://doc.rust-lang.org/error_codes/E0004.html
    title: Rust error codes index, E0004
    locator: the description of E0004 (the compiler cannot guarantee a matching pattern for one or more possible inputs)
  - url: https://doc.rust-lang.org/book/ch06-02-match.html#catch-all-patterns-and-the-_-placeholder
    title: The Rust Programming Language, 6.2 The match Control Flow Construct
    locator: Catch-All Patterns and the _ Placeholder
  - url: https://raw.githubusercontent.com/swiftlang/swift-book/main/TSPL.docc/LanguageGuide/Enumerations.md
    title: The Swift Programming Language, Enumerations
    locator: Matching Enumeration Values with a Switch Statement, and Associated Values
  - url: https://raw.githubusercontent.com/swiftlang/swift-book/main/TSPL.docc/LanguageGuide/Enumerations.md
    title: The Swift Programming Language, Enumerations
    locator: Recursive Enumerations (the indirect keyword)
  - url: https://www.typescriptlang.org/docs/handbook/2/narrowing.html#discriminated-unions
    title: TypeScript Handbook, Narrowing
    locator: Discriminated unions
  - url: https://www.typescriptlang.org/docs/handbook/2/narrowing.html#exhaustiveness-checking
    title: TypeScript Handbook, Narrowing
    locator: Exhaustiveness checking (the assignment to a never variable, and the error for a missing case)
  - url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-8.html#jls-8.1.1.2
    title: The Java Language Specification, Java SE 25 Edition
    locator: 8.1.1.2 sealed, non-sealed, and final Classes, and 8.1.6 Permitted Direct Subclasses
  - url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-14.html#jls-14.11.1.1
    title: The Java Language Specification, Java SE 25 Edition
    locator: 14.11.1.1 Exhaustive Switch Blocks, and 14.11.2 (an enhanced switch statement must be exhaustive)
  - url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-14.html#jls-14.11.3
    title: The Java Language Specification, Java SE 25 Edition
    locator: 14.11.3 Execution of a switch Statement (MatchException when no switch label applies)
  - url: https://openjdk.org/jeps/409
    title: "JEP 409: Sealed Classes"
    locator: Sealing and record classes (record classes express product types, sealed classes express sum types)
  - url: https://openjdk.org/jeps/409
    title: "JEP 409: Sealed Classes"
    locator: Sealed classes and pattern matching (an if-else chain of instanceof tests that the compiler cannot check)
  - url: https://openjdk.org/jeps/441
    title: "JEP 441: Pattern Matching for switch"
    locator: Exhaustiveness and sealed classes (the synthetic default clause for a new implementation that appears at run time)
  - url: https://openjdk.org/jeps/395
    title: "JEP 395: Records"
    locator: the header (Release 16) and the Summary
  - url: https://openjdk.org/projects/jdk/16/
    title: JDK 16
    locator: the schedule (General Availability on 16 March 2021)
  - url: https://openjdk.org/projects/jdk/17/
    title: JDK 17
    locator: the schedule (General Availability on 14 September 2021) and the feature list (409, Sealed Classes)
  - url: https://openjdk.org/projects/jdk/21/
    title: JDK 21
    locator: the schedule (General Availability on 19 September 2023) and the feature list (441, Pattern Matching for switch)
  - url: https://openjdk.org/jeps/441
    title: "JEP 441: Pattern Matching for switch"
    locator: the header (Release 21) and the History section (previews in JEPs 406, 420, 427 and 433)
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/operators/switch-expression#nonexhaustive-switch-expressions
    title: C# language reference, the switch expression
    locator: Non-exhaustive switch expressions (a compiler warning, and SwitchExpressionException at run time)
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/whats-new/csharp-15
    title: What's new in C# 15
    locator: the introduction (the .NET 11 SDK is still prerelease), Union types, and Closed hierarchies
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/builtin-types/union#union-exhaustiveness
    title: C# language reference, Union types
    locator: the lowering to a struct with an object Value property, the boxing of value-type cases, and Union exhaustiveness
  - url: https://kotlinlang.org/docs/sealed-classes.html#use-sealed-classes-with-when-expression
    title: Kotlin documentation, Sealed classes and interfaces
    locator: Use sealed classes with when expression
  - url: https://kotlinlang.org/docs/sealed-classes.html#inheritance
    title: Kotlin documentation, Sealed classes and interfaces
    locator: the introduction (all direct subclasses are known at compile time) and Inheritance
  - url: https://docs.python.org/3.14/library/typing.html#typing.assert_never
    title: Python documentation, typing
    locator: typing.assert_never
  - url: https://docs.python.org/3.14/library/dataclasses.html#dataclasses.dataclass
    title: Python documentation, dataclasses
    locator: the match_args parameter of dataclasses.dataclass
  - url: https://go.dev/doc/faq#variant_types
    title: Go FAQ
    locator: Why does Go not have variant types?
  - url: https://go.dev/ref/spec#Uniqueness_of_identifiers
    title: The Go Programming Language Specification
    locator: Uniqueness of identifiers
  - url: https://go.dev/ref/spec#Type_switches
    title: The Go Programming Language Specification
    locator: Type switches (the rewrite as a chain of type assertions)
  - url: https://go.dev/src/go/ast/ast.go
    title: Go source, go/ast/ast.go
    locator: the Expr interface with the method exprNode, and the comment above the exprNode methods
  - url: https://timsong-cpp.github.io/cppwp/n4950/variant.variant.general
    title: N4950, Working Draft, Programming Languages C++ (C++23)
    locator: "[variant.variant.general] paragraph 1"
  - url: https://timsong-cpp.github.io/cppwp/n4950/variant.visit
    title: N4950, Working Draft, Programming Languages C++ (C++23)
    locator: "[variant.visit] paragraphs 5, 7 and 8"
  - url: https://timsong-cpp.github.io/cppwp/n4950/variant.get
    title: N4950, Working Draft, Programming Languages C++ (C++23)
    locator: "[variant.get] paragraph 4"
  - url: https://port70.net/~nsz/c/c11/n1570.html#6.7.2.1p16
    title: N1570, ISO/IEC 9899:201x Committee Draft (C11)
    locator: 6.7.2.1 paragraphs 6 and 16 (the members of a union overlap, and its size fits the largest member)
  - url: https://port70.net/~nsz/c/c11/n1570.html#6.5.2.3p6
    title: N1570, ISO/IEC 9899:201x Committee Draft (C11)
    locator: 6.5.2.3 paragraph 6 (common initial sequence) and footnote 95 (type punning)
  - url: https://port70.net/~nsz/c/c11/n1570.html#6.8.4.2p5
    title: N1570, ISO/IEC 9899:201x Committee Draft (C11)
    locator: 6.8.4.2 paragraph 5 (no matching case and no default label)
  - url: https://gcc.gnu.org/onlinedocs/gcc/Warning-Options.html#index-Wswitch
    title: GCC manual, Options to Request or Suppress Warnings
    locator: -Wswitch and -Wswitch-enum
  - url: https://www.math.bas.bg/softeng/bantchev/place/algol68/a68rr.html
    title: A. van Wijngaarden et al. (eds.), Revised Report on the Algorithmic Language Algol 68
    locator: 2.1.3.6.a Acceptability of values (united modes) and 3.4.1.q (the conformity clause and its example CASE uir IN (INT i) ... ESAC)
  - url: https://archive.org/details/1973-the-programming-language-pascal-revised-report-wirth
    title: Niklaus Wirth, The Programming Language Pascal (Revised Report), ETH Zurich
    locator: 6.2.2 Record types (the variant part and the tag field)
  - paper: doi:10.1145/234286.1057812
    title: Niklaus Wirth, Recollections about the development of Pascal, History of Programming Languages II, 1996
    locator: p. 99 (the compiler was completed by mid 1970, and the language definition was published then)
  - paper: doi:10.1145/234286.1057812
    title: Niklaus Wirth, Recollections about the development of Pascal, History of Programming Languages II, 1996
    locator: p. 103 (the variant record as the single most unfortunate construct, and the run-time checks that most implementors left out)
---

## Idea

Some values come in a few distinct forms. A shape is a circle, a rectangle or a triangle. A parse result is a value or an error. A lookup finds an item or finds nothing. Each form needs its own data. A circle has a radius, and a rectangle has a width and a height.

A sum type puts these forms into one type. Its value is exactly one of a fixed list of cases, and each case carries its own fields. The value also records which case it is. This record is the tag, and a `match` on the tag selects the code for each case. In Rust, an `enum` is a sum type, and each variant can have different types and amounts of data.[^3]

```rust run id=idea
enum Shape {
    Circle { radius: f64 },
    Rect { width: f64, height: f64 },
}

fn area(shape: &Shape) -> f64 {
    match shape {
        Shape::Circle { radius } => 3.14159 * radius * radius,
        Shape::Rect { width, height } => width * height,
    }
}

fn main() {
    println!("{:.2}", area(&Shape::Circle { radius: 1.0 }));
    println!("{:.2}", area(&Shape::Rect { width: 2.0, height: 3.0 }));
}
```

```output for=idea
3.14
6.00
```

The list of cases is closed. So the compiler knows every case, and it can check that a `match` handles each one. In Rust, matches are exhaustive. A `match` that leaves out a case does not compile.[^4] When a programmer adds a case to the type, the compiler shows each `match` that must change. This check is the main reason to use a sum type in place of a set of unrelated types.

## How it works

**Product and sum.** A struct or a record holds all of its fields at the same time. Its set of values is the cartesian product of the sets of its fields, so it is a product type. A sum type holds one of its cases. Its set of values is the disjoint union of the sets of its cases. JEP 409 states the pair in these words: record classes express product types, and sealed classes express sum types.[^14] HOPE wrote the two operators as `#` for the product and `++` between the constructors of a data type.[^2]

**The tag.** Each value of a sum type must record its case. The value of a tag is one of the case names. Some languages store the tag as a small integer next to the data, as a C program does by hand. Others use the class of an object as the tag, as Java, C# and Kotlin do with a closed class hierarchy. TypeScript uses a field with a literal type, such as `kind: "circle"`, as the tag. The handbook calls this field the discriminant.[^9]

**Construction and matching.** Each case has a constructor, which builds a value of that case. A `match`, a `switch` or a `when` reads the tag and binds the fields of the case. The fields are visible only inside the branch of their case. So a program cannot read the radius of a rectangle.

**The exhaustiveness check.** The compiler compares the cases of a `match` with the list of cases of the type. If a case has no branch, the result depends on the language:

| Language | A missing case gives |
|---|---|
| Rust | error E0004[^5] |
| Java, `switch` over a sealed type | a compile error[^12] |
| Swift, `switch` over an enum | an error, because a `switch` must be exhaustive[^7] |
| TypeScript, with a `never` check | a type error from `tsc`[^10] |
| Kotlin, `when` over a sealed class | no `else` is needed when every subclass is covered[^25] |
| C#, `switch` expression over a class hierarchy | a warning, then `SwitchExpressionException` at run time[^22] |
| Python, `match` with `assert_never` | an error from a type checker only[^27] |
| Go, type switch | nothing; the switch does no check[^31] |
| C, `switch` on an `enum` tag | a warning with `-Wswitch`[^39] |

Before this check, a Java program tested the cases with a chain of `instanceof` tests. The chain ended with a `throw` that the compiler could not prove unreachable.[^15] The check needs a closed set of cases. An ordinary class or interface is open, because any code can add one more subclass. A `sealed` class or interface in Java names its permitted subclasses, and these must be in the same module or package.[^11] The direct subclasses of a sealed Kotlin class are known at compile time and must be in the same module and package.[^26]

**Recursive types.** A case can contain a value of the same sum type. An expression tree is the standard example: a number, or a sum of two expressions, or a product of two expressions. In Rust, a recursive variant holds a `Box`, so that the enum has a fixed size (see the Rust example). Swift marks such a case with the keyword `indirect`.[^8] HOPE declared the tree `numtree` as `empty ++ tip(num) ++ node(numtree # numtree)`.[^2]

**Memory layout.** A value of a sum type needs space for the tag and for its largest case. A C union has the size of its largest member, because the members share their storage.[^36] `std::variant` holds its value inside the variant object and may not allocate extra memory.[^33] Java, C# and Kotlin, with a class hierarchy, store each case as a separate object on the heap, and a reference to it.

## In languages

The example files show the same three shapes in each language. The table gives the verdict for each tier 1 language.

| Language | Verdict | Form |
|---|---|---|
| Python | idiom | a union of dataclasses with `match`; `assert_never` for a type checker[^28][^27] |
| JavaScript | idiom | an object with a `kind` field and a `switch`; no check |
| TypeScript | native | a discriminated union, narrowed by its `kind` field; a `never` check for exhaustiveness[^9][^10] |
| Java | native | a `sealed` interface with `record` cases and a `switch` with patterns[^11][^12] |
| C | idiom | a struct with an `enum` tag and a `union`; no check of the tag |
| C++ | stdlib | `std::variant` with `std::visit`[^33][^34] |
| C# | idiom | an abstract record with sealed records and a `switch` expression[^22] |
| Go | idiom | an interface with an unexported method and a type switch[^29][^30] |
| Rust | native | `enum` with `match`[^3][^4] |
| Kotlin | native | a `sealed` class or interface with `when`[^25][^26] |
| PowerShell | absent | a class hierarchy, or a hashtable with a kind key, and a `switch` |
| Bash | n/a | no types |
| SQL | absent | a table with a kind column and one nullable column for each field of each case |
| Regex | n/a | a pattern has no types |
| HTML/CSS | n/a | markup and style rules have no types |

**Swift.** An enumeration case can have associated values of any type, and the cases can differ in their types. A `switch` over an enumeration must consider every case.[^7] This makes Swift enums sum types with the same checks as Rust enums.

**Kotlin.** A `when` expression over a sealed class lets the compiler check that all possible cases are covered, and the `when` then needs no `else` branch.[^25] Kotlin is not installed on the machine that recorded the examples, so it has no example file.

**C#.** In the C# of the examples, a record hierarchy is open. Any assembly can derive another class from `Shape`. So the compiler cannot treat a `switch` over three cases as complete, and it gives a warning when the `switch` has no discard arm. If no arm matches at run time, the `switch` expression throws `SwitchExpressionException`.[^22] C# 15 adds two native forms, in a .NET 11 SDK that is still prerelease. A `union` declaration, such as `union Pet(Cat, Dog, Bird)`, lists its case types, and a `switch` expression over it is exhaustive when it covers every case type. A `closed` class can only be derived from inside its own assembly, so a `switch` that handles each direct descendant needs no default arm.[^23]

**Go.** Go has no variant types. The FAQ says that variant types overlap in confusing ways with interfaces. It suggests an interface value and a type switch in their place.[^29] To close the set of cases, the interface gets an unexported method. Unexported identifiers in different packages are always different, so no type outside the package can implement that method.[^30] The `go/ast` package closes its `Expr` interface this way, with the method `exprNode`.[^32]

**C.** A union holds the value of at most one of its members at a time.[^36] The tag is a separate field, and nothing in the language ties the tag to the member that holds a value. The Pitfalls show the two classic bugs.

## Variations and related ideas

- **Enumerations.** A C `enum` is a sum type whose cases carry no data. Rust and Swift extend `enum` so that each case can carry its own fields.[^3][^7]
- **Option and Result.** A type with the cases "a value" and "no value" replaces the null reference. Rust's `Option<T>` is an enum, and the compiler makes the program handle `None` before it uses the value.[^3] [[null-references]] compares the approaches.
- **Sealed hierarchies and subtyping.** Java, Kotlin and C# build sum types from classes. Each case is a subtype of the base type, so a case can be passed where the base type is expected. [[subtyping]] describes the rules.
- **Unions in C and ALGOL 68.** A C `union` and an ALGOL 68 `UNION` both hold one value of several types. The C union stores no tag, so the program must keep one. In ALGOL 68, a conformity clause can test which mode a union holds.[^40]
- **Open and closed sets.** An interface is open to more implementations, and each added type brings its own methods. A sum type is closed, and each added operation is one more function with a `match`. Adding a case to a sum type changes every `match`, and the exhaustiveness check finds them. [[interfaces]] describes the open form.
- **Generic sum types.** A sum type can take type parameters, as `Option<T>` and `Result<T, E>` do. [[parametric-polymorphism]] describes type parameters.
- **Pattern matching.** Patterns can nest, so one `case` can test the case of an outer value and the case of a field. Python's `match` statement is described in [[python/match-statement]].

## Pitfalls

### A match that misses a case

A programmer adds a case to `Shape`, but does not change `area`. In Rust, the `match` no longer covers every case, so the program does not compile.[^4]

```rust fails expect=compile id=rust-missing
enum Shape {
    Circle { radius: f64 },
    Rect { width: f64, height: f64 },
    Triangle { base: f64, height: f64 },
}

fn area(shape: &Shape) -> f64 {
    match shape {
        Shape::Circle { radius } => 3.14159 * radius * radius,
        Shape::Rect { width, height } => width * height,
    }
}

fn main() {
    println!("{}", area(&Shape::Triangle { base: 4.0, height: 5.0 }));
}
```

`rustc` stops with `` error[E0004]: non-exhaustive patterns: `&Shape::Triangle { .. }` not covered ``. The fix is an arm for `Shape::Triangle`. Java gives a compile error for the same mistake in a `switch` over a sealed interface.[^12]

```java fails expect=compile id=java-missing
public class Main {
    sealed interface Shape permits Circle, Rect, Triangle {}
    record Circle(double radius) implements Shape {}
    record Rect(double width, double height) implements Shape {}
    record Triangle(double base, double height) implements Shape {}

    static double area(Shape shape) {
        return switch (shape) {
            case Circle(double r) -> Math.PI * r * r;
            case Rect(double w, double h) -> w * h;
        };
    }

    public static void main(String[] args) {
        System.out.println(area(new Triangle(4, 5)));
    }
}
```

`javac` reports `the switch expression does not cover all possible input values`. Here the check is the fix. The error shows the place that needs one more case before the program runs.

### A catch-all arm hides an added case

A `_` arm matches every value that the arms above it did not match.[^6] That includes cases that are added later. Here `corners` was written when `Shape` had only circles and rectangles. A triangle then falls into `_`, and the compiler has nothing to report.

```rust wrong id=rust-wildcard
enum Shape {
    Circle(f64),
    Rect(f64, f64),
    Triangle(f64, f64),
}

fn corners(shape: &Shape) -> u32 {
    match shape {
        Shape::Rect(..) => 4,
        _ => 0,
    }
}

fn main() {
    let shapes = [Shape::Circle(1.0), Shape::Rect(2.0, 3.0), Shape::Triangle(4.0, 5.0)];
    for shape in &shapes {
        println!("{}", corners(shape));
    }
}
```

```output for=rust-wildcard
0
4
0
```

A triangle has 3 corners, not 0. The fix is one arm for each case: `Shape::Circle(_) => 0` and `Shape::Triangle(..) => 3`. Then the next added case gives error E0004 again. Use `_` only where every later case needs the same answer.

### A C# switch over an open hierarchy

The C# compiler cannot know all subclasses of an abstract record, because another assembly can add one. A `switch` expression that leaves out `Triangle` gives a warning, and the program still builds. A triangle then matches no arm, and the `switch` throws.[^22]

```csharp fails expect=exception:SwitchExpressionException id=csharp-missing
Shape[] shapes = { new Circle(1), new Rect(2, 3), new Triangle(4, 5) };
foreach (Shape shape in shapes)
{
    Console.WriteLine($"{Area(shape):F2}");
}

static double Area(Shape shape) => shape switch
{
    Circle(var r) => Math.PI * r * r,
    Rect(var w, var h) => w * h,
};

abstract record Shape;
sealed record Circle(double Radius) : Shape;
sealed record Rect(double Width, double Height) : Shape;
sealed record Triangle(double Base, double Height) : Shape;
```

The fix is an arm for `Triangle`. Treat the warning as an error, so that the next missing case stops the build. A discard arm `_` that throws makes the failure clear, but it also hides the warning for the next added case. The `closed` classes and `union` types of C# 15 make the compiler check the cases.[^23]

### A Go type switch with a missing case

A Go type switch is a chain of type assertions.[^31] Nothing requires a case for each type that implements the interface. If no case matches and there is no `default`, the switch does nothing.

```go wrong id=go-missing
package main

import "fmt"

type Shape interface{ isShape() }

type Rect struct{ Width, Height float64 }
type Triangle struct{ Base, Height float64 }

func (Rect) isShape()     {}
func (Triangle) isShape() {}

func area(s Shape) float64 {
	switch s := s.(type) {
	case Rect:
		return s.Width * s.Height
	}
	return 0
}

func main() {
	fmt.Println(area(Rect{2, 3}))
	fmt.Println(area(Triangle{4, 5}))
}
```

```output for=go-missing
6
0
```

The area of the triangle is 10. The fix is a `case Triangle`, and a `default` that panics, so that the next missing case fails at once and does not return a wrong number.

### A Python match without a fallback

A `match` statement with no matching case does nothing. The function then ends and returns `None`.

```python wrong id=python-missing
from dataclasses import dataclass


@dataclass
class Rect:
    width: float
    height: float


@dataclass
class Triangle:
    base: float
    height: float


def area(shape: Rect | Triangle) -> float:
    match shape:
        case Rect(w, h):
            return w * h


print(area(Rect(2, 3)))
print(area(Triangle(4, 5)))
```

```output for=python-missing
6
None
```

The fix is a `case Triangle(b, h)` and a final `case _: assert_never(shape)`. A type checker then reports an error when a case is missing, and at run time `assert_never` raises an exception.[^27] The Python example shows the complete form.

### The tag and the data disagree

In C, the tag and the union are two separate fields. Here `make_circle` sets the wrong tag. The union holds a circle, but the tag says square, so `area` reads the radius as the side of a square.

```c wrong id=c-tag
#include <stdio.h>

enum kind { CIRCLE, SQUARE };

struct circle { double radius; };
struct square { double side; };

struct shape {
    enum kind kind;
    union {
        struct circle circle;
        struct square square;
    } as;
};

static double area(const struct shape *s) {
    switch (s->kind) {
    case CIRCLE:
        return 3.141592653589793 * s->as.circle.radius * s->as.circle.radius;
    case SQUARE:
        return s->as.square.side * s->as.square.side;
    }
    return 0.0;
}

static struct shape make_circle(double radius) {
    struct shape s;
    s.kind = SQUARE;
    s.as.circle.radius = radius;
    return s;
}

int main(void) {
    struct shape c = make_circle(2.0);
    printf("%.2f\n", area(&c));
}
```

```output for=c-tag
4.00
```

The area of a circle of radius 2 is 12.57. This program has defined behaviour. Both structs start with a `double`, so they share a common initial sequence, and C allows a read of that part through either member.[^37] With members of other types, the read reinterprets the bytes as the other type, and the result can be a trap representation.[^37] The fix is to set the tag and the member in one place, such as one constructor function for each case.

### A C switch without the added case

A `switch` with no matching `case` and no `default` runs none of its body.[^38] Here `TRIANGLE` was added to the enum, but `area` has no case for it, so the function returns its initial value.

```c wrong id=c-switch
#include <stdio.h>

enum kind { RECT, TRIANGLE };

struct shape {
    enum kind kind;
    double a, b;
};

static double area(const struct shape *s) {
    double result = 0.0;
    switch (s->kind) {
    case RECT:
        result = s->a * s->b;
        break;
    default:
        break;
    }
    return result;
}

int main(void) {
    struct shape r = {RECT, 2, 3};
    struct shape t = {TRIANGLE, 4, 5};
    printf("%.2f\n", area(&r));
    printf("%.2f\n", area(&t));
}
```

```output for=c-switch
6.00
0.00
```

The `default` label makes the mistake worse, because it turns off the warning. GCC's `-Wswitch` warns when a `switch` on an enum misses an enumerator, but a `default` label prevents the warning. `-Wswitch-enum` warns even when there is a `default`.[^39] The fix is a `case TRIANGLE`, no `default` in a `switch` on the tag, and `-Wall -Werror` so that the warning stops the build.

### std::get with the wrong alternative

`std::get<T>` reads a `std::variant` as the alternative `T`. If the variant holds another alternative, it throws `std::bad_variant_access`.[^35]

```cpp fails expect=exception:bad_variant_access id=cpp-get
#include <cstdio>
#include <variant>

struct Circle { double radius; };
struct Rect { double width, height; };

int main() {
    std::variant<Circle, Rect> shape = Circle{1.0};
    std::printf("%.1f\n", std::get<Rect>(shape).width);
}
```

The exception is better than the silent wrong read of the C version. The fix is `std::visit`, which calls the right branch for each alternative. `std::get_if` is another fix, because it returns a null pointer for the wrong alternative (see the C++ example).

## Cost

- **Space.** A value needs space for its largest case and for the tag.[^36] A case with a large payload makes every value large. A `Box` or another pointer for the large case keeps the other values small. `std::variant` keeps its value inside the object and does not allocate.[^33]
- **Dispatch.** A `match` reads the tag and goes to the code of its case. For one variant, `std::visit` takes constant time, whatever the number of alternatives.[^34] A sealed class hierarchy tests the class of the object, and it stores each case as a separate object on the heap.
- **Boxing.** A C# 15 `union` stores its value in a property of type `object`. So a case of a value type is boxed.[^24]
- **The check is free at run time.** The exhaustiveness check runs in the compiler. Java still inserts a hidden `default` that throws, because another subclass can appear at run time through separate compilation.[^16][^13]

## History

- **1970 and 1973.** Niklaus Wirth published the definition of Pascal in mid 1970.[^42] The Revised Report defines a record type with several variants. A tag field, common to all variants, indicates which variant the record holds.[^41] Wirth later called the variant record the single most unfortunate construct of Pascal. Most implementations left out the run-time check of the tag, and programmers used variant records to break the type system.[^43]
- **1976.** The Revised Report on ALGOL 68 defines united modes, such as `UNION (INT, REAL)`. A conformity clause, such as `CASE uir IN (INT i): print (i), (REAL): print ("no") ESAC`, tests which mode a union holds.[^40]
- **1980.** R. M. Burstall, D. B. MacQueen and D. T. Sannella described HOPE. A `data` declaration listed the constructors of a type, and the designers asked that a compiler check whether a case analysis over the constructors is exhaustive. This check, they wrote, avoids mistakes such as a missing test for the empty list.[^1][^2]
- **2021.** JDK 16, released on 16 March 2021, added records (JEP 395).[^17][^18] JDK 17, released on 14 September 2021, added sealed classes (JEP 409).[^19] JEP 409 named the combination of records and sealed classes algebraic data types.[^14]
- **2023.** JDK 21, released on 19 September 2023, added pattern matching for `switch` (JEP 441), with the exhaustiveness check over sealed types. Four preview JEPs came before it.[^20][^21]
- **2026.** C# 15, in the .NET 11 SDK, adds `union` types and `closed` class hierarchies. The documentation states that the SDK is still prerelease.[^23]

## References
