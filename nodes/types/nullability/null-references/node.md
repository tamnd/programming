---
tag: NEW-null-references
kind: concept
slug: null-references
name: Null reference
aliases: [null pointer, null, nil, None, nullable type, null safety]
area: types/nullability
level: core
summary: >-
  A null reference is a reference value that refers to no object. It marks a missing value. In many languages every reference type includes it, so the type does not show if a value can be absent.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1965
  people: [C. A. R. Hoare]
  note: Hoare's proposal "Record Handling" added the reference value null, which refers to no record, for relationships that do not always yield a value.
see_also: [pointers-and-references, exception-handling, static-and-dynamic-typing, parametric-polymorphism, subtyping]
same_as:
  wikidata: Q19848926
  wikipedia: Null_pointer
sources:
  - url: http://archive.computerhistory.org/resources/text/algol/algol_bulletin/A21/P36.HTM
    title: C. A. R. Hoare, Record Handling, ALGOL Bulletin 21, AB21.3.6, pages 39 to 69 (scanned pages at the Computer History Museum)
    locator: AB21 p.43, 2.6 Other operations defined for references (the reference value null)
  - url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-4.html#jls-4.1
    title: The Java Language Specification, Java SE 25 Edition, chapter 4
    locator: 4.1 The Kinds of Types and Values (the null type and the null reference)
  - url: https://kotlinlang.org/docs/null-safety.html
    title: Kotlin documentation, Null safety
    locator: the introduction, and the sections Nullable types and non-nullable types, Check for null with the if conditional, Safe call operator, Elvis operator and Not-null assertion operator
  - url: https://port70.net/~nsz/c/c11/n1570.html#6.3.2.3p3
    title: "ISO/IEC 9899:201x, committee draft N1570 (C11)"
    locator: 6.3.2.3 Pointers, paragraphs 3 and 4
  - url: https://timsong-cpp.github.io/cppwp/n4950/lex.nullptr
    title: Working Draft, Standard for Programming Language C++, N4950
    locator: "[lex.nullptr] paragraph 1"
  - url: https://timsong-cpp.github.io/cppwp/n4950/conv.ptr
    title: Working Draft, Standard for Programming Language C++, N4950
    locator: "[conv.ptr] paragraph 1"
  - url: https://go.dev/ref/spec#The_zero_value
    title: The Go Programming Language Specification
    locator: The zero value
  - url: https://tc39.es/ecma262/2026/#sec-ecmascript-language-types-null-type
    title: ECMAScript 2026 Language Specification (ECMA-262)
    locator: 6.1.1 The Undefined Type, 6.1.2 The Null Type and 4.4.15 null value
  - url: https://docs.python.org/3.14/reference/datamodel.html#none
    title: The Python Language Reference, 3.14, Data model
    locator: 3.2.1 None
  - url: https://port70.net/~nsz/c/c11/n1570.html#6.5.3.2p4
    title: "ISO/IEC 9899:201x, committee draft N1570 (C11)"
    locator: 6.5.3.2 Address and indirection operators, paragraph 4 and footnote 102
  - url: https://timsong-cpp.github.io/cppwp/n4950/dcl.ref
    title: Working Draft, Standard for Programming Language C++, N4950
    locator: "[dcl.ref] paragraph 5 and note 2"
  - url: https://go.dev/ref/spec#Address_operators
    title: The Go Programming Language Specification
    locator: Address operators, and Selectors (a nil pointer and a struct field)
  - url: https://docs.oracle.com/javase/specs/jvms/se25/html/jvms-6.html#jvms-6.5.getfield
    title: The Java Virtual Machine Specification, Java SE 25 Edition, chapter 6
    locator: 6.5 getfield, Run-time Exception
  - url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-15.html#jls-15.12.4.4
    title: The Java Language Specification, Java SE 25 Edition, chapter 15
    locator: 15.12.4.4 Locate Method to Invoke
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.nullreferenceexception
    title: .NET API reference, NullReferenceException Class
    locator: the class summary
  - url: https://tc39.es/ecma262/2026/#sec-toobject
    title: ECMAScript 2026 Language Specification (ECMA-262)
    locator: 7.1.18 ToObject, and 6.2.5.5 GetValue
  - url: https://smlfamily.github.io/Basis/option.html
    title: The Standard ML Basis Library, The Option structure
    locator: "Interface (datatype 'a option = NONE | SOME of 'a)"
  - url: https://www.haskell.org/onlinereport/haskell2010/haskellch21.html#x29-25600021.1
    title: Haskell 2010 Language Report, chapter 21 Data.Maybe
    locator: 21.1 The Maybe type and operations
  - url: https://doc.rust-lang.org/reference/behavior-considered-undefined.html#r-undefined.validity.reference-box
    title: The Rust Reference, Behavior considered undefined
    locator: undefined.validity.reference-box
  - url: https://doc.rust-lang.org/std/option/index.html#representation
    title: "The Rust Standard Library, Module std::option"
    locator: Representation
  - url: https://docs.swift.org/swift-book/documentation/the-swift-programming-language/thebasics#Optionals
    title: The Swift Programming Language, The Basics
    locator: Optionals, nil, Optional Binding and Force Unwrapping
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/Optional.html
    title: "Java SE 25 API, Class Optional<T>"
    locator: "the class description, the API Note and Since: 1.8"
  - url: https://timsong-cpp.github.io/cppwp/n4950/optional.optional.general
    title: Working Draft, Standard for Programming Language C++, N4950
    locator: "[optional.optional.general] paragraph 1"
  - url: https://docs.python.org/3.14/library/typing.html#typing.Optional
    title: The Python Standard Library, 3.14, typing
    locator: typing.Optional
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/nullable-references
    title: "C# documentation, Nullable reference types"
    locator: the introduction, Nullable context and Null-state analysis
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/compiler-messages/nullable-warnings
    title: "C# language reference, Resolve nullable warnings"
    locator: the list of warnings (CS8600, CS8602)
  - url: https://www.typescriptlang.org/tsconfig/#strictNullChecks
    title: TypeScript, TSConfig Reference
    locator: strictNullChecks
  - url: https://www.typescriptlang.org/docs/handbook/2/everyday-types.html#null-and-undefined
    title: TypeScript Handbook, Everyday Types
    locator: "null and undefined, and Non-null Assertion Operator (Postfix !)"
  - url: https://tc39.es/ecma262/2026/#sec-optional-chaining-evaluation
    title: ECMAScript 2026 Language Specification (ECMA-262)
    locator: "13.3.9.1 Runtime Semantics: Evaluation (OptionalExpression)"
  - url: https://tc39.es/ecma262/2026/#sec-binary-logical-operators-runtime-semantics-evaluation
    title: ECMAScript 2026 Language Specification (ECMA-262)
    locator: "13.13.1 Runtime Semantics: Evaluation (CoalesceExpression)"
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/operators/member-access-operators#null-conditional-operators--and-
    title: "C# language reference, Member access operators"
    locator: "Null-conditional operators ?. and ?[]"
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/operators/null-coalescing-operator
    title: "C# language reference, ?? and ??= operators"
    locator: the introduction
  - url: https://learn.microsoft.com/en-us/powershell/module/microsoft.powershell.core/about/about_automatic_variables#null
    title: PowerShell, about_Automatic_Variables
    locator: $null
  - url: https://tc39.es/ecma262/2026/#sec-islooselyequal
    title: ECMAScript 2026 Language Specification (ECMA-262)
    locator: 7.2.13 IsLooselyEqual, steps for null and undefined
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.nullable-1
    title: ".NET API reference, Nullable<T> Struct"
    locator: the struct summary and declaration
  - url: https://docs.python.org/3.14/reference/simple_stmts.html#the-return-statement
    title: The Python Language Reference, 3.14, Simple statements
    locator: 7.6 The return statement
  - url: https://go.dev/ref/spec#Map_types
    title: The Go Programming Language Specification
    locator: Map types (a nil map is equivalent to an empty map except that no elements may be added)
  - url: https://learn.microsoft.com/en-us/powershell/scripting/learn/deep-dives/everything-about-null
    title: PowerShell, Everything you wanted to know about $null
    locator: Impact of $null, Method on a null-valued expression, and Checking for $null
  - url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-5.html#jls-5.1.8
    title: The Java Language Specification, Java SE 25 Edition, chapter 5
    locator: 5.1.8 Unboxing Conversion
  - url: https://openjdk.org/jeps/358
    title: "JEP 358: Helpful NullPointerExceptions"
    locator: Summary, Non-Goals and the Release field (14)
  - url: https://go.dev/doc/faq#nil_error
    title: Frequently Asked Questions (FAQ), The Go Programming Language
    locator: Why is my nil error value not equal to nil?
  - url: https://go.dev/ref/spec#Index_expressions
    title: The Go Programming Language Specification
    locator: "Index expressions (for a of map type: a nil map, and assignment to an element of a nil map)"
  - url: https://tc39.es/ecma262/2026/#sec-typeof-operator-runtime-semantics-evaluation
    title: ECMAScript 2026 Language Specification (ECMA-262)
    locator: "13.5.3.1 Runtime Semantics: Evaluation (the typeof operator, the case of null)"
  - url: https://doc.rust-lang.org/std/option/enum.Option.html#method.unwrap
    title: The Rust Standard Library, Enum Option
    locator: "Option::unwrap (the Panics section and the stability mark 1.0.0)"
  - url: https://timsong-cpp.github.io/cppwp/n4950/optional.observe
    title: Working Draft, Standard for Programming Language C++, N4950
    locator: "[optional.observe] paragraphs 1, 4 and 13"
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/Optional.html#orElseThrow()
    title: "Java SE 25 API, Class Optional<T>"
    locator: orElseThrow()
  - url: https://softwarepreservation.computerhistory.org/ALGOL/standards.html#Hoare_1965
    title: Computer History Museum, Software Preservation Group, ALGOL standards
    locator: the entry C.A.R. Hoare, Record Handling (ALGOL Bulletin Number 21, November 1965, pages 39-69)
  - url: https://openjdk.org/projects/jdk8/
    title: OpenJDK, JDK 8
    locator: the project description (General Availability on 18 March 2014)
  - url: https://blog.rust-lang.org/2015/05/15/Rust-1.0/
    title: Announcing Rust 1.0, The Rust Blog
    locator: the post header (May 15, 2015)
  - url: https://devblogs.microsoft.com/typescript/announcing-typescript-2-0/
    title: "TypeScript 2.0 is now available!, TypeScript blog"
    locator: the post header (September 22nd, 2016) and the section on null- and undefined-aware types
  - url: https://www.typescriptlang.org/docs/handbook/release-notes/typescript-2-0.html#null--and-undefined-aware-types
    title: TypeScript 2.0 release notes
    locator: Null- and undefined-aware types, --strictNullChecks
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/whats-new/csharp-version-history#c-version-80
    title: "The history of C#"
    locator: "C# version 8.0 (Released September 2019, Nullable reference types)"
  - url: https://openjdk.org/projects/jdk/14/
    title: OpenJDK, JDK 14
    locator: the project description (General Availability on 17 March 2020) and the Features list (358)
