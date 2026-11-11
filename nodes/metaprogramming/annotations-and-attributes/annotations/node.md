---
tag: VZD8YG
kind: concept
slug: annotations
name: Annotations
aliases: [annotation, attributes, attribute, metadata annotations, decorators]
area: metaprogramming/annotations-and-attributes
level: core
summary: >-
  Structured metadata attached to declarations in source code, such as @Override in Java or #[derive] in Rust, which a compiler, a tool or the running program reads.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 2004
  people: [Joshua Bloch]
  note: JSR 175, A Metadata Facility for the Java Programming Language, reached final release on 30 September 2004 and gave Java annotations, with Joshua Bloch as specification lead.
see_also: [decorator-pattern, runtime-reflection, syntactic-macros, constant-evaluation, higher-order-functions, interfaces, java, rust]
same_as:
  wikidata: Q2722260
  wikipedia: Attribute_(programming)
sources:
  - url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-9.html
    title: The Java Language Specification, Java SE 25 Edition, Chapter 9 Interfaces
    locator: 9.6 Annotation Interfaces; 9.6.4.1 @Target; 9.6.4.2 @Retention; 9.6.4.4 @Override; 9.7 Annotations
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/lang/annotation/RetentionPolicy.html
    title: Java SE 25 API, java.lang.annotation.RetentionPolicy
    locator: Enum Constant Summary; Since
  - url: https://jcp.org/en/jsr/detail?id=175
    title: JSR 175, A Metadata Facility for the Java Programming Language
    locator: Description; Stage timeline (Final Release, 30 Sep 2004); Section 1 Identification; Section 2.1
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.compiler/javax/annotation/processing/Processor.html
    title: Java SE 25 API, javax.annotation.processing.Processor
    locator: the interface description; Since
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.compiler/javax/annotation/processing/Filer.html
    title: Java SE 25 API, javax.annotation.processing.Filer
    locator: the interface description
  - url: https://jcp.org/en/jsr/detail?id=269
    title: JSR 269, Pluggable Annotation Processing API
    locator: Description; Stage timeline (Final Release, 11 Dec 2006)
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/advanced-topics/reflection-and-attributes/
    title: C# documentation, Attributes and reflection
    locator: the introduction and the list of properties; Work with attributes; Attribute parameters
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/advanced-topics/reflection-and-attributes/creating-custom-attributes
    title: C# documentation, Create custom attributes
    locator: the first paragraph and the AuthorAttribute example
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/attributes/general
    title: C# language reference, Attributes interpreted by the compiler, Miscellaneous attributes
    locator: Conditional attribute; Obsolete and Deprecated attribute
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/roslyn-sdk/source-generators-overview
    title: Source Generators overview
    locator: the first paragraphs
  - url: https://doc.rust-lang.org/reference/attributes.html
    title: The Rust Reference, Attributes
    locator: attributes.intro; attributes.inner; attributes.outer; attributes.kind
  - url: https://doc.rust-lang.org/reference/attributes/derive.html
    title: The Rust Reference, Derive
    locator: attributes.derive.intro and its example
  - url: https://doc.rust-lang.org/reference/attributes/diagnostics.html
    title: The Rust Reference, Diagnostic attributes
    locator: attributes.diagnostics.lint.level; lint.deny; deprecated.intro; must_use.intro; must_use.type
  - url: https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2023/n4950.pdf
    title: N4950, Working Draft, Standard for Programming Language C++ (the C++23 draft)
    locator: 9.12.1 [dcl.attr.grammar] paragraphs 1, 5 and 6 with Note 5; 9.12.5 [dcl.attr.deprecated]; 9.12.9 [dcl.attr.nodiscard] paragraphs 1 and 4; C.5.7 paragraph 5
  - url: https://www.open-std.org/jtc1/sc22/wg14/www/docs/n3220.pdf
    title: N3220, working draft of ISO/IEC 9899:2024 (C23)
    locator: 6.7.13.2 General, paragraphs 2 and 3 and footnote 173; 6.7.13.3 The nodiscard attribute; M.1 Attribute Changes; M.2 Fifth Edition
  - url: https://docs.python.org/3/reference/compound_stmts.html
    title: The Python Language Reference, 8 Compound statements
    locator: 8.7 Function definitions (decorator expressions); 8.8 Class definitions
  - url: https://peps.python.org/pep-0318/
    title: PEP 318, Decorators for Functions and Methods
    locator: the header (Created 05-Jun-2003, Python-Version 2.4)
  - url: https://docs.python.org/3/library/functools.html
    title: The Python Standard Library, functools
    locator: update_wrapper; wraps
  - url: https://docs.python.org/3/library/dataclasses.html
    title: The Python Standard Library, dataclasses
    locator: the introduction; the dataclass decorator; Frozen instances
  - url: https://docs.python.org/3/glossary.html
    title: Python Glossary
    locator: annotation
  - url: https://go.dev/ref/spec
    title: The Go Programming Language Specification
    locator: Struct types, the paragraph on tags
  - url: https://pkg.go.dev/reflect
    title: Go standard library, package reflect
    locator: type StructTag; StructTag.Get; StructTag.Lookup
  - url: https://pkg.go.dev/cmd/vet
    title: Go command documentation, cmd/vet
    locator: the list of analyzers, structtag
  - url: https://kotlinlang.org/docs/annotations.html
    title: Kotlin documentation, Annotations
    locator: the introduction; Usage (meta-annotations @Target and @Retention)
  - url: https://kotlinlang.org/docs/ksp-overview.html
    title: Kotlin Symbol Processing API
    locator: the introduction
  - url: https://devblogs.microsoft.com/typescript/announcing-typescript-5-0/
    title: Daniel Rosenwasser, Announcing TypeScript 5.0, TypeScript Blog, 16 March 2023
    locator: Decorators; Differences with Experimental Legacy Decorators
  - url: https://github.com/tc39/proposal-decorators
    title: TC39, Decorators proposal
    locator: the README header (Stage 2.7) and Introduction
  - url: https://learn.microsoft.com/en-us/powershell/module/microsoft.powershell.core/about/about_functions_advanced_parameters?view=powershell-7.5
    title: PowerShell documentation, about_Functions_Advanced_Parameters
    locator: Long description; Parameter declaration; Attributes of parameters
  - url: https://doc.rust-lang.org/reference/procedural-macros.html
    title: The Rust Reference, Procedural macros
    locator: macro.proc.derive (the input and output token streams); macro.proc.attribute
