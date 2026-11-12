---
tag: FE1QH3
kind: concept
slug: type
name: Type
aliases: [type, data type, datatype, primitive type, built-in type, user-defined type, static type, dynamic type, run-time type, value type, reference type, type name, type alias, newtype]
area: types/type-systems
level: core
summary: >-
  A type is a set of values together with the operations that are valid on them. It tells the compiler and the reader what a piece of data means, how it is stored and what a program can do with it.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1956
  people: [John Backus]
  note: The 1956 FORTRAN manual had two types of variable, fixed point and floating point, and the first letter of the name gave the type. ALGOL 60 added type declarations for integer, real and Boolean variables.
see_also: [static-and-dynamic-typing, type-conversion, subtyping, sum-types, parametric-polymorphism, integer-representation]
same_as:
  wikidata: Q190087
  wikipedia: Data_type
sources:
- url: https://en.wikipedia.org/wiki/Data_type
  title: Data type, Wikipedia
  locator: Introduction; Concept
- url: https://docs.python.org/3/reference/datamodel.html
  title: Data model, The Python Language Reference
  locator: 3.1 Objects, values and types
- url: https://go.dev/ref/spec
  title: The Go Programming Language Specification
  locator: Types
- url: https://doc.rust-lang.org/reference/types.html
  title: Types, The Rust Reference
  locator: Introduction
- url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-4.html
  title: The Java Language Specification, Java SE 25 Edition, Chapter 4
  locator: Chapter 4 introduction
- url: https://en.cppreference.com/w/cpp/language/type.html
  title: Type, C++ reference, cppreference.com
  locator: Introduction; Static type; Dynamic type
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Guide/Data_structures
  title: JavaScript data types and data structures, MDN Web Docs
  locator: Dynamic and weak typing; Primitive values
- url: https://learn.microsoft.com/en-us/dotnet/csharp/fundamentals/types/
  title: The C# type system, Microsoft Learn
  locator: Introduction
- url: https://archive.computerhistory.org/resources/text/Fortran/102649787.05.01.acc.pdf
  title: The FORTRAN Automatic Coding System for the IBM 704 EDPM, Programmer's Reference Manual, IBM, 1956
  locator: Chapter 2, Constants, Variables, and Subscripts
- url: https://www.masswerk.at/algol60/report.htm
  title: Revised Report on the Algorithmic Language ALGOL 60
  locator: 2.8 Values and types; 5.1 Type declarations
---

## Idea

The memory of a computer holds bits, and the same 32 bits can be an integer, a floating-point number or four characters. A type says which of these the bits are. It gives a set of values, such as the integers from -2147483648 to 2147483647, and the operations that are valid on them, such as `+` and `<`. With types, a program says `42` and `"42"` and means two different things, and the language can reject `len(42)` because an integer has no length.

```python run id=idea
# A type is a set of values and the operations on them.
x = 42
print(type(x), type(x).__name__)
print(type(3.5).__name__, type("hi").__name__, type([1]).__name__, type(None).__name__)

# The type decides which operations work.
print(len("hello"))
try:
    len(42)
except TypeError as e:
    print("TypeError:", e)

# The type decides what an operation means.
print(2 + 3, "2" + "3", [2] + [3])


# A class makes a new type, and a type is an object too.
class Celsius:
    def __init__(self, degrees):
        self.degrees = degrees


t = Celsius(21)
print(type(t).__name__, isinstance(t, Celsius), type(Celsius).__name__)

# The object keeps its type; the name can refer to an object of another type.
x = "now a string"
print(type(x).__name__)
```

```output for=idea
<class 'int'> int
float str list NoneType
5
TypeError: object of type 'int' has no len()
5 23 [2, 3]
Celsius True type
str
```

Each value has a type, and `type()` returns it. `len("hello")` works and `len(42)` raises `TypeError`, because the type of the value decides which operations work. `+` adds integers but joins strings and lists, because the type also decides what an operation means. The class `Celsius` makes a new type, and the class is itself an object whose type is `type`. At the end, the name `x` refers to a string, but the object `42` is still an `int`: the type belongs to the object, not to the name.

## How it works

