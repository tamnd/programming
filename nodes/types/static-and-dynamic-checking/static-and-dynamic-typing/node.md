---
tag: NEW-static-and-dynamic-typing
kind: concept
slug: static-and-dynamic-typing
name: Static and dynamic typing
aliases: [static typing, dynamic typing, static type checking, dynamic type checking]
area: types/static-and-dynamic-checking
level: core
summary: >-
  Static typing finds type errors before the program runs, from the types of its expressions. Dynamic typing finds them during the run, from the type that each value carries.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1978
  people: [Robin Milner]
  note: Milner proved that programs which pass the type check of his polymorphic type discipline cannot go wrong at run time. This is the soundness property of a static type system.
see_also: [parametric-polymorphism, exception-handling, garbage-collection]
same_as:
  wikidata: Q4001197
sources:
  - book: isbn:9780262162098
    title: Benjamin C. Pierce, Types and Programming Languages, MIT Press, 2002
    locator: 1.1 Types in Computer Science
  - paper: doi:10.1145/6041.6042
    title: Luca Cardelli and Peter Wegner, On understanding types, data abstraction, and polymorphism, ACM Computing Surveys 17(4), 1985
    locator: 1.2 Static and Strong Typing
  - paper: doi:10.1016/0022-0000(78)90014-4
    title: Robin Milner, A theory of type polymorphism in programming, Journal of Computer and System Sciences 17(3), 1978, pages 348 to 375
    locator: the semantic soundness theorem and the well-typing algorithm W
  - url: https://jsiek.github.io/home/siek06gradual.pdf
    title: Jeremy G. Siek and Walid Taha, Gradual Typing for Functional Languages, Scheme and Functional Programming Workshop, 2006, pages 81 to 92
    locator: Abstract; 2 Introduction to Gradual Typing
  - url: https://docs.python.org/3.14/reference/datamodel.html#objects-values-and-types
    title: The Python Language Reference, Data model, Python 3.14
    locator: 3.1 Objects, values and types
  - url: https://docs.python.org/3.14/library/typing.html
    title: The Python Standard Library, typing, Python 3.14
    locator: the note at the top of the page
  - url: https://peps.python.org/pep-0484/#non-goals
    title: "PEP 484: Type Hints"
    locator: Rationale and Goals, Non-goals; the header fields Created and Python-Version
  - url: https://docs.python.org/3.14/glossary.html#term-duck-typing
    title: Python Glossary, Python 3.14
    locator: duck-typing
  - url: https://docs.python.org/3.14/c-api/structures.html#c.PyObject
    title: The Python/C API Reference Manual, Common Object Structures, Python 3.14
    locator: PyObject
  - url: https://www.typescriptlang.org/docs/handbook/2/basic-types.html#erased-types
    title: The TypeScript Handbook, The Basics
    locator: Erased Types
  - url: https://www.typescriptlang.org/docs/handbook/2/everyday-types.html#type-assertions
    title: The TypeScript Handbook, Everyday Types
    locator: Type Assertions; any
  - url: https://www.typescriptlang.org/docs/handbook/type-compatibility.html#a-note-on-soundness
    title: The TypeScript Handbook, Type Compatibility
    locator: A Note on Soundness; the introduction on structural typing
  - url: https://nodejs.org/api/typescript.html#type-stripping
    title: Node.js documentation, Modules, TypeScript
    locator: Type stripping
  - url: https://tc39.es/ecma262/2026/#sec-applystringornumericbinaryoperator
    title: ECMA-262, ECMAScript 2026 Language Specification
    locator: 13.15.3 ApplyStringOrNumericBinaryOperator
  - url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-4.html
    title: The Java Language Specification, Java SE 25 Edition
    locator: Chapter 4 Types, Values, and Variables, the introduction
  - url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-5.html#jls-5.5
    title: The Java Language Specification, Java SE 25 Edition
    locator: 5.5 Casting Contexts
  - url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-10.html#jls-10.5
    title: The Java Language Specification, Java SE 25 Edition
    locator: 10.5 Array Store Exception
  - url: https://docs.oracle.com/javase/specs/jvms/se25/html/jvms-6.html#jvms-6.5.checkcast
    title: The Java Virtual Machine Specification, Java SE 25 Edition
    locator: 6.5 Instructions, checkcast
  - url: https://www.open-std.org/jtc1/sc22/wg14/www/docs/n3220.pdf
    title: Working Draft, Programming Languages, C (N3220, the C23 draft)
    locator: 6.3.1.3 Signed and unsigned integers, paragraph 2; 6.3.1.8 Usual arithmetic conversions
  - url: https://www.open-std.org/jtc1/sc22/wg14/www/docs/n3220.pdf
    title: Working Draft, Programming Languages, C (N3220, the C23 draft)
    locator: 6.3.1.4 Real floating and integer, paragraph 1
  - url: https://timsong-cpp.github.io/cppwp/n4950/expr.dynamic.cast
    title: Working Draft, Programming Languages, C++ (N4950, the C++23 draft)
    locator: "[expr.dynamic.cast], paragraphs 7 and 9"
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/fundamentals/types/
    title: C# fundamentals, The C# type system
    locator: the introduction; Compile-time type and run-time type
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/advanced-topics/interop/using-type-dynamic
    title: C# advanced topics, Using type dynamic
    locator: the introduction; Overload resolution with arguments of type dynamic; Dynamic language runtime
  - url: https://go.dev/ref/spec#Introduction
    title: The Go Programming Language Specification
    locator: Introduction
  - url: https://go.dev/ref/spec#Variables
    title: The Go Programming Language Specification
    locator: Variables (static type and dynamic type)
  - url: https://go.dev/ref/spec#Type_assertions
    title: The Go Programming Language Specification
    locator: Type assertions
  - url: https://go.dev/ref/spec#Numeric_types
    title: The Go Programming Language Specification
    locator: Numeric types, the last paragraph
  - url: https://doc.rust-lang.org/book/ch03-02-data-types.html
    title: The Rust Programming Language, 3.2 Data Types
    locator: the introduction
  - url: https://doc.rust-lang.org/std/any/index.html
    title: The Rust Standard Library, Module std::any
    locator: the module summary; the methods is and downcast_ref of dyn Any
  - url: https://kotlinlang.org/docs/typecasts.html
    title: Kotlin documentation, Type checks and casts
    locator: Smart casts; Unsafe cast operator; Safe (nullable) cast operator
  - url: https://learn.microsoft.com/en-us/powershell/module/microsoft.powershell.core/about/about_variables#types-of-variables
    title: PowerShell documentation, about_Variables
    locator: Types of variables
  - url: https://www.sqlite.org/datatype3.html
    title: SQLite documentation, Datatypes In SQLite
    locator: 1 Datatypes In SQLite; 3 Type Affinity
  - url: https://www.sqlite.org/stricttables.html
    title: SQLite documentation, STRICT Tables
    locator: 1 Introduction; 2 STRICT Tables, item 3
  - url: https://www.gnu.org/software/bash/manual/bash.html#Bash-Builtins
    title: Bash Reference Manual
    locator: 4.2 Bash Builtin Commands, declare, option -i
  - url: https://tc39.es/ecma262/2026/#sec-ecmascript-language-types
    title: ECMA-262, ECMAScript 2026 Language Specification
    locator: 6.1 ECMAScript Language Types
