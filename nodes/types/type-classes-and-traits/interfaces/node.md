---
tag: 3W80ZN
kind: concept
slug: interfaces
name: Interfaces
aliases: [interface, protocol, trait, type class, abstract base class, pure abstract class, contract]
area: types/type-classes-and-traits
level: core
summary: >-
  An interface is a named set of operations that a type promises to provide. Code written against it works with every type that keeps the promise.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1989
  people: [Philip Wadler, Stephen Blott]
  note: At POPL 1989, Wadler and Blott proposed type classes for Haskell. A class names a set of operations, an instance supplies them for one type, and the compiler translates the program so that it passes the operations as a dictionary.
see_also: [subtyping, parametric-polymorphism, strategy-pattern, type-erasure, static-and-dynamic-typing, template-method-pattern, null-references, sum-types]
same_as:
  wikidata: Q7251920
  wikipedia: Interface_(object-oriented_programming)
sources:
  - url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-9.html
    title: The Java Language Specification, Java SE 25 Edition
    locator: Chapter 9 Interfaces, introduction (a common supertype for otherwise unrelated classes; no instance variables; not directly instantiated)
  - paper: doi:10.1145/75277.75283
    title: Philip Wadler and Stephen Blott, How to make ad-hoc polymorphism less ad hoc, POPL 1989
    locator: Sections 3 and 4, Figure 2 (type classes and their translation into code that passes method dictionaries)
  - url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-8.html#jls-8.1.5
    title: The Java Language Specification, Java SE 25 Edition
    locator: 8.1.5 Superinterfaces (unless the class is abstract, it must implement the abstract methods of each direct superinterface) and 8.1.1.1 abstract Classes
  - url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-9.html#jls-9.4
    title: The Java Language Specification, Java SE 25 Edition
    locator: 9.4 Method Declarations (default methods; a method without private, default or static is implicitly abstract) and 9.4.3 Interface Method Body
  - url: https://jcp.org/en/jsr/detail?id=335
    title: JSR 335, Lambda Expressions for the Java Programming Language
    locator: Section 2.1 (Virtual Extension Methods, so that interfaces can gain methods without breaking existing implementation classes) and the Final Release of 04 March 2014
  - url: https://docs.oracle.com/javase/8/docs/technotes/guides/language/enhancements.html
    title: Java Programming Language Enhancements
    locator: Enhancements in Java SE 8 (default methods and binary compatibility; static methods in interfaces)
  - url: http://titanium.cs.berkeley.edu/doc/java-langspec-1.0/9.doc.html
    title: James Gosling, Bill Joy and Guy Steele, The Java Language Specification, first edition (Java 1.0), Sun Microsystems, 1996
    locator: Chapter 9 Interfaces, introduction (a class must be declared to implement the interface; implementing all the methods is not sufficient)
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/keywords/interface#default-interface-members
    title: C# language reference, interface keyword
    locator: the introduction (an interface defines a contract) and Default interface members
  - url: https://github.com/dotnet/csharplang/blob/68b7855d1189437a6ca4e6ee948f70c5afb7833c/proposals/csharp-8.0/default-interface-methods.md
    title: C# language proposal, default interface methods (C# 8.0)
    locator: Summary (virtual extension methods, like Java's default methods) and Concrete methods in interfaces (a class does not inherit members from its interfaces)
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/whats-new/csharp-version-history#c-version-80
    title: The history of C#
    locator: C# version 8.0 (released September 2019; default interface members) and C# version 1.0 (released January 2002; interfaces)
  - url: https://go.dev/ref/spec#Interface_types
    title: The Go Programming Language Specification
    locator: Interface types, Basic interfaces and Implementing an interface
  - url: https://go.dev/ref/spec#Method_sets
    title: The Go Programming Language Specification
    locator: Method sets (the method set of *T includes the methods with receiver *T or T; the method set of T only those with receiver T)
  - url: https://go.dev/doc/faq#guarantee_satisfies_interface
    title: Go Frequently Asked Questions
    locator: How can I guarantee my type satisfies an interface? (var _ T = (*MyType)(nil)) and Why doesn't Go have "implements" declarations?
  - url: https://go.dev/doc/faq#nil_error
    title: Go Frequently Asked Questions
    locator: Why is my nil error value not equal to nil? (an interface value is a type and a value, and is nil only when both are unset)
  - url: https://research.swtch.com/interfaces
    title: Russ Cox, Go Data Structures, Interfaces, 1 December 2009
    locator: Interface Values and Computing the Itable (a two-word pair of an itable pointer and a data pointer; the runtime computes the itable and caches it)
  - url: https://doc.rust-lang.org/reference/types/trait-object.html#r-type.trait-object.unsized
    title: The Rust Reference, Trait objects
    locator: "[type.trait-object.intro] and [type.trait-object.unsized] (a pointer to a trait object holds a data pointer and a vtable pointer; calls use virtual dispatch)"
  - url: https://doc.rust-lang.org/reference/items/traits.html#r-items.traits.dyn-compatible
    title: The Rust Reference, Traits
    locator: "[items.traits.dyn-compatible] and [items.traits.dyn-compatible.associated-functions] (dispatchable and explicitly non-dispatchable functions)"
  - url: https://doc.rust-lang.org/reference/items/implementations.html#r-items.impl.trait.orphan-rule.intro
    title: The Rust Reference, Implementations
    locator: "[items.impl.trait.coherence.intro] and [items.impl.trait.orphan-rule.intro] (the trait or one of the types must be local to the crate)"
  - url: https://doc.rust-lang.org/book/ch18-02-trait-objects.html#performing-dynamic-dispatch
    title: The Rust Programming Language, Using Trait Objects to Abstract over Shared Behavior
    locator: Performing Dynamic Dispatch (monomorphization gives static dispatch; trait objects give dynamic dispatch, with a cost at run time and no inlining)
  - url: https://www.haskell.org/onlinereport/haskell2010/haskellch4.html#x10-750004.3
    title: Haskell 2010 Language Report
    locator: 4.3 Type Classes and Overloading, 4.3.1 Class Declarations and 4.3.2 Instance Declarations
  - url: https://peps.python.org/pep-0544/#abstract
    title: "PEP 544: Protocols: Structural subtyping (static duck typing)"
    locator: Header (Created 05-Mar-2017, Python-Version 3.8), Abstract, and Explicitly declaring implementation
  - url: https://docs.python.org/3.14/library/abc.html#abc.abstractmethod
    title: The Python Standard Library, abc, Python 3.14
    locator: abc (the module provides the infrastructure of PEP 3119) and abc.abstractmethod (a class with ABCMeta cannot be instantiated unless all abstract methods are overridden)
  - url: https://docs.python.org/3.14/library/typing.html#typing.Protocol
    title: The Python Standard Library, typing, Python 3.14
    locator: typing.Protocol and typing.runtime_checkable (the check looks only for the presence of the attributes)
  - url: https://www.typescriptlang.org/docs/handbook/2/classes.html#implements-clauses
    title: TypeScript Handbook, Classes
    locator: implements Clauses and Cautions (an implements clause is only a check and does not change the type of the class)
  - url: https://www.typescriptlang.org/docs/handbook/type-compatibility.html#starting-out
    title: TypeScript Handbook, Type Compatibility
    locator: the introduction (structural subtyping) and Starting out
  - url: https://www.typescriptlang.org/docs/handbook/2/basic-types.html#erased-types
    title: TypeScript Handbook, The Basics
    locator: Erased Types
  - url: https://www.typescriptlang.org/docs/handbook/2/narrowing.html#using-type-predicates
    title: TypeScript Handbook, Narrowing
    locator: Using type predicates
  - url: https://tc39.es/ecma262/2026/#sec-common-iteration-interfaces
    title: ECMAScript 2026 Language Specification
    locator: 27.1.1 Common Iteration Interfaces (an interface is a set of property keys; it is not represented by a distinct object) and 27.1.1.1 The Iterable Interface
  - url: https://timsong-cpp.github.io/cppwp/n4950/class.abstract
    title: Working Draft, Programming Languages, C++ (N4950, the C++23 draft)
    locator: "[class.abstract], paragraphs 1, 2 and 4 (pure virtual functions; no objects of an abstract class except as subobjects)"
  - url: https://timsong-cpp.github.io/cppwp/n4950/temp.concept
    title: Working Draft, Programming Languages, C++ (N4950, the C++23 draft)
    locator: "[temp.concept], paragraph 1 (a concept is a template that defines constraints on its template arguments)"
  - url: https://itanium-cxx-abi.github.io/cxx-abi/abi.html#vtable-general
    title: Itanium C++ ABI
    locator: 2.5.1 General (a virtual table is used to dispatch virtual functions)
  - url: https://docs.oracle.com/javase/specs/jvms/se25/html/jvms-6.html#jvms-6.5.invokeinterface
    title: The Java Virtual Machine Specification, Java SE 25 Edition
    locator: 6.5 invokeinterface, Description (a method is selected with respect to the class of objectref and the resolved method)
  - url: https://kotlinlang.org/docs/interfaces.html#implementing-interfaces
    title: Kotlin documentation, Interfaces
    locator: the introduction (interfaces can contain abstract methods and method implementations but cannot store state) and Implementing interfaces
  - url: https://learn.microsoft.com/en-us/powershell/module/microsoft.powershell.core/about/about_classes_inheritance?view=powershell-7.5#implementing-interfaces
    title: PowerShell documentation, about_Classes_Inheritance
    locator: Long description (a class that does not implement an interface that it inherits gives a parsing error) and Implementing interfaces
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/keywords/interface#static-abstract-and-virtual-members
    title: C# language reference, interface keyword
    locator: Static abstract and virtual members
  - url: https://go.dev/doc/devel/release#go1
    title: Go release history
    locator: go1 (released 2012-03-28)
  - url: https://doc.rust-lang.org/book/ch10-02-traits.html#default-implementations
    title: The Rust Programming Language, Defining Shared Behavior with Traits
    locator: Implementing a Trait on a Type and Default Implementations
---

## Idea

An interface is a named set of operations. A type that implements the interface promises to provide each operation. Code that is written against the interface can then work with any such type. It does not need to know which type it has.

Java describes an interface as "a common supertype for otherwise unrelated classes". An interface has no instance variables, and a program cannot create an object of the interface itself.[^1] Other languages use other names for the same idea. Python has protocols, Rust has traits, Haskell has type classes, and C++ uses abstract classes.

In the program below, `expired` needs only one operation: the current time. The interface `Clock` names that operation. The program passes a fixed clock and the system clock, and `expired` treats them the same way.

```java run id=idea
public class Main {
    interface Clock {
        long millis();
    }

    static final class SystemClock implements Clock {
        public long millis() {
            return System.currentTimeMillis();
        }
    }

    record FixedClock(long millis) implements Clock {}

    static boolean expired(Clock clock, long deadline) {
        return clock.millis() > deadline;
    }

    public static void main(String[] args) {
        System.out.println(expired(new FixedClock(100), 50));
        System.out.println(expired(new FixedClock(10), 50));
        System.out.println(expired(new SystemClock(), 50));
    }
}
```

```output
true
false
true
```

The record `FixedClock` gets a method `millis()` from its component, and that method implements the interface. A test can use a fixed clock, and the real program can use the system clock. This is the main use of an interface: it separates the code that needs an operation from the code that provides it. The [[strategy-pattern]] is this idea used as a design pattern.

## How it works

An interface involves three things. The declaration lists the operations. An implementation provides them for one type. A call site uses them through the interface. Languages differ in each part.

### Declaring that a type implements an interface

- **Nominal.** The type names the interface. In Java a class lists it in an `implements` clause. Unless the class is abstract, it must implement every abstract method of the interface, or the program does not compile.[^3] The first edition of the Java specification states the rule plainly. It is not sufficient that a class happens to have all the methods. The class must be declared to implement the interface.[^7] C#, Kotlin and C++ work the same way.[^8][^33]
- **Structural.** The type implements the interface when it has the operations, and nothing names the interface. In Go, a type implements an interface when its method set contains the methods of the interface.[^11] TypeScript compares types by their members.[^25] Python protocols give the same rule to a static type checker.[^21]
- **Separate instances.** In Rust, an `impl Shape for Square` block implements a trait for a type. The block is separate from the type. So a crate can implement its own trait for a type from another crate, or a trait from another crate for its own type. The orphan rule forbids the remaining case: the trait or at least one of the types must be local to the crate.[^18] Haskell instance declarations work in the same way.[^20]

The [[subtyping]] node compares nominal and structural relations in more detail.

### Default methods

An interface can also give a body to some operations. A type that implements the interface gets the body unless it provides its own.

Java added default methods in Java SE 8. A default method is an instance method that is declared in an interface with the `default` modifier.[^4] The reason was library evolution. Without default methods, an added method in an interface breaks every class that implements the interface. JSR 335 listed "virtual extension methods", so that interfaces could gain methods "after the fact" without breaking existing implementation classes.[^5] The Java SE 8 notes say that default methods keep binary compatibility with code written for older versions.[^6]

C# 8.0 added default interface members in September 2019.[^10] The proposal calls them virtual extension methods, like Java's default methods.[^9] Rust traits and Haskell classes can give default bodies too.[^38][^20] In C++, a non-virtual member function of an abstract class plays the same part. The `describe` function in the C++ example calls the pure virtual functions of its own class.

### Static dispatch

A call through an interface needs the code of the right implementation. When the compiler knows the concrete type at the call site, it can call that code directly. This is static dispatch.

Rust does this for a generic function such as `fn total<T: Shape>(items: &[T])`. The compiler generates a separate copy of the function for each concrete type that the program uses. The Rust book calls this monomorphization. Each copy calls the method directly, and the compiler can inline it.[^19] C++ templates work the same way. A C++20 concept is "a template that defines constraints on its template arguments".[^30] It checks the type at compile time and gives no run-time dispatch.

Haskell takes a third path. Wadler and Blott showed how to translate a program with type classes into a program without them. Each instance becomes a record of functions, which they call a method dictionary. A function with a class constraint gets an extra parameter, the dictionary, and calls the operations through it. In their example, the class `Num` becomes a data type `NumD` with one field for each operation.[^2]

### Dynamic dispatch

When the compiler does not know the concrete type, the program must find the code at run time. All the common designs use a table of function pointers. They differ in where the table pointer lives.

- **A table pointer in the object.** A C++ object of a class with virtual functions holds a pointer to a virtual table. A call loads the table pointer, loads the function address from the table, and calls it.[^31] A Java call through an interface compiles to the instruction `invokeinterface`. The JVM selects the method from the class of the object.[^32] The JVM specification defines which method is selected but not the data structure that finds it.
- **A table pointer next to the object.** A Rust trait object `&dyn Shape` is a fat pointer. It holds a pointer to the data and a pointer to a vtable for the concrete type.[^16] The Rust example prints the sizes: 8 bytes for `&Square` and 16 bytes for `&dyn Shape` on a 64-bit target. A Go interface value is also two words. Russ Cox describes them as a pointer to an interface table, or itable, and a pointer to the data. Because Go types do not declare their interfaces, the runtime computes each itable when it first needs it, and caches it.[^15]
- **No table.** Python and JavaScript look up the method by name in the object and its class at each call. Cox contrasts this with the tables that C++ and Java prepare statically.[^15]

C has no interfaces, but the same design is a common idiom. A struct of function pointers is the interface, and one constant instance of it for each type is the implementation. The C example pairs a pointer to such a table with a pointer to the data. A Go interface value has the same layout.

Dynamic dispatch lets one container hold values of many types. Static dispatch gives faster calls but needs one type per instantiation. The Rust example shows both forms of the same function.

## In languages

The example files show interfaces in each language. The table gives the verdict for each tier 1 language.

| Language | Verdict | Form |
|---|---|---|
| Python | native | abstract base classes in `abc` (nominal); `typing.Protocol` (structural, for a type checker)[^22][^23] |
| JavaScript | idiom | no declaration; an object conforms when it has the properties, as with the iterable interface[^28] |
| TypeScript | native | `interface`, compared by structure; erased before the program runs[^25][^26] |
| Java | native | `interface` and `implements`, with default and static methods[^1][^4] |
| C | idiom | a struct of function pointers, with one constant table for each type |
| C++ | native | abstract classes with pure virtual functions; C++20 concepts for compile-time constraints[^29][^30] |
| C# | native | `interface`, with default members and static abstract members[^8][^35] |
| Go | native | structural; a type implements an interface through its method set[^11][^12] |
| Rust | native | traits and `impl Trait for Type`; generics or `dyn Trait`[^16][^18] |
| Kotlin | native | `interface`, with method bodies and abstract properties but no state[^33] |
| PowerShell | native | classes can implement .NET interfaces[^34] |
| Bash | n/a | no types or methods |
| SQL | n/a | no user-defined types with operations |
| Regex | n/a | a pattern has no types |
| HTML | n/a | markup has no types |
| CSS | n/a | style rules have no types |

Kotlin interfaces can contain abstract methods and method implementations. Unlike abstract classes, they cannot store state.[^33] A PowerShell class can implement a .NET interface. Some operators depend on this: `-lt` and `-gt` work only on classes that implement `System.IComparable`.[^34]

## Variations and related ideas

- **Abstract classes.** An abstract class can mix abstract methods with fields and constructors. C++ has no separate interface construct. The standard notes that an abstract class can be used to define an interface whose implementations are in derived classes.[^29] The [[template-method-pattern]] uses an abstract class with a fixed algorithm and abstract steps.
- **Static members.** C# interfaces can declare static abstract members, including operators. A generic method can then call `T.Zero` or `+` on a type parameter.[^35] This is close to a Haskell type class, where `Num` declares `(+)` for a type.[^20] The C# example shows it.
- **Generic bounds.** A bound such as `<T extends Comparable<T>>` or `T: Shape` combines an interface with [[parametric-polymorphism]]. The bound says which operations the generic code may use.
- **Closed sets of types.** An interface is open: any type can implement it later. When the set of variants is fixed, a [[sum-types]] declaration lets the compiler check that a `match` handles each case.
- **Hiding the concrete type.** A caller that holds a value through an interface or a trait object does not see its concrete type. [[type-erasure]] covers the ways that languages remove or hide type information.
- **Duck typing.** Python and JavaScript call a method if the object has it. No declaration is needed, and a missing method fails at run time. The ECMAScript specification uses this model for its own interfaces. It defines an interface as a set of property keys, and says that an interface is not represented by a distinct object.[^28] [[static-and-dynamic-typing]] explains the difference.

## Pitfalls

### A class that misses a method

A Java class that implements an interface must implement all its abstract methods, unless the class is abstract.[^3]

```java fails expect=compile id=java-missing-method
public class Main {
    interface Shape {
        double area();
        double perimeter();
    }

    static class Square implements Shape {
        public double area() {
            return 4;
        }
    }

    public static void main(String[] args) {
        System.out.println(new Square().area());
    }
}
```

javac reports that `Square` is not abstract and does not override the abstract method `perimeter()` in `Shape`. Add the method, or give it a default body in the interface.

### A pointer receiver in Go

In Go, a method with a pointer receiver is in the method set of `*T` but not in the method set of `T`.[^12] So the value `Counter{}` does not implement `fmt.Stringer`, although `&Counter{}` does.

```go fails expect=compile id=go-method-set
package main

import "fmt"

type Counter struct{ n int }

func (c *Counter) String() string { return fmt.Sprintf("count %d", c.n) }

var _ fmt.Stringer = Counter{}

func main() {
	fmt.Println(&Counter{})
}
```

The compiler says that `Counter` does not implement `fmt.Stringer`, because method `String` has a pointer receiver. A type with a missing method gives the same kind of error. The line `var _ fmt.Stringer = (*Counter)(nil)` is the check that the Go FAQ recommends.[^13]

### Creating an object of an abstract class

In C++, no objects of an abstract class can be created, except as subobjects of a derived class. A class is abstract when it has at least one pure virtual function, or when a pure virtual function is not overridden.[^29]

```cpp fails expect=compile id=cpp-abstract
#include <iostream>

struct Shape {
    virtual ~Shape() = default;
    virtual double area() const = 0;
    virtual double perimeter() const = 0;
};

struct Square : Shape {
    double area() const override { return 4; }
};

int main() {
    Square square;
    std::cout << square.area() << "\n";
}
```

`Square` does not override `perimeter`, so it is still abstract, and `Square square;` does not compile.

### A missing abstract method in Python

Python checks an abstract base class when the program creates an object, not when it defines the class. A class with the metaclass `ABCMeta` cannot be instantiated unless all its abstract methods are overridden.[^22]

```python fails expect=exception:TypeError id=python-abc
from abc import ABC, abstractmethod


class Shape(ABC):
    @abstractmethod
    def area(self) -> float: ...

    @abstractmethod
    def perimeter(self) -> float: ...


class Square(Shape):
    def __init__(self, side: float) -> None:
        self.side = side

    def area(self) -> float:
        return self.side * self.side


print("class defined")
print(Square(2).area())
```

The class definition succeeds, and the first line prints. The call `Square(2)` raises `TypeError`, because `perimeter` has no implementation.

### A nil pointer inside a Go interface

A Go interface value holds a type and a value. It is nil only when both are unset. If a function stores a nil `*MyError` in a result of type `error`, the result has a type, so it is not nil.[^14]

```go wrong id=go-nil-interface
package main

import "fmt"

type MyError struct{}

func (e *MyError) Error() string { return "failed" }

func check(fail bool) error {
	var p *MyError
	if fail {
		p = &MyError{}
	}
	return p
}

func main() {
	err := check(false)
	fmt.Println(err == nil)
	fmt.Printf("%T\n", err)
}
```

```output
false
*main.MyError
```

The caller expects `true`, but the result is `false`. The interface holds the type `*main.MyError` and a nil pointer. Return a literal `nil` for success, as the FAQ advises.

```go run id=go-nil-interface-fixed
package main

import "fmt"

type MyError struct{}

func (e *MyError) Error() string { return "failed" }

func check(fail bool) error {
	if fail {
		return &MyError{}
	}
	return nil
}

func main() {
	fmt.Println(check(false) == nil)
	fmt.Println(check(true))
}
```

```output
true
failed
```

### A trait that cannot be a trait object

A Rust trait can be used as `dyn Trait` only if it is dyn compatible. Each method must be dispatchable through a vtable, or must be explicitly excluded with `where Self: Sized`. A dispatchable method must not use `Self` except in the receiver.[^17] The method `scaled` below returns `Self`, so the vtable cannot hold it.

```rust fails expect=compile id=rust-dyn-compatible
trait Shape {
    fn area(&self) -> f64;
    fn scaled(&self, k: f64) -> Self;
}

struct Square {
    side: f64,
}

impl Shape for Square {
    fn area(&self) -> f64 {
        self.side * self.side
    }

    fn scaled(&self, k: f64) -> Self {
        Square { side: self.side * k }
    }
}

fn main() {
    let shapes: Vec<Box<dyn Shape>> = vec![Box::new(Square { side: 2.0 })];
    println!("{}", shapes[0].area());
}
```

rustc rejects the program with error E0038: the trait `Shape` is not dyn compatible. Write `fn scaled(&self, k: f64) -> Self where Self: Sized;`. The method is then not available on a trait object, but the rest of the trait is.

### A default member called through the class in C#

A C# class does not inherit the members of its interfaces. A default member is therefore not a member of the class.[^9]

```csharp fails expect=compile timeout=30 id=csharp-default-member
var square = new Square(2);
Console.WriteLine(square.Describe());

interface IShape
{
    double Area();
    string Describe() => $"area {Area():F2}";
}

class Square(double side) : IShape
{
    public double Area() => side * side;
}
```

The compiler reports that `Square` does not contain a definition for `Describe`. Call the member through the interface: `IShape shape = square;` and then `shape.Describe()`.

### Testing for a TypeScript interface at run time

TypeScript erases interfaces before the program runs.[^26] An `implements` clause only asks the compiler to check the class.[^24] So no value named `Shape` exists at run time. The runner here only strips the types, so the program fails when the line runs.

```typescript fails expect=exception:ReferenceError id=ts-instanceof
interface Shape {
  area(): number;
}

const value: unknown = { area: () => 4 };
console.log("checking");
console.log(value instanceof Shape);
```

The program prints `checking` and then stops with `ReferenceError: Shape is not defined`. Check the members instead, with a type predicate such as `isShape` in the TypeScript example.[^27]

## Cost

- **Static dispatch.** A monomorphized call is a direct call, which the compiler can inline.[^19] Each concrete type gets its own copy of the generic function, so the program has more machine code.
- **Dynamic dispatch.** A call through a vtable or an itable is an indirect call through a table. It prevents inlining. The Rust book names this run-time cost.[^19]
- **Fat pointers.** A Rust `&dyn Trait` and a Go interface value are two words, not one.[^16][^15] A C++ object of a class with virtual functions pays instead with a table pointer inside the object.[^31]
- **Itable construction.** Go computes the itable for a pair of interface and concrete type at run time, when the pair first meets. It caches the result.[^15]
- **Run-time checks.** An `isinstance` check against a runtime-checkable protocol in Python looks for each attribute.[^23]

## History

- **1989.** Philip Wadler and Stephen Blott presented type classes at POPL. The work grew out of the design of Haskell. They gave a translation into dictionary passing.[^2]
- **1996.** The first edition of the Java Language Specification defined interfaces as types separate from classes. A class had to declare the interfaces that it implemented.[^7]
- **January 2002.** C# 1.0 was released with interfaces.[^10]
- **March 2012.** Go 1 was released. Its interfaces are structural, and types implement them without a declaration.[^36][^13]
- **March 2014.** The Final Release of JSR 335 was published. The JSR covered lambda expressions and virtual extension methods, which Java SE 8 calls default methods.[^5][^6]
- **2017.** PEP 544 was created in March 2017. It added protocols to Python 3.8 for structural subtyping in type checkers.[^21]
- **September 2019.** C# 8.0 added default interface members.[^10]

## References