**Values and operations.** A data type, or simply a type, "is a collection or grouping of data values, usually specified by a set of possible values, a set of allowed operations on these values, and/or a representation of these values as machine types"[^1]. The language references say the same thing in their own words. In Python, "An object’s type determines the operations that the object supports" and "also defines the possible values for objects of that type"[^2]. In Go, "A type determines a set of values together with operations and methods specific to those values"[^3]. In Rust, "The type of a value defines the interpretation of the memory holding it and the operations that may be performed on the value"[^4].

**Representation.** A type also fixes how a value is stored. "An explicit data type declaration typically allows the compiler to choose an efficient machine representation"[^1]. An `i64` in Rust takes 8 bytes and a `u8` takes 1, a `char` takes 4 because it holds any Unicode scalar value, and `()` takes none because it has only one value. The bits `0x3F800000` are the integer 1065353216 and the `float` 1.0. See [integer representation](/integer-representation/) and [floating-point arithmetic](/floating-point-arithmetic/).

**Built-in and user-defined types.** Each language has built-in types for numbers, characters, Booleans and text. Java divides its types "into two kinds: primitive types and reference types", where the primitive types "are the boolean type and the numeric types"[^5]. A program makes new types from old ones with a class, a struct, a record, an enum or a type declaration. A new type can have the same representation as an old one and still be a different type: Go `type Celsius float64` needs a conversion before it mixes with a `float64`, and a Rust tuple struct `Meters(f64)` does not take the operations of `f64`. A type alias, such as Go `type Temp = float64`, C `typedef` or C++ `using`, is only a second name for the same type.

**Types of expressions and types of values.** A type can belong to an expression in the program text, or to a value at run time. C++ says that "The type of an expression that results from the compile-time analysis of the program is known as the static type of the expression", and that when an expression refers to a polymorphic object, "the type of its most derived object is known as the dynamic type"[^6]. In Java, the variable `Object o = "hello"` has the static type `Object`, and its value has the class `String`. In Python and JavaScript, a name has no type, and only the values have one. MDN says that "Variables in JavaScript are not directly associated with any particular value type, and any variable can be assigned (and re-assigned) values of all types"[^7]. See [static and dynamic typing](/static-and-dynamic-typing/).

**Checks.** The language uses types to reject operations that make no sense. C# is "a strongly typed language. Every variable, constant, and expression has a type. The compiler enforces type safety by checking that every operation in your code is valid for the types involved"[^8]. Python makes the check when the operation runs, so `1 + "b"` raises `TypeError`. JavaScript "allows implicit type conversion when an operation involves mismatched types, instead of throwing type errors"[^7], so `1 + "2"` is `"12"`. See [type conversion](/type-conversion/).

**Types at run time.** Many languages keep a description of each type when the program runs. Python `type(x)` returns a type object, and "Like its identity, an object’s type is also unchangeable"[^2]. Java has `getClass()`, C# has `GetType()` and `typeof`, Go has `reflect.TypeOf` and type switches, and C++ has `typeid` for polymorphic types. C keeps no type information at run time. Rust keeps a table of methods for each trait object, and the `std::any::Any` trait gives a type identifier.

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | native | each object has a type; `type()`; classes make types; annotations are not checked at run time |
| JavaScript | native | 7 primitive types and Object; `typeof`; variables have no type |
| TypeScript | native | static types over JavaScript values; structural types; erased before the program runs |
| Java | native | primitive and reference types; classes, records, enums; `getClass()` |
| Kotlin | native | classes, data classes, value classes; nullable types such as `String?` |
| C# | native | value types and reference types; `struct`, `class`, `record`; `typeof` and `GetType()` |
| Go | native | defined types and aliases; zero values; interface values hold a dynamic type |
| Rust | native | primitive, compound and user-defined types; newtypes; `size_of` |
| C | native | basic, derived and incomplete types; `typedef` aliases; `_Generic` |
| C++ | native | fundamental and compound types; `decltype`, `auto`, `typeid`, type traits |
| Bash | idiom | every value is a string; `declare -i` and `declare -a` change how a variable is used |
| PowerShell | native | .NET types; `[int]` casts and type constraints on variables |
| SQL | native | a type for each column, such as `INTEGER`, `VARCHAR(n)` and `DATE`; `CAST` |
| Regex | n/a | patterns match text and have no types of values |
| HTML | idiom | the `type` attribute of `input` selects the kind of data a field accepts |
| CSS | idiom | each property accepts values of given types, such as `<length>` and `<color>` |

