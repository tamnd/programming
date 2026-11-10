---
tag: NEW-runtime-reflection
kind: concept
slug: runtime-reflection
name: Runtime reflection
aliases: [reflection, reflective programming, introspection, type introspection]
area: metaprogramming/reflection
level: core
summary: >-
  The ability of a running program to examine its own structure, such as the type of a value, its fields and its methods, and to use them by name.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1982
  people: [Brian Cantwell Smith]
  note: Smith's MIT doctoral thesis, Procedural Reflection in Programming Languages, gave an account of reflection in programming languages and built the reflective Lisp dialect 3-LISP.
see_also: [annotations, syntactic-macros, static-and-dynamic-typing, type-erasure, interfaces, subtyping, java, go]
same_as:
  wikidata: Q1056451
  wikipedia: Reflective_programming
sources:
  - url: https://hdl.handle.net/1721.1/149550
    title: Brian Cantwell Smith, Procedural Reflection in Programming Languages, Volume I, PhD thesis, MIT, February 1982 (MIT-LCS-TR-272)
    locator: Abstract, page 2; Extended Abstract, page 3; Prologue, page 17; 1.a General Overview, page 27; page 41
  - url: https://go.dev/blog/laws-of-reflection
    title: Rob Pike, The Laws of Reflection, The Go Blog, 6 September 2011
    locator: the introduction; The first, second and third laws of reflection; Conclusion
  - url: https://pkg.go.dev/reflect
    title: Go standard library, package reflect
    locator: Overview
  - url: https://go.dev/ref/spec
    title: The Go Programming Language Specification
    locator: Struct types, the paragraph on tags
  - url: https://docs.oracle.com/javase/tutorial/reflect/index.html
    title: The Java Tutorials, Trail, The Reflection API
    locator: Uses of Reflection; Drawbacks of Reflection
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/lang/Class.html
    title: Java SE 25 API, java.lang.Class
    locator: getDeclaredFields; getDeclaredMethods; getDeclaredConstructors; getRecordComponents; getDeclaredMethod
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/lang/reflect/AccessibleObject.html
    title: Java SE 25 API, java.lang.reflect.AccessibleObject
    locator: the class description; setAccessible(boolean)
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/lang/reflect/Method.html
    title: Java SE 25 API, java.lang.reflect.Method
    locator: invoke, Throws
  - url: https://learn.microsoft.com/en-us/dotnet/fundamentals/reflection/reflection
    title: Reflection in .NET
    locator: the introduction and the list of classes
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.type.getproperties
    title: Type.GetProperties Method
    locator: Remarks
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.reflection.methodbase.invoke
    title: MethodBase.Invoke Method
    locator: Exceptions
  - url: https://learn.microsoft.com/en-us/dotnet/core/deploying/trimming/fixing-warnings
    title: Fixing trim warnings
    locator: Understanding warning categories; Approach 1, Eliminate reflection; RequiresUnreferencedCode
  - url: https://docs.python.org/3/library/inspect.html
    title: Python documentation, inspect, Inspect live objects
    locator: the introduction; inspect.signature
  - url: https://docs.python.org/3/library/functions.html
    title: Python documentation, Built-in Functions
    locator: getattr
  - url: https://docs.python.org/3/glossary.html
    title: Python documentation, Glossary
    locator: type hint
  - url: https://tc39.es/ecma262/2025/multipage/reflection.html
    title: ECMA-262, 16th edition (ECMAScript 2025), 28 Reflection
    locator: 28.1 The Reflect Object; 28.1.1 Reflect.apply; 28.1.10 Reflect.ownKeys
  - url: https://www.typescriptlang.org/docs/handbook/2/basic-types.html
    title: The TypeScript Handbook, The Basics
    locator: Erased Types
  - url: https://doc.rust-lang.org/std/any/index.html
    title: The Rust Standard Library, Module std::any
    locator: the module description; Any and TypeId
  - url: https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2023/n4950.pdf
    title: N4950, Working Draft, Standard for Programming Language C++ (the C++23 draft)
    locator: 7.6.1.8 [expr.typeid], paragraphs 1, 3 and 4; 17.7.3 [type.info], name, paragraph 8
  - url: https://kotlinlang.org/docs/reflection.html
    title: Kotlin documentation, Reflection
    locator: the introduction; JVM dependency; Class references
  - url: https://learn.microsoft.com/en-us/powershell/module/microsoft.powershell.utility/get-member
    title: PowerShell documentation, Get-Member
    locator: Description
  - url: https://www.postgresql.org/docs/current/information-schema.html
    title: PostgreSQL documentation, The Information Schema
    locator: the first paragraph