---

## Idea

A declaration in source code says what a program element is: a class with these fields, a method with these parameters. Tools often need more. A test runner must know which methods are tests. A serializer must know the name of a field in JSON. A compiler must know that a method should override another one. An annotation is a piece of structured data that the programmer attaches to a declaration for such a reader.

Java defines it this way: "An annotation is a marker which associates information with a program element, but has no effect at run time."[^1] The annotation does nothing by itself. Something else reads it and acts. There are three readers:

- **The compiler.** It reads the annotation while it compiles and checks or changes its output. `@Override` in Java, `[[nodiscard]]` in C++ and `#[derive(Debug)]` in Rust work this way.[^1][^14][^12]
- **A tool at compile time.** An annotation processor or a source generator reads the annotations and writes additional source files.[^4][^5][^10]
- **The running program.** A library reads the annotations through reflection and changes what it does. A web framework finds its request handlers this way. See [[runtime-reflection]].

The name differs by language. Java and Kotlin say annotation. C#, C, C++ and Rust say attribute. Python has decorators, which look like Java annotations but are function calls that run when the definition runs.[^16] Go has struct tags, which are strings on fields.[^21]

Before annotations, programs carried this information in other ways. JSR 175, the Java request that introduced them, describes "a growing trend towards annotating fields, methods, and classes as having particular attributes that indicate they should be processed in special ways by development tools, deployment tools, or run-time libraries". It names naming patterns such as getFoo and setFoo in JavaBeans as one such ad hoc mechanism.[^3] A naming pattern fails without a message when a name is misspelled. An annotation is a declared type, so the compiler checks its name and its arguments.

## How it works

### A typed declaration

In Java, an annotation is an instance of an annotation interface, declared with `@interface`.[^1] The elements of the interface are the parameters of the annotation, and they can have default values. The java example `runtime` declares this:

```java norun reason=fragment
@Retention(RetentionPolicy.RUNTIME)
@Target(ElementType.METHOD)
@interface Command {
    String value();
    String help() default "";
}
```

Annotations on the annotation interface, called meta-annotations, control its use. `@Target` limits the kinds of declarations that the annotation can mark.[^1] `@Retention` controls how long the annotation lives.

