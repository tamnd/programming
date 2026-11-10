---
tag: NEW-generics
kind: concept
slug: parametric-polymorphism
name: Generics
aliases: [generics, generic programming, type parameters, templates]
area: types/generics
level: core
summary: >-
  Generics let one definition of a function or a type work for many types. The definition takes type parameters, each use supplies type arguments, and the compiler checks the types.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1977
  people: [Barbara Liskov, Alan Snyder, Russell Atkinson, Craig Schaffert]
  note: CLU let a cluster take a type as a parameter, with a where clause that names the operations that the type must have.
see_also: [static-and-dynamic-typing, iterators, closures, exception-handling, garbage-collection]
same_as:
  wikidata: Q1051282
  wikipedia: Generic_programming
sources:
  - paper: doi:10.1023/A:1010000313106
    title: Christopher Strachey, Fundamental Concepts in Programming Languages, Higher-Order and Symbolic Computation 13(1-2), 2000 (lecture notes of August 1967)
    locator: 3.6.4 Polymorphism (ad hoc polymorphism and parametric polymorphism)
  - paper: doi:10.1145/359763.359789
    title: Barbara Liskov, Alan Snyder, Russell Atkinson and Craig Schaffert, Abstraction mechanisms in CLU, Communications of the ACM 20(8), 1977
    locator: 4.1 Properties of the Sorted_bag Abstraction (the parameterized cluster sorted_bag[t] and its where clause)
  - paper: doi:10.1145/359763.359789
    title: Barbara Liskov, Alan Snyder, Russell Atkinson and Craig Schaffert, Abstraction mechanisms in CLU, Communications of the ACM 20(8), 1977
    locator: 6 Implementation (code shared among all instantiations of a parameterized module)
  - paper: doi:10.1007/3-540-06859-7_148
    title: John C. Reynolds, Towards a theory of type structure, Programming Symposium, LNCS 19, 1974
    locator: the polymorphic typed lambda calculus, with abstraction over types and application to types
  - paper: doi:10.1016/0022-0000(78)90014-4
    title: Robin Milner, A theory of type polymorphism in programming, Journal of Computer and System Sciences 17(3), 1978
    locator: the introduction (a polymorphic type discipline for ML, with types inferred by the compiler)
  - paper: doi:10.1145/75277.75283
    title: Philip Wadler and Stephen Blott, How to make ad-hoc polymorphism less ad hoc, POPL 1989
    locator: type classes and their translation into code that passes method dictionaries
  - paper: doi:10.1007/3-540-51084-2_2
    title: David R. Musser and Alexander A. Stepanov, Generic programming, ISSAC 1988, LNCS 358, 1989
    locator: the introduction (algorithms written once for a family of data abstractions)
  - paper: doi:10.1145/1238844.1238848
    title: Bjarne Stroustrup, Evolving a language in and for the real world, C++ 1991-2006, HOPL III, 2007
    locator: the timeline in section 2 (1990, templates accepted; 1991, Cfront Release 3.0) and section 3 (Cfront 3.0 was the first release to support templates)
  - paper: doi:10.1145/1238844.1238848
    title: Bjarne Stroustrup, Evolving a language in and for the real world, C++ 1991-2006, HOPL III, 2007
    locator: 3.2 Chronology (1994, the STL) and 4.1 The STL
  - paper: doi:10.1145/286936.286957
    title: Gilad Bracha, Martin Odersky, David Stoutamire and Philip Wadler, Making the future safe for the past, Adding genericity to the Java programming language, OOPSLA 1998
    locator: the abstract (translation that erases type parameters and inserts casts) and the section on raw types
  - url: https://jcp.org/en/jsr/detail?id=14
    title: "JSR 14: Add Generic Types To The Java Programming Language"
    locator: the description and the Final Release date (30 Sep 2004)
  - url: https://docs.oracle.com/javase/1.5.0/docs/relnotes/features.html
    title: New Features and Enhancements, J2SE 5.0
    locator: Java Language Features, Generics
  - paper: doi:10.1145/378795.378797
    title: Andrew Kennedy and Don Syme, Design and implementation of generics for the .NET Common Language Runtime, PLDI 2001
    locator: the design of generics as a part of the runtime, not only of the compiler
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/whats-new/csharp-version-history
    title: The history of C#
    locator: C# version 2.0 (November 2005, generics) and C# version 4.0 (April 2010, generic covariance and contravariance)
  - url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-4.html#jls-4.5.1
    title: The Java Language Specification, Java SE 25 Edition
    locator: 4.5.1 Type Arguments of Parameterized Types
  - url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-4.html#jls-4.6
    title: The Java Language Specification, Java SE 25 Edition
    locator: 4.6 Type Erasure
  - url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-4.html#jls-4.7
    title: The Java Language Specification, Java SE 25 Edition
    locator: 4.7 Reifiable Types
  - url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-4.html#jls-4.8
    title: The Java Language Specification, Java SE 25 Edition
    locator: 4.8 Raw Types
  - url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-4.html#jls-4.10.2
    title: The Java Language Specification, Java SE 25 Edition
    locator: 4.10.2 Subtyping among Class and Interface Types
  - url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-4.html#jls-4.12.2
    title: The Java Language Specification, Java SE 25 Edition
    locator: 4.12.2 Variables of Reference Type (heap pollution)
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/programming-guide/generics/generics-in-the-run-time
    title: C# programming guide, Generics in the runtime
    locator: value types as type arguments, reference types as type arguments
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/programming-guide/generics/constraints-on-type-parameters
    title: C# programming guide, Constraints on type parameters
    locator: the table of constraints and the paragraph on unbounded type parameters
  - url: https://learn.microsoft.com/en-us/dotnet/standard/generics/covariance-and-contravariance
    title: .NET documentation, Covariance and contravariance in generics
    locator: the introduction and the list of variant generic interfaces and delegates
  - url: https://doc.rust-lang.org/book/ch10-01-syntax.html#performance-of-code-using-generics
    title: The Rust Programming Language, 10.1 Generic Data Types
    locator: Performance of Code Using Generics
  - url: https://doc.rust-lang.org/book/ch10-01-syntax.html#in-function-definitions
    title: The Rust Programming Language, 10.1 Generic Data Types
    locator: In Function Definitions (error E0369 for the function largest without a bound)
  - url: https://github.com/golang/proposal/blob/master/design/generics-implementation-dictionaries-go1.18.md
    title: Go 1.18 Implementation of Generics via Dictionaries and Gcshape Stenciling
    locator: the overview, Gcshapes and Dictionary Format
  - url: https://go.dev/ref/spec#Type_parameter_declarations
    title: The Go Programming Language Specification
    locator: Type parameter declarations
  - url: https://go.dev/ref/spec#Type_constraints
    title: The Go Programming Language Specification
    locator: Type constraints
  - url: https://go.dev/doc/go1.18#generics
    title: Go 1.18 Release Notes
    locator: Changes to the language, Generics
  - url: https://go.dev/doc/devel/release
    title: Go release history
    locator: go1.18 (released 2022-03-15) and go1.27.0 (released 2026-08-19)
  - url: https://kotlinlang.org/docs/generics.html#declaration-site-variance
    title: Kotlin documentation, Generics, in, out, where
    locator: Declaration-site variance and Type projections
  - url: https://kotlinlang.org/docs/generics.html#type-erasure
    title: Kotlin documentation, Generics, in, out, where
    locator: Type erasure and Generics type checks and casts
  - url: https://www.typescriptlang.org/docs/handbook/2/basic-types.html#erased-types
    title: TypeScript Handbook, The Basics
    locator: Erased Types
  - url: https://docs.python.org/3.14/library/typing.html#user-defined-generic-types
    title: The Python Standard Library, typing, Python 3.14
    locator: the note at the top of the page, Generics, and User-defined generic types
  - url: https://timsong-cpp.github.io/cppwp/n4950/temp.concept
    title: Working Draft, Programming Languages, C++ (N4950, the C++23 draft)
    locator: "[temp.concept], paragraph 1"
  - url: https://port70.net/~nsz/c/c11/n1570.html#6.5.1.1
    title: ISO/IEC 9899:201x, Committee Draft N1570 (C11)
    locator: 6.5.1.1 Generic selection
  - url: https://port70.net/~nsz/c/c11/n1570.html#7.22.5.2
    title: ISO/IEC 9899:201x, Committee Draft N1570 (C11)
    locator: 7.22.5.2 The qsort function
  - url: https://learn.microsoft.com/en-us/powershell/module/microsoft.powershell.core/about/about_calling_generic_methods?view=powershell-7.5
    title: PowerShell documentation, about_Calling_Generic_Methods
    locator: Short description and Long description
  - url: https://go.dev/ref/spec#Method_declarations
    title: The Go Programming Language Specification
    locator: Method declarations (a method declaration that specifies type parameters denotes a generic method)
  - url: https://go.dev/ref/spec#Basic_interfaces
    title: The Go Programming Language Specification
    locator: Basic interfaces (interface methods cannot declare type parameters)
  - url: https://go.dev/doc/go1.27#language
    title: Go 1.27 Release Notes
    locator: Changes to the language (generic methods; interface methods cannot be implemented by generic methods)
