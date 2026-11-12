---
tag: Z1Y171
kind: concept
slug: value
name: Value
aliases: [value, data value, r-value, rvalue, l-value, lvalue, value category, place expression, value expression, first-class value, primitive value, reference value, zero value, default value]
area: data-and-values/value-and-reference-semantics
level: core
summary: >-
  A value is a piece of data that a program computes with, such as 42, "hi" or a list. Expressions give values, variables hold them, and functions take and return them.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1967
  people: [Christopher Strachey]
  note: Strachey's lecture notes Fundamental Concepts in Programming Languages, given at Copenhagen in August 1967, named the L-value and the R-value of an expression, and made first-class values a common idea. Wikipedia says that CPL introduced the notion of l-values and r-values.
see_also: [type, binding, pointers-and-references, value-equality-and-identity, immutability, statements-and-expressions, higher-order-functions]
same_as:
  wikidata: Q1196827
  wikipedia: Value_(computer_science)
sources:
- url: https://en.wikipedia.org/wiki/Value_(computer_science)
  title: Value (computer science), Wikipedia
  locator: Introduction; Assignment, l-values and r-values
- url: https://go.dev/ref/spec
  title: The Go Programming Language Specification
  locator: Types; The zero value
- url: https://en.cppreference.com/w/c/language/value_category.html
  title: Value categories, C reference, cppreference.com
  locator: Introduction; Lvalue expressions; Modifiable lvalue expressions
- url: https://en.cppreference.com/w/cpp/language/value_category.html
  title: Value categories, C++ reference, cppreference.com
  locator: Introduction; Primary categories
- url: https://doc.rust-lang.org/reference/expressions.html
  title: Expressions, The Rust Reference
  locator: Place expressions and value expressions
- url: https://docs.python.org/3/reference/datamodel.html
  title: Data model, The Python Language Reference
  locator: 3.1 Objects, values and types
- url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-4.html
  title: The Java Language Specification, Java SE 25 Edition, Chapter 4
  locator: 4.1 The Kinds of Types and Values; 4.3.1 Objects
- url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/builtin-types/value-types
  title: Value types, C# language reference
  locator: Introduction
- url: https://developer.mozilla.org/en-US/docs/Glossary/Primitive
  title: Primitive, MDN Web Docs Glossary
  locator: Introduction
- url: https://en.wikipedia.org/wiki/First-class_citizen
  title: First-class citizen, Wikipedia
  locator: Introduction; History
- url: https://en.wikipedia.org/wiki/Fundamental_Concepts_in_Programming_Languages
  title: Fundamental Concepts in Programming Languages, Wikipedia
  locator: Introduction
---

## Idea

A program computes with data: numbers, text, lists, records and functions. Each such piece of data is a value. An expression such as `3 * 4 + 1` is evaluated to a value, a variable holds a value, and a function takes values as arguments and returns a value. Most other ideas about data, such as types, variables, references and equality, are ideas about values and the places that hold them.

```python run id=idea
# Evaluation turns an expression into a value.
x = 3 * 4 + 1
print(x, type(x).__name__)


# A value can be stored, passed as an argument and returned.
def double(v):
    return v * 2


print(double(x), double("ab"), double([0]))

# Two expressions can give equal values that are different objects.
a = [1, 2]
b = [1] + [2]
print(a == b, a is b)

# On the left of = the name means a location; on the right it means the value in it.
count = 0
count = count + 1
print(count)

# Functions are values too, so a list can hold them.
steps = [abs, str, len]
print([f(-5) if f is not len else f("value") for f in steps])
```

```output for=idea
13 int
26 abab [0, 0]
True False
1
[5, '-5', 5]
```

`3 * 4 + 1` gives the value `13`, whose type is `int`. The function `double` accepts an `int`, a `str` and a `list`, because `*` is defined for each of them. The lists `a` and `b` have equal values but are two objects. In `count = count + 1`, the name `count` on the right means the value that the variable holds, and the same name on the left means the variable itself. The last line stores three functions in a list and calls each of them, so functions are values as well.