A C# attribute is a class that derives from `System.Attribute`. Its constructor parameters are the positional parameters, and its public read-write fields and properties are the named parameters.[^8] By convention, the name of the class ends in `Attribute`, and code can leave out the suffix: `[DllImport]` is the same as `[DllImportAttribute]`.[^7] `AttributeUsage` plays the role of `@Target`, and the csharp example `usage` shows the compile error when an attribute is put in the wrong place.

Rust, C and C++ do not let a program declare an attribute as a type. In Rust, "An attribute is a general, free-form metadatum that is interpreted according to name, convention, language, and compiler version."[^11] The compiler defines the built-in attributes, and procedural macros can define more.[^11] In C and C++, the standard defines a small set of attributes, and vendors add their own attributes with a vendor prefix.[^14][^15]

### Retention

An annotation can live in three places, and Java names them with `RetentionPolicy`:[^2]

- `SOURCE`: "Annotations are to be discarded by the compiler."
- `CLASS`: "Annotations are to be recorded in the class file by the compiler but need not be retained by the VM at run time."
- `RUNTIME`: "Annotations are to be recorded in the class file by the compiler and retained by the VM at run time, so they may be read reflectively."

If an annotation interface has no `@Retention`, the compiler treats it as `CLASS`.[^1] This default surprises many programmers. The java example `default-retention` declares an annotation without `@Retention`, and reflection finds no annotated methods.

Kotlin has the same choice. `@Retention` "specifies whether the annotation is stored in the compiled class files and whether it's visible through reflection at runtime", and by default both are true.[^24] C# attributes add metadata to the program, and "you can query the attribute at run time by using a technique called reflection".[^7] C and C++ define no standard way to read an attribute at run time.

### Attributes that the compiler interprets

Many standard attributes ask the compiler for a diagnostic or change what it emits.

- **Checks.** Java's `@Override` causes a compile-time error if the method does not override a method of a supertype.[^1] The java example `override` shows the classic case from the specification: `equals(Point)` overloads `equals(Object)` and does not override it.[^1]
- **Warnings about results.** `[[nodiscard]]` in C and C++ and `#[must_use]` in Rust mark a value that the caller should use.[^14][^15][^13]
- **Deprecation.** `[[deprecated]]` in C and C++, `@Deprecated` in Java, `#[deprecated]` in Rust and `Obsolete` in C# mark an element whose use is discouraged.[^14][^1][^13][^9] In C#, `Obsolete` with a second argument of `true` makes each use a compile error.[^9]
- **Conditional code.** The Rust attribute `cfg` "Controls conditional compilation".[^11] C#'s `Conditional` attribute makes the compiler leave out calls to a method when a preprocessing symbol is not defined.[^9] The csharp example `custom` shows a call that disappears.
- **Generated code.** Rust's `derive` "invokes one or more derive macros, allowing new items to be automatically generated for data structures".[^12] The rust example `derive` gets `Debug`, `Clone`, `PartialEq` and `Default` from one line.

C and C++ give their standard attributes optional semantics. In C, "Standard attributes specified by this document can be parsed but ignored by an implementation without changing the semantics of a correct program".[^15] In C++, removing one of the standard attributes from a correct program gives a program whose possible executions are a subset of those of the original.[^14] For `nodiscard`, both standards only recommend a warning.[^14][^15] The c and cpp examples `nodiscard` compile with `-Werror`, so the warning stops the build.

### Annotation processors and source generators

An annotation processor is a plug-in for the compiler. In Java, the interface is `javax.annotation.processing.Processor`. "Annotation processing happens in a sequence of rounds." In each round, the compiler asks the processors to handle the annotations found on the source and class files of the previous round.[^4] The `Filer` interface "supports the creation of new files by an annotation processor", and the compiler processes the generated source files in a later round.[^5] JSR 269 defined this API.[^6]

C# has the same idea in source generators, which "enable compile time metaprogramming, that is, code that can be created at compile time and added to the compilation".[^10] Kotlin has KSP: "With the KSP API, you can create processors that inspect static information about your source code and generate new code from it. The most common use case is generating code based on annotations."[^25] Rust's derive macros have the same role. A derive macro receives the token stream of the item, and the items that it returns are appended after the input item.[^29] See [[syntactic-macros]].

None of the examples on this page runs a processor. A processor must be compiled before the code that it processes, and each example here is a single file.

### Reading annotations at run time

A program that reads `RUNTIME` annotations uses reflection. The java example `runtime` collects the methods marked `@Command`, sorts them by name and calls them. The csharp example `custom` does the same with `GetCustomAttribute`. Go has no annotations, but a struct tag is a string that "becomes an attribute for all the fields in the corresponding field declaration". "The tags are made visible through a reflection interface and take part in type identity for structs but are otherwise ignored."[^21] The package `encoding/json` reads the key `json` in the go example `tags`.

