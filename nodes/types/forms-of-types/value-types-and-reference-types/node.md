---
tag: CAMQ1D
kind: concept
slug: value-types-and-reference-types
name: Value types and reference types
aliases: [value type, reference type, value semantics, reference semantics, struct versus class, primitive types and reference types, call by sharing]
area: types/forms-of-types
level: core
summary: >-
  A variable of a value type holds the value, so assignment copies it. A variable of a reference type holds a reference, so assignment shares one object between two names.
applies_to: core-languages
viz: optional
formal: none
see_also: [pointers-and-references, value-equality-and-identity, parameter-passing, immutability, call-stack-and-heap, composite-type, top-type, in-place-and-copying-operations]
same_as:
  wikidata: Q59627259
  wikipedia: Value_type_and_reference_type
sources:
- url: https://en.wikipedia.org/wiki/Value_type_and_reference_type
  title: Value type and reference type, Wikipedia
  locator: Introduction; Properties of value types and reference types; Reference types and call by sharing; Reference types vs. explicit pointers
- url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/builtin-types/value-types
  title: Value types, C# language reference
  locator: Introduction
- url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-4.html
  title: The Java Language Specification, Java SE 25 Edition, Chapter 4
  locator: Introduction; 4.3.1 Objects
- url: https://developer.mozilla.org/en-US/docs/Glossary/Primitive
  title: Primitive, MDN Web Docs Glossary
  locator: Whole entry
- url: https://docs.python.org/3.14/reference/datamodel.html
  title: Data model, The Python Language Reference 3.14
  locator: 3.1 Objects, values and types
- url: https://go.dev/doc/faq
  title: Frequently Asked Questions, The Go Programming Language
  locator: When are function parameters passed by value?
- url: https://doc.rust-lang.org/std/marker/trait.Copy.html
  title: Trait Copy, The Rust Standard Library
  locator: Description
- url: https://en.cppreference.com/w/cpp/language/reference.html
  title: Reference declaration, C++ reference, cppreference.com
  locator: Introduction; Explanation
- url: https://openjdk.org/jeps/401
  title: JEP 401, Value Objects (Preview), OpenJDK
  locator: Summary; Goals; Non-Goals
- url: https://learn.microsoft.com/en-us/archive/blogs/ericlippert/the-stack-is-an-implementation-detail-part-one
  title: The Stack Is An Implementation Detail, Part One, Eric Lippert, 2009
  locator: Whole post
- url: https://learn.microsoft.com/en-us/dotnet/csharp/programming-guide/types/boxing-and-unboxing
  title: Boxing and Unboxing, C# programming guide
  locator: Boxing
---

## Idea

When a program assigns one variable to another, it can copy the value, or it can copy a reference to one shared object. A value type gives the first behavior and a reference type gives the second. The difference decides what a change through one variable does to the other, what `==` compares, and what a function can change in its caller. C# has both kinds side by side: a `struct` is a value type and a `class` is a reference type.

```csharp run id=idea
using System;

var s1 = new PointS { X = 1 };
var s2 = s1; // copies the whole struct
s2.X = 9;

var c1 = new PointC { X = 1 };
var c2 = c1; // copies the reference
c2.X = 9;

Console.WriteLine($"struct: {s1.X} {s2.X}");
Console.WriteLine($"class: {c1.X} {c2.X}");
Console.WriteLine($"{s1.Equals(new PointS { X = 1 })} {c1.Equals(new PointC { X = 9 })} {ReferenceEquals(c1, c2)}");

struct PointS
{
    public int X;
}

class PointC
{
    public int X;
}
```

```output for=idea
struct: 1 9
class: 9 9
True False True
```

`PointS` and `PointC` have the same field. The assignment `s2 = s1` copies the struct, so the change to `s2` leaves `s1` as it was. The assignment `c2 = c1` copies the reference, so `c1` and `c2` name one object, and the change shows through both. The default `Equals` of a struct compares the fields, and the default `Equals` of a class compares identity, so a second object with the same field is not equal to `c1`.

## How it works

**What a variable holds.** "In certain computer programming languages, data types are classified as either value types or reference types, where values referenced by reference type variables are always implicitly accessed via references stored in those variables, whereas value type variables directly contain the values themselves"[^1]. In C#, "A variable of a value type contains an instance of the type. This behavior differs from a variable of a reference type, which contains a reference to an instance of the type"[^2].

**Copying.** "Assigning to a variable of reference type simply copies the reference, whereas assigning to a variable of value type copies the value"[^1]. The same rule holds for a call: "parameters of reference type are copies of the reference, whereas parameters of value type are copies of the value"[^1]. So "If a reference type is mutable, then mutations made via one reference are visible via any other"[^1].

**Identity and equality.** "Reference types support the notion of identity"[^1]: two references can point to one object or to two objects with equal contents, and the language can tell these cases apart. A value of a value type has no identity of its own, so equality compares the contents. See [value equality and identity](/value-equality-and-identity/).

**Call by sharing.** Java passes every argument by value, and C# does so by default, but the value of a reference type is a reference, so a function that changes the object changes it for the caller too. "This behavior is sometimes called call by sharing"[^1]. Call by sharing is not call by reference: when a function assigns another object to its parameter, "this assignment will not be visible to the caller, because the caller and the function have separate references, even though both references initially point to the same object"[^1]. See [parameter passing](/parameter-passing/).

**How languages split the types.** In Java, "There are, correspondingly, two kinds of data values that can be stored in variables, passed as arguments, returned by methods, and operated on: primitive values (§4.2) and reference values (§4.3)"[^3], and "The reference values (often just references) are pointers to these objects, and a special null reference, which refers to no object"[^3]. In JavaScript, "a primitive (primitive value, primitive data type) is data that is not an object and has no methods or properties"[^4], and objects are shared by reference. In Python, every value is an object, and "Every object has an identity, a type and a value"[^5], so every variable holds a reference; immutable objects such as numbers, strings and tuples act like values because no one can change them.

