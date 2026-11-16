---
tag: QH79DF
kind: concept
slug: extension-methods
name: Extension methods
aliases: [extension method, extension function, extension member, extension property, extension block, Swift extension, extension trait]
area: objects-and-classes/classes
level: core
summary: >-
  An extension method is a function declared outside a type that code calls with method syntax, as if it were a member of that type.
applies_to: core-languages
viz: optional
history:
  year: 2007
  note: C# 3.0 added extension methods so that LINQ could add query methods to every collection; Smalltalk and Objective-C categories had let code add methods to existing classes long before.
see_also: [add-a-method-to-an-existing-type, open-classes-and-monkey-patching, partial-classes, define-a-method-on-a-type, method-concept, static-members, interfaces, decorator-pattern]
same_as:
  wikipedia: Extension_method
sources:
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/programming-guide/classes-and-structs/extension-methods
    title: Extension members, C# programming guide
    locator: Introduction; Declare extension members; Binding extension members at compile time
  - url: https://kotlinlang.org/docs/extensions.html
    title: Extensions, Kotlin documentation
    locator: Introduction; Extension or member functions?
  - url: https://docs.swift.org/swift-book/documentation/the-swift-programming-language/extensions/
    title: Extensions, The Swift Programming Language
    locator: Introduction
  - url: https://doc.rust-lang.org/book/ch10-02-traits.html
    title: Defining Shared Behavior with Traits, The Rust Programming Language
    locator: Implementing a Trait on a Type
  - url: https://en.wikipedia.org/wiki/Extension_method
    title: Extension method, Wikipedia
    locator: Introduction; Support in programming languages
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/whats-new/csharp-version-history
    title: The history of C#, Microsoft Learn
    locator: C# version 3.0; C# version 14
---

## Idea

An extension method is a function that code calls as if it were a method of a type, but that is declared outside the type. It lets code add a method to a type that it cannot change, such as `string` or a class from a library. The C# guide says: "Extension members enable you to "add" methods to existing types without creating a new derived type, recompiling, or otherwise modifying the original type."[^1]

The type does not change. The compiler turns the call `s.WordCount()` into a call of a static function with `s` as its first argument. In C#:

```csharp run id=extension-method-call
using System;
using System.Linq;

string s = "Hello Extension Methods";
Console.WriteLine(s.WordCount()); // called like a method of string
Console.WriteLine(TextExtensions.WordCount(s)); // the same static method, called directly
int[] numbers = [10, 45, 15];
Console.WriteLine(string.Join(" ", numbers.OrderBy(n => n))); // LINQ: extension methods on IEnumerable<T>

static class TextExtensions
{
    public static int WordCount(this string text) =>
        text.Split(' ', StringSplitOptions.RemoveEmptyEntries).Length;
}
```

```output for=extension-method-call
3
3
10 15 45
```

`this string text` makes `WordCount` an extension method of `string`, so `s.WordCount()` and `TextExtensions.WordCount(s)` are the same call. `OrderBy` is an extension method too: LINQ adds its query methods to every type that implements `IEnumerable<T>`.

## How it works

- **A static function with a receiver parameter.** C# marks the first parameter with `this`. "Extension methods are static methods, but they're called as if they were instance methods on the extended type."[^1] Kotlin writes the type before the name, as in `fun String.wordCount()`, and Swift puts the methods in an `extension String` block.
- **The type does not change.** Kotlin says: "extensions don't modify the classes or interfaces they extend. When you define an extension, you don't add new members."[^2] An extension method can use only the public members of the type, so it cannot break encapsulation.
- **Scope by import.** An extension method is visible only where its namespace, package or trait is imported. In C#, "Extension methods are only in scope when you explicitly import the namespace into your source code with a using directive."[^1]
- **Static binding.** The compiler picks the extension method from the declared type of the expression, at compile time. "Extension functions are dispatched statically, meaning the compiler determines which function to call based on the receiver type at compile time."[^2] An extension method is not virtual, so a subclass cannot override it.
- **Members first.** The compiler looks for a member of the type before it looks for an extension. "At compile time, extension members always have lower priority than instance (or static) members defined in the type itself."[^1]

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | absent | a plain function; a subclass; user classes can get new methods at run time (monkey patching) |
| JavaScript | absent | a plain function; code can add a method to a prototype, which changes the type for all code |
| TypeScript | absent | as JavaScript; declaration merging can add the method to the type |
| Java | absent | static methods in a utility class; default methods in interfaces |
| Kotlin | native | extension functions and extension properties, `fun Type.name()` |
| C# | native | `this` on the first parameter of a static method; C# 14 extension blocks with properties and operators |
| Go | absent | a plain function, or a new type declared in the package, which can have methods |
| Rust | idiom | an extension trait: a local trait implemented for a type from another crate |
| C | n/a | no methods |
| C++ | absent | free functions found by argument-dependent lookup; no call syntax with a dot |
| Bash | n/a | no types or methods |
| PowerShell | native | `Update-TypeData -MemberType ScriptMethod` adds script methods to a .NET type for the session |
| SQL | n/a | user-defined functions take a value as an argument; no method syntax |
| Regex | n/a | no types or methods |
| HTML | n/a | no types or methods |
| CSS | n/a | no types or methods |