---

## Idea

A reference names an object in memory. A program often needs a reference that names no object. Examples are the end of a linked list, a key that is not in a map, and a field that has no value yet. A null reference fills this role. It is one special value of a reference type, and it refers to no object.

C. A. R. Hoare proposed this value in 1965, in a paper on records for an extension of ALGOL 60. He added `null` so that a reference field could represent a relationship that has no value for some records. His paper then says: "This value fails to refer to a record, and any attempt to use it to refer to a record leads to an undefined result."[^1]

The second half of that sentence is the problem. In Java, the null reference can be assigned to every reference type.[^2] A variable of type `String` holds a string or `null`, and the type does not tell which. The compiler accepts `name.length()` in both cases. The error comes at run time, at the place where the program uses the value, and not at the place where the `null` came from.

```java fails expect=exception:NullPointerException id=idea
import java.util.Map;

public class Main {
    static final Map<String, String> EMAILS = Map.of("ada", "ada@example.org");

    static String email(String name) {
        return EMAILS.get(name);
    }

    public static void main(String[] args) {
        System.out.println(email("ada").length());
        System.out.println(email("bob").length());
    }
}
```

The program prints `15` and then stops with `java.lang.NullPointerException: Cannot invoke "String.length()" because the return value of "Main.email(String)" is null`. The method `email` returned `null` for a missing key, and its return type `String` did not show that it could.