### Python decorators

Python's `@` syntax looks like an annotation, but its meaning is different. "Decorator expressions are evaluated when the function is defined, in the scope that contains the function definition. The result must be a callable, which is invoked with the function object as the only argument. The returned value is bound to the function name instead of the function object."[^16] So this code:

```python norun reason=fragment
@f1(arg)
@f2
def func(): pass
```

is "roughly equivalent to" `func = f1(arg)(f2(func))`.[^16] Classes take decorators in the same way.[^16] A decorator is a higher-order function, see [[higher-order-functions]]. It can register the function, as `command` does in the python example `registry`. It can return a wrapper that adds behavior around each call, which is the [[decorator-pattern]] applied to a function. It can also change a class: `@dataclass` "examines the class to find fields", where "A field is defined as a class variable that has a type annotation", and it adds methods such as `__init__` and `__repr__`.[^19]

A decorator that returns a wrapper replaces the function. The name, the docstring and the other attributes that tools read are now those of the wrapper. The python example `no-wraps` prints `wrapper None` for the name and the docstring. `functools.wraps` copies `__module__`, `__name__`, `__qualname__`, `__annotations__`, `__type_params__` and `__doc__` from the original, and it adds `__wrapped__`, a reference to the original function.[^18] The python example `wraps` shows the fix.

The word annotation also has a meaning in Python, and it is not this one. A Python annotation is "A label associated with a variable, a class attribute or a function parameter or return value, used by convention as a type hint."[^20]

## In languages

| Language | Verdict | Form |
|---|---|---|
| Java | native | `@interface`, `@Target`, `@Retention`; annotation processors; reflection |
| C# | native | classes derived from `Attribute`, `AttributeUsage`; source generators; reflection |
| Kotlin | native | `annotation class`, `@Target`, `@Retention`; KSP |
| Rust | native | `#[...]` and `#![...]`; `derive`; attributes from procedural macros |
| C++ | native | `[[...]]` with standard and vendor attributes |
| C | native | C23 `[[...]]` with standard and vendor attributes |
| TypeScript | native | decorators of the ECMAScript proposal |
| Python | idiom | decorators: functions applied when a definition runs |
| Go | idiom | struct tags read through `reflect` |
| JavaScript | absent | decorators are a proposal at stage 2.7 |
| PowerShell | native | `[CmdletBinding()]`, `[Parameter()]` and validation attributes |
| Bash | absent | no metadata on functions |
| SQL | n/a | no user-defined annotations |
| Regex | n/a | no declarations |
| HTML/CSS | n/a | HTML attributes are markup, not metadata on code |

**Java** annotations apply to declarations. An annotation whose interface is applicable in type contexts, such as with `@Target(ElementType.TYPE_USE)`, is a type annotation and applies to a type.[^1] An annotation on a local variable declaration is never kept in the class file, whatever its retention.[^1]

**C#** attributes can apply to assemblies, modules, types, members, parameters and return values. A target prefix such as `[assembly: ...]` or `[return: ...]` selects the element.[^7]

**Kotlin** annotations "are tags that you can use to attach metadata to elements in your code. Tools and frameworks process this metadata during compilation and runtime".[^24] No Kotlin example runs on this page, because kotlinc is not installed on the machine that recorded the examples.

**Rust** has inner attributes, `#![...]`, which apply to the item that contains them, and outer attributes, `#[...]`, which apply to the item that follows.[^11] The kinds are built-in attributes, attributes of procedural macros, helper attributes of derive macros and tool attributes.[^11] Lint attributes such as `deny` change the level of a lint, and the rust example `must-use` turns the lint `unused_must_use` into an error.[^13]

**C++** reserves attribute names that are not in a namespace for future standards, and "any such attribute-token that is not recognized by the implementation is ignored".[^14] An attribute that the standard defines but that is put on the wrong kind of entity makes the program ill-formed.[^14] **C** adopted the same syntax in C23 with the standard attributes `deprecated`, `fallthrough`, `maybe_unused`, `nodiscard`, `noreturn`, `_Noreturn`, `unsequenced` and `reproducible`.[^15]