---

## Idea

Many functions and data structures do not depend on the type of the values that they hold. A list, a stack and a sort work the same way for numbers, strings or records. Without generics, a statically typed language gives two poor choices. You can write one copy of the code for each type. Or you can use one universal type, such as `Object` in Java or `void *` in C, and convert back with a cast that the compiler cannot check.

Generics give a third choice. A definition takes type parameters, and each use supplies type arguments. Strachey called this parametric polymorphism: one function works uniformly for all types, and the type of its result follows from the types of its arguments by one rule.[^1] In the Java program below, `last` has the type parameter `T`. The compiler infers `T = String` for the first call and `T = Integer` for the second call, so each result has its exact type and needs no cast.

```java run id=idea
import java.util.ArrayList;
import java.util.List;

public class Main {
    static <T> T last(List<T> items) {
        return items.get(items.size() - 1);
    }

    public static void main(String[] args) {
        List<String> names = new ArrayList<>(List.of("Ada", "Grace"));
        List<Integer> sizes = new ArrayList<>(List.of(3, 5, 8));
        String name = last(names);
        int size = last(sizes);
        System.out.println(name.toUpperCase() + " " + (size + 1));
    }
}
```

```output for=idea
GRACE 9
```

The compiler also checks every use against the type argument. Here `names` is a `List<String>`, so a call that adds a number to it does not compile.