The Kotlin documentation calls null references "The Billion-Dollar Mistake".[^3] Languages use two families of fixes. An option type puts the absence into the type, as `Option<String>` or `Maybe String`. The program must handle the empty case before it can use the value. Null safety keeps `null` but splits each reference type in two, such as `String` and `String?`. The compiler rejects a member access on the nullable form. Both fixes move the check from run time to compile time.

## How it works

### The null value

In C, an integer constant expression with the value 0, or such an expression cast to `void *`, is a null pointer constant. Conversion of a null pointer constant to a pointer type gives a null pointer. A null pointer compares unequal to a pointer to any object or function, and any two null pointers compare equal.[^4] C++ adds the keyword `nullptr`, a literal of type `std::nullptr_t` that is a null pointer constant.[^5] Its null pointer value is distinct from every pointer to an object or a function.[^6]

Java gives the null reference a type of its own. The null type has no name, and the null reference is its only value. The null reference can be assigned or cast to any reference type.[^2] Go uses the predeclared `nil` as the zero value of pointers, functions, interfaces, slices, channels and maps.[^7] JavaScript has two values for absence. `null` is the only value of the Null type, and it represents the intentional absence of any object value. `undefined` is the only value of the Undefined type.[^8] Python has the object `None`, the single value of its type. Python uses it for the absence of a value, and a function that does not return a value explicitly returns `None`.[^9]

