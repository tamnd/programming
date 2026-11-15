---
tag: QAC2V6
kind: task
slug: define-a-class
name: Define a class
aliases: [declare a class, write a class, class definition, class declaration, struct with methods, user-defined type]
area: objects-and-classes/classes
level: intro
summary: Define a Rectangle class with two fields, a constructor and methods, then make an instance, call its methods and change its state.
contract: contract.yaml
category: objects
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust]
see_also: [classes-and-instances, constructors, method-concept, field-and-property, define-a-method-on-a-type, encapsulation-and-visibility]
sources:
  - url: https://docs.python.org/3/tutorial/classes.html
    title: 9. Classes, The Python Tutorial
    locator: 9.3.2 Class Objects; 9.3.5 Class and Instance Variables
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Classes
    title: Classes, MDN Web Docs
    locator: Defining classes; Class body
  - url: https://www.typescriptlang.org/docs/handbook/2/classes.html
    title: Classes, TypeScript Handbook
    locator: Fields; --strictPropertyInitialization
  - url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-8.html
    title: Chapter 8, Classes, The Java Language Specification, Java SE 25 Edition
    locator: 8.8.9 Default Constructor
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/fundamentals/types/classes
    title: Introduction to classes, C# fundamentals
    locator: Reference types; Constructors and initialization
  - url: https://en.cppreference.com/cpp/language/classes
    title: Classes, cppreference.com
    locator: Introduction
  - url: https://go.dev/ref/spec
    title: The Go Programming Language Specification
    locator: Method declarations
  - url: https://doc.rust-lang.org/book/ch05-03-method-syntax.html
    title: Methods, The Rust Programming Language
    locator: Method Syntax; Associated Functions
---

## Task

Define a class `Rectangle` with two fields, `width` and `height`, a constructor that sets them, and three methods: `area()`, `perimeter()` and `scale(factor)`, which multiplies both sides by `factor` and so changes the instance. Then use it. The function `rectangle_report(width, height, factor)` makes one rectangle, reads its area and perimeter, scales it, and reads them again. It returns the four numbers. For example, `rectangle_report(3, 4, 2)` is `[12, 14, 48, 28]`.

The sides and the factor are 0 or more, and the area after the scale fits in a signed 32-bit integer. The tests also check that two instances have their own fields: a call to `scale` on one does not change the other.

## Solutions

| Language | The type | The constructor | The methods |
|---|---|---|---|
| Python | `class Rectangle` | `__init__(self, width, height)` | functions in the class body, with `self` first |
| JavaScript | `class Rectangle` | `constructor(width, height)` | methods in the class body; `this` is the instance |
| TypeScript | `class Rectangle` with typed fields | `constructor(width: number, height: number)` | as in JavaScript, with types |
| Java | `final class Rectangle` with `private` fields | `Rectangle(int width, int height)` | instance methods |
| C# | `sealed class Rectangle` with properties | `Rectangle(int width, int height)` | instance methods |
| C++ | `class Rectangle` with `private` members | a constructor with a member initializer list | member functions; `const` for the ones that only read |
| Go | `type Rectangle struct` | the function `New` | methods with a value or a pointer receiver |
| Rust | `struct Rectangle` | the associated function `new` | methods in an `impl` block with `&self` or `&mut self` |
| C | `struct rectangle` | the function `rectangle_make` | functions that take a pointer to the struct |

- **The class body holds the fields and the methods.** In Python, "When a class defines an __init__() method, class instantiation automatically invokes __init__() for the newly created class instance."[^1] The arguments of the call `Rectangle(3, 4)` go to `__init__`, and `__init__` sets the fields on `self`. A JavaScript class body is where "you define class members, such as methods or constructor"[^2], and `new Rectangle(3, 4)` calls the constructor. TypeScript adds a type to each field, and its setting `strictPropertyInitialization` "controls whether class fields need to be initialized in the constructor".[^3]
- **Java and C# keep the fields private.** The Java solution has `private` fields and methods that read them. If a Java class declares no constructor, "a default constructor is implicitly declared"[^4], which takes no arguments, so the fields keep their default values until something sets them. In C#, "A class is a reference type that defines a blueprint for objects."[^5] The C# solution uses properties with a `private set`, so other code can read `Width` and only the class can change it. C# also has primary constructors, which "add parameters directly to the class declaration"[^5], for example `class Rectangle(int width, int height)`.
- **C++ marks the methods that only read.** "A class is a user-defined type."[^6] The `const` after the parameter list of `area()` tells the compiler that the method does not change the object, so it can be called on a `const Rectangle`. The short methods are in the class body, and `scale` is defined in the `.cpp` file as `Rectangle::scale`.
- **Go and Rust have no classes.** A struct holds the fields, and the methods are declared outside it. In Go, "A receiver base type cannot be a pointer or interface type and it must be declared in the same package as the method."[^7] `Area` has a value receiver and reads a copy; `Scale` has a pointer receiver, `*Rectangle`, because it changes the rectangle. Neither language has constructors: Go uses a function `New`, and Rust uses an associated function with the conventional name `new`. In Rust, "Within an impl block, the type Self is an alias for the type that the impl block is for."[^8] `&self` borrows the rectangle to read it, and `&mut self` borrows it to change it. The Rust sides are `u32`, so a negative side cannot be made.
- **C writes the pattern by hand.** A struct holds the fields, and each "method" is a function whose first parameter is a pointer to the struct. A `const struct rectangle *` parameter marks a function that only reads. C has no access control, so any code can change `r.width`.

