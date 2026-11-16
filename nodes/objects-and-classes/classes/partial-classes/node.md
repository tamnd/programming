---
tag: 8YVJGW
kind: concept
slug: partial-classes
name: Partial classes
aliases: [partial class, partial type, partial struct, partial method, partial member, split class definition]
area: objects-and-classes/classes
level: advanced
summary: >-
  A partial class is one class whose definition is split into parts, often in separate files, which the compiler joins into one type.
applies_to: core-languages
viz: optional
history:
  year: 2005
  note: C# 2.0 and Visual Studio 2005 added partial types, so that the code a designer tool generates could be in a separate file from the code a person writes.
see_also: [classes-and-instances, define-a-class, extension-methods, open-classes-and-monkey-patching, mixin-pattern, large-class, extract-class, method-concept]
sources:
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/programming-guide/classes-and-structs/partial-classes-and-methods
    title: Partial classes and members, C# programming guide
    locator: Partial Classes; Restrictions; Partial Members
  - url: https://go.dev/ref/spec
    title: The Go Programming Language Specification
    locator: Method declarations
  - url: https://doc.rust-lang.org/reference/items/implementations.html
    title: Implementations, The Rust Reference
    locator: Inherent implementations
  - url: https://learn.microsoft.com/en-us/dotnet/visual-basic/language-reference/modifiers/partial
    title: Partial (Visual Basic), language reference
    locator: Introduction; Remarks
  - url: https://www.typescriptlang.org/docs/handbook/declaration-merging.html
    title: Declaration Merging, TypeScript Handbook
    locator: Introduction; Merging Interfaces; Module Augmentation
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/whats-new/csharp-version-history
    title: The history of C#, Microsoft Learn
    locator: C# version 2.0; C# version 3.0; C# version 9; C# version 13
---

## Idea

A partial class is one class whose definition is split into two or more parts, usually in two or more source files. The compiler joins the parts into one type. The C# guide says: "Each source file contains a section of the type or member definition, and all parts are combined when the application is compiled."[^1]

The main use is generated code. A tool writes one part, such as the code for a form or for a database table, and a person writes the other part. The tool can write its file again at any time and the code of the person does not change. In C#:

```csharp run id=generated-and-written-parts
using System;

var o = new Order(3, 40m);
Console.WriteLine(o.Total());
Console.WriteLine($"{o} {o is IComparable<Order>}");

// The part that a tool generates, for example Order.g.cs
public partial class Order
{
    public int Count { get; }
    public decimal Price { get; }

    public Order(int count, decimal price)
    {
        Count = count;
        Price = price;
        OnCreated(); // a hook that another part can implement
    }

    partial void OnCreated();

    public override string ToString() => $"Order({Count}, {Price})";
}

// The part that a person writes, for example Order.cs
public partial class Order : IComparable<Order>
{
    partial void OnCreated() => Console.WriteLine($"created {Count}");

    public decimal Total() => Count * Price;

    public int CompareTo(Order? other) => other is null ? 1 : Total().CompareTo(other.Total());
}
```

```output for=generated-and-written-parts
created 3
120
Order(3, 40) True
```

The two `partial class Order` declarations make one class. The first part declares the properties, the constructor and a partial method `OnCreated`, which is only a signature. The second part implements `OnCreated`, adds `Total` and adds the interface `IComparable<Order>`. Code that uses `Order` cannot see that it has two parts.

## How it works