### What happens on use

A null reference is safe to store, copy and compare. The danger is an access through it: a field read, a method call or an index.

In C, the unary `*` operator on an invalid pointer value has undefined behaviour, and a footnote names the null pointer as one such value.[^10] The language does not check. On the test machine, the program below stopped with a fault. AddressSanitizer reported it as a `SEGV` on the address `0x000000000000`, and added the hint that the address points to the zero page.

```c fails expect=sanitizer:address id=c-null
#include <stdio.h>

struct point {
    int x;
    int y;
};

int main(void) {
    struct point *p = NULL;
    printf("before\n");
    printf("%d\n", p->x);
    return 0;
}
```

C++ forbids the null reference outright. A reference must be initialized to refer to a valid object or function. A note in the standard says that a null reference cannot exist in a well-defined program. The only way to make one is indirection through a null pointer, which is undefined behaviour.[^11] Pointers in C++ still have the null value, with the same rules as in C.

Managed languages check every access and turn the fault into an error that the program can see. In Go, `*x` on a `nil` pointer, and a field access through a `nil` pointer, cause a run-time panic.[^12]

```go fails expect=panic id=go-nil
package main

import "fmt"

type Point struct {
	X, Y int
}

func main() {
	var p *Point
	fmt.Println("before")
	fmt.Println(p.X)
}
```

The program prints `before`, and then the runtime stops it with `panic: runtime error: invalid memory address or nil pointer dereference`. On the test machine, the next line was `[signal SIGSEGV: segmentation violation code=0x2 addr=0x0 pc=...]`. The runtime caught the memory fault at address 0 and turned it into a panic.

The Java Virtual Machine throws a `NullPointerException` when the object reference of a `getfield` instruction is `null`.[^13] The Java language throws it when the target of a method invocation is `null`.[^14] .NET throws `NullReferenceException` when there is an attempt to dereference a null object reference.[^15] In JavaScript, a property access on `null` or `undefined` throws a `TypeError`. The property access calls ToObject on the base value, and ToObject throws for these two values.[^16] Python raises an `AttributeError` when a program reads an attribute of `None`, because `None` is an ordinary object without the attribute (see Pitfalls).

### Absence in the type: option types

An option type is a separate type for "a value or nothing". Standard ML defines `datatype 'a option = NONE | SOME of 'a`.[^17] Haskell defines `data Maybe a = Nothing | Just a`. A value of type `Maybe a` either contains a value of type `a`, as `Just a`, or is empty, as `Nothing`.[^18] A function that may find no result returns `Maybe a`, not `a`. The caller cannot use the result as an `a` before it matches the two cases.

Rust has no null references in safe code. A reference or a `Box<T>` must be non-null, and a null one is undefined behaviour.[^19] A value that may be absent has the type `Option<T>`. For some types `T`, Rust guarantees that `Option<T>` has the same size, alignment and call ABI as `T`. These types include references, `Box`, function pointers, `NonNull` pointers and the `NonZero` integer types. The compiler uses the all-zero bit pattern, which these types cannot have, for `None`. The documentation calls this the null pointer optimization.[^20] The Rust example shows that `Option<&i32>` and `&i32` have the same size.

