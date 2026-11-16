---
tag: 3HJCC5
kind: task
slug: use-an-enum-with-data
name: Use an enum with data
aliases: [enum with associated values, tagged union, discriminated union, sealed class hierarchy, variant type, sum type example]
area: types/algebraic-data-types
level: core
summary: Define a shape type whose variants carry different data, build a value from a kind and a list of sides, and match on the variant to get the perimeter, in each tier 1 language.
contract: contract.yaml
category: collections
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
  - url: https://doc.rust-lang.org/book/ch06-01-defining-an-enum.html#enum-values
    title: The Rust Programming Language, 6.1 Defining an Enum
    locator: Enum Values
    accessed: 2026-11-16
  - url: https://docs.swift.org/swift-book/documentation/the-swift-programming-language/enumerations/
    title: The Swift Programming Language, Enumerations
    locator: Associated Values
    accessed: 2026-11-16
  - url: https://kotlinlang.org/docs/sealed-classes.html
    title: Kotlin documentation, Sealed classes and interfaces
    locator: the introduction
    accessed: 2026-11-16
  - url: https://doc.rust-lang.org/book/ch06-02-match.html#matches-are-exhaustive
    title: The Rust Programming Language, 6.2 The match Control Flow Construct
    locator: Matches Are Exhaustive; Catch-All Patterns and the _ Placeholder
    accessed: 2026-11-16
  - url: https://openjdk.org/jeps/409
    title: JEP 409, Sealed Classes
    locator: Summary; Goals
    accessed: 2026-11-16
  - url: https://docs.oracle.com/javase/specs/jls/se27/html/jls-14.html#jls-14.11.1.1
    title: The Java Language Specification, Java SE 27 Edition
    locator: 14.11.1.1 Exhaustive Switch Blocks
    accessed: 2026-11-16
  - url: https://www.typescriptlang.org/docs/handbook/2/narrowing.html#discriminated-unions
    title: TypeScript Handbook, Narrowing
    locator: Discriminated unions; Exhaustiveness checking
    accessed: 2026-11-16
  - url: https://docs.python.org/3/tutorial/controlflow.html#match-statements
    title: The Python Tutorial, More Control Flow Tools
    locator: 4.7 match Statements
    accessed: 2026-11-16
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/operators/switch-expression
    title: C# reference, switch expression
    locator: Nonexhaustive switch expressions
    accessed: 2026-11-16
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/whats-new/csharp-15
    title: What's new in C# 15
    locator: Union types
    accessed: 2026-11-16
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/builtin-types/union
    title: C# reference, Union types
    locator: the introduction
    accessed: 2026-11-16
  - url: https://go.dev/doc/faq#variant_types
    title: Go FAQ
    locator: Why does Go not have variant types?
    accessed: 2026-11-16
  - url: https://port70.net/~nsz/c/c11/n1570.html#6.5.2.3
    title: ISO/IEC 9899:201x, committee draft N1570 (C11)
    locator: 6.2.6.1 paragraph 7; 6.5.2.3 footnote 95
    accessed: 2026-11-16
  - url: https://en.cppreference.com/w/cpp/utility/variant.html
    title: cppreference.com, std::variant
    locator: the description
    accessed: 2026-11-16
  - url: https://en.cppreference.com/w/cpp/utility/variant/visit2.html
    title: cppreference.com, std::visit
    locator: the description
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec
    title: The Go Programming Language Specification
    locator: Method sets; Type switches
    accessed: 2026-11-16
  - url: https://en.cppreference.com/w/cpp/utility/variant/get.html
    title: cppreference.com, std::get (std::variant)
    locator: the description
    accessed: 2026-11-16
  - url: https://tiswww.case.edu/php/chet/bash/bashref.html
    title: Bash Reference Manual
    locator: 6.5 Shell Arithmetic
    accessed: 2026-11-16
---

## Task

