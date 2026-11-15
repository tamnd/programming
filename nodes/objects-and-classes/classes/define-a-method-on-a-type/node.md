---
tag: QADG78
kind: task
slug: define-a-method-on-a-type
name: Define a method on a type
aliases: [add a method to a struct, method on a record, receiver function, impl block, member function on a struct, method on a value type]
area: objects-and-classes/classes
level: intro
summary: Define an immutable Point type with a translate method that returns another point and a manhattan method that takes a second point, using records, structs and receivers.
contract: contract.yaml
category: objects
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust]
see_also: [method-concept, define-a-class, classes-and-instances, field-and-property, immutability, value-equality-and-identity]
sources:
  - url: https://docs.python.org/3/library/dataclasses.html
    title: dataclasses, Data Classes, The Python Standard Library
    locator: Module contents; Frozen instances
  - url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-8.html
    title: Chapter 8, Classes, The Java Language Specification, Java SE 25 Edition
    locator: 8.10 Record Classes
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/builtin-types/record
    title: Records, C# reference
    locator: Introduction; Nondestructive mutation
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/programming-guide/classes-and-structs/methods
    title: Methods, C# programming guide
    locator: Method signatures
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Classes
    title: Classes, MDN Web Docs
    locator: Methods
  - url: https://go.dev/ref/spec
    title: The Go Programming Language Specification
    locator: Method declarations; Method sets
  - url: https://doc.rust-lang.org/book/ch05-03-method-syntax.html
    title: Methods, The Rust Programming Language
    locator: Defining Methods; Associated Functions
  - url: https://doc.rust-lang.org/reference/items/implementations.html
    title: Implementations, The Rust Reference
    locator: Inherent implementations
  - url: https://en.cppreference.com/cpp/language/member_functions
    title: Non-static member functions, cppreference.com
    locator: const- and volatile-qualified member functions
---

## Task

Define a type `Point` with two integer fields, `x` and `y`, and two methods. `translate(dx, dy)` returns another point, moved by `dx` and `dy`, and leaves the original point as it was. `manhattan(other)` takes a second point and returns the grid distance `|x - other.x| + |y - other.y|`. Then use them. The function `point_report(x, y, dx, dy)` makes the point `p = (x, y)`, computes `q = p.translate(dx, dy)`, and returns `[q.x, q.y, p.manhattan(q), q.manhattan(origin)]`. For example, `point_report(1, 2, 3, 4)` is `[4, 6, 7, 10]`.

Each input has an absolute value of 100,000,000 or less, so every result fits in a signed 32-bit integer. The tests also check that `translate` does not change `p`.

The type holds plain data, so each solution uses the lightest kind of type that a language offers for it: a record, a data class or a struct, not a full class. See [[define-a-class]] for a class with changing state.

## Solutions

| Language | The type | Where the methods go | The receiver |
|---|---|---|---|
| Python | `@dataclass(frozen=True) class Point` | the class body | `self`, the first parameter |
| JavaScript | `class Point`, frozen in the constructor | the class body | `this` |
| TypeScript | `class Point` with `readonly` fields | the class body | `this` |
| Java | `record Point(int x, int y)` | the record body | `this`, hidden |
| C# | `readonly record struct Point(int X, int Y)` | the record body | `this`, hidden |
| C++ | `struct Point` | member functions, declared in the struct | `this`; `const` member functions |
| Go | `type Point struct` | method declarations beside the type | a named value receiver `p Point` |
| Rust | `struct Point`, `Copy` | an `impl Point` block | `self` or `&self` |
| C | `struct point` | functions named `point_...` | the struct as the first parameter |

- **Python and Java generate the boilerplate.** The `dataclasses` module provides "a decorator and functions for automatically adding generated special methods such as __init__() and __repr__() to user-defined classes".[^1] With `frozen=True`, an assignment such as `p.x = 9` raises `FrozenInstanceError`. A Java record does the same work: each record component corresponds to "a private field declared implicitly, and a public accessor method declared explicitly or implicitly",[^2] so `q.x()` reads the field. The methods `translate` and `manhattan` go in the record body like the methods of any class, and the record also gets `equals`, `hashCode` and `toString`.
- **C# uses a record struct.** "The record struct syntax defines a value type."[^3] A `Point` is copied on assignment, and `readonly` makes every field read-only, so `Translate` must return a value made with `new`. C# methods "are declared in a class, struct, or interface",[^4] and a record struct is a struct. For a small value change, the `with` expression copies a record with some properties changed: `p with { X = 0 }`.
- **JavaScript has no record type.** The solution uses a class and calls `Object.freeze(this)` in the constructor. "Methods are defined on the prototype of each class instance and are shared by all instances."[^5] TypeScript marks the fields `readonly`, which the compiler checks; the object is not frozen at run time.
- **Go declares methods beside the type.** A method declaration "associates the method with the receiver's base type", and the base type "must be declared in the same package as the method".[^6] Any defined type can have methods, not only a struct: see the pitfall below. `Translate` and `Manhattan` have value receivers, so each call gets a copy of the point and cannot change the caller's point. Two `Point` values compare with `==`, because all their fields are comparable.
- **Rust puts methods in an `impl` block.** "Everything within this impl block will be associated with the Rectangle type", says the Rust book of its own example,[^7] and the same holds for `impl Point`. `translate` takes `self` by value; because `Point` derives `Copy`, the call copies the point and the caller can still use `p`. `manhattan` takes `&self` and `&Point`. The Reference allows inherent methods only where the type is defined: "An implementing type must be defined within the same crate as the original type definition."[^8]
- **C++ marks the methods that only read.** A `struct` is a class whose members are public by default. `translate` and `manhattan` are `const` member functions, so `this` has the type `const Point*` in them, and they can be called on a `const Point`.[^9] The defaulted `operator==` compares the fields.
- **C passes the struct by value.** `point_translate(struct point p, int dx, int dy)` gets a copy of the point and returns another one. A struct of two `int` fields is small, so the copy costs about as much as passing a pointer, and the function cannot change the caller's point.