---

## Idea

A type tells which values an expression can have and which operations are valid on them. A type error is an operation on a value of the wrong kind, for example the addition of a string and a number. A language must find type errors at some time, or let the program continue with a wrong result. Static typing finds them before the program runs. A type checker reads the program text and gives a type to each expression.[^2] Dynamic typing finds them while the program runs. Each value carries its type, and each operation checks the types of its operands.[^5]

Pierce defines a type system as "a tractable syntactic method for proving the absence of certain program behaviors by classifying phrases according to the kinds of values they compute".[^1] Under this definition, a type system is always static. For this reason, Pierce prefers the term "dynamically checked" to "dynamically typed".[^1] This page uses the usual names, static typing and dynamic typing.

The Python function below has a type error on one branch. Python accepts the function, and the error appears only when that branch runs.

```python run id=idea
def describe(flag):
    if flag:
        return "count: " + str(3)
    return "count: " + 3


print(describe(True))
try:
    print(describe(False))
except TypeError as error:
    print("TypeError:", error)
```

```output
count: 3
TypeError: can only concatenate str (not "int") to str
```

A static checker rejects the same function before any call runs. The next section shows this in Go.

## How it works

**Static checking.** A static type checker runs before the program, usually as a part of the compiler. Java is an example: every variable and every expression has a type that is known at compile time.[^15] The checker rejects the program when an operation does not fit the types of its operands. The Go version of `describe` does not compile, although `main` calls only the good branch.