Define a type `Shape` with three variants, each with its own data: a square has one side, a rectangle has a width and a height, and a triangle has three sides. The function `perimeter(kind, sides)` builds a shape from a kind name and a list of side lengths, then matches on the variant to compute the perimeter. For example, `perimeter("rectangle", [2, 5])` is 14. If the data is not a valid shape, the result is no value (`None`, `null`, an empty optional or a false flag).

The data is valid when the kind is `square` with 1 side, `rectangle` with 2 sides or `triangle` with 3 sides, every side is greater than 0, and for a triangle each side is less than the sum of the other two. The kind is case-sensitive.

An [[enumeration]] has a fixed set of plain values (see [[define-an-enumeration]]). An enum with data is a [[sum-types|sum type]]: each variant is a small record. Its type makes illegal combinations, such as a square with two sides, impossible to build (see [[make-illegal-states-unrepresentable]]).

## Solutions

| Language | Shape type | Match |
|---|---|---|
| Python | three frozen dataclasses and `type Shape = Square \| Rectangle \| Triangle` | `match` with class patterns |
| JavaScript | frozen objects with a `kind` field | `switch` on `kind`, with a `default` that throws |
| TypeScript | a discriminated union on `kind` | `switch` on `kind`, with a `never` check |
| Java | `sealed interface Shape` and three records | `switch` with record patterns and no `default` |
| C# | an abstract record with a private constructor and three nested sealed records | a `switch` expression with positional patterns |
| Go | an interface with an unexported method, and three structs | a type switch |
| Rust | `enum Shape` with struct variants | `match` |
| C | a tagged union: an `enum` tag and a `union` of structs | `switch` on the tag |
| C++ | `std::variant<Square, Rectangle, Triangle>` | `std::visit` with one lambda for each alternative |
| Bash | the string `kind side...` | `case` on the kind |

- **Variants with different data.** In Rust, an enum variant can carry data, and "Each variant can have different types and amounts of associated data."[^1] Swift has the same feature: "You can define Swift enumerations to store associated values of any given type, and the value types can be different for each case of the enumeration if needed."[^2] Kotlin uses sealed classes: "All direct subclasses of a sealed class are known at compile time."[^3]
- **The compiler checks the match.** "Matches in Rust are exhaustive: We must exhaust every last possibility in order for the code to be valid."[^4] The Rust solution has no catch-all arm, so a new variant makes every `match` on `Shape` fail to compile until it handles the variant. Kotlin says: "When you combine sealed classes and interfaces with the when expression, you can cover the behavior of all possible subclasses".[^3]
- **Java seals the interface.** "Sealed classes and interfaces restrict which other classes or interfaces may extend or implement them."[^5] A `switch` whose case patterns cover every permitted subtype of a sealed interface is exhaustive, so it needs no `default`.[^6]
- **TypeScript narrows on a tag.** "When every type in a union contains a common property with literal types, TypeScript considers that to be a discriminated union, and can narrow out the members of the union."[^7] The `default` branch assigns the shape to a variable of type `never`. This is valid only when every member is handled, so "Adding a new member to the Shape union, will cause a TypeScript error".[^7] JavaScript has no such check, so its `default` branch throws at run time.
- **Python matches classes.** Class patterns can "use positional parameters with some builtin classes that provide an ordering for their attributes (e.g. dataclasses)".[^8] Python does not check that the cases are complete: "If no case matches, none of the branches is executed."[^8] The solution uses a type union so that a type checker can check the cases, but the interpreter does not.
- **C# uses records today.** In most cases, the compiler of C# "generates a warning if a switch expression doesn't handle all possible input values."[^9] The compiler does not know that the nested records are the only subtypes, so the solution has a discard arm that throws. "C# 15 introduces union types, which represent a value that can be one of several case types."[^10] For a union, "The compiler ensures that switch expressions cover all case types."[^11] The solution uses records so that it also builds with older compilers.
- **Go has no variant types.** The Go FAQ says: "We considered adding variant types to Go, but after discussion decided to leave them out because they overlap in confusing ways with interfaces." Instead, "The error example is easy to express using an interface value to hold the error and a type switch to discriminate cases."[^12] The unexported method `isShape` keeps other packages from adding a variant. The compiler does not check that the type switch has every case.
- **C pairs a tag with a union.** The `kind` field tells which member of the union holds the data. The compiler does not check this: the program must read only the member that the tag names.[^13]
- **C++ uses `std::variant`.** "The class template std::variant represents a type-safe union."[^14] `std::visit` "Applies the visitor v (a Callable that can be called with any combination of types from Variants) to the Variants values".[^15] The visitor must accept every alternative, so a missing lambda is a compile error.
- **Bash uses a string.** Bash has no types other than strings and arrays. The solution keeps a shape as words in a string, and `case` selects the code for each kind.

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`.

- `unknown-kind`: `circle` is not one of the three kinds.
- `wrong-case`: `Square` is not `square`.
- `too-few-sides`, `too-many-sides` and `no-sides`: the number of sides must match the kind.
- `zero-side` and `negative-side`: every side must be greater than 0.
- `not-a-triangle`: the sides 1, 2 and 3 do not make a triangle, because 3 is not less than 1 + 2.
- `largest`: three sides of 1000000 give 3000000, which fits in a 32-bit integer.

## Pitfalls

- **Python a missing case returns `None`.** "If no case matches, none of the branches is executed."[^8] A function that does not handle a new variant ends without a `return` and gives `None`. Add a last `case _:` that raises an error.

```python run id=python-missing-case
from dataclasses import dataclass