## How it works

**Values and types.** "The members of a type are the values of that type"[^1]. The Go specification says that "A type determines a set of values together with operations and methods specific to those values"[^2]. So `13` is a value of `int`, `"abab"` is a value of `str`, and the operations that a program can apply to a value depend on its type. See [type](/type/).

**Location and contents.** Wikipedia says that "In languages with assignable variables, it becomes necessary to distinguish between the r-value (or contents) and the l-value (or location) of a variable"[^1]. The names come from the two sides of an assignment. In `x = y + 1`, the left side must name a place that can hold a value, and the right side must give a value. The language references classify expressions in this way:

- C: "Every expression belongs to one of three value categories: lvalue, non-lvalue object (rvalue), and function designator"[^3]. A "modifiable lvalue is any lvalue expression of complete, non-array type which is not const-qualified"[^3], so a `const` variable and an array are lvalues that an assignment cannot change.
- C++: "Each C++ expression (an operator with its operands, a literal, a variable name, etc.) is characterized by two independent properties: a type and a value category"[^4]. The three primary categories are prvalue, xvalue and lvalue[^4]. An xvalue, such as the result of `std::move(x)`, names an object whose resources can be moved.
- Rust: "Expressions are divided into two main categories: place expressions and value expressions"[^5], and "A place expression is an expression that represents a memory location"[^5]. Local variables, dereferences, field references and index expressions are place expressions[^5].

**Values and objects.** In many languages, a value lives in an object, and the object also has an identity. Python says that "Every object has an identity, a type and a value"[^6], and that "The value of some objects can change"[^6]. Two lists can have equal values and be different objects, which is the difference between `==` and `is`. See [equality and identity](/value-equality-and-identity/).

**Primitive values and reference values.** Java has "two kinds of data values that can be stored in variables, passed as arguments, returned by methods, and operated on"[^7]: primitive values, such as an `int`, and reference values. "The reference values (often just references) are pointers to these objects, and a special null reference, which refers to no object"[^7]. C# puts the line between types: "A variable of a value type contains an instance of the type. This behavior differs from a variable of a reference type, which contains a reference to an instance of the type"[^8]. In JavaScript, "a primitive (primitive value, primitive data type) is data that is not an object and has no methods or properties"[^9], and "All primitives are immutable"[^9]. When an assignment copies a reference value, the two variables refer to one object. See [pointers and references](/pointers-and-references/).

**Default values.** Some languages give a value to every variable that has no initializer. In Go, each element "is set to the zero value for its type: false for booleans, 0 for numeric types, "" for strings, and nil for pointers, functions"[^2], and for some other types. Java and C# give default values to fields, and C# `default(T)` gives the default value of a type. C gives no value to a local variable without an initializer, and a read of it gives an indeterminate value.

**First-class values.** A value is first class when the language lets a program use it in every usual way. Wikipedia says that a first-class citizen "is an entity which supports all the operations generally available to other entities. These operations typically include being passed as an argument, returned from a function, and assigned to a variable"[^10]. Numbers are first class in all the core languages. Functions are first class in Python, JavaScript, Go, Rust and C#, and C and C++ pass them through function pointers and function objects. See [higher-order functions](/higher-order-functions/).

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | native | every value is an object with an identity, a type and a value; `==` and `is` |
| JavaScript | native | seven primitive types and objects; primitives compare by value and objects by identity |
| TypeScript | native | the values of JavaScript; literal types such as `"on"` and `42` describe single values |
| Java | native | primitive values and reference values; `null`; boxing of primitives |
| Kotlin | native | all values are objects in the language; the compiler stores many of them as JVM primitives |
| C# | native | value types (`struct`, `enum`, `record struct`) and reference types; boxing; `default` |
| Go | native | values of each type, zero values, and pointer, slice and map values that refer to shared data |
| Rust | native | place expressions and value expressions; move and `Copy` |
| C | native | lvalues, modifiable lvalues and rvalues; struct values copy on assignment |
| C++ | native | lvalue, xvalue and prvalue; copy and move |
| Bash | idiom | each variable holds a string; arrays hold strings; arithmetic reads strings as integers |
| PowerShell | native | each value is a .NET object, and the pipeline passes objects |
| SQL | native | each column holds values of its type, and `NULL` marks a missing value |
| Regex | n/a | a pattern matches text; it has no values of its own |
| HTML | idiom | attribute values and text content are strings |
| CSS | idiom | each property takes values of given types, such as lengths, percentages and colors |