```go fails expect=compile id=static-reject
package main

import "fmt"

func describe(flag bool) string {
	n := 3
	if flag {
		return "count: " + fmt.Sprint(n)
	}
	return "count: " + n
}

func main() {
	fmt.Println(describe(true))
}
```

The Go compiler stops with `invalid operation: "count: " + n (mismatched types untyped string and int)`. Go also needs an explicit conversion when an expression mixes two numeric types, for example `int` and `int64`.[^27]

**Dynamic checking.** In a dynamically typed language, a variable has no fixed type. The value carries the type. In Python, every object has an identity, a type and a value, and the type of an object does not change.[^5] Below, the name `x` refers to an `int` and then to a `str`.

```python run id=tags
x = 3
print(type(x).__name__)
x = "three"
print(type(x).__name__)
print(isinstance(x, str))
```

```output
int
str
True
```

The type is a field of each object. In a release build of CPython, the header of each object holds a reference count and a pointer to the type object.[^9] An operation such as `+` reads the type of each operand and looks for an implementation for that pair of types. When it finds none, it raises `TypeError`, as in Idea.

**Mixed checking.** Most languages do some checks at each time. A static language keeps run-time checks for operations that the static types cannot decide, such as a cast to a subclass. A dynamic language can add optional annotations that a separate tool checks. The table gives the rule of each tier 1 language.

| Language | Checked before the run | Checked during the run | Escape from the static check |
|---|---|---|---|
| Python | only by a separate tool; the runtime ignores annotations[^6] | each operation, from the type of each object[^5] | not needed |
| JavaScript | no | each operation, from the language type of each value[^35] | not needed |
| TypeScript | by `tsc`; the types are removed from the output[^10] | as JavaScript | `any` and type assertions[^11] |
| Java | every expression[^15] | casts and array stores[^16][^17] | casts |
| C | every expression | nothing that the language defines | casts and implicit conversions[^19] |
| C++ | every expression | `dynamic_cast`[^21] | casts |
| C# | every expression[^22] | casts, and every operation on `dynamic`[^23] | `dynamic`[^23] |
| Go | every expression[^24] | type assertions on interface values[^26] | interface types such as `any` |
| Rust | every expression[^28] | `downcast_ref` on `dyn Any`[^29] | `dyn Any`[^29] |
| Kotlin | every expression | `as` and `is`[^30] | casts with `as` |
| PowerShell | no | each operation; a typed variable converts each value that it gets[^31] | not needed |
| SQL (SQLite) | no | columns of a `STRICT` table[^33] | not needed |
| Bash | no | no; the values are strings[^34] | not needed |

**Strong and weak typing.** This is a separate question. Cardelli and Wegner call a language strongly typed when all its expressions are type-consistent. They state that every statically typed language is strongly typed, but that the converse is not necessarily true.[^2] In other texts, "weak typing" often means that the language converts an operand to fit an operation instead of reporting an error. This page does not use the two words as labels. It shows what each language does with a mixed operation. JavaScript converts. For `+`, a string operand makes the operation a concatenation. For `-`, both operands become numbers.[^14]