@dataclass(frozen=True)
class Square:
    side: int


@dataclass(frozen=True)
class Triangle:
    a: int
    b: int
    c: int


def perimeter(shape):
    match shape:
        case Square(side):
            return 4 * side
    # no case for Triangle: the function ends here


print(perimeter(Square(3)))
print(perimeter(Triangle(3, 4, 5)))
```

```output for=python-missing-case
12
None
```

- **JavaScript a wrong tag matches no case.** A misspelled `kind`, here `Rectangle` in place of `rectangle`, matches no `case`. The function returns `undefined`, and a sum that uses it becomes `NaN`. Add a `default` that throws, as the solution does.

```javascript run id=javascript-misspelled-kind
function perimeter(shape) {
  switch (shape.kind) {
    case "square":
      return 4 * shape.side;
    case "rectangle":
      return 2 * (shape.width + shape.height);
  }
}

const shapes = [
  { kind: "square", side: 3 },
  { kind: "Rectangle", width: 2, height: 5 },
];
const total = shapes.reduce((sum, s) => sum + perimeter(s), 0);
console.log(perimeter(shapes[1]), total);
```

```output for=javascript-misspelled-kind
undefined NaN
```

- **Go a pointer is another variant.** The method set of a pointer type `*T` includes the methods of `T`, so `*Square` also implements `Shape`.[^16] A type switch matches "actual types T against the dynamic type of the expression"[^16], and `*Square` is not `Square`. Use one form for each variant, and make the `default` case report the type.

```go run id=go-pointer-variant
package main

import "fmt"

type Shape interface{ isShape() }

type Square struct{ Side int }

func (Square) isShape() {}

func perimeter(s Shape) (int, error) {
	switch s := s.(type) {
	case Square:
		return 4 * s.Side, nil
	default:
		return 0, fmt.Errorf("unknown shape %T", s)
	}
}

func main() {
	fmt.Println(perimeter(Square{3}))
	fmt.Println(perimeter(&Square{3})) // a *Square is also a Shape
}
```

```output for=go-pointer-variant
12 <nil>
0 unknown shape *main.Square
```

- **Rust a catch-all arm hides a new variant.** A `_` arm "will match all values not specifically listed".[^4] It makes the match exhaustive, so the compiler accepts it, also after someone adds a variant. Here the arm was written when `Shape` had two variants, and a triangle now gets the perimeter 0. List the variants, and do not use `_` for an enum that can grow.

```rust run id=rust-wildcard-arm
#[allow(dead_code)]
enum Shape {
    Square { side: i64 },
    Rectangle { width: i64, height: i64 },
    Triangle { a: i64, b: i64, c: i64 }, // added later
}