Every general-purpose language works with values, so the verdict is native for all of them except Bash, in which all values are strings. The languages differ in where a value lives, in whether an assignment copies the value or a reference to it, and in which kinds of value are first class.

The example files show these differences. Python shows that a list keeps its identity when its value changes, and that `+=` on an `int` makes a new object. JavaScript shows the result of `typeof` for each kind of value, and compares primitives by value and objects by identity. Java shows that a method can change the object that a reference value points to but not the variable of the caller, and shows `null` and boxing. C# copies a `struct` and shares a `class`, and shows that a boxed copy does not change. Go shows zero values, the copy of a struct, and a slice that shares its array. Rust shows place expressions, a move and a copy. C shows lvalues, a `const` lvalue, struct copy and a compound literal. C++ prints the value category of six expressions.

## Variations and related ideas

- **Value semantics and reference semantics.** With value semantics, an assignment copies the value, as with a C struct or a Go array. With reference semantics, an assignment copies a reference, as with a Java object or a Python list.
- **Mutable and immutable values.** An immutable value never changes, so sharing it is safe. See [immutability](/immutability/).
- **Values and expressions.** An expression is text in the program, and a value is what its evaluation gives. See [statements and expressions](/statements-and-expressions/).
- **Values and bindings.** A binding links a name to a value or to a location that holds a value. See [binding](/binding/).
- **Special values.** `null`, `nil`, `None` and `undefined` mark the absence of a value. `NaN` is a floating-point value that is not equal to itself.

## Pitfalls

- **A shared reference that looks like a copy.** `b = a` with a list, an array or an object makes a second name for one object, so a change through `b` shows through `a`. Copy the value when two independent values are necessary.
- **`==` on references.** In Java, `==` on two `String` objects compares references, so it can be `false` for equal text. Use `equals`.
- **Reading an indeterminate value.** In C and C++, a local variable without an initializer has no defined value. Initialize every variable.
- **A dangling reference to a temporary.** In C++, a reference or a `string_view` to a temporary value refers to a destroyed object after the end of the full expression.
- **`NaN` and `-0`.** In JavaScript, `NaN === NaN` is false and `0 === -0` is true. `Object.is` gives the other answer for both.

## Cost

A small value, such as an integer, a float or a Boolean, fits in a register, and an operation on it takes one instruction or a few. A copy of a large value, such as a big struct or an array, costs time in proportion to its size, which is why many languages pass large values by reference. Boxing a primitive value into an object costs an allocation and an indirection, and the garbage collector must later free the box.

## History

The distinction between the location and the contents of a variable came with the first languages that had assignment. Wikipedia says that "The notion of l-values and r-values was introduced by Combined Programming Language (CPL)"[^1]. Christopher Strachey gave a set of lectures in 1967 that became the notes Fundamental Concepts in Programming Languages, written "for the International Summer School in Computer Programming at Copenhagen in August 1967"[^11]. According to Wikipedia, the notes "introduced much programming language terminology still in use today"[^11], including R-value and L-value. Strachey also contrasted values that ALGOL lets a program use freely with values that it restricts: "In ALGOL, a real number may appear in an expression or be assigned to a variable", but a procedure "may only appear in another procedure call"[^10]. This is the source of the terms first-class and second-class.

Later, the move semantics of C++11 added the xvalue, an expiring value whose resources can be reused, and the categories glvalue and prvalue[^1].

## References