```java fails expect=compile id=idea-check
import java.util.ArrayList;
import java.util.List;

public class Main {
    public static void main(String[] args) {
        List<String> names = new ArrayList<>();
        names.add(42);
        System.out.println(names);
    }
}
```

`javac` reports `incompatible types: int cannot be converted to String`. The check happens once, when the program is compiled, not each time the code runs.

Generics are a feature of static type systems. In a dynamically typed language, such as JavaScript or Python at run time, every function already accepts values of any type, and checks happen when an operation runs. [[static-and-dynamic-typing]] explains the difference.

## How it works

A generic definition has three parts:

- **Type parameters.** Names such as `T` that stand for types inside the definition. Java and C# write `<T>`, Go and Python write `[T]`, and C++ writes `template <typename T>`.[^27][^34]
- **Constraints, also called bounds.** A constraint states what a type argument must provide. Inside the body, a value of type `T` supports only the operations of its constraint. CLU wrote the constraint as a `where` clause: the type `t` of `sorted_bag[t]` must have the operations `lt` and `equal`.[^2] C# also uses `where`, Java and TypeScript use `extends`, Rust uses traits, Go uses interfaces, and C++ uses concepts.[^22][^28][^35]
- **Instantiation.** A use with type arguments, such as `List<String>` or `Max[int]`. Often the compiler infers the type arguments from the arguments of a call.

The constraint lets the compiler check the body once, for all type arguments. The Rust Book shows a function `largest<T>` that compares two values of type `T` with `>`. It does not compile, because not every type supports `>`. The fix is the bound `T: PartialOrd`.[^25]

```rust fails expect=compile id=rust-no-bound
fn largest<T>(items: &[T]) -> &T {
    let mut result = &items[0];
    for item in items {
        if item > result {
            result = item;
        }
    }
    result
}

fn main() {
    println!("{}", largest(&[3, 9, 4]));
}
```