**TypeScript** 5.0 implemented the decorators of the ECMAScript proposal. The older form needs the compiler flag `--experimentalDecorators`.[^26] No TypeScript example runs on this page, because Node.js type stripping does not transform decorators. **JavaScript** does not have decorators yet. The TC39 proposal is at stage 2.7, and it describes decorators as "functions called on classes, class elements, or other JavaScript syntax forms during definition".[^27] **PowerShell** advanced functions use attributes such as `[CmdletBinding()]` and `[Parameter()]` on functions and parameters.[^28]

## Variations and related ideas

- **Naming conventions.** Before annotations, Java tools used patterns in names, such as `getFoo` and `setFoo` in JavaBeans.[^3] The compiler cannot check such a pattern.
- **Doc comment tags.** JSR 175 considered "a doc comment tag @meta" as one possible syntax for the metadata facility.[^3]
- **Macros.** A Rust attribute macro receives the tokens of the item. "The item to which the attribute is applied is replaced by the zero or more items in the returned TokenStream."[^29] An annotation processor in Java can only add files.[^5] See [[syntactic-macros]].
- **Reflection.** An annotation with run-time retention is useful only with reflection. See [[runtime-reflection]].

## Pitfalls

### The default retention hides the annotation

A Java annotation without `@Retention` has the retention `CLASS`, and reflection cannot see it.[^1][^2] A framework that looks for it finds nothing and reports no error. The java example `default-retention` prints `commands found: 0`. Add `@Retention(RetentionPolicy.RUNTIME)` to each annotation that a program reads at run time.

### A misspelled attribute

C++ ignores an attribute that the implementation does not recognize.[^14] The cpp example `typo` writes `[[nodiscrd]]`, and the program compiles and runs without the check. In Java, the name in an annotation "must name an accessible annotation interface (§6.6), or a compile-time error occurs".[^1]

### A malformed struct tag

A Go struct tag is a free string. By convention, tag strings are a concatenation of optionally space-separated `key:"value"` pairs.[^22] If a tag does not have this format, the value that `Get` and `Lookup` return is unspecified.[^22] The go example `bad-tag` has a space after the colon, and `encoding/json` uses the field name `Name`. The analyzer `structtag` of `go vet` checks "that struct field tags conform to reflect.StructTag.Get".[^23]

### A wrapper that hides the function

A Python decorator that returns a different function replaces the name and the docstring that tools see. Use `functools.wraps` on each wrapper.[^18]

### Bounds from derive

A derived implementation adds bounds on each type parameter. The Reference example shows `impl<T: PartialEq> PartialEq for Foo<T>`.[^12] The rust example `derive-bound` derives `Clone` for `Id<T>` with a `PhantomData<T>` field. The derived `impl` requires `T: Clone`, so `Id<Meters>` has no `clone` method. Write the `impl` by hand when the bound is wrong.

### Behavior that the reader cannot see

An annotation can change behavior a long way from the code that it marks. A processor generates classes that are not in the source tree, and a framework calls methods that no code calls directly. Keep the number of annotations that change behavior small, and document what reads each one.

## Cost

- **Compile time.** Processors, source generators and derive macros run in each build. They add time to compilation.
- **Run time.** Annotations with run-time retention are data in the binary. Reading them uses reflection, with the costs that [[runtime-reflection]] describes.
- **Size.** Metadata kept for run time increases the size of the program.
- **Clarity.** A short annotation can stand for a large amount of generated or framework code. The reader must know the processor or the framework to know what the program does.

## History

- **2003.** PEP 318 proposed decorators for Python functions and methods. It was created on 5 June 2003, for Python 2.4.[^17]
- **2004.** JSR 175, with Joshua Bloch as specification lead, reached final release on 30 September 2004.[^3] The Java API marks `java.lang.annotation.RetentionPolicy` with "Since: 1.5".[^2]
- **2006.** JSR 269, the Pluggable Annotation Processing API, reached final release on 11 December 2006.[^6] The Java API marks `Processor` with "Since: 1.6".[^4]
- **C++11.** The C++ standard lists, as a change from C++ 2003, that "Names of attribute identifiers may not be used as macro names". C++ 2003 code that defines `carries_dependency` or `noreturn` as a macro is invalid.[^14]
- **2023.** TypeScript 5.0, announced on 16 March 2023, implemented the decorators of the ECMAScript proposal.[^26]
- **C23.** C added the attribute feature with `deprecated`, `fallthrough`, `maybe_unused`, `nodiscard`, `noreturn`, `unsequenced` and `reproducible`.[^15]

## References
