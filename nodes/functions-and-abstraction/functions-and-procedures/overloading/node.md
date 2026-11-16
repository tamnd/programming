---
tag: ZDBXDA
kind: concept
slug: overloading
name: Overloading
aliases: [function overloading, method overloading, overload resolution]
area: functions-and-abstraction/functions-and-procedures
level: core
summary: >-
  Overloading gives one name to several functions that differ in the number or the types of their parameters. At each call, the language picks one of them from the arguments.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1968
  people: [Adriaan van Wijngaarden]
  note: ALGOL 68 let programmers define new operators and overload them. C++, Java and C# later overloaded functions and methods by their parameters, and Go, Rust, C and Python chose not to.
see_also: [function-signature, method, function-call, operator-overloading, overload-an-operator, dynamic-dispatch, double-dispatch, parametric-polymorphism]
sources:
- url: https://en.wikipedia.org/wiki/Function_overloading
  title: Function overloading, Wikipedia
  locator: the introduction; Rules in function overloading; Complications; Caveats
- url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-8.html#jls-8.4.9
  title: Classes, The Java Language Specification, Java SE 25 Edition
  locator: 8.4.9 Overloading
- url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/language-specification/basic-concepts#75-signatures-and-overloading
  title: Basic concepts, C# language specification
  locator: 7.5 Signatures and overloading
- url: https://en.cppreference.com/w/cpp/language/overload_resolution.html
  title: Overload resolution, C++ reference, cppreference.com
  locator: the introduction
- url: https://docs.python.org/3/reference/compound_stmts.html#function-definitions
  title: Compound statements, The Python Language Reference
  locator: 8.7 Function definitions
- url: https://docs.python.org/3/library/typing.html#typing.overload
  title: typing, Support for type hints, The Python Standard Library
  locator: typing.overload
- url: https://docs.python.org/3/library/functools.html#functools.singledispatch
  title: functools, Higher-order functions and operations on callable objects, The Python Standard Library
  locator: functools.singledispatch
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Statements/function
  title: function, MDN Web Docs JavaScript Reference
  locator: Redeclarations
- url: https://www.typescriptlang.org/docs/handbook/2/functions.html#function-overloads
  title: More on Functions, The TypeScript Handbook
  locator: Function Overloads
- url: https://en.cppreference.com/w/c/language/generic.html
  title: Generic selection, C reference, cppreference.com
  locator: the introduction
- url: https://go.dev/doc/faq#overloading
  title: Frequently Asked Questions, The Go Programming Language
  locator: Why does Go not support overloading of methods and operators?
- url: https://www.gnu.org/software/bash/manual/html_node/Shell-Functions.html
  title: Shell Functions, Bash Reference Manual
  locator: 3.3 Shell Functions
- url: https://en.wikipedia.org/wiki/ALGOL_68
  title: ALGOL 68, Wikipedia
  locator: History; Operators
- url: https://kotlinlang.org/docs/functions.html#parameters-with-default-values
  title: Functions, Kotlin Documentation
  locator: Parameters with default values
---

## Idea

Overloading lets a program define several functions with the same name. The functions differ in the number or the types of their parameters. At each call, the language uses the arguments to pick one of them.

Wikipedia defines function overloading as "the ability to create multiple functions of the same name with different implementations."[^1] The functions "must have different type signatures, i.e. differ in the number or the types of their formal parameters (as in C++) or additionally in their return type (as in Ada)."[^1]

```java run id=idea
public class Main {
    static String show(int x) {
        return "int " + x;
    }

    static String show(String s) {
        return "string " + s;
    }

    static String show(int x, int y) {
        return "two ints " + x + " " + y;
    }

    public static void main(String[] args) {
        System.out.println(show(42));
        System.out.println(show("hi"));
        System.out.println(show(3, 4));
    }
}
```

```output for=idea
int 42
string hi
two ints 3 4
```

The three functions have the name `show`, and each has a different [[function-signature]]. The compiler reads the number and the types of the arguments in each call and picks the function that matches.

## How it works

1. **The overload set.** The functions with the same name that are visible at the call make the overload set. In Java, if two methods of a class "have the same name but signatures that are not override-equivalent, then the method name is said to be overloaded."[^2]
2. **The signature.** The parameters tell the overloads apart. In C#, "The signature of a method consists of the name of the method, the number of type parameters, and the type and parameter-passing mode of each of its parameters, considered in the order left to right."[^3] The return type is not in this list. Java also says: "There is no required relationship between the return types or between the throws clauses of two methods with the same name, unless their signatures are override-equivalent."[^2]
3. **The resolution.** At each call, the compiler collects the candidate functions, removes the ones that the arguments cannot match, and then chooses the best of the rest[^4]. cppreference says: "In simple terms, the overload whose parameters match the arguments most closely is the one that is called."[^4]
4. **Compile time.** The choice uses the types that the compiler knows. In Java, "the number of actual arguments (and any explicit type arguments) and the compile-time types of the arguments are used, at compile time, to determine the signature of the method that will be invoked"[^2]. Wikipedia says: "For any particular call, the compiler determines which overloaded function to use and resolves this at compile time."[^1]
5. **No match or a tie.** If no function matches the arguments, or two functions match equally well, the call does not compile. See the pitfall on ambiguous calls below.

## In languages