`rustc` stops with error E0369, `` binary operation `>` cannot be applied to type `&T` ``, and suggests the bound `std::cmp::PartialOrd`.

Languages differ in what the compiled program does with the type arguments. There are four main strategies.

| Strategy | What the compiled program contains | Languages |
|---|---|---|
| Erasure | one copy; type arguments are removed, type variables become their bounds, and the compiler inserts casts[^10][^16] | Java, Kotlin on the JVM, TypeScript[^32][^33] |
| Specialization at run time | one shared copy for reference types and one copy for each value type; type arguments exist at run time[^21] | C#, the other .NET languages |
| Monomorphization | one copy for each set of type arguments, made by the compiler[^24] | C++, Rust |
| Dictionary passing | one copy, or one copy for each group of types; a hidden argument holds the operations that depend on the type[^6][^26] | Haskell, Go (with GC shapes) |

**Erasure.** The JLS defines type erasure as a mapping from types, possibly parameterized, to types that are never parameterized types or type variables.[^16] After erasure, a `List<String>` and a `List<Integer>` are the same class at run time.

```java run id=erasure
import java.util.ArrayList;
import java.util.List;

public class Main {
    public static void main(String[] args) {
        List<String> names = new ArrayList<>();
        List<Integer> numbers = new ArrayList<>();
        System.out.println(names.getClass() == numbers.getClass());
        System.out.println(names.getClass().getName());
    }
}
```

```output for=erasure
true
java.util.ArrayList
```

Erasure kept the class files compatible with code that was written before generics. That was the design goal of GJ, the proposal that Java's generics came from.[^10] The price is that a program cannot ask for the type argument at run time. The types that keep their full information at run time are the reifiable types: types that are not generic, raw types, primitive types, parameterized types whose type arguments are all unbounded wildcards (`List<?>`), and arrays of these.[^17] TypeScript erases more. Its compiler removes all type annotations, so they never change what the program does.[^33]

**Specialization at run time.** In .NET, generic types are a part of the runtime. Kennedy and Syme designed generics as a part of the .NET Common Language Runtime.[^13] The runtime makes one specialized type for each value type that is used as a type argument, such as `List<int>`. All reference types share one specialization, because all references have the same size.[^21] The type argument stays available to reflection.

```csharp run id=reified
var names = new List<string>();
var numbers = new List<int>();
Console.WriteLine(names.GetType() == numbers.GetType());
Console.WriteLine(names.GetType().GetGenericArguments()[0].Name);
Console.WriteLine(numbers.GetType().GetGenericArguments()[0].Name);
```

```output for=reified
False
String
Int32
```

**Monomorphization.** Rust fills in the concrete types when it compiles, and turns generic code into specific code. The Rust Book uses `Option<i32>` and `Option<f64>` as the example: the compiled program has two separate enums.[^24] C++ does the same for templates. In the Rust program below, each instantiation of `name_of` knows its own `T`.

```rust run id=mono
use std::any::type_name;

fn name_of<T>(_: &T) -> &'static str {
    type_name::<T>()
}

fn main() {
    println!("{}", name_of(&1u8));
    println!("{}", name_of(&2.5f64));
    println!("{}", name_of(&"text"));
}
```

```output for=mono
u8
f64
&str
```

**Dictionary passing.** Wadler and Blott translated Haskell type classes into code that passes a dictionary of methods as an extra argument.[^6] CLU already shared the code of a parameterized module among all its instantiations, with a small section that holds the actual parameters.[^3] Go uses a mixed scheme, which the design document of its implementation describes. The compiler makes one copy for each GC shape and passes a dictionary as a hidden first argument. Two types have the same GC shape when they have the same underlying type, or when both are pointer types. `int` and `float64` have different shapes, so they get different copies.[^26]

## In languages

The example files show a generic function in each language. The table gives the verdict for each tier 1 language.