Every general-purpose language has types, so the verdict is native for all of them except Bash, where all values are strings. The languages differ in whether the type belongs to the name or to the value, in when they check types, and in how much type information they keep at run time.

The example files show these differences. Python prints the type of 11 values, shows that `bool` is a subtype of `int` and that annotations are not checked, and makes a class with `type()`. JavaScript prints `typeof` for each primitive type, including `typeof null`, and shows that a variable holds values of many types. Java shows the ranges of primitive types, the cache of boxed `Integer` values, the static type and the class of a value, and a record. C# copies a struct and a class, and compares `typeof` with `GetType()`. Go shows zero values, a defined type and an alias, and a type switch on interface values. Rust prints the inferred type of literals and the size of types, and shows that `Option<&u8>` is as small as `&u8`. C uses `_Generic` to name the type of an expression and reads the bits of a `float` as an integer. C++ uses `decltype`, `typeid`, an `enum class` and type traits.

## Variations and related ideas

- **Primitive and composite types.** A primitive type, such as `int` or `bool`, has no parts. A composite type, such as an array, a struct or a tuple, is made from other types.
- **Value types and reference types.** In C#, assignment copies the value of a `struct` but only the reference to a `class` object. Java has this split between primitive and reference types, and Go and Rust let the program choose with pointers and references.
- **Nominal and structural types.** In Java and C#, two classes with the same fields are different types because they have different names. In TypeScript and in Go interfaces, a value fits a type when it has the right shape. See [interfaces](/interfaces/).
- **Algebraic data types.** Product types such as tuples and records combine values, and sum types such as Rust `enum` hold one of several alternatives. See [sum types](/sum-types/).
- **Types with parameters.** `List<T>` and `Vec<T>` are families of types, one for each type `T`. See [parametric polymorphism](/parametric-polymorphism/).
- **Types that are not data.** Rust has `!`, "a type with no values"[^4], for code that never returns, and many languages have a unit type, such as `()` or `void`, that carries no information.

## Pitfalls

- **Treating a name as a type.** In Python and JavaScript, a name can refer to a value of any type, so a check of the type in one place says nothing about the same name later.
- **Aliases that do not protect.** A C `typedef double celsius` or a Go alias lets a temperature mix with any `double`. Use a new type, such as a struct or a Go defined type, to make the compiler reject the mix.
- **`typeof null` in JavaScript.** "typeof null returns "object", so one has to use === null to test for null"[^7].
- **Boxed values and identity.** In Java, `==` on two `Integer` objects compares references. It is true for small values such as 127, which come from a cache, and false for 1000.
- **Annotations that are not checks.** Python type hints and TypeScript types do not exist when the program runs, so wrong data from a file or a network passes without an error.
- **Sizes that differ between platforms.** `long` in C is 4 bytes on 64-bit Windows and 8 bytes on 64-bit Linux. Use `int64_t` and the other exact-width types when the size matters.

## Cost

A type has no cost of its own in a compiled language, because the compiler uses it to choose the machine instructions and the layout of the data. A language that keeps types at run time pays for it: each Python object carries a pointer to its type, each Java object has a header with its class, and an interface value in Go holds a pointer to a table of the dynamic type. Boxing a primitive value, as with `Integer` in Java, adds an allocation and a level of indirection.

## History

The 1956 FORTRAN manual had "Two types of variable": "fixed point (restricted to integral values) and floating point". "Fixed point variables are distinguished by the fact that their first character is I, J, K, L, M, or N"[^9]. The name gave the type, and a program had no way to declare it.

ALGOL 60 added declarations of type. Its report names three types, integer, real and Boolean, and says that they "basically denote properties of values"[^10]. It also says that "Type declarations serve to declare certain identifiers to represent simple variables of a given type"[^10]. Later languages added types that a program defines, such as records in Pascal and classes in Simula 67, so that a type could describe the data of the problem and not only the data of the machine.

## References