- **Python.** Python has no overloading[^1]. A `def` statement is executed, and "Its execution binds the function name in the current local namespace to a function object"[^5]. So a second `def` with the same name replaces the first. The `typing.overload` decorator describes several signatures for a type checker only: "@overload-decorated definitions are for the benefit of the type checker only, since they will be overwritten by the non-@overload-decorated definition."[^6] The standard library also has `functools.singledispatch`, which will "Transform a function into a single-dispatch generic function."[^7] It chooses an implementation at run time: "When called, the generic function dispatches on the type of the first argument"[^7].
- **JavaScript.** JavaScript has no overloading. A second function declaration with the same name replaces the first. MDN shows `function a(b) {}` followed by `function a(b, c) {}`, and then `a.length` is 2[^8]. One function checks the number and the types of its arguments.
- **TypeScript.** TypeScript adds overloads to the types: "In TypeScript, we can specify a function that can be called in different ways by writing overload signatures."[^9] The program writes one body after the overload signatures, and "Functions have an implementation signature, but this signature can’t be called directly."[^9]
- **Java.** Methods and constructors can be overloaded. Constructors, "Like methods, they may be overloaded"[^2]. The compiler chooses the overload[^2].
- **C#.** "Overloading of methods permits a class, struct, or interface to declare multiple methods with the same name, provided their signatures are unique within that class, struct, or interface."[^3]
- **C.** C has no overloading[^1]. C11 added the generic selection `_Generic`, which "Provides a way to choose one of several expressions at compile time, based on a type of a controlling expression"[^10]. A macro with `_Generic` can send a call to a different function for each argument type.
- **C++.** Free functions, member functions, constructors and operators can be overloaded. The compiler runs overload resolution at each call[^4].
- **Go.** Go has no overloading. The Go FAQ says: "Method dispatch is simplified if it doesn’t need to do type matching as well. Experience with other languages told us that having a variety of methods with the same name but different signatures was occasionally useful but that it could also be confusing and fragile in practice."[^11] A Go program gives each function a different name.
- **Rust.** Rust has no overloading[^1]. The Rust example on this page defines a trait with one method and implements the trait for each argument type.
- **Bash.** A function declaration has no parameter list: "fname () compound-command [ redirections ]"[^12]. The arguments "become the positional parameters during its execution"[^12]. So a Bash function cannot have overloads. One function checks `$#` and the values of its arguments.

## Variations and related ideas

- **Operator overloading.** Many languages also let a program give a new meaning to an operator for its own types. In ALGOL 68, "The programmer may define new operators and both those and the pre-defined ones may be overloaded and their priorities may be changed by the coder."[^13] See [[operator-overloading]] and [[overload-an-operator]].
- **Overloading on the return type.** Ada can also tell overloads apart by their return type[^1]. Java, C# and C++ cannot[^2][^3].
- **Default parameter values.** A default value can replace an overload that only skips a parameter. The Kotlin documentation says: "Optional parameters reduce the need for multiple overloads, since you don't have to declare different versions of a function just to allow skipping a parameter with a reasonable default."[^14]
- **Union types.** In TypeScript, one signature with a union type is often simpler than several overloads. The handbook says: "Always prefer parameters with union types instead of overloads when possible"[^9].
- **Overloading and overriding.** Overloading chooses a function at compile time. Overriding chooses a method at run time from the type of the receiver. Wikipedia says: "Function overloading differs from forms of polymorphism where the choice is made at runtime, e.g. through virtual functions, instead of statically."[^1] See [[dynamic-dispatch]] and [[double-dispatch]].
- **Generic functions.** A generic function has one body that works for many types. An overloaded name has one body for each signature. See [[parametric-polymorphism]].

## Pitfalls

### Java chooses the overload from the static type

The compiler chooses the overload from "the compile-time types of the arguments"[^2], not from the class of the object at run time:

```java wrong id=java-static-type
public class Main {
    static String show(Object o) {
        return "object";
    }

    static String show(String s) {
        return "string";
    }

    public static void main(String[] args) {
        Object x = "hi";
        System.out.println(show(x));
    }
}
```

```output for=java-static-type
object
```

The variable `x` has the type `Object`, so the call uses `show(Object)`, though the value is a `String`. To choose by the type at run time, put the behavior in a method of the class and override it. See [[dynamic-dispatch]].

### A second def replaces the first in Python

Python executes each `def` and binds the name again[^5]. The second `area` replaces the first:

```python wrong id=python-second-def
def area(side):
    return side * side


def area(width, height):
    return width * height


print(area(3, 4))
try:
    print(area(5))
except TypeError as e:
    print(e)
```

```output for=python-second-def
12
area() missing 1 required positional argument: 'height'
```

Use one function with a default value, as in `def area(width, height=None)`, or use `functools.singledispatch` to choose by type[^7].

### Ambiguous calls

A call can match two overloads equally well. cppreference gives this example[^4]:

```cpp norun reason=fragment
void f(long);
void f(float);

f(0L); // calls f(long)
f(0);  // error: ambiguous overload
```

The literal `0` is an `int`. It is not a `long` and not a `float`, and the compiler finds no single best function[^4]. Wikipedia names the causes: "Two issues interact with and complicate function overloading: Name masking (due to scope) and implicit type conversion."[^1] Many overloads also make code hard to read: "If a method is designed with an excessive number of overloads, it may be difficult for developers to discern which overload is being called simply by reading the code."[^1]

## Cost

In Java, C# and C++, the compiler chooses the overload[^2][^4]. The program then makes a normal call, so overloading adds no cost at run time. It adds work for the compiler and for the reader. `functools.singledispatch` in Python chooses at run time from the type of the first argument[^7], so each call does a lookup first. The Go designers left overloading out partly for this reason: "Method dispatch is simplified if it doesn’t need to do type matching as well."[^11]

## History

ALGOL 68 had overloaded operators. "On 20 December 1968, the language was formally adopted by the group"[^13], and its programmers could define operators and overload them[^13]. Ada, C++, Java and C# overload functions and methods by their parameters[^1][^2][^3]. Later languages such as Go and Rust left overloading out[^11][^1].

## References