**Values with explicit pointers.** C, C++, Go and Rust treat every type as a value type and make sharing explicit with pointers or references. In Go, "everything in Go is passed by value", and "passing a pointer value makes a copy of the pointer, but not the data it points to"[^6]. But "Map and slice values behave like pointers: they are descriptors that contain pointers to the underlying map or slice data"[^6]. In Rust, a type that implements `Copy` is one whose values "can be duplicated simply by copying bits"[^7], and other types move on assignment. In C++, a reference is "an alias to an already-existing object or function"[^8], and once bound it cannot be bound to another object, so it is not a reference type in the sense of this page[^1].

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | idiom | every variable holds a reference to an object; immutable types such as `int`, `str` and `tuple` act like values |
| JavaScript | native | primitives such as numbers and strings are values; objects, arrays and functions are shared by reference |
| TypeScript | native | the same split as JavaScript; `readonly` types limit changes but not sharing |
| Java | native | primitive types such as `int` are values; classes, interfaces and arrays are reference types |
| Kotlin | native | classes are reference types; the basic types such as `Int` and `Double` are immutable values |
| C# | native | `struct`, `enum` and the numeric types are value types; `class`, `interface`, `delegate` and arrays are reference types |
| Go | idiom | all types are values, including arrays and structs; pointers share; maps, slices and channels hold internal pointers |
| Rust | idiom | all types are values that move or copy; `&T`, `&mut T`, `Box`, `Rc` and `Arc` share |
| C | idiom | all types are values, including structs; pointers share; an array argument becomes a pointer |
| C++ | idiom | all types are values with copy constructors; pointers and smart pointers share; a reference is an alias |
| Bash | n/a | every variable holds a string, and assignment copies it |
| PowerShell | native | .NET value types and reference types, as in C# |
| SQL | n/a | a query works on values in rows; there are no references between variables |
| Regex | n/a | a pattern has no variables |
| HTML | n/a | HTML has no variables |
| CSS | n/a | custom properties hold values, not references |

C#, Java, JavaScript and Kotlin give each type one kind of behavior, and the programmer chooses it when they declare the type. C, C++, Go and Rust make every type a value type and let the programmer choose sharing at each use, with a pointer or a reference. Python makes every type a reference type and gets value behavior from immutable objects.

The example files show these differences. Python shows that a list is shared and that `[[0] * 3] * 3` repeats one inner list. JavaScript shows that `structuredClone` makes a deep copy. Java shows that a method can change an object it receives but cannot rebind the caller's variable. C# shows that a struct read from a `List` is a copy. Go shows that an array and a struct are copied while a slice and a map share their data. Rust shows `Copy`, a move and `clone`. C shows that a struct assignment copies the whole struct. C++ shows copies, a reference that is an alias, and a `shared_ptr` that shares.

## Variations and related ideas

- **Value objects in Java.** JEP 401 would "Introduce value objects, which are immutable and lack object identity. Value objects are distinguished solely by the values of their fields"[^9]. It is a preview feature, and it keeps "just two kinds of data: primitives and object references"[^9].
- **Immutable reference types.** A reference type whose objects cannot change, such as `String` in Java or `str` in Python, behaves like a value type in most code, because sharing cannot be seen. See [immutability](/immutability/).
- **Boxing.** C# and Java convert a value to an object when the program needs a reference type, for example to store an `int` in a `List<Object>`. See [top type](/top-type/).
- **Explicit pointers.** "Many languages have explicit pointers or references. Reference types differ from these in that the entities they refer to are always accessed via references"[^1]. See [pointers and references](/pointers-and-references/).

## Pitfalls

- **Calling value types stack types.** A value type is not a type whose values live on the stack. Eric Lippert wrote that "value types are allocated on the stack sometimes", and that "the memory for an integer field in a class type is part of the class instance"[^10]. The useful fact about a value type is "that they are always copied" by value[^10]. See [the call stack and the heap](/call-stack-and-heap/).
- **Changing a copy.** In C#, reading a struct from a `List<T>` gives a copy, so a change to that copy does not change the list. The C# example shows this. Make structs immutable, or use a class when the program must change a shared object.
- **Unexpected sharing.** "If a reference type is mutable, then mutations made via one reference are visible via any other"[^1]. A list that two parts of a program share can change under one of them. Copy the object at the boundary, or make it immutable.
- **Rebinding a parameter.** A function that assigns a different object to its parameter does not change the caller's variable[^1]. Return the new object, or change the existing one.
- **Object slicing in C++.** Because C++ copies objects by value, "use of non-references can lead to problems such as object slicing, at least when inheritance is used"[^1]. Copying a derived object into a base class variable drops the derived part. Pass such objects by reference or by pointer.

## Cost

A value type costs a copy on each assignment and call, and the cost grows with the size of the value. A reference type costs one allocation for the object and one indirection on each access, and the copy of a reference is small. In C#, converting a value to `object` boxes it: boxing "allocates an object instance on the heap and copies the value"[^11]. Lippert notes that, for value types, "the relevant thing is their copying and identity semantics"[^10], not where they are stored. JEP 401 aims to let a JVM represent immutable data "in ways that improve memory footprint, locality, and garbage collection efficiency"[^9].

## History

The Java Language Specification divides the types into primitive types and reference types[^3], and C# lets a program declare its own value types with `struct`[^2]. In 2009, Eric Lippert argued that the C# documentation should explain value types by copying, not by the stack[^10]. JEP 401 proposes value objects for Java, which would let a class give up identity[^9].

## References