- **Kotlin and Swift** make extensions part of the everyday style. Swift extensions "add new functionality to an existing class, structure, enumeration, or protocol type",[^3] and they can also make a type conform to a protocol. Neither can replace a method: Swift says that extensions "can't override existing functionality."
- **Rust** has no extension methods, but a trait does the same job. A crate can define a trait and implement it for `str` or `Vec<T>`, because "we can implement a trait on a type only if either the trait or the type, or both, are local to our crate."[^4] The methods are in scope where the trait is imported.
- **Python, Ruby and JavaScript** can change a class at run time, so they need no extension methods. Wikipedia says: "In dynamic languages like Python, the concept of an extension method is unnecessary because classes (excluding built-in classes) can be extended without any special syntax".[^5] That is [monkey patching](/open-classes-and-monkey-patching/), and the change is visible to all code, not only to code that imports it.
- **Go** does not allow methods on a type from another package. Code declares a new type, such as `type Text string`, or uses a plain function.

## Variations and related ideas

- **Extension properties and static extensions.** Kotlin has extension properties, and C# 14 added extension blocks: "Extension blocks support multiple member types: methods, properties, and operators."[^1] A static extension adds a member to the type itself, not to its instances.
- **Fluent interfaces.** LINQ, Kotlin's collection functions and Rust iterator adapters let code write a chain such as `xs.Where(...).Select(...).OrderBy(...)` on any collection, with no change to the collection types.
- **Default methods.** A Java `default` method or a Rust trait method with a body adds a method to every type that implements an interface. The author of the interface adds it, so it can be overridden.
- **Related tasks.** [Add a method to an existing type](/add-a-method-to-an-existing-type/) compares the ways to get the result in each language, and [partial classes](/partial-classes/) split a type that code owns.

## Pitfalls

- **No dynamic dispatch, and members win.** The compiler picks an extension from the declared type, and it never calls an extension that has the same signature as a member. A C# extension method can also be called on `null`, because the call is a static call:

```csharp run id=static-binding-and-null
using System;

Shape s = new Square();
Console.WriteLine(s.Describe()); // chosen by the declared type, Shape
Console.WriteLine(new Square().Describe()); // chosen by the declared type, Square
Console.WriteLine(new Square().Name()); // a member always wins over an extension
Square? none = null;
Console.WriteLine(none.Describe()); // no NullReferenceException: it is a static call

class Shape
{
}

class Square : Shape
{
    public string Name() => "member Name";
}

static class ShapeExtensions
{
    public static string Describe(this Shape s) => "a shape";

    public static string Describe(this Square? s) => s is null ? "no square" : "a square";

    public static string Name(this Square s) => "extension Name"; // never called
}
```

```output for=static-binding-and-null
a shape
a square
member Name
no square
```

- **A new member hides an extension.** When a later version of a library adds a member with the same name and signature, the compiler calls the member, and the extension method is silently ignored.
- **Name clashes between extensions.** Two imported namespaces or traits with extension methods of the same name make the call ambiguous, and the code does not compile until the call names one of them.
- **Hard to find.** A reader of `s.WordCount()` can think that `WordCount` is part of `string`. An IDE shows the declaring class, but a reader of plain text cannot see it.
- **Too many extensions.** Extensions on a common type such as `string` or `object` show in every completion list. Put them in a namespace that only the code that needs them imports.

## Cost

An extension method call is a static call, with the receiver as the first argument, so it costs the same as a call of a static method, and the compiler can inline it. It never costs a virtual dispatch. A Rust extension trait method is also called with static dispatch, unless the code calls it through a `dyn` trait object. Extension methods do not change the size or the layout of the type.

## History

Smalltalk let code add a method to any class: "In Smalltalk, any code can add a method to any class at any time".[^5] Objective-C categories did the same in a compiled language, and Swift extensions are "similar to categories in Objective-C."[^3] C# 3.0, released in November 2007, had "Extension methods" in its list of major features,[^6] so that LINQ could add query methods to all collections. Kotlin and Swift made extensions a main part of their style, and C# 14, released in November 2025, added "Extension members" with properties and operators.

## References