Swift writes an optional type with a question mark: the type of an optional `Int` is `Int?`. An optional contains a value of the type or no value, and the special value `nil` sets the valueless state. Assigning `nil` to a variable that is not optional is a compile-time error. Optional binding in an `if`, `guard` or `while` statement checks for a value and makes it available in one step. Force unwrapping with a postfix `!` gives the value, and on `nil` it triggers a run-time error.[^21]

Some languages that have `null` also have an option type in the library. Java has `java.util.Optional<T>`, a container that may or may not hold a non-null value. Its documentation says that it is primarily for method return types. It also says that a variable of type `Optional` should never itself be `null`.[^22] C++ has `std::optional<T>`. It stores the contained value within the storage of the `optional` object and may not use dynamic memory for it.[^23] In Python, `Optional[X]` is the same as `X | None`. It is a hint for a type checker, and the interpreter does not check it.[^24]

### Absence in the type: null safety

Null safety keeps `null` in the language but gives each reference type two forms. The compiler then tracks which form each expression has.

In Kotlin, a variable of type `String` cannot hold `null`, and a variable of type `String?` can. The compiler reports an error when the code accesses `length` directly on a `String?`. After a test such as `if (b != null)`, the compiler smart casts `b` to `String`. The safe call operator `?.` returns `null` when the receiver is `null`, and the Elvis operator `?:` gives an alternative value. The not-null assertion `!!` converts a value to a non-nullable type and throws a `NullPointerException` if the value is `null`. The documentation lists the remaining causes of a `NullPointerException`. They are an explicit `throw`, the `!!` operator, data that is not consistent during initialization, and Java code.[^3]

C# has nullable reference types. In a nullable context, `string?` is a reference that may be `null`, and `string` is a reference that should not be `null`. The compiler runs a null-state analysis and gives warnings. The feature is entirely a compile-time feature, and the run-time behaviour of the program does not change.[^25] A typical warning is CS8602, "Dereference of a possibly null reference".[^26]

TypeScript has the compiler option `strictNullChecks`. When it is off, the language ignores `null` and `undefined`, which can then appear in place of any type. When it is on, `null` and `undefined` have their own types. The checker then reports an error where the code uses them in place of a concrete value.[^27] The checker narrows a union such as `string | null` after a test. The postfix `!` operator removes `null` and `undefined` from a type without any check at run time.[^28]

### Null-aware operators

Several languages have operators that test for `null` as part of an expression. In JavaScript, an optional chain `a?.b` evaluates to `undefined` when `a` is `null` or `undefined`.[^29] The coalescing operator `a ?? b` returns `a` unless it is `undefined` or `null`.[^30] C# has `?.` and `?[]`, which return `null` for a `null` operand.[^31] It also has `??`, which returns its left operand when that operand is not `null`, and `??=`.[^32] Kotlin has `?.` and `?:`.[^3] These operators make a check shorter, but they do not make it required. A plain `.` on a nullable value still compiles in JavaScript and in C#.

## In languages

The example files show how each language marks a value that may be absent. The table gives the verdict for each tier 1 language.

| Language | Verdict | Form |
|---|---|---|
| Python | native | `None`; `Optional[X]` is a hint for a type checker |
| JavaScript | native | `null` and `undefined`; `?.` and `??` |
| TypeScript | native | `null` and `undefined` as types of their own under `strictNullChecks` |
| Java | native | `null` in every reference type; `Optional<T>` in the library |
| C | native | the null pointer, written `NULL` or `0` |
| C++ | native | `nullptr` for pointers; references cannot be null; `std::optional<T>` |
| C# | native | `null`; nullable annotations with warnings; `Nullable<T>` for value types |
| Go | native | `nil` for pointers, functions, interfaces, slices, channels and maps |
| Rust | stdlib | no null references in safe code; `Option<T>` |
| Kotlin | native | `T?` separate from `T`, checked by the compiler |
| PowerShell | native | the automatic variable `$null`[^33] |
| Bash | n/a | variables hold strings, not references |
| SQL | n/a | `NULL` marks a missing value in a column; it is not a reference |
| Regex | n/a | a pattern has no references |
| HTML/CSS | n/a | markup and style rules have no references |

SQL also uses the word `NULL`, for a different idea. A `NULL` in a column marks a missing value, and a comparison with it gives an unknown result. A query does not dereference anything, so the concept of this page does not apply.