- **The compiler joins the parts.** All the parts must have the `partial` keyword, and "All the parts must be available at compile time to form the final type."[^1] The parts must be in the same assembly. A partial class is a feature of the compiler only: the program at run time has one ordinary class.
- **Members, interfaces and attributes are merged.** "Any class, struct, or interface members declared in a partial definition are available to all the other parts."[^1] A private field in one part can be used by a method in another part. The final type implements the interfaces of all the parts, and the attributes of all the parts apply to it.
- **The parts must agree.** If one part declares a base class, the other parts must name the same base class or none. If one part is `abstract` or `sealed`, the whole class is.
- **Partial methods.** One part declares a method signature, and another part can implement it. A generated part calls hooks such as `OnCreated`, and the person writes only the hooks that are needed. A partial method with no implementation must be `void` and private, and then "The method and all calls to the method are removed at compile time when there's no implementation."[^1]
- **Other languages split by other rules.** Go puts the methods of a type in any file of its package, because of this rule: "A receiver base type cannot be a pointer or interface type and it must be declared in the same package as the method."[^2] The spec also says: "A package in turn is constructed from one or more source files". Rust lets a type have more than one `impl` block: "A type can also have multiple inherent implementations. An implementing type must be defined within the same crate as the original type definition."[^3] In both, the fields stay in one declaration.

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | absent | mixin base classes combine parts from separate modules; code can also add functions to a class after it is defined |
| JavaScript | absent | code can add methods to `Class.prototype` from another module |
| TypeScript | absent | interfaces merge, and an interface can add members to the type of a class; classes do not merge |
| Java | absent | a class is one declaration; annotation processors write separate classes |
| Kotlin | absent | a class is one declaration; extension functions in other files add functions that look like members |
| C# | native | `partial` classes, structs, interfaces and records; partial methods, properties and constructors |
| Go | idiom | the fields are in one `struct` type, and its methods can be in any file of the same package |
| Rust | idiom | the fields are in one `struct`, and the type can have many `impl` blocks in any module of the same crate |
| C | n/a | no classes |
| C++ | idiom | the class definition is one declaration, but its member functions can be defined in many source files |
| Bash | n/a | no classes |
| PowerShell | absent | a class is one declaration |
| SQL | n/a | no classes |
| Regex | n/a | no classes |
| HTML | n/a | no classes |
| CSS | n/a | no classes |

- **Visual Basic** has the same feature as C#. Its `Partial` keyword lets code "divide the definition of a type among several declarations", and "all the declarations must be in the same assembly and the same namespace".[^4]
- **TypeScript** merges declarations of types, not of classes: "the compiler merges two separate declarations declared with the same name into a single definition."[^5] An interface with the name of a class adds members to the type of that class, but code must still add the methods to the prototype.
- **Go and Rust** split the methods of a type, but not its fields. The data is in one place, and the methods for each job can be in their own file.

## Variations and related ideas

- **Partial members.** Since C# 9, a partial method can return a value and have an access modifier, and properties, indexers, events and constructors can be partial too. Such a member must have an implementation, which "might be supplied by a source generator."[^1]
- **Source generators.** A C# source generator writes a part of a partial class at compile time. For example, it can implement a partial method that a person declared with an attribute.
- **Open classes and extension methods.** A partial class must be complete at compile time and in one assembly. [Open classes and monkey patching](/open-classes-and-monkey-patching/) add members to a class at run time, and [extension methods](/extension-methods/) add methods that only look like members.
- **Mixins and traits.** A [mixin](/mixin-pattern/) splits a class into parts that are classes too, so a part can be used in more than one class.
- **A large class.** Splitting a [large class](/large-class/) into partial files does not make it smaller. To give each job its own class, use [extract class](/extract-class/).

## Pitfalls

- **A missing partial method removes its arguments too.** When no part implements a `void` partial method, the compiler removes each call, and "any results that would occur from evaluation of arguments in the calls, have no effect at run time."[^1] An argument with a side effect does not run:

```csharp run id=removed-partial-call
using System;

var log = new Log();
log.Run();
Console.WriteLine(log.Calls);

partial class Log
{
    public int Calls;

    int Next() => ++Calls;

    partial void Trace(int n); // no part implements it

    public void Run()
    {
        for (int i = 0; i < 3; i++)
        {
            Trace(Next()); // the compiler removes the call and its argument
        }
    }
}
```

```output for=removed-partial-call
0
```

- **Hiding a large class.** Partial files can make a class with too many jobs look small, because each file is short. Read all the parts before a change.
- **Edits to generated parts.** A change in the generated file is lost when the tool writes the file again. Put hand-written code in the other part.
- **Conflicts between parts.** Two parts that declare the same member, or different base classes, do not compile. The error can be in a generated file that the person did not write.
- **Same assembly only.** A partial class cannot add members to a class from a library. For that job, use an [extension method](/extension-methods/) or a subclass.

## Cost

A partial class costs nothing at run time: the compiler makes one ordinary class, and the parts leave no trace in the compiled program. A partial method with no implementation costs less than nothing, because the compiler removes the calls. The cost is to readers and tools: the definition of a type is in more than one file, so a reader needs an editor that can find all the parts.

## History

C# 2.0, "released in 2005, along with Visual Studio 2005", had "Partial types" in its list of major features.[^6] The C# guide gives the first use: "Visual Studio uses this approach when it creates Windows Forms, Web service wrapper code, and so on."[^1] C# 3.0 added partial methods. C# 9 added more for code generators: "Partial methods support new accessibly modifiers and non-void return types."[^6] C# 13 allowed partial properties and indexers, and later versions added partial events and constructors.

## References