---

## Idea

A compiler knows the types, fields and methods of a program. In many languages this knowledge is gone when the program runs: the machine code has addresses and offsets, not names. Runtime reflection keeps a description of the program in memory and gives the program access to it. Rob Pike puts it in one sentence: "Reflection in computing is the ability of a program to examine its own structure, particularly through types; it's a form of metaprogramming."[^2]

With reflection, one piece of code can handle types that it has never seen. A serializer can write any object as JSON by walking its fields. A test framework can find every method whose name starts with `test`. A dependency injection container can call a constructor that it found by name in a configuration file. The Java tutorial lists class browsers, debuggers, test tools and extensibility through class names as the uses.[^5]

The Go program below prints any struct without knowing its type. It asks the value for its type, then asks the type for its fields.

```go run id=idea
package main

import (
	"fmt"
	"reflect"
)

type User struct {
	Name  string
	Age   int
	Admin bool
}

type Order struct {
	ID    int
	Total float64
}

func describe(v any) {
	t := reflect.TypeOf(v)
	val := reflect.ValueOf(v)
	fmt.Println(t.Name())
	for i := 0; i < t.NumField(); i++ {
		fmt.Printf("  %s (%s) = %v\n", t.Field(i).Name, t.Field(i).Type, val.Field(i))
	}
}

func main() {
	describe(User{"Ada", 36, true})
	describe(Order{7, 19.5})
}
```

```output for=idea
User
  Name (string) = Ada
  Age (int) = 36
  Admin (bool) = true
Order
  ID (int) = 7
  Total (float64) = 19.5
```

The price is that the compiler can no longer check the code. A field or a method named by a string may not exist, and the error appears only when the program runs. Reflective code is also slower, and it can reach parts of an object that its author meant to hide.

## How it works

### The description of the program

A language with reflection keeps one object for each type at run time: `java.lang.Class` in Java, `System.Type` in .NET, `reflect.Type` in Go and the class object in Python. Each value can return its type object. In Java the call is `getClass()`, in C# `GetType()`, in Go `reflect.TypeOf` and in Python `type`. The type object then lists the members, and each member is an object too: Java has `Field`, `Method` and `Constructor`, and .NET has `FieldInfo`, `PropertyInfo`, `MethodInfo` and `ConstructorInfo`.[^6][^9]

A member object can do two things. It can describe the member: its name, its type, its parameters and its modifiers. And it can use the member on a given object: read or set a field, or call a method with an array of arguments.[^9] In Go, `reflect.ValueOf` returns a `reflect.Value` that holds the data, and methods on the value read it, set it and call it.[^3]

Pike states the Go rules as three laws. Reflection goes from an interface value to a reflection object. Reflection goes from a reflection object back to an interface value. To modify a reflection object, the value must be settable.[^2] The third law explains a common panic. `reflect.ValueOf(x)` receives a copy of `x`, so a change to it could not change `x`, and a call to `SetFloat` panics with "reflect.Value.SetFloat using unaddressable value". A reflection value made from `&x` and `Elem()` is settable.[^2] The Go example `not-settable` shows the panic, and `settable` shows the fix.

### Where the information comes from

Reflection needs a description of the program at run time. In .NET, assemblies contain modules, modules contain types, and types contain members, and reflection provides objects that encapsulate each of them.[^9] Go keeps struct tags for reflection alone. The Go specification says that tags "are made visible through a reflection interface" and are "otherwise ignored".[^4] Python and JavaScript keep all of it by design, because objects are tables of named attributes.

Languages that compile to machine code without this metadata have little or no reflection. In C++, `typeid` gives a `std::type_info` for the dynamic type of a polymorphic object, and `dynamic_cast` tests it.[^19] The result of `type_info::name()` is "an implementation-defined ntbs".[^19] There is no list of fields or methods. Rust's `std::any` describes itself as "utilities for dynamic typing or type reflection". It can give a `TypeId` and can test whether a `dyn Any` holds a given concrete type, but it "cannot be used to test whether a type implements a trait".[^18]