| Language | Verdict | Form |
|---|---|---|
| Python | native | `def f[T](...)`, `class C[T]`; checked only by a type checker[^34] |
| JavaScript | n/a | no static types; every function accepts any value |
| TypeScript | native | `<T>` on functions, classes, interfaces and type aliases; erased |
| Java | native | `<T>` on classes, interfaces, methods and constructors; erased |
| C | absent | `void *` with casts, as in `qsort`; `_Generic` for ad hoc selection |
| C++ | native | templates, with concepts as constraints; monomorphized |
| C# | native | `<T>` with `where` constraints; specialized at run time |
| Go | native | `[T constraint]` on functions and types |
| Rust | native | `<T: Trait>`; monomorphized |
| Kotlin | native | `<T>` with declaration-site variance; erased, or `reified` in inline functions |
| PowerShell | stdlib | the generic types and methods of .NET, as in `[List[int]]`[^38] |
| Bash | n/a | no static types |
| SQL | n/a | a query has no user-defined type parameters |
| Regex | n/a | a pattern has no types |
| HTML/CSS | n/a | markup and style rules have no types |

ISO C has no type parameters. A library function that works for any type takes a `void *` and the size of an element, and the caller gives a function that compares two elements. `qsort` is the standard example.[^37] The compiler cannot check that the comparison function and the array agree. C11 added generic selection, `_Generic`, which chooses one expression from a list by the type of a controlling expression. It is ad hoc polymorphism: each type needs its own entry, as in the type-generic macro `cbrt` of the standard.[^36]

## Variations and related ideas

- **Parametric and ad hoc polymorphism.** A parametric function works the same way for all types, like `last` in Idea. An ad hoc polymorphic function has a separate case for each type, as with overloaded operators or `_Generic`.[^1][^36] Type classes and traits join the two. The function is parametric, and the constraint brings in the operations that differ for each type.[^6]
- **Variance.** A generic type can follow the subtyping of its type argument (covariant), reverse it (contravariant), or ignore it (invariant). Java decides at each use, with wildcards such as `List<? extends Number>`.[^15] C# decides at the declaration with `out` and `in`, and only for interfaces and delegates. Variance in .NET applies only to reference types.[^23] Kotlin has both: `out` and `in` at the declaration, and type projections at the use.[^31] The first pitfall shows why a mutable list must be invariant.
- **Generic types and generic functions.** A generic type, such as `Stack<T>`, fixes `T` for the whole object. A generic function, such as `last<T>`, fixes `T` for one call. Go allows type parameters on functions, types and methods, but not on the methods of an interface (see Pitfalls).[^27][^39][^40]
- **Values as parameters.** C++ templates also take values. The size of `std::array<int, 3>` is part of its type (see the C++ example).
- **Generic programming.** Musser and Stepanov described algorithms that are written once for a family of data abstractions, with their requirements stated as properties of the types.[^7] The C++ STL took this approach, and the iterators of the STL connect each algorithm to each container. [[iterators]] describes the iterator protocol.
- **Duck typing.** A dynamically typed language reaches the same reuse without type parameters. A function works for any value that supports the operations that it calls, and an error shows at run time.

## Pitfalls

### A list of a subtype is not a list of the supertype

`Integer` is a subtype of `Number`, but `List<Integer>` is not a subtype of `List<Number>`. In Java, two parameterized types of the same class are in a subtype relation only when the type arguments of the supertype contain the type arguments of the subtype.[^19][^15] If the assignment were allowed, code could add a `Double` through the `List<Number>` and break the `List<Integer>`.

```java fails expect=compile id=java-invariant
import java.util.ArrayList;
import java.util.List;

public class Main {
    public static void main(String[] args) {
        List<Integer> ints = new ArrayList<>(List.of(1, 2));
        List<Number> numbers = ints;
        numbers.add(2.5);
        System.out.println(ints);
    }
}
```

`javac` stops with `incompatible types: List<Integer> cannot be converted to List<Number>`. When the code only reads, the fix is a wildcard. `List<? extends Number>` accepts a list of any subtype of `Number`, and the method reads each element as a `Number`.

```java run id=java-wildcard
import java.util.List;

public class Main {
    static double sum(List<? extends Number> numbers) {
        double total = 0;
        for (Number n : numbers) {
            total += n.doubleValue();
        }
        return total;
    }

    public static void main(String[] args) {
        System.out.println(sum(List.of(1, 2)));
        System.out.println(sum(List.of(0.5, 1.5)));
    }
}
```

```output for=java-wildcard
3.0
2.0
```