## Variations and related ideas

- **Two kinds of absence.** JavaScript has both `null` and `undefined`. A property that does not exist reads as `undefined`, and `null == undefined` is `true`.[^34] A test `x == null` therefore matches both values.
- **Nullable value types.** In C#, `int?` is `Nullable<int>`, a value type that can be assigned `null`.[^35] It is a struct with `HasValue` and `Value`, and not an annotation like `string?`.
- **A missing return value.** In Python, a `return` statement without an expression returns `None`, and so does the end of a function body.[^36] A function can thus return `None` on one path by accident (see Pitfalls).
- **Methods on nil.** In Go, a method with a pointer receiver can run with a `nil` receiver. A `nil` map reads as an empty map, and a `nil` slice works with `len` and `append`.[^37] The Go example uses a `nil` pointer as the empty list.
- **Null in collections.** In PowerShell, `$null` counts as an element of a collection, and it becomes an empty string inside a string.[^33][^38]
- **Related concepts.** A null reference is a value of a reference or pointer type ([[pointers-and-references]]). An access through it raises an error in managed languages ([[exception-handling]]). Option types are generic types ([[parametric-polymorphism]]). Null safety is a static check ([[static-and-dynamic-typing]]).

## Pitfalls

### Unboxing a null wrapper

Java converts between `int` and `Integer` without a visible cast. If the `Integer` is `null`, the unboxing conversion throws a `NullPointerException`.[^39] The line that fails can look like arithmetic.

```java fails expect=exception:NullPointerException id=java-unbox
import java.util.HashMap;
import java.util.Map;

public class Main {
    public static void main(String[] args) {
        Map<String, Integer> stock = new HashMap<>();
        stock.put("apple", 3);
        int apples = stock.get("apple");
        int pears = stock.get("pear");
        System.out.println(apples + pears);
    }
}
```

The message is `java.lang.NullPointerException: Cannot invoke "java.lang.Integer.intValue()" because the return value of "java.util.Map.get(Object)" is null`. The JVM computes such a message to describe precisely which variable was `null`.[^40] The fix is `stock.getOrDefault("pear", 0)`, or a variable of type `Integer` with a test.

### A nil pointer in an interface is not nil

In Go, an interface value holds a type and a value. It is `nil` only when both are unset. A `nil` pointer stored in an interface has a type, so the interface is not `nil`.[^41] A function that returns a `nil` `*MyError` as an `error` therefore returns an error.

```go wrong id=go-nil-interface
package main

import "fmt"

type MyError struct{}

func (e *MyError) Error() string { return "my error" }

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
}
```

```output
false
```

The program prints `false`, although `check` had no error to report. The fix is to return the literal `nil` on the success path, and a value of type `error` only when there is an error.

### A write to a nil map

A `nil` map reads as an empty map, but no elements may be added to it. An assignment to an element of a `nil` map causes a run-time panic.[^37][^42]

```go fails expect=panic id=go-nil-map
package main

import "fmt"

func main() {
	var counts map[string]int
	fmt.Println(counts["a"])
	counts["a"]++
	fmt.Println(counts)
}
```

The panic message is `panic: assignment to entry in nil map`. The fix is to create the map with `make(map[string]int)` or a literal before the first write.

### typeof null is "object"

In JavaScript, `typeof null` evaluates to `"object"`.[^43] A test with `typeof` for an object therefore also accepts `null`, and the next property access fails.

```javascript wrong id=js-typeof
function describe(value) {
  if (typeof value === "object") {
    return "object with " + Object.keys(value ?? {}).length + " keys";
  }
  return typeof value;
}

console.log(describe({ a: 1 }));
console.log(describe(null));
```

```output
object with 1 keys
object with 0 keys
```

The second line says that `null` is an object with 0 keys. The fix is the test `value !== null && typeof value === "object"`. A property access on `null` throws a `TypeError`:

```javascript fails expect=exception:TypeError id=js-null-access
const config = { server: null };
console.log(config.server.port);
```

The message is `TypeError: Cannot read properties of null (reading 'port')`.

### Nullable warnings do not stop the program

The C# nullable analysis only gives warnings, and the program keeps the same run-time behaviour.[^25] A warning that a build ignores becomes a `NullReferenceException` at run time.