```javascript run id=coerce
console.log("5" + 2);
console.log("5" - 2);
console.log(typeof ("5" - 2));
```

```output
52
3
number
```

Python raises `TypeError` for the same `+`, as Idea shows. C also converts, but at compile time and with no message. A `double` assigned to an `int` loses its fraction.[^20] A negative value converted to an unsigned type becomes a large positive value.[^19] The C example and Pitfalls show both.

**Soundness.** A static type system is sound when no program that passes the checker can have a type error at run time. Milner proved this for his polymorphic type discipline. His Semantic Soundness Theorem states that well-typed programs cannot "go wrong".[^3] Many languages accept a few unsound rules and keep a run-time check behind them. In Java, an array of `String` is also an array of `Object`. The compiler accepts the store of an `Integer` through the `Object[]` view, and the store fails at run time.[^17]

```java fails expect=exception:ArrayStoreException id=array-store
public class Main {
    public static void main(String[] args) {
        Object[] items = new String[2];
        items[0] = "one";
        items[1] = 2;
        System.out.println(items[1]);
    }
}
```

The TypeScript handbook states that the type system of TypeScript is not sound by design. It allows some operations that the compiler cannot prove safe.[^12] TypeScript removes all types from the output, so no run-time check stands behind these rules.[^10] The second pitfall shows the result.

## In languages

The example files show, for each language, the type of a value at run time and one checked operation. The table gives the verdict for each tier 1 language.

| Language | Verdict | Form |
|---|---|---|
| Python | native | dynamic; annotations are data that the runtime does not check |
| JavaScript | native | dynamic; `typeof`, with implicit conversion of operands |
| TypeScript | native | static checks by `tsc` over the dynamic rules of JavaScript |
| Java | native | static; checked casts and array stores at run time |
| C | native | static; implicit arithmetic conversions; no type data at run time |
| C++ | native | static; `dynamic_cast` and `std::any` for checks at run time |
| C# | native | static; `dynamic` moves the checks to run time |
| Go | native | static; interface values with type assertions and type switches |
| Rust | native | static; `dyn Any` with `downcast_ref` |
| Kotlin | native | static; `is` with smart casts, and `as?` |
| PowerShell | native | dynamic; a type in square brackets fixes the type of a variable |
| Bash | native | values are strings; `declare -i` adds arithmetic on assignment |
| SQL | native | in SQLite, the type goes with the value; `STRICT` tables check columns |
| Regex | n/a | a pattern has no typed values |
| HTML/CSS | n/a | markup and style rules have no expressions that have types |

In SQLite, the datatype of a value is associated with the value itself, not with its container.[^32] A column type gives only a type affinity, a preferred type. A `STRICT` table checks the type of each value that goes into a column.[^33] The SQL example and the pitfalls use SQLite.

In Bash, a variable holds a string, and arithmetic expansion reads the string as a number. The attribute `declare -i` makes the shell evaluate each assigned value as an arithmetic expression.[^34] An assignment of text that is not a number gives no error (see the Bash example).

## Variations and related ideas