### Alternatives

- **A record.** When the fields never change after construction, a Java `record Rectangle(int width, int height)`, a C# `record`, a Python `@dataclass(frozen=True)` or a Kotlin `data class` writes the constructor, the accessors, equality and the text form for you. Then `scale` returns another rectangle and leaves the first one as it was.
- **Private fields in JavaScript.** A field whose name starts with `#`, such as `#width`, can be used only inside the class body. "In JavaScript, private elements use a special identifier syntax, so modifier keywords like public and private should not be used either."[^2]

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`.

- `zero-side`: a side of 0 gives an area of 0 and a perimeter of twice the other side.
- `zero-factor`: a factor of 0 makes both sides 0, so the area and the perimeter after the scale are 0.
- `identity`: a factor of 1 leaves the rectangle as it was.
- `limit`: 1000 by 1000 scaled by 46 has the area 2,116,000,000, which is less than 2^31 - 1, the largest signed 32-bit integer. A factor of 47 gives 2,209,000,000, which does not fit, so the contract does not allow it.

## Pitfalls

- **A Python class attribute is shared by all instances.** An attribute assigned in the class body belongs to the class, not to each instance. If it is a list, every instance appends to the same list. The tutorial warns that "shared data can have possibly surprising effects involving mutable objects such as lists and dictionaries".[^1] Make the list in `__init__`:

```python run id=shared-class-attribute
class Basket:
    items = []  # a class attribute: one list for all instances

    def add(self, item):
        self.items.append(item)


class FixedBasket:
    def __init__(self):
        self.items = []  # an instance attribute: one list per instance

    def add(self, item):
        self.items.append(item)


a, b = Basket(), Basket()
a.add("apple")
print(b.items)

c, d = FixedBasket(), FixedBasket()
c.add("apple")
print(d.items)
```

```output for=shared-class-attribute
['apple']
[]
```

- **A Go method with a value receiver changes a copy.** `Scale` below has the receiver `r Rectangle`, so it gets a copy of the rectangle, and the change is lost. `Grow` has the receiver `r *Rectangle` and changes the caller's rectangle:

```go run id=value-receiver
package main

import "fmt"

type Rectangle struct{ width, height int }

// A value receiver: Scale changes a copy.
func (r Rectangle) Scale(factor int) {
	r.width *= factor
	r.height *= factor
}

// A pointer receiver: Grow changes the caller's rectangle.
func (r *Rectangle) Grow(factor int) {
	r.width *= factor
	r.height *= factor
}

func main() {
	r := Rectangle{3, 4}
	r.Scale(2)
	fmt.Println(r)
	r.Grow(2)
	fmt.Println(r)
}
```

```output for=value-receiver
{3 4}
{6 8}
```

- **A Java or C# parameter with the name of a field hides the field.** In a constructor `Rectangle(int width, int height)`, the statement `width = width;` assigns the parameter to itself, and the field keeps its default value 0. Write `this.width = width;`. Python has the same trap without `self.`: `width = width` in `__init__` makes a local variable.
- **A JavaScript class cannot be used before its declaration.** Function declarations can be called before they appear in the code, but class declarations "behave as if they are not hoisted".[^2] Code that runs `new Rectangle(1, 2)` above `class Rectangle` throws a `ReferenceError`.

## Related tasks

- [[define-a-method-on-a-type]] adds a method to a struct, a record or another type.
- [[constructors]] explains constructors, overloads and initialization order.
- [[classes-and-instances]] explains what a class is and how instances share its methods.

## References