```csharp fails expect=exception:NullReferenceException id=cs-warning
#nullable enable

string? Find(string key) => key == "ada" ? "Ada Lovelace" : null;

string name = Find("bob");
Console.WriteLine(name.Length);
```

The compiler gives the warning CS8600 for the assignment of a possibly null value to `string`, and CS8602 for `name.Length`. The build succeeds. The run then fails with `System.NullReferenceException: Object reference not set to an instance of an object.` The fix is to keep the type `string?` and handle the `null` case. A project can also treat the nullable warnings as errors.

### The not-null assertion throws

Kotlin's `!!` tells the compiler that a value is not `null`. If the value is `null`, `!!` throws a `NullPointerException`.[^3] Each `!!` moves a null check from compile time back to run time.

```kotlin fails expect=exception:NullPointerException timeout=30 id=kotlin-bang
fun main() {
    val ages = mapOf("ada" to 36)
    val age: Int = ages["bob"]!!
    println(age)
}
```

The run stops with `java.lang.NullPointerException`, with no message and the line number of the `!!`. The fix is `ages["bob"] ?: 0`, or a test with `if`.

### A function that returns None on one path

A Python function returns `None` when control reaches the end of its body.[^36] If one branch forgets its `return`, the caller gets `None`, and the error appears later, at the first use.

```python fails expect=exception:AttributeError id=py-none
def title(name):
    if name:
        return name.title()


names = ["ada lovelace", ""]
for n in names:
    print(title(n).upper())
```

The program prints `ADA LOVELACE`, and then stops with `AttributeError: 'NoneType' object has no attribute 'upper'`. A type checker reports this when the function has the return annotation `-> str`. The fix is an explicit `return` on every path.

### Unwrapping an empty option

An option type helps only when the program handles the empty case. Rust's `unwrap` returns the value in `Some` and panics on `None`. Its documentation discourages it for this reason.[^44]

```rust fails expect=panic id=rust-unwrap
use std::collections::HashMap;

fn main() {
    let ages: HashMap<&str, i32> = HashMap::from([("ada", 36)]);
    let age = ages.get("bob").unwrap();
    println!("{age}");
}
```

The panic message is ``called `Option::unwrap()` on a `None` value``. In C++, `value()` on an empty `std::optional` throws `std::bad_optional_access`. The operators `*` and `->` have the precondition that the `optional` contains a value, so on an empty `optional` they are undefined behaviour.[^45] Java `Optional.orElseThrow()` throws `NoSuchElementException`.[^46] The fix in each language is a `match`, a test or a default value.

## Cost

- **A test.** A check against `null` compares one value. Null-aware operators such as `?.` and `??` do the same comparison.
- **Detection.** In C, the language does not check an access through a null pointer.[^10] Managed runtimes detect it and raise an exception, as the Java and Go programs above show.
- **Option types.** Rust stores `Option<&T>` and `Option<Box<T>>` in the same size as the reference, so the option costs no memory.[^20] C++ `std::optional<T>` stores the value in place, without dynamic memory.[^23] Java `Optional` is a class, so an `Optional` result is a reference to an object.[^22]
- **Nullable value types.** C# `Nullable<T>` is a struct, so an `int?` needs no heap object.[^35]
- **Null safety by annotation.** C# nullable reference types exist only at compile time.[^25] TypeScript types also do not exist at run time, so `strictNullChecks` adds no cost to the program.

## History

- **1965.** C. A. R. Hoare proposed records and references for an extension of ALGOL 60 in "Record Handling", ALGOL Bulletin 21, November 1965. The proposal added the reference value `null` for relationships that do not always yield a value.[^1][^47]
- **2014.** JDK 8, the reference implementation of Java SE 8, reached General Availability on 18 March 2014.[^48] `java.util.Optional` dates from Java 1.8.[^22]
- **2015.** Rust 1.0 was released on 15 May 2015.[^49] `Option::unwrap` has been stable since Rust 1.0.0.[^44]
- **2016.** TypeScript 2.0, released on 22 September 2016, added the `strictNullChecks` mode and the types `null` and `undefined`.[^50][^51]
- **2019.** C# 8.0, released in September 2019, added nullable reference types.[^52]
- **2020.** JDK 14 reached General Availability on 17 March 2020. It included JEP 358, Helpful NullPointerExceptions.[^53][^40]

## References