- **Gradual typing.** Siek and Taha add an unknown type, written `?`, to a static type system. A part of the program with annotations gets static checks. A part without annotations gets checks at run time. The checker uses a consistency relation between types instead of equality, and `?` is consistent with every type.[^4] TypeScript `any` turns off the checks for a value,[^11] and C# `dynamic` makes the compiler skip the checks for an expression.[^23]
- **Optional checkers.** The Python runtime does not enforce annotations.[^6] PEP 484 states that Python will remain a dynamically typed language, and that the authors have no desire to make type hints mandatory.[^7] A separate tool, such as mypy, reads the annotations and reports errors. Node.js runs TypeScript files by replacing the types with whitespace, and it does no type check.[^13]
- **Type inference.** A static checker can find the types of variables without annotations. Milner gave an algorithm that finds the most general type of each expression of an ML program.[^3] Rust `let`, Go `:=`, C# and Java `var`, C++ `auto` and Kotlin `val` all use inference for local variables (see the examples). Inference does not make a language dynamic. The type is still fixed at compile time.
- **Static type and dynamic type of one variable.** A Go variable of interface type has a static type, the interface. It also has a dynamic type, the type of the value that it holds at run time.[^25] C# uses the terms compile-time type and run-time type in the same way.[^22] A type switch or a cast reads the dynamic type.
- **Duck typing.** A Python program often does not test the type of an object. It calls the method that it needs and lets the object answer.[^8] A missing method gives an `AttributeError` at run time.
- **Structural and nominal typing.** TypeScript compares types by their members, not by their names.[^12] In a nominal system, one type is a subtype of another only when a declaration says so.
- **Type parameters.** [[parametric-polymorphism]] let a static type system describe a container of any element type, so the code needs no casts and no checks at run time.

## Pitfalls

### Python annotations do not check anything

An annotation in Python is data. The runtime does not enforce it.[^6] The call below passes a `str` where the annotation says `int`, and the function returns a wrong value with no error.

```python wrong id=annotation
def area(width: int, height: int) -> int:
    return width * height


print(area(2, 3))
print(area("ab", 2))
```

```output
6
abab
```

Run a static checker, such as mypy, on the code before it ships. Where wrong values can come from outside the program, add a check that runs.

```python run id=annotation-fix
def area(width: int, height: int) -> int:
    if not isinstance(width, int) or not isinstance(height, int):
        raise TypeError("area needs two int values")
    return width * height


try:
    area("ab", 2)
except TypeError as error:
    print("TypeError:", error)
```

```output
TypeError: area needs two int values
```

### A TypeScript type assertion is not a check

A type assertion with `as` tells the compiler the type of a value. TypeScript removes the assertion from the output, so nothing checks the value at run time.[^11] Data from `JSON.parse` has the type that the text gives. Here `quantity` is a string, and `+ 1` concatenates.

```typescript wrong id=assertion
const order = JSON.parse('{"quantity": "2"}') as { quantity: number };
console.log(order.quantity + 1);
```

```output
21
```

Give outside data the type `unknown`. Then test it with `typeof` before the code uses it. The compiler accepts the use only after the test.

```typescript run id=assertion-fix
function quantityOf(value: unknown): number {
  if (typeof value === "object" && value !== null && "quantity" in value && typeof value.quantity === "number") {
    return value.quantity;
  }
  throw new TypeError("quantity is not a number");
}

try {
  console.log(quantityOf(JSON.parse('{"quantity": "2"}')) + 1);
} catch (error) {
  console.log(String(error));
}
console.log(quantityOf(JSON.parse('{"quantity": 2}')) + 1);
```

```output
TypeError: quantity is not a number
3
```

### C unsigned arithmetic wraps

The C compiler checks the types, but it converts operands with no message. `strlen` returns a `size_t`, which is unsigned. The difference of two `size_t` values is also unsigned. When the result is negative, the conversion adds one more than the maximum value of the type.[^19] So the test is true for a shorter string.

```c wrong id=unsigned
#include <stdio.h>
#include <string.h>

int main(void) {
    const char *a = "ab";
    const char *b = "abc";
    if (strlen(a) - strlen(b) > 0) {
        puts("a is longer");
    } else {
        puts("a is not longer");
    }
    printf("%zu\n", strlen(a) - strlen(b));
    return 0;
}
```

```output
a is longer
18446744073709551615
```

Compare the two lengths. Do not subtract them.

```c run id=unsigned-fix
#include <stdio.h>
#include <string.h>

int main(void) {
    const char *a = "ab";
    const char *b = "abc";
    if (strlen(a) > strlen(b)) {
        puts("a is longer");
    } else {
        puts("a is not longer");
    }
    return 0;
}
```

```output
a is not longer
```

### A Java downcast fails at run time