C# and Kotlin mark read-only interfaces as covariant at the declaration, so an `IEnumerable<string>` is an `IEnumerable<object>` and a Kotlin `List<Int>` is a `List<Number>` (see the C# and Kotlin examples).[^23][^31]

### A type argument does not exist at run time

Under erasure, a test or a cast against a type argument cannot work. Java rejects an `instanceof` test with a type argument when the compiler cannot prove it safe.

```java fails expect=compile id=java-instanceof
import java.util.List;

public class Main {
    public static void main(String[] args) {
        Object value = List.of(1, 2);
        if (value instanceof List<String> names) {
            System.out.println(names.get(0).length());
        }
    }
}
```

`javac` stops with `Object cannot be safely cast to List<String>`. Kotlin rejects the same kind of check on a type parameter.[^32]

```kotlin fails expect=compile timeout=30 id=kotlin-erased
fun <T> isInstance(value: Any): Boolean = value is T

fun main() {
    println(isInstance<String>(1))
}
```

The Kotlin compiler reports `cannot check for instance of erased type 'T (of fun <T> isInstance)'`. The fix in Java is a test against the reifiable type `List<?>`, then a check of each element. The fix in Kotlin is an `inline` function with a `reified` type parameter, as in the Kotlin example.[^32] In C#, the same test works, because the type argument exists at run time.[^21]

### A raw type pollutes the heap

A raw type is the name of a generic class with no type arguments, such as `List`. Java allows it for compatibility with code that was written before generics.[^18] Through a raw type, code can put a value of the wrong type into a parameterized collection. The JLS calls this heap pollution: a variable of a parameterized type refers to an object that is not of that type.[^20] The error shows later, far from its cause, at the cast that the compiler inserted.

```java fails expect=exception:ClassCastException id=heap-pollution
import java.util.ArrayList;
import java.util.List;

public class Main {
    public static void main(String[] args) {
        List<String> names = new ArrayList<>();
        List raw = names;
        raw.add(42);
        String first = names.get(0);
        System.out.println(first);
    }
}
```

The `add` runs without an error. The exception comes from the line `String first = names.get(0)`. `javac` gives only the note `Main.java uses unchecked or unsafe operations`. The fix is to never use raw types, and to compile with `-Xlint:unchecked`, so that each unchecked operation is a warning with a line number. An unchecked cast such as `(List<String>) obj` has the same risk.

### A TypeScript type argument is a claim, not a check

TypeScript erases its types, so a type argument does not change the program.[^33] A function that returns `T` from untyped data only claims that the data has the type `T`.

```typescript wrong id=ts-claim
interface User {
  name: string;
}

function parse<T>(json: string): T {
  return JSON.parse(json) as T;
}

const user = parse<User>('{"title": "admin"}');
console.log(user.name);
```

```output for=ts-claim
undefined
```

The compiler accepts the program, and `user.name` is `undefined` at run time. The fix is a check at run time: a type guard that tests the shape of the value before the program uses it.

```typescript run id=ts-guard
interface User {
  name: string;
}

function isUser(value: unknown): value is User {
  return typeof value === "object" && value !== null && typeof (value as User).name === "string";
}

const data: unknown = JSON.parse('{"title": "admin"}');
console.log(isUser(data) ? data.name : "not a user");
```

```output for=ts-guard
not a user
```

### An unconstrained template fails inside its body

A C++ template with no constraint accepts any type argument. The compiler checks the operations only when it instantiates the body for that type. The error then points into the body, which can be deep inside a library.

```cpp fails expect=compile id=cpp-unconstrained
#include <print>
#include <vector>

struct Point {
    int x;
    int y;
};

template <typename T>
T max_of(const std::vector<T>& items) {
    T result = items.front();
    for (const T& item : items) {
        if (item > result) {
            result = item;
        }
    }
    return result;
}

int main() {
    Point p = max_of(std::vector<Point>{{1, 2}, {3, 4}});
    std::println("{}", p.x);
}
```

The compiler reports `invalid operands to binary expression ('const Point' and 'Point')` at the line of `item > result`, and then the chain of instantiations that led there. A concept states the requirement at the declaration.[^35] With `template <std::totally_ordered T>`, as in the C++ example, the error points at the call and names the concept that `Point` does not satisfy. The fix for `Point` is an ordering, for example `auto operator<=>(const Point&) const = default;`.