fn perimeter(shape: &Shape) -> i64 {
    match shape {
        Shape::Square { side } => 4 * side,
        Shape::Rectangle { width, height } => 2 * (width + height),
        _ => 0, // written when there were two variants
    }
}

fn main() {
    let shapes = [
        Shape::Square { side: 3 },
        Shape::Rectangle { width: 2, height: 5 },
        Shape::Triangle { a: 3, b: 4, c: 5 },
    ];
    for s in &shapes {
        println!("{}", perimeter(s));
    }
}
```

```output for=rust-wildcard-arm
12
14
0
```

- **C the tag and the data can disagree.** Nothing ties the tag to the member that holds the data. "If the member used to read the contents of a union object is not the same as the member last used to store a value in the object, the appropriate part of the object representation of the value is reinterpreted as an object representation in the new type".[^13] Here a bug changes the tag, and the program reads the side of the square as the width of a rectangle. The other bytes of the union "take unspecified values"[^13], so the height must not be read at all. Set the tag and the data in one function, as `make_shape` does.

```c run id=c-tag-mismatch
#include <stdio.h>

enum shape_kind { SHAPE_SQUARE, SHAPE_RECTANGLE };

struct shape {
    enum shape_kind kind;
    union {
        struct {
            int side;
        } square;
        struct {
            int width, height;
        } rectangle;
    } as;
};

int main(void) {
    struct shape s = {.kind = SHAPE_SQUARE, .as.square.side = 3};
    s.kind = SHAPE_RECTANGLE; // a bug: the tag changes, the data does not
    if (s.kind == SHAPE_RECTANGLE) {
        printf("width: %d\n", s.as.rectangle.width);
    }
    return 0;
}
```

```output for=c-tag-mismatch
width: 3
```

- **C++ `std::get` throws for the wrong alternative.** `std::get<T>` returns the value when the variant holds `T`. "Otherwise, throws std::bad_variant_access."[^17] Use `std::visit`, which calls the right lambda, in place of a guess.

```cpp run id=cpp-wrong-alternative
#include <iostream>
#include <variant>

struct Square {
    int side;
};

struct Rectangle {
    int width, height;
};

int main() {
    std::variant<Square, Rectangle> shape = Square{3};
    try {
        std::cout << std::get<Rectangle>(shape).width << '\n';
    } catch (const std::bad_variant_access &) {
        std::cout << "bad_variant_access\n";
    }
    std::cout << std::get<Square>(shape).side << '\n';
}
```

```output for=cpp-wrong-alternative
bad_variant_access
3
```

- **Bash a missing value counts as 0.** `read` gives an empty value to a name that has no word. In arithmetic, "A shell variable that is null or unset evaluates to 0 when referenced by name in an expression."[^18] A triangle with two sides then has a perimeter and no error. Check the number of values when the shape is built, as `make_shape` does.

```bash run id=bash-missing-side
shape_perimeter() {
  local kind a b c
  read -r kind a b c <<<"$1"
  case "$kind" in
    triangle) echo $((a + b + c)) ;;
  esac
}

shape_perimeter "triangle 3 4 5"
shape_perimeter "triangle 3 4"
```

```output for=bash-missing-side
12
7
```

## Related tasks

- [[define-an-enumeration]]: a type with a fixed set of plain values.
- [[define-a-record-type]]: one variant alone is a record.
- [[use-a-tuple]]: a variant can carry a tuple, such as the Rust `V4(u8, u8, u8, u8)`.
- [[make-illegal-states-unrepresentable]]: the design practice behind this task.

## References