A cast from `Object` to `String` passes the compiler. At run time, the cast checks the class of the object and throws `ClassCastException` when the class does not fit.[^16][^18]

```java fails expect=exception:ClassCastException id=downcast
import java.util.ArrayList;
import java.util.List;

public class Main {
    public static void main(String[] args) {
        List<Object> items = new ArrayList<>();
        items.add("one");
        items.add(2);
        for (Object item : items) {
            String text = (String) item;
            System.out.println(text.length());
        }
    }
}
```

Test the class with `instanceof` before the code uses the value. The pattern form binds a variable of the narrower type.

```java run id=downcast-fix
import java.util.ArrayList;
import java.util.List;

public class Main {
    public static void main(String[] args) {
        List<Object> items = new ArrayList<>();
        items.add("one");
        items.add(2);
        for (Object item : items) {
            if (item instanceof String text) {
                System.out.println(text.length());
            } else {
                System.out.println("not a string: " + item);
            }
        }
    }
}
```

```output
3
not a string: 2
```

A list of a precise type, `List<String>`, is the better design. Then the compiler rejects `items.add(2)` (see [[parametric-polymorphism]]).

### SQLite stores any value in any column

In an ordinary SQLite table, the declared column type is a preference. SQLite converts text that looks like a number. It stores other text as text, in an `INTEGER` column.[^32]

```sql wrong id=affinity
CREATE TABLE item (qty INTEGER);
INSERT INTO item VALUES (2), ('3'), ('abc');
SELECT qty, typeof(qty) FROM item;
```

```output
2|integer
3|integer
abc|text
```

Declare the table `STRICT`. Then an insert of a value that does not fit the column type fails with an `SQLITE_CONSTRAINT_DATATYPE` error.[^33]

```sql fails expect=runtime id=strict
CREATE TABLE item (qty INTEGER) STRICT;
INSERT INTO item VALUES (2), ('3');
INSERT INTO item VALUES ('abc');
```

## Cost

- **Space for the type.** A dynamically typed value must carry its type. Each CPython object has a header with a reference count and a pointer to its type.[^9] A C `int` holds only its bits. On the machine of these runs, the two sizes are as follows.

```python run id=size
import sys

print(sys.getsizeof(1))
```

```output
28
```

```c run id=size-c
#include <stdio.h>

int main(void) {
    printf("%zu\n", sizeof(int));
    return 0;
}
```

```output
4
```

- **Time for the check.** A dynamic operation reads the types of its operands before it does the work. A static check runs once, at compile time, and adds no work to the run. The run-time checks that a static language keeps have a cost at each use. A Java cast compiles to the `checkcast` instruction, which tests the class of the object.[^18] A Java array store also tests the class of the value.[^17]
- **Late binding.** An operation on a C# `dynamic` value is bound at run time by the dynamic language runtime. Overload resolution also happens at run time.[^23]
- **Erased types.** TypeScript types do not change the behaviour of the program.[^10] They cost nothing at run time, and they also check nothing at run time.
- **Gradual code.** Siek and Taha designed gradual typing so that the cost of dynamic checks applies only to the parts of a program that use the unknown type.[^4]

## History

- **1978.** Robin Milner described a polymorphic type discipline for the language ML. He proved that well-typed programs cannot go wrong, and he gave an algorithm that infers the types.[^3]
- **1985.** Luca Cardelli and Peter Wegner separated static typing from strong typing in their survey of types and polymorphism.[^2]
- **2002.** Benjamin Pierce defined a type system as a static method that proves the absence of some behaviours, in Types and Programming Languages.[^1]
- **2006.** Jeremy Siek and Walid Taha described gradual typing, with the unknown type and the consistency relation.[^4]
- **2014.** PEP 484 defined type hints for Python. It became part of Python 3.5, and it kept the hints optional.[^7]
- **2021.** SQLite 3.37.0 added `STRICT` tables, which check the type of each value that goes into a column.[^33]

## References