TypeScript erases its types. The handbook says: "Type annotations never change the runtime behavior of your program."[^17] A TypeScript interface does not exist at run time, so a TypeScript program can only reflect on the JavaScript objects.

### Reflection in dynamic languages

In Python, every attribute access is already a lookup by name at run time. The built-in `getattr(x, 'foobar')` is equivalent to `x.foobar`, and it raises `AttributeError` if the attribute does not exist.[^14] The module `inspect` gets information about live objects: modules, classes, methods, functions, tracebacks, frames and code objects.[^13] `inspect.signature` reads the parameters of a function and their annotations.[^13] Python keeps type hints at run time, but it does not enforce them.[^15]

JavaScript exposes the same model through the `Reflect` object. `Reflect` is an ordinary object that cannot be called or constructed. Its functions, such as `Reflect.apply`, `Reflect.get` and `Reflect.ownKeys`, perform the internal operations of objects.[^16]

In these languages reflection can change the program as well as read it. Python's `setattr` adds an attribute to an object, and an assignment to a class attribute adds a method to every instance. The Python example `change` shows both.

### Smith's procedural reflection

Brian Cantwell Smith's 1982 thesis asked a larger question. He wanted a computational system that could "reason" about its own inferential processes.[^1] He noted that "the term 'reflection' does not name a previously well-defined question", and so he had to give an account of what reflection is.[^1] His system had to make "fully articulated descriptions of the state of the reasoning process" available "for inspection and modification" at any point, and it had to resume the computation from such descriptions.[^1] He called this particular form "procedural reflection".[^1] He built three dialects of Lisp, and the third, 3-LISP, was "endowed with reflective powers".[^1]

The reflection of Java, C#, Go and Python is a much smaller part of this idea. It describes the structure of the program, its types and members. It does not, in general, give access to the state of the interpreter that runs the program.

## In languages

| Language | Verdict | Form |
|---|---|---|
| Java | native | `Class`, `java.lang.reflect`; access checks apply, `setAccessible` subject to modules |
| C# | native | `System.Type`, `System.Reflection`; limited by trimming |
| Go | native | package `reflect`: `TypeOf`, `ValueOf`, struct tags |
| Python | native | `type`, `getattr`, `setattr`, `vars`, the module `inspect` |
| JavaScript | native | `typeof`, `Object.keys`, prototypes, the `Reflect` object |
| TypeScript | absent | types are erased; the reflection of JavaScript only |
| Kotlin | native | `::class`, callable references; the library `kotlin-reflect` |
| C++ | absent | `typeid` and `dynamic_cast` only; no members at run time |
| Rust | absent | `std::any`: `TypeId`, `type_name`, downcasting |
| C | absent | no type information at run time |
| PowerShell | native | objects of .NET; `Get-Member` lists properties and methods |
| Bash | absent | strings and arrays, no typed objects |
| SQL | idiom | the information schema describes tables and columns |
| Regex | n/a | no types or members |
| HTML/CSS | n/a | no program to inspect |

**Java** keeps access control in reflection. Every reflected object checks that the code using it may access the member, and `setAccessible(true)` can suppress the check only in some cases: the caller and the member are in the same module, the member is public in an exported package, or the package is open to the caller.[^7] The Java example `encapsulation` shows the call failing for a private field of `String`, and `private-own` shows it working for a class of the program. Order is not guaranteed: "The elements in the returned array are not sorted and are not in any particular order" for `getDeclaredFields`.[^6] `getRecordComponents` is the exception. It returns the components "in the same order that they are declared".[^6]

**C#** reflection can load assemblies, find types and create instances, and the classes of `System.Reflection.Emit` can build types at run time.[^9] Before .NET 7, `GetProperties` returned the properties in no particular order. From .NET 7, the order is "deterministic based upon the metadata ordering in the assembly".[^10]

**Kotlin** has class references (`ClassName::class` and `value::class`) and callable references in the language and the standard library. For more extensive introspection, a program adds the library `kotlin-reflect`. On the JVM, this is a separate artifact, so that applications without reflection can leave it out and reduce the size of the classpath.[^20] No Kotlin example runs on this page, because kotlinc is not installed on the machine that recorded the examples.

**PowerShell** works on .NET objects, and the cmdlet `Get-Member` "gets the members, the properties and methods, of objects".[^21] **SQL** describes a database to itself through the information schema, a set of views that "is defined in the SQL standard".[^22] A query on `information_schema.columns` is reflection over the tables of the database.