### Alternatives

- **Methods that change the point.** If the point must change in place, give the method a pointer receiver in Go, `&mut self` in Rust, and drop `frozen`, `readonly` or the record. [[define-a-class]] shows this with `scale`.
- **Free functions.** Go, Rust and C++ also allow a plain function `manhattan(p, q)`. A method fits when the operation belongs to the type and should show in its documentation and in code completion; a free function fits when both arguments have equal parts, as in a distance.

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`.

- `zero-move`: `translate(0, 0)` returns an equal point, and the distance between `p` and `q` is 0.
- `negative`: negative coordinates and moves; `abs` makes each part of the distance positive.
- `origin`: the move ends at the origin, so the last result is 0.
- `limit`: every input is 100,000,000 in absolute value. The largest result is 400,000,000, which fits in a signed 32-bit integer. Inputs of up to 1,000,000,000 would not fit: a coordinate of 2,000,000,000 after the move is still below 2^31 - 1, but the distance to the origin, 4,000,000,000, is not.

## Pitfalls

- **Go and Rust allow methods only on local types.** A Go method cannot be declared on `int` or on a type from another package, and a Rust `impl i32` block is an error. Go code defines a local type with the built-in type as its underlying type, and that type can have methods even when it is not a struct:

```go run id=methods-on-a-defined-type
package main

import "fmt"

// Celsius is a defined type with float64 as its underlying type.
type Celsius float64

func (c Celsius) Fahrenheit() float64 { return float64(c)*9/5 + 32 }

// String makes Celsius a fmt.Stringer, so Println calls it.
func (c Celsius) String() string { return fmt.Sprintf("%.1f C", float64(c)) }

// Path is a defined slice type, and it can have methods too.
type Path []Celsius

func (p Path) Max() Celsius {
	m := p[0]
	for _, c := range p[1:] {
		m = max(m, c)
	}
	return m
}

func main() {
	t := Celsius(21.5)
	fmt.Println(t)
	fmt.Printf("%.1f\n", t.Fahrenheit())
	fmt.Println(Path{18, 23.5, 21}.Max())
}
```

```output for=methods-on-a-defined-type
21.5 C
70.7
23.5 C
```

Rust code declares a local trait and implements it for the foreign type, or wraps the type in a newtype:

```rust run id=trait-and-newtype
// A trait adds methods to a type of another crate, here the built-in i32.
trait Parity {
    fn is_even(&self) -> bool;
}

impl Parity for i32 {
    fn is_even(&self) -> bool {
        self % 2 == 0
    }
}

// A newtype wraps a type of another crate in a local type with its own methods.
struct Meters(f64);

impl Meters {
    fn to_feet(&self) -> f64 {
        self.0 / 0.3048
    }
}

fn main() {
    println!("{} {}", 4.is_even(), 7.is_even());
    println!("{:.2}", Meters(100.0).to_feet());
}
```

```output for=trait-and-newtype
true false
328.08
```

- **A Go value receiver and a pointer receiver do not mix freely.** A method with a pointer receiver `*Point` is not in the method set of `Point`, so a `Point` value in an interface variable does not have it. Choose one kind of receiver for all methods of a type.
- **A mutable struct in a C# collection or property.** Without `readonly`, a method that changes a struct changes a copy when the struct comes from a property or a list element. The `readonly record struct` avoids this, because no method can change the fields.
- **A frozen object in JavaScript is shallow.** `Object.freeze` stops assignments to `x` and `y`, but if a field holds an array, the array can still change. The same holds for Python's `frozen=True` and for Java records with a list component.
- **TypeScript `readonly` is a compile-time check.** JavaScript code, or TypeScript code that uses a type assertion, can still assign the field at run time.
- **`==` on JavaScript objects compares identity.** Two `Point` objects with the same fields are not equal under `===`, because JavaScript classes have no generated equality. Java records, C# records, Python dataclasses, Go structs and the C++ defaulted `operator==` compare the fields. In JavaScript, compare the fields or write an `equals` method.

## Related tasks

- [[define-a-class]] defines a class whose methods change its state.
- [[method-concept]] explains how a call finds the method and passes the receiver.
- [[classes-and-instances]] explains what a class is and how instances share its methods.

## References
