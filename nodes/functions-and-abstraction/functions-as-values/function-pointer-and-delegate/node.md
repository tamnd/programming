---
tag: QXSYSW
kind: concept
slug: function-pointer-and-delegate
name: Function pointers and delegates
aliases: [function pointer, procedure pointer, subroutine pointer, delegate, multicast delegate, method reference, std::function]
area: functions-and-abstraction/functions-as-values
level: core
summary: >-
  A function pointer holds the address of a function, and a call through it runs that function. A delegate in C# is a typed reference to a method and can hold an object and several methods.
applies_to: core-languages
viz: optional
formal: none
see_also: [first-class-function, higher-order-functions, anonymous-function, callback-pattern, closures, function-signature, dynamic-dispatch, method]
sources:
- url: https://en.wikipedia.org/wiki/Function_pointer
  title: Function pointer, Wikipedia
  locator: the introduction
- url: https://learn.microsoft.com/en-us/dotnet/csharp/programming-guide/delegates/
  title: Delegates, C# Programming Guide
  locator: the introduction
- url: https://en.cppreference.com/w/c/language/declarations.html
  title: Declarations, C reference, cppreference.com
  locator: Declarators, the example
- url: https://en.cppreference.com/w/c/language/pointer.html
  title: Pointer declaration, C reference, cppreference.com
  locator: Pointers to functions
- url: https://en.cppreference.com/w/c/algorithm/qsort.html
  title: qsort, qsort_s, C reference, cppreference.com
  locator: the declaration
- url: https://en.cppreference.com/w/c/language/typedef.html
  title: Typedef declaration, C reference, cppreference.com
  locator: Example
- url: https://en.cppreference.com/w/cpp/language/pointer.html
  title: Pointer declaration, C++ reference, cppreference.com
  locator: Pointers to member functions
- url: https://en.cppreference.com/w/cpp/utility/functional/function.html
  title: std::function, C++ reference, cppreference.com
  locator: the introduction
- url: https://learn.microsoft.com/en-us/dotnet/csharp/programming-guide/delegates/using-delegates
  title: Using Delegates, C# Programming Guide
  locator: the whole page
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/function/package-summary.html
  title: Package java.util.function, Java SE 25 API
  locator: the package description
- url: https://docs.oracle.com/javase/tutorial/java/javaOO/methodreferences.html
  title: Method References, The Java Tutorials
  locator: the introduction; Kinds of Method References
- url: https://go.dev/ref/spec#Function_types
  title: The Go Programming Language Specification
  locator: Function types
- url: https://doc.rust-lang.org/book/ch20-04-advanced-functions-and-closures.html
  title: Advanced Functions and Closures, The Rust Programming Language
  locator: Function Pointers
- url: https://docs.python.org/3/reference/datamodel.html#callable-types
  title: Data model, The Python Language Reference
  locator: 3.2.8 Callable types
- url: https://www.gnu.org/software/bash/manual/html_node/Shell-Functions.html
  title: Shell Functions, Bash Reference Manual
  locator: 3.3 Shell Functions
- url: https://en.wikipedia.org/wiki/First-class_function
  title: First-class function, Wikipedia
  locator: Concepts
- url: https://en.wikipedia.org/wiki/Delegate_(CLI)
  title: Delegate (CLI), Wikipedia
  locator: the introduction
---

## Idea

A function pointer is a value that holds the address of a function. Wikipedia gives the definition: "A function pointer, also called a subroutine pointer or procedure pointer, is a pointer referencing executable code, rather than data."[^1] A program can change the pointer at run time, so one call in the code can run different functions.

```c run id=idea
#include <stdio.h>

static int add(int a, int b) { return a + b; }

static int mul(int a, int b) { return a * b; }

int main(void) {
    int (*op)(int, int) = add;
    printf("%d\n", op(6, 7));
    op = mul;
    printf("%d\n", op(6, 7));
    return 0;
}
```

```output for=idea
13
42
```

The call `op(6, 7)` is the same both times. The first time `op` points to `add`, and the second time it points to `mul`.

A delegate is the form of this idea in C# and .NET. "A delegate is a type that represents references to methods with a particular parameter list and return type."[^2] A delegate can also hold the object for an instance method and a list of several methods.

## How it works

1. **The type.** The pointer has a type that gives the parameter types and the result type. In C, `int (*op)(int, int)` declares `op` as a pointer to a function that takes two `int` and returns `int`. The C reference reads the declarator `(*pf)(double)` as an object of the type pointer to function that takes `double` and returns `int`[^3].
2. **Taking the address.** The name of a function converts to a pointer. "Because of the function-to-pointer conversion, the address-of operator is optional"[^4], so `op = add` and `op = &add` are the same.
3. **The call.** "A pointer to function can be used on the left-hand side of the function call operator; this invokes the pointed-to function"[^4]. The compiled code jumps to the address in the pointer.
4. **A value like any other.** "Unlike functions, pointers to functions are objects and thus can be stored in arrays, copied, assigned, passed to other functions as arguments, etc."[^4] The C library uses this for callbacks. `qsort` takes the comparison as the parameter `int (*comp)(const void*, const void*)`[^5].