## Variations and related ideas

- **Static reflection and code generation.** A derive macro or an annotation processor reads the structure of a type at compile time and generates code. The compiler checks the result, and the program needs no metadata at run time. See [[syntactic-macros]] and [[annotations]].
- **Annotations read by reflection.** Java annotations with run-time retention and C# attributes are stored in the metadata and read through reflection. Frameworks use them to find the classes and methods they should handle. See [[annotations]].
- **Dynamic typing.** In Python and JavaScript, the ordinary operations already look up names at run time. Reflection there adds the ability to list the names. See [[static-and-dynamic-typing]].
- **Type erasure.** If a language erases the type arguments of generics, reflection sees only the erased types. TypeScript erases all of its types.[^17] See [[type-erasure]].
- **Dynamic dispatch.** A virtual call or an interface call chooses a method at run time, but the compiler has checked that the method exists. Reflection drops that check. See [[interfaces]] and [[subtyping]].

## Pitfalls

### Names as strings

A method name passed to `getDeclaredMethod`, `GetMethod`, `MethodByName` or `getattr` is a string. The compiler does not check it. The Java example `typo` throws `NoSuchMethodException`, the C# example `typo` throws `NullReferenceException` because `GetMethod` returned `null`, and the Python example `typo` raises `AttributeError`.[^6][^14] A rename in an IDE does not change the strings, so code that worked can break after a refactoring. In JavaScript the error does not even stop the program: the example `typo` gets `undefined` and computes `NaN`.

### Exceptions in a wrapper

A method called through reflection that throws does not throw its own exception to the caller. Java wraps it in `InvocationTargetException`, and .NET wraps it in `TargetInvocationException`.[^8][^11] A `catch` for the original type does not catch it. The C# example `wrapped` shows the inner `FormatException`.

### A copy instead of the original

In Go, `reflect.ValueOf(p)` reflects a copy of `p`, so the fields are not settable, and the `Set` methods panic. Only exported fields are settable at all.[^2] The Go example `not-settable` shows the panic.

### Depending on the order of members

The Java methods `getDeclaredFields` and `getDeclaredMethods` return members in no particular order.[^6] Code that writes fields in the order that reflection returns them can produce a different format with a different JVM. Sort the members, or use a source with a defined order, such as `getRecordComponents`.[^6]

### Breaking encapsulation

Reflection can read and set private fields where access control allows it. The Java tutorial warns that reflective code "breaks abstractions and therefore may change behavior with upgrades of the platform".[^5] The Java example `private-own` sets a private balance to a negative value that the class's own methods would never allow.

### Reflection and trimming

A .NET app that is trimmed loses the code that the trimmer does not see used. Code that loads a type by a name read at run time "fundamentally can't be made analyzable", and the SDK marks it with `RequiresUnreferencedCodeAttribute`.[^12] The first fix the documentation gives is to remove the reflection and use generics checked at compile time.[^12] Kotlin keeps its full reflection in a separate library for a similar reason: size.[^20]

## Cost

- **Speed.** "Because reflection involves types that are dynamically resolved, certain Java virtual machine optimizations can not be performed." Reflective operations are slower, and the Java tutorial says to avoid them in code that runs often.[^5]
- **Static checks.** Names, argument counts and argument types are checked when the program runs, not when it is compiled. In Go, a call of `SetFloat` on a value that is not settable compiles and then panics.[^2]
- **Size.** The metadata must be in the program. .NET trimming and the separate `kotlin-reflect` artifact both trade reflection for a smaller program.[^12][^20]
- **Clarity.** Pike's conclusion: "It's a powerful tool that should be used with care and avoided unless strictly necessary."[^2] The Java tutorial: "If it is possible to perform an operation without using reflection, then it is preferable to avoid using it."[^5]

## History

- **1982.** Brian Cantwell Smith's MIT thesis gave an account of reflection in programming languages, defined procedural reflection, and presented 3-LISP, a Lisp dialect with reflective powers.[^1]
- **Java 1.1.** The Java API marks `getDeclaredFields`, `getDeclaredMethods` and `getDeclaredConstructors` of `Class` with "Since: 1.1".[^6]
- **2011.** Rob Pike published "The Laws of Reflection" for Go.[^2]

## References
