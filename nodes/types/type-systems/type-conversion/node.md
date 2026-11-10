---
tag: NEW-type-conversion
kind: concept
slug: type-conversion
name: Type conversion
aliases: [type casting, casting, cast, coercion, type coercion, implicit conversion, explicit conversion]
area: types/type-systems
level: core
summary: >-
  A type conversion makes a value of one type from a value of another. Some keep the value, some lose part of it, and some fail. Languages differ in which ones need no cast.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1975
  people: [Adriaan van Wijngaarden, Barry Mailloux, John Peck, Kees Koster, Michel Sintzoff, Charles Lindsey, Lambert Meertens, Richard Fisker]
  note: The Revised Report on ALGOL 68 defined six coercions, such as widening and dereferencing, and the syntactic positions where each one can happen.
see_also: [static-and-dynamic-typing, integer-overflow, subtyping, null-references, exception-handling, type-erasure, parametric-polymorphism]
same_as:
  wikidata: Q1333515
  wikipedia: Type_conversion
sources:
  - url: https://docs.python.org/3/library/stdtypes.html#numeric-types-int-float-complex
    title: The Python Standard Library, Built-in Types
    locator: Numeric Types, int, float, complex (mixed arithmetic widens the narrower operand)
  - url: https://docs.python.org/3/library/functions.html#int
    title: The Python Standard Library, Built-in Functions
    locator: int (truncation toward zero for floats, and the accepted string form)
  - url: https://port70.net/~nsz/c/c11/n1570.html#6.3.1.1
    title: ISO/IEC 9899:201x Committee Draft N1570 (C11)
    locator: 6.3.1.1 Boolean, characters, and integers, paragraphs 2 and 3 (the integer promotions)
  - url: https://port70.net/~nsz/c/c11/n1570.html#6.3.1.8
    title: ISO/IEC 9899:201x Committee Draft N1570 (C11)
    locator: 6.3.1.8 Usual arithmetic conversions, paragraph 1
  - url: https://port70.net/~nsz/c/c11/n1570.html#6.3.1.3
    title: ISO/IEC 9899:201x Committee Draft N1570 (C11)
    locator: 6.3.1.3 Signed and unsigned integers, paragraphs 1 to 3
  - url: https://port70.net/~nsz/c/c11/n1570.html#6.3.1.4
    title: ISO/IEC 9899:201x Committee Draft N1570 (C11)
    locator: 6.3.1.4 Real floating and integer, paragraph 1
  - url: https://tc39.es/ecma262/2026/#sec-islooselyequal
    title: ECMA-262, ECMAScript 2026 Language Specification
    locator: 7.2.13 IsLooselyEqual
  - url: https://tc39.es/ecma262/2026/#sec-toprimitive
    title: ECMA-262, ECMAScript 2026 Language Specification
    locator: 7.1.1 ToPrimitive and 7.1.1.1 OrdinaryToPrimitive
  - url: https://tc39.es/ecma262/2026/#sec-stringtonumber
    title: ECMA-262, ECMAScript 2026 Language Specification
    locator: 7.1.4.1.1 StringToNumber and 7.1.4.1.2 StringNumericValue (white space alone gives +0)
  - url: https://tc39.es/ecma262/2026/#sec-applystringornumericbinaryoperator
    title: ECMA-262, ECMAScript 2026 Language Specification
    locator: 13.15.3 ApplyStringOrNumericBinaryOperator
  - url: https://tc39.es/ecma262/2026/#sec-toboolean
    title: ECMA-262, ECMAScript 2026 Language Specification
    locator: 7.1.2 ToBoolean
  - url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-5.html#jls-5.1.2
    title: The Java Language Specification, Java SE 25 Edition
    locator: 5.1.2 Widening Primitive Conversion
  - url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-5.html#jls-5.1.3
    title: The Java Language Specification, Java SE 25 Edition
    locator: 5.1.3 Narrowing Primitive Conversion
  - url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-5.html#jls-5.5
    title: The Java Language Specification, Java SE 25 Edition
    locator: 5.5 Casting Contexts (checked casts and ClassCastException)
  - url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-5.html#jls-5.2
    title: The Java Language Specification, Java SE 25 Edition
    locator: 5.2 Assignment Contexts (narrowing of a constant expression)
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/builtin-types/numeric-conversions#implicit-numeric-conversions
    title: Built-in numeric conversions (C# reference)
    locator: Implicit numeric conversions (the note on loss of precision)
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/builtin-types/numeric-conversions#explicit-numeric-conversions
    title: Built-in numeric conversions (C# reference)
    locator: Explicit numeric conversions (integral truncation, and double or float to an integral type)
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/statements/checked-and-unchecked#default-overflow-checking-context
    title: The checked and unchecked statements (C# reference)
    locator: Default overflow-checking context
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/programming-guide/types/casting-and-type-conversions#type-conversion-exceptions-at-run-time
    title: Casting and type conversions (C# Programming Guide)
    locator: Type conversion exceptions at run time (InvalidCastException)
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/operators/type-testing-and-cast#the-as-operator
    title: Type-testing operators and cast expressions (C# reference)
    locator: The as operator
  - url: https://go.dev/ref/spec#Conversions
    title: The Go Programming Language Specification
    locator: Conversions, the subsection Conversions between numeric types
  - url: https://go.dev/ref/spec#Numeric_types
    title: The Go Programming Language Specification
    locator: Numeric types (explicit conversions are required when numeric types are mixed)
  - url: https://go.dev/ref/spec#Conversions_to_and_from_a_string_type
    title: The Go Programming Language Specification
    locator: Conversions to and from a string type (an integer converts to a UTF-8 code point)
  - url: https://go.dev/ref/spec#Type_assertions
    title: The Go Programming Language Specification
    locator: Type assertions
  - url: https://doc.rust-lang.org/reference/expressions/operator-expr.html#r-expr.as.numeric
    title: The Rust Reference, Operator expressions
    locator: Type cast expressions, numeric casts (r-expr.as.numeric.int-truncation and r-expr.as.numeric.float-as-int)
  - url: https://doc.rust-lang.org/std/convert/trait.TryFrom.html
    title: Rust standard library, Trait std::convert::TryFrom
    locator: the description (no From conversion from i64 to i32) and the stability version 1.34.0
  - url: https://eel.is/c++draft/dcl.init.list#7
    title: Working Draft, Programming Languages C++
    locator: "[dcl.init.list] paragraph 7 (definition of a narrowing conversion) and paragraph 3 (the program is ill-formed)"
  - url: https://gcc.gnu.org/onlinedocs/gcc/C_002b_002b-Dialect-Options.html#index-Wnarrowing
    title: Using the GNU Compiler Collection, Options Controlling C++ Dialect
    locator: -Wno-narrowing (C++ and Objective-C++ only)
  - url: https://eel.is/c++draft/expr.dynamic.cast#10
    title: Working Draft, Programming Languages C++
    locator: "[expr.dynamic.cast] paragraph 10 (a failed cast gives a null pointer or throws std::bad_cast)"
  - url: https://eel.is/c++draft/expr.cast#4
    title: Working Draft, Programming Languages C++
    locator: "[expr.cast] paragraph 4 (the casts that the cast notation tries, in order)"
  - url: https://kotlinlang.org/docs/numbers.html#type-conversion
    title: Kotlin documentation, Numbers
    locator: Type conversion, and Mixed numeric expressions
  - url: https://kotlinlang.org/docs/typecasts.html#unsafe-cast-operator
    title: Kotlin documentation, Type checks and casts
    locator: as and as? cast operators
  - url: https://learn.microsoft.com/en-us/powershell/module/microsoft.powershell.core/about/about_type_conversion#operator-conversions
    title: about_Type_Conversion (PowerShell)
    locator: Operator conversions (the left operand usually decides the type)
  - url: https://port70.net/~nsz/c/c11/n1570.html#7.22.1.4
    title: ISO/IEC 9899:201x Committee Draft N1570 (C11)
    locator: 7.22.1.4 The strtol, strtoll, strtoul, and strtoull functions, paragraphs 7 and 8
  - url: https://port70.net/~nsz/c/c11/n1570.html#7.22.1.2
    title: ISO/IEC 9899:201x Committee Draft N1570 (C11)
    locator: 7.22.1 paragraph 1 and 7.22.1.2 The atoi, atol, and atoll functions
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/lang/Integer.html#parseInt(java.lang.String)
    title: Java SE 25 API, Class Integer
    locator: parseInt(String) (throws NumberFormatException)
  - url: https://tc39.es/ecma262/2026/#sec-parseint-string-radix
    title: ECMA-262, ECMAScript 2026 Language Specification
    locator: 19.2.5 parseInt ( string, radix )
  - url: https://pkg.go.dev/strconv#ParseInt
    title: Go standard library, package strconv
    locator: ParseInt (ErrSyntax and ErrRange) and Atoi
  - url: https://ecma-international.org/wp-content/uploads/ECMA-262_1st_edition_june_1997.pdf
    title: ECMA-262, ECMAScript, 1st edition, June 1997
    locator: 11.9 Equality Operators and 11.9.3 The Abstract Equality Comparison Algorithm
  - url: https://ecma-international.org/wp-content/uploads/ECMA-262_3rd_edition_december_1999.pdf
    title: ECMA-262, ECMAScript Language Specification, 3rd edition, December 1999
    locator: 11.9.4 The Strict Equals Operator ( === ) and 11.9.6 The Strict Equality Comparison Algorithm
  - url: https://www.open-std.org/jtc1/sc22/wg14/www/C99RationaleV5.10.pdf
    title: Rationale for International Standard, Programming Languages, C, Revision 5.10
    locator: 6.3.1.1 Booleans, characters, and integers (unsigned preserving and value preserving), pages 43 and 44
  - paper: doi:10.1007/BF00265077
    title: A. van Wijngaarden, B. J. Mailloux, J. E. L. Peck, C. H. A. Koster, M. Sintzoff, C. H. Lindsey, L. G. L. T. Meertens and R. G. Fisker, Revised report on the algorithmic language ALGOL 68, Acta Informatica 5, 1975
    locator: 6 Coercion (the six coercions and the sorts of syntactic position)
  - url: https://blog.rust-lang.org/2020/07/16/Rust-1.45.0/
    title: Announcing Rust 1.45.0, The Rust Blog, 16 July 2020
    locator: Fixing unsoundness in casts
  - url: https://www.gnu.org/software/bash/manual/html_node/Shell-Arithmetic.html
    title: Bash Reference Manual
    locator: 6.5 Shell Arithmetic (variables as operands, a null or unset variable is 0, a leading 0 means octal)
  - url: https://www.typescriptlang.org/docs/handbook/2/everyday-types.html#type-assertions
    title: The TypeScript Handbook, Everyday Types
    locator: Type Assertions (no run-time check)
  - url: https://go.dev/ref/spec#Operators
    title: The Go Programming Language Specification
    locator: Operators (an untyped constant converts to the type of the other operand)
  - url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-5.html#jls-5.1.7
    title: The Java Language Specification, Java SE 25 Edition
    locator: 5.1.7 Boxing Conversion
---

## Idea

A program often holds a value of one type and needs a value of another type. A count read from a file is text, and the code needs an integer. An integer takes part in a sum with a fraction, and the result must be a floating-point number. A type conversion makes the second value from the first.

Each language decides two things. First, which conversions happen implicitly, with no mark in the source. Second, what happens when the target type cannot hold the value. The Python program below shows four different answers.

```python run id=idea
print(1 + 2.5)
print(int(3.9), int(-3.9))
print(int("42") + 1)
try:
    print("1" + 1)
except TypeError as error:
    print("TypeError:", error)
```

```output for=idea
3.5
3 -3
43
TypeError: can only concatenate str (not "int") to str
```

In `1 + 2.5`, Python converts the integer to a float. In mixed arithmetic, the operand of the narrower type is widened to the type of the other operand.[^1] `int(3.9)` is an explicit conversion that drops the fraction. It truncates toward zero, so `int(-3.9)` is -3.[^2] `int("42")` does not change a representation. It parses text, and it can fail. `"1" + 1` raises `TypeError`, because Python never converts between text and numbers on its own. JavaScript gives `"11"` for the same expression (see Pitfalls).

Two words name the two directions. A cast is the syntax for an explicit conversion, such as `(int) x` in C and Java or `x as u8` in Rust. A coercion is a conversion that the compiler or the runtime inserts without a mark in the source. ALGOL 68 used "coercion" as a technical term in this sense.[^42]

## How it works

### Four kinds of conversion

- **Widening.** The target type can represent every value of the source type, or at least its magnitude. An `int` becomes a `long`, or a `float` becomes a `double`. Most statically typed languages allow it implicitly. Java lists 19 widening primitive conversions, and none of them throws an exception at run time.[^12]
- **Narrowing.** The target type cannot represent every value of the source type. A `long` becomes an `int`, or a `double` becomes an `int`. The language must say what happens to a value that does not fit. It can keep the low bits, saturate, throw an exception, or leave the behavior undefined. Most languages require a cast.
- **Reference conversion.** The value stays the same object, and only its static type changes. An upcast from `Circle` to `Shape` is always safe. A downcast from `Shape` to `Circle` needs a check at run time. [[subtyping]] explains why the upcast is safe.
- **Parsing and formatting.** Text becomes a number, or a number becomes text. This is a function with its own grammar, not a change of representation. Parsing can fail on input like `"42abc"`, so it needs a way to report an error.

### What the machine does

A conversion to a wider integer type adds bits at the top. The Go spec states the rule for all integer conversions. A signed value is sign extended, an unsigned value is zero extended, and then the value is truncated to the size of the result type.[^21] So a conversion to a narrower integer type keeps the low bits. The Rust Reference gives the example `1234u16 as u8 == 210`.[^25] The value 1234 is `0x04D2`, and `0xD2` is 210.

A conversion between an integer and a floating-point number changes the encoding, and the processor has instructions for it. C, Java, C#, Go and Rust all truncate toward zero when they convert a float to an integer.[^6][^13][^17][^21][^25] They differ on a value that does not fit (see the table in Pitfalls).

An upcast of a reference changes nothing at run time. A checked downcast compares the run-time type of the object with the target type. On a mismatch it throws an exception or gives a null value.

### C: integer promotions and the usual arithmetic conversions

C converts implicitly in all arithmetic. Two rules apply before an operation on integers.

1. **Integer promotions.** This rule applies to a type with a rank lower than `int`, such as `char`, `short` or `unsigned char`. A value of such a type becomes an `int` if `int` can represent all values of the original type. Otherwise it becomes an `unsigned int`.[^3] So when `a` and `b` are `unsigned char` values 200 and 100, `a + b` is 300 and not 44 (see the C example).
2. **Usual arithmetic conversions.** After the promotions, the two operands get a common type. If one operand is floating, the other converts to that floating type. If both are integers with the same signedness, the operand of lower rank converts to the higher rank. Suppose the unsigned operand has a rank greater than or equal to the rank of the signed operand. Then the signed operand converts to the unsigned type.[^4]

The last rule causes a classic bug, shown in Pitfalls: `-1 < 1u` is false. A negative value converts to an unsigned type by repeated addition of one more than the maximum value of that type, so `-1` becomes `UINT_MAX`.[^5] A conversion to a signed type that cannot hold the value gives an implementation-defined result or raises an implementation-defined signal.[^5] A conversion of a floating value whose integer part does not fit the integer type is undefined behavior.[^6]

### JavaScript: ToPrimitive, ToNumber and loose equality

JavaScript converts implicitly in almost every operator. The spec defines each conversion as an abstract operation.

- **ToPrimitive** turns an object into a primitive value. With the hint "number" it calls `valueOf` and then `toString`.[^8]
- **ToNumber** turns a primitive into a number. A string that is not a valid numeric literal becomes `NaN`. A string that is empty or holds only white space becomes 0.[^9]
- **ToBoolean** gives false for `undefined`, `null`, `+0`, `-0`, `NaN`, `0n` and the empty string. Every other value is true, including the string `"0"`.[^11]

The `+` operator first applies ToPrimitive to both operands. If either result is a string, it concatenates. Otherwise it converts both operands to numbers. A mix of a BigInt and a Number throws a `TypeError`.[^10] The other arithmetic operators always convert to numbers. So `"3" + 1` is `"31"`, and `"3" - 1` is 2.

The `==` operator follows the IsLooselyEqual algorithm.[^7] If both operands have the same type, it compares them strictly. `null` and `undefined` are equal to each other and to no other value. If one operand is a number and the other is a string, the string converts to a number. A boolean operand converts to a number first. An object operand goes through ToPrimitive. The `===` operator never converts.

### Java and C#: implicit widening, explicit narrowing

Java allows widening primitive conversions in assignments and method calls. A narrowing conversion needs a cast, with one exception. A constant expression of type `int` can be assigned to a `byte`, `short` or `char` variable if the value fits, as in `byte theAnswer = 42;`.[^15] A narrowing integer conversion keeps the low bits, so `(byte) 200` is -56. A conversion from `double` to `int` maps NaN to 0 and saturates a value that is too large or too small.[^13]

Three widening conversions in Java can lose precision: `int` to `float`, `long` to `float` and `long` to `double`. The result is the nearest value of the target type.[^12] A primitive value can also become an object. Boxing converts an `int` to an `Integer`.[^47]

C# follows the same pattern. The implicit numeric conversions can lose precision, but never an order of magnitude.[^16] An explicit conversion from a floating type to an integral type rounds toward zero. If the result is out of range, a checked context throws `OverflowException`, and an unchecked context gives an unspecified value.[^17] An explicit integral conversion in an unchecked context discards the extra high bits.[^17] Non-constant expressions are unchecked by default. The compiler option `CheckForOverflowUnderflow` changes the default.[^18]

### Go and Rust: no implicit numeric conversion

Go requires an explicit conversion when an expression or an assignment mixes different numeric types. `int32` and `int` are different types, even on a machine where they have the same size.[^22] Untyped constants are the exception. If one operand is an untyped constant and the other is not, the constant converts to the type of the other operand.[^46]

Rust has no implicit numeric conversion either. It offers two explicit forms.

- **`as`** is a primitive cast that never fails. Between integers it truncates or extends. From a float to an integer it rounds toward zero, saturates a value out of range, and maps NaN to 0.[^25]
- **`From` and `TryFrom`** are traits. `From` is for conversions that cannot fail, such as `i32` to `i64`. `TryFrom` is for conversions that can fail, and it returns a `Result`. Its documentation gives the example that `From` cannot convert an `i64` into an `i32`.[^26]

### C++: named casts and narrowing in braces

C++ keeps the conversions of C. It adds four named casts: `static_cast`, `dynamic_cast`, `const_cast` and `reinterpret_cast`. The C cast notation `(T) x` tries a list of these in order: `const_cast`, `static_cast`, `static_cast` then `const_cast`, `reinterpret_cast`, and `reinterpret_cast` then `const_cast`.[^30] The first interpretation in the list is used. A reader of `(T) x` cannot see which one it is.

`dynamic_cast` checks a downcast at run time. A failed cast to a pointer type gives a null pointer. A failed cast to a reference type throws `std::bad_cast`.[^29]

Initialization with braces, called list-initialization, rejects narrowing conversions.[^27] A narrowing conversion includes a conversion from a floating type to an integer type. It also includes a conversion between integer types where the target cannot represent all values of the source. A constant source whose value fits is allowed.[^27] GCC reports a narrowing from a constant as an error, and a narrowing from a non-constant as a warning.[^28]

### Checked casts on references

Java checks a cast to a reference type at run time and throws `ClassCastException` when the object is not of the target type.[^14] C# throws `InvalidCastException`.[^19] The C# `as` operator returns `null` instead of throwing.[^20] Kotlin has the same pair. `as` throws `ClassCastException`, and `as?` returns `null`.[^32] In Go, `x.(T)` panics when `x` does not hold a `T`, and the form `v, ok := x.(T)` sets `ok` to false.[^24]

The TypeScript `as` is different. It is a type assertion, and the compiler removes it. There is no check at run time, so a wrong assertion gives no exception and no `null`.[^45]

## In languages

The example files show the main conversions in each language. The table gives the verdict for each tier 1 language.

| Language | Verdict | Implicit numeric conversion | Explicit forms |
|---|---|---|---|
| Python | native | `int` to `float` to `complex` in arithmetic[^1] | `int()`, `float()`, `str()` |
| JavaScript | native | in almost every operator, and in `==`[^7][^10] | `Number()`, `String()`, `parseInt` |
| TypeScript | native | as JavaScript at run time | as JavaScript; `as` checks nothing[^45] |
| Java | native | widening[^12] | `(T) x`, checked for references[^14] |
| C | native | promotions and usual arithmetic conversions[^3][^4] | `(T) x`, `strtol` |
| C++ | native | as C; braces reject narrowing[^27] | `static_cast`, `dynamic_cast` and two more |
| C# | native | widening[^16] | `(T) x`, `checked`, `as`, `int.Parse` |
| Go | native | none, except for untyped constants[^22][^46] | `T(x)`, `x.(T)`, `strconv` |
| Rust | native | none | `as`, `From`, `TryFrom`, `parse`[^25][^26] |
| Kotlin | native | none in assignments and arguments[^31] | `toInt()`, `toDouble()`, `as`, `as?` |
| PowerShell | native | the left operand usually decides the type[^33] | `[int]$x` |
| Bash | native | a string is a number in arithmetic[^44] | `$(( ))` |
| SQL | native | the rules differ between engines | `CAST(x AS T)` |
| Regex | n/a | a pattern has no types | |
| HTML/CSS | n/a | markup and style rules have no conversions between types | |

Kotlin numeric types are not subtypes of one another. A function that expects a `Double` does not accept an `Int`, and the code must call a conversion function such as `toDouble()`. In arithmetic, Kotlin does combine different numeric types. The sum of an `Int` and a `Long` is a `Long`.[^31]

PowerShell converts in the other direction. The left operand of an operator usually decides the type, and PowerShell converts the right operand to it. `10 - ' 9 '` is 1, and `'10' + '2'` is `'102'`.[^33]

Bash stores every value as a string. Inside an arithmetic expression, the value of a variable is evaluated as an arithmetic expression. A null or unset variable is 0, and a constant with a leading 0 is octal.[^44] So a typo in a variable name gives 0 and no error (see the Bash example).

## Variations and related ideas

- **Coercion and subtyping.** An upcast keeps the same object and changes only its static type. A widening conversion makes a value with a different representation. Both are implicit in many languages, but only the first is subtyping. [[subtyping]] covers the first.
- **User-defined conversions.** C++ converting constructors and conversion functions, C# `implicit` and `explicit` operators, and the Rust `From` trait let a type define its own conversions.[^19][^26][^30] An implicit user-defined conversion has the same risk as a built-in one. A reader cannot see it at the call.
- **Boxing.** A conversion from a primitive value to an object, such as `int` to `Integer` in Java, allocates or reuses an object.[^47] Generics in Java need it, because a type argument must be a reference type. [[type-erasure]] explains why.
- **Truthiness.** A condition converts its value to a boolean. Python and JavaScript have rules for every type, and the rules differ: `[]` is false in Python and true in JavaScript.[^11] [[null-references]] covers the related test for a missing value.
- **Type assertions.** A TypeScript `as` or an unchecked Java cast of a generic type changes only what the compiler believes.[^45] No conversion happens, and an error shows later.
- **Checked and saturating arithmetic.** The same choices that a narrowing conversion makes also apply to an arithmetic result that does not fit. [[integer-overflow]] covers them.

## Pitfalls

### Loose equality converts both sides

In JavaScript, `==` converts its operands before it compares them. A value from a form field is a string, and many strings are loosely equal to 0.

```javascript wrong id=loose-equality
function isEmptyCart(quantity) {
  return quantity == 0;
}

console.log(isEmptyCart(0), isEmptyCart(""), isEmptyCart(" "), isEmptyCart("0"), isEmptyCart([]));
console.log([] == ![]);
```

```output for=loose-equality
true true true true true
true
```

The empty string and a string of white space convert to 0 under ToNumber.[^9] The array `[]` goes through ToPrimitive to `""` and then to 0.[^8] In `[] == ![]`, the right side is `false`, because every object is true under ToBoolean.[^11] Then both sides convert to 0. The fix is to convert once, check the result, and compare with `===`.

```javascript run id=strict-equality
function isEmptyCart(input) {
  const quantity = input.trim() === "" ? NaN : Number(input);
  if (!Number.isInteger(quantity)) {
    return "not a number";
  }
  return quantity === 0;
}

console.log(isEmptyCart("0"), isEmptyCart(""), isEmptyCart("3"));
```

```output for=strict-equality
true not a number false
```

### The plus operator joins strings

In JavaScript, `+` concatenates if either operand is a string. The other arithmetic operators convert to numbers.[^10]

```javascript wrong id=plus-joins
const price = "19.99";
const shipping = 5;
console.log(price + shipping);
console.log(price - 1);
```

```output for=plus-joins
19.995
18.99
```

The first line prints the text `19.995`. The fix is to convert at the edge of the program, where the text comes in: `Number(price) + shipping` is 24.99. TypeScript does not help here when the type of `price` is wrong. A value from `JSON.parse` has the type `any`, and an assertion to `number` does not convert it (see the TypeScript example).[^45]

### A negative number meets an unsigned number

In C, the usual arithmetic conversions turn a signed operand into an unsigned one when the unsigned type has the same or a higher rank.[^4] `strlen` returns a `size_t`, which is unsigned.

```c wrong id=signed-unsigned
#include <stdio.h>
#include <string.h>

int main(void) {
    const char *name = "Ada";
    int offset = -1;
    if (offset < strlen(name)) {
        printf("offset %d is inside the string\n", offset);
    } else {
        printf("offset %d is past the end\n", offset);
    }
    return 0;
}
```

```output for=signed-unsigned
offset -1 is past the end
```

The value -1 converts to the largest `size_t` value, so the test is false.[^5] The fix is to test the sign first and then compare with an explicit conversion: `offset >= 0 && (size_t)offset < strlen(name)`. GCC and Clang warn about this comparison with `-Wsign-compare`. Go and Rust reject the comparison, because they have no implicit conversion between signed and unsigned types.[^22]

### A narrowing cast keeps the low bits

A cast to a narrower integer type succeeds for every value. It keeps the low bits and gives a wrong value without an error.[^13][^17][^25]

```rust wrong id=as-truncates
fn main() {
    let requested: i64 = 300;
    let level = requested as u8;
    println!("volume level {level}");
}
```

```output for=as-truncates
volume level 44
```

300 is `0x12C`, and the low byte `0x2C` is 44. The fix in Rust is `TryFrom`, which returns an error for a value that does not fit.[^26]

```rust run id=try-from
fn main() {
    for requested in [200_i64, 300] {
        match u8::try_from(requested) {
            Ok(level) => println!("volume level {level}"),
            Err(error) => println!("{requested}: {error}"),
        }
    }
}
```

```output for=try-from
volume level 200
300: out of range integral type conversion attempted
```

C# has the same problem in its default unchecked context. A cast from `long` to `int` discards the high bits.[^17]

```csharp wrong timeout=30 id=unchecked-cast
long bytes = 3_000_000_000;
int size = (int)bytes;
Console.WriteLine(size);
```

```output for=unchecked-cast
-1294967296
```

The fix is a checked conversion. Then the cast throws `OverflowException` at the point of the error, not later.[^17]

```csharp fails expect=exception:OverflowException timeout=30 id=checked-cast
long bytes = 3_000_000_000;
int size = checked((int)bytes);
Console.WriteLine(size);
```

The languages differ on a float that does not fit an integer type:

| Language | `int` of a float out of range | NaN to `int` |
|---|---|---|
| C, C++ | undefined behavior[^6] | undefined behavior[^6] |
| Java | saturates to the minimum or maximum[^13] | 0[^13] |
| C# | unspecified value, or `OverflowException` when checked[^17] | as out of range[^17] |
| Go | implementation-dependent[^21] | implementation-dependent[^21] |
| Rust | saturates[^25][^43] | 0[^25] |

### A widening conversion can lose precision

Java calls `int` to `float` a widening conversion, and it happens without a cast. A `float` cannot hold every `int`, so the result is the nearest `float`.[^12]

```java wrong id=float-precision
public class Main {
    public static void main(String[] args) {
        int id = 16_777_217;
        float key = id;
        System.out.println(id + " -> " + (int) key);
    }
}
```

```output for=float-precision
16777217 -> 16777216
```

The fix is to keep identifiers and counts in an integer type. If a floating type is necessary, `double` holds every `int` exactly. C# has the same rule. Its implicit conversions from `int` to `float` and from `long` to `double` can lose precision.[^16]

### A downcast fails at run time

A cast to a subtype compiles, and the check happens when the program runs. In Java, a failed check throws `ClassCastException`.[^14]

```java fails expect=exception:ClassCastException id=bad-downcast
import java.util.List;

public class Main {
    public static void main(String[] args) {
        List<Object> values = List.of(1, "2", 3);
        int total = 0;
        for (Object value : values) {
            total += (Integer) value;
        }
        System.out.println(total);
    }
}
```

The string `"2"` is not an `Integer`, and a cast cannot turn it into one. The fix is to test the type and convert each case on its own. A string needs a parse, not a cast.

```java run id=checked-downcast
import java.util.List;

public class Main {
    public static void main(String[] args) {
        List<Object> values = List.of(1, "2", 3);
        int total = 0;
        for (Object value : values) {
            if (value instanceof Integer n) {
                total += n;
            } else if (value instanceof String s) {
                total += Integer.parseInt(s);
            }
        }
        System.out.println(total);
    }
}
```

```output for=checked-downcast
6
```

`Integer.parseInt` can also fail. It throws `NumberFormatException` when the string does not contain a parsable integer.[^36]

### Go needs an explicit conversion

Go does not convert between numeric types implicitly, even when no value can be lost.[^22]

```go fails expect=compile id=mixed-types
package main

import "fmt"

func main() {
	var count int32 = 3
	var total int64 = 10
	fmt.Println(total + count)
}
```

The compiler reports `invalid operation: total + count (mismatched types int64 and int32)`. The fix is `total + int64(count)`. The rule removes the C bugs of implicit conversion, and the price is a conversion at each place where types meet.

### Braces reject narrowing in C++

In C++, list-initialization makes a narrowing conversion ill-formed.[^27]

```cpp fails expect=compile id=brace-narrowing
#include <print>

int main() {
    int count{3.5};
    std::println("{}", count);
}
```

The compiler stops at `int count{3.5}`. The form `int count = 3.5;` compiles and gives 3. Braces are a way to make the compiler check each initialization. When the truncation is wanted, `static_cast<int>(3.5)` says so. For a value that is not a constant, GCC gives only a warning by default, so a build that uses `-Werror` catches both cases.[^28]

### A cast is not a parse

A conversion between a number and text is a parse or a format. It is not a cast. Go has a conversion from an integer to a string, and it gives the character with that code point.[^23]

```go wrong id=string-of-int
package main

import "fmt"

func main() {
	id := 65
	label := "item-" + string(rune(id))
	fmt.Println(label)
}
```

```output for=string-of-int
item-A
```

The fix is `strconv.Itoa(id)`, which gives `"item-65"`. In the other direction, `strconv.Atoi` parses text and returns an error value for bad input.[^38]

In C, `atoi` need not set `errno` on an error, and its behavior is undefined if the value does not fit.[^35] `strtol` reports where it stopped and sets `errno` to `ERANGE` on overflow.[^34] The C example uses it to reject `"42abc"`. In JavaScript, `Number("42abc")` is `NaN`, but `parseInt("42abc", 10)` is 42, because `parseInt` stops at the first character that is not a digit.[^9][^37]

## Cost

- **Integer to integer.** A conversion between integer sizes is a sign extension, a zero extension or a truncation of bits.[^21] Narrowing has no run-time check unless the language or the context adds one, as `checked` does in C#.[^17]
- **Float to integer.** Before Rust 1.45, `rustc` compiled `x as u8` to the LLVM instruction `fptoui`, which gives a poison value for an out-of-range input. Saturation adds checks. The Rust team wrote that the compiler can often remove them, and added the unsafe method `to_int_unchecked` to skip them.[^43]
- **Checked downcasts.** A cast to a subtype in Java or C#, and a `dynamic_cast` in C++, does a type test at run time.[^14][^19][^29] A `static_cast` to a derived class does no test, so a wrong cast is undefined behavior.
- **Parsing.** A parse reads each character and must report errors. A program should parse once, at its edge, and pass typed values inside.

## History

- **1975.** The Revised Report on ALGOL 68 defined six coercions: deproceduring, dereferencing, uniting, widening, rowing and voiding. It also defined sorts of syntactic position. In a strong position all six coercions can occur. In a soft position only deproceduring is allowed.[^42]
- **1989.** Before C89, C compilers used two rules for promoting `unsigned char` and `unsigned short`: unsigned preserving and value preserving. The C89 committee chose value preserving, although the UNIX C compilers had moved toward unsigned preserving. The Rationale calls this the most serious semantic change that the committee made to widespread practice.[^41]
- **1997.** The first edition of ECMAScript had only `==` and `!=`, defined by the abstract equality comparison algorithm.[^39]
- **1999.** The third edition of ECMAScript added `===` and `!==`, with a strict equality comparison that does not convert.[^40]
- **2011.** C++11 made a narrowing conversion inside braces ill-formed. GCC diagnoses it by default for C++11 and later.[^28]
- **2020.** Rust 1.45.0, released on 16 July 2020, made `as` from a float to an integer saturating. Before, an out-of-range cast was undefined behavior, although it needed no `unsafe` block. The issue about it was opened in October 2013.[^43]

## References