### A generic method does not implement an interface method

A Go method can declare type parameters of its own.[^39] A method of an interface cannot, and a generic method does not implement a method of an interface.[^40][^41] Here `Plain` has a method `Format[T any]`, so it does not satisfy `Formatter`.

```go fails expect=compile id=go-method
package main

import "fmt"

type Formatter interface {
	Format(value int) string
}

type Plain struct{}

func (Plain) Format[T any](value T) string {
	return fmt.Sprint(value)
}

func main() {
	var f Formatter = Plain{}
	fmt.Println(f.Format(42))
}
```

The compiler reports `Plain does not implement Formatter (wrong type for method Format)`. It shows the method that `Plain` has, `Format[T any](T) string`, and the method that the interface wants, `Format(int) string`. The fix is a method with the exact signature of the interface. That method can call a generic function to do the work.

```go run id=go-fix
package main

import "fmt"

type Formatter interface {
	Format(value int) string
}

type Plain struct{}

func format[T any](value T) string {
	return fmt.Sprint(value)
}

func (Plain) Format(value int) string {
	return format(value)
}

func main() {
	var f Formatter = Plain{}
	fmt.Println(f.Format(42))
}
```

```output
42
```

## Cost

- **Monomorphization.** A call of generic code costs the same as a call of code written for the concrete type.[^24] The price is paid at build time and in size. The compiled program holds one copy for each set of type arguments, so many instantiations make a larger binary.
- **Erasure.** One copy of the code serves all type arguments. In Java, a type argument must be a reference type, so a `List<Integer>` holds boxed `Integer` objects, not `int` values.[^15] The compiler inserts casts where the erased code needs them.[^10]
- **Specialization at run time.** .NET shares one copy among all reference types and makes one copy for each value type. A `List<int>` stores `int` values without boxing.[^21]
- **Dictionaries.** Go shares one copy among the types of one GC shape. Operations that depend on the exact type, such as method calls on a value of type `T`, go through the dictionary.[^26] Haskell type classes pass a dictionary of methods in the same way.[^6]
- **Type checking.** A constraint lets the compiler check the body of a generic definition once.[^25] The body of an unconstrained C++ template is checked for each instantiation (see Pitfalls).

## History

- **1967.** Christopher Strachey named two kinds of polymorphism, ad hoc and parametric, in his lecture notes for a summer school in Copenhagen. His example of the parametric kind was a function `Map` of the type `(α ⇒ β, α list) ⇒ β list`.[^1]
- **1974.** John Reynolds described a typed lambda calculus with abstraction over types, a formal base for parametric polymorphism.[^4]
- **1977.** CLU, by Barbara Liskov and her group, let clusters and procedures take types as parameters, with `where` clauses as constraints. The compiler checked each use, and the implementation shared the code among instantiations.[^2][^3]
- **1978.** Robin Milner gave a polymorphic type discipline for ML, with types inferred by the compiler.[^5]
- **1989.** Philip Wadler and Stephen Blott described type classes, for Haskell.[^6] David Musser and Alexander Stepanov published "Generic programming".[^7]
- **1990 and 1991.** The C++ committee accepted templates in 1990. AT&T Cfront Release 3.0, in October 1991, was the first release of Cfront that supported templates.[^8]
- **1994.** The C++ committee accepted the STL, by Alexander Stepanov, at its meeting in San Diego.[^9]
- **1998.** Gilad Bracha, Martin Odersky, David Stoutamire and Philip Wadler described GJ, generics for Java by erasure.[^10]
- **2001.** Andrew Kennedy and Don Syme described the design of generics for the .NET Common Language Runtime.[^13]
- **2004.** JSR 14 reached its final release on 30 September 2004, and J2SE 5.0 added generics to Java.[^11][^12]
- **2005 and 2010.** C# 2.0 added generics in November 2005. C# 4.0 added covariance and contravariance of generic interfaces and delegates in April 2010.[^14]
- **2022.** Go 1.18, released on 15 March 2022, added type parameters.[^29][^30]
- **2026.** Go 1.27, released on 19 August 2026, let a method declare its own type parameters.[^41][^30]

## References