## In languages

- **C.** Function pointers as above. A `typedef` gives a short name to a function pointer type, as in `typedef arr_t* (*fp)(void);`[^6]. "Equality comparison operators are defined for pointers to functions (they compare equal if pointing to the same function)."[^4] A C function pointer holds only the function, so a callback API can take one more argument for the data. C11 `qsort_s` passes its parameter `void* context` to the comparison function[^5].
- **C++.** C++ has the function pointers of C. It also has pointers to member functions: "A pointer to non-static member function f which is a member of class C can be initialized with the expression &C::f exactly."[^7] `std::function` holds any callable value: "Instances of std::function can store, copy, and invoke any CopyConstructible Callable target -- functions (via pointers thereto), lambda expressions, bind expressions, or other function objects, as well as pointers to member functions and pointers to data members."[^8]
- **C#.** Delegates. "A delegate is a type that safely encapsulates a method, similar to a function pointer in C and C++. Unlike C function pointers, delegates are object-oriented, type safe, and secure."[^9] "When a delegate is constructed to wrap an instance method, the delegate references both the instance and the method."[^9]
- **Java.** Java has no function pointers. A method reference such as `String::length` makes an object of a functional interface: "Functional interfaces provide target types for lambda expressions and method references."[^10] Method references "are compact, easy-to-read lambda expressions for methods that already have a name"[^11].
- **Go.** Function values have a function type, and "The value of an uninitialized variable of function type is nil."[^12]
- **Rust.** "The fn type is called a function pointer."[^13] "Function pointers implement all three of the closure traits (Fn, FnMut, and FnOnce), meaning you can always pass a function pointer as an argument for a function that expects a closure."[^13]
- **Python, JavaScript and TypeScript.** Functions are objects, so a variable holds the function itself and no pointer type is needed. In Python, "A user-defined function object is created by a function definition"[^14]. See [[first-class-function]].
- **Bash.** Bash has no function pointers. A variable holds the name of a function. "When the name of a shell function is used as a simple command name, the shell executes the list of commands associated with that function name."[^15]

## Variations and related ideas

- **Function pointers as the first step.** "Languages where functions are not first-class often still allow one to write higher-order functions through the use of features such as function pointers or delegates."[^16]
- **Bound methods.** A C function pointer has no data. A C# delegate for an instance method keeps the object too[^9]. "Delegates specify a method to call and optionally an object to call the method on."[^17]
- **Multicast delegates.** A C# delegate can call several methods. "To add an extra method to the delegate's list of methods—the invocation list—simply requires adding two delegates using the addition or addition assignment operators ('+' or '+=')."[^9] "Delegates are used, among other things, to implement callbacks and event listeners."[^17]

```csharp run id=csharp-multicast
using System;

class Program
{
    static void Main()
    {
        Action<string> log = s => Console.WriteLine("screen: " + s);
        log += s => Console.WriteLine("file: " + s);
        log("saved");
    }
}
```

```output for=csharp-multicast
screen: saved
file: saved
```

- **Wrappers for any callable.** A C++ `std::function` can hold a function pointer, a lambda with captured variables or another function object[^8]. One type covers all of them.

## Pitfalls

### A multicast delegate returns only the last result

A delegate with a return value and two methods runs both methods, but the caller gets only one result. "If the delegate has a return value and/or out parameters, it returns the return value and parameters of the last method invoked."[^9]

```csharp wrong id=csharp-multicast-result
using System;

class Program
{
    static void Main()
    {
        Func<int, int> f = x => x + 1;
        f += x => x * 10;
        Console.WriteLine(f(5));
    }
}
```

```output for=csharp-multicast-result
50
```

The first method gives 6, and the program loses it. Use multicast only for delegates that return `void`, such as `Action` and event handlers. To get all the results, keep the functions in a list and call each one.

### A call through an empty wrapper

A C function pointer can be a null pointer[^4], a Go function value can be `nil`[^12], and a C++ `std::function` can be empty. The C++ reference says: "Invoking the target of an empty std::function results in std::bad_function_call exception being thrown."[^8] Check the value before the call, or make sure that it always has a target.

### Hard declarations in C

The C declarator syntax nests. The C reference takes apart `int (*(*foo)(double))[3]` in five steps to show that `foo` is a "pointer to function taking double and returning pointer to array of 3 int"[^3]. Give each function pointer type a name with `typedef`[^6].

## Cost

A call through a function pointer is an indirect call, "because the function is being invoked indirectly through a variable instead of directly through a fixed identifier or address"[^1]. The program reads the address from the variable before the jump. A C function pointer is one address. A C# delegate for an instance method also holds the object[^9], and a C++ `std::function` holds a copy of its target[^8].

## History

Wikipedia lists PL/I, COBOL, Fortran and C among the third-generation languages with function pointers, and C++, C# and D among the object-oriented languages with them[^1]. Wikipedia describes the delegate of .NET as "a form of type-safe function pointer used by the Common Language Infrastructure (CLI)"[^17].

## References
