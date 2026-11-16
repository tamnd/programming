---
tag: TAEWCN
kind: concept
slug: function-object
name: Function object
aliases: [functor, callable object, function-like object, invocable object]
area: functions-and-abstraction/functions-as-values
level: advanced
summary: >-
  A function object is an object that a program can call like a function. It can keep state between calls. Examples are C++ classes with operator(), Python classes with __call__ and Rust closures.
applies_to: core-languages
viz: optional
formal: none
see_also: [first-class-function, function-pointer-and-delegate, anonymous-function, closures, higher-order-functions, callback-pattern, method, dynamic-dispatch]
sources:
- url: https://en.wikipedia.org/wiki/Function_object
  title: Function object, Wikipedia
  locator: the introduction; Description; In C++, Maintaining state; In Java; In D
- url: https://docs.python.org/3/reference/datamodel.html#object.__call__
  title: Data model, The Python Language Reference
  locator: 3.2.8 Callable types; 3.3.6 Emulating callable objects
- url: https://en.cppreference.com/w/cpp/language/operators.html
  title: Operator overloading, C++ reference, cppreference.com
  locator: Function call operator
- url: https://en.cppreference.com/w/cpp/named_req/FunctionObject.html
  title: C++ named requirements, FunctionObject, cppreference.com
  locator: the introduction
- url: https://en.cppreference.com/w/cpp/language/lambda.html
  title: Lambda expressions, C++ reference, cppreference.com
  locator: Closure type
- url: https://en.cppreference.com/w/cpp/utility/functional/function.html
  title: std::function, C++ reference, cppreference.com
  locator: the introduction
- url: https://doc.rust-lang.org/std/ops/trait.Fn.html
  title: Trait Fn, The Rust Standard Library
  locator: the description
- url: https://doc.rust-lang.org/book/ch13-01-closures.html
  title: Closures, The Rust Programming Language
  locator: Moving Captured Values Out of Closures and the Fn Traits
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/function/package-summary.html
  title: Package java.util.function, Java SE 25 API
  locator: the package description
- url: https://learn.microsoft.com/en-us/dotnet/csharp/programming-guide/delegates/using-delegates
  title: Using Delegates, C# Programming Guide
  locator: the whole page
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Function
  title: Function, MDN Web Docs
  locator: the introduction
- url: https://www.typescriptlang.org/docs/handbook/2/functions.html#call-signatures
  title: More on Functions, The TypeScript Handbook
  locator: Call Signatures
- url: https://go.dev/ref/spec#Method_values
  title: The Go Programming Language Specification
  locator: Method values
- url: https://pkg.go.dev/net/http#HandlerFunc
  title: Package http, Go standard library
  locator: type HandlerFunc
- url: https://en.cppreference.com/w/cpp/algorithm/for_each.html
  title: std::for_each, C++ reference, cppreference.com
  locator: the declaration; Return value
---

## Idea

A function object is an object that the program can call with the syntax of a function call. Wikipedia describes a function object as "a construct allowing an object to be invoked or called as if it were an ordinary function, usually with the same syntax"[^1]. "In some languages, particularly C++, function objects are often called functors (not related to the functional programming concept)."[^1]

A function object is an object, so it has fields. It can keep data from one call to the next call.

```python run id=idea
class Multiplier:
    def __init__(self, factor):
        self.factor = factor
        self.calls = 0

    def __call__(self, x):
        self.calls += 1
        return x * self.factor


triple = Multiplier(3)
print(triple(5))
print(list(map(triple, [1, 2, 3])))
print(triple.calls)
```

```output for=idea
15
[3, 6, 9]
4
```

`triple` is an object of the class `Multiplier`. The call `triple(5)` runs the method `__call__`. `map` takes `triple` as it takes any function. The field `calls` counts the four calls.

## How it works

1. **A call operator on the type.** The type defines what a call does. In Python, `__call__` is "Called when the instance is “called” as a function"[^2]. In C++, "When a user-defined class overloads the function call operator operator(), it becomes a FunctionObject type."[^3]
2. **The call.** A call `f(x)` on an object runs that method with the object as the receiver. Python says that `x(arg1, arg2, ...)` "roughly translates to type(x).__call__(x, arg1, ...)"[^2].
3. **The state.** The fields of the object hold the data that the function needs. Wikipedia says that "Another advantage of function objects is their ability to maintain a state that affects operator() between calls."[^1]
4. **Use as a function.** Code that takes a function can take the object. In C++, "A FunctionObject type is the type of an object that can be used on the left of the function call operator."[^4]

## In languages

- **Python.** Any class with a `__call__` method. "Instances of arbitrary classes can be made callable by defining a __call__() method in their class."[^2] Functions, methods and classes are callable too: "Classes are callable."[^2]
- **C++.** A class with `operator()`[^3]. A lambda is a function object of a class that the compiler makes, "known as closure type"[^5]. `std::function` can hold any function object with the right signature[^6].
- **Rust.** A closure is a function object. The traits `Fn`, `FnMut` and `FnOnce` describe how it uses its captured data. `Fn` is "The version of the call operator that takes an immutable receiver."[^7] "FnMut applies to closures that don’t move captured values out of their body but might mutate the captured values."[^8]
- **Java.** An object of a class that implements a functional interface. "Java has no first-class functions, so function objects are usually expressed by an interface with a single method"[^1]. For example, `IntUnaryOperator` "Represents an operation on a single int-valued operand that produces an int-valued result."[^9] The caller calls the method of the interface by its name.
- **C#.** A delegate for an instance method is a function object. "When a delegate is constructed to wrap an instance method, the delegate references both the instance and the method."[^10]
- **JavaScript.** "In JavaScript, every function is actually a Function object."[^11] A function can have properties, so a function is already a function object.
- **TypeScript.** A call signature in an object type describes a function with properties. "If we want to describe something callable with properties, we can write a call signature in an object type"[^12].
- **Go.** Go has no call operator on objects. A method value `x.M` gives a function: "The method value x.M is a function value that is callable with the same arguments as a method call of x.M."[^13] Go also goes the other way. A function type can have methods, as in `http.HandlerFunc`: "If f is a function with the appropriate signature, HandlerFunc(f) is a Handler that calls f."[^14]
- **C.** C has no function objects. A struct holds the data, and a function takes a pointer to the struct.
- **Bash.** Bash has no objects. A function keeps data between calls in global variables.

## Variations and related ideas

- **Closures.** A closure and a function object both join code and data. "Many uses of functors in languages like C++ are simply emulations of the missing closure constructor."[^1] A C++ lambda is a function object with the captured variables as its fields[^5]. See [[closures]].
- **Function pointers.** A function pointer has no data. "A callback in procedural languages, such as C, may be performed by using function pointers."[^1] "However it can be difficult or awkward to pass a state into or out of the callback function."[^1] See [[function-pointer-and-delegate]].
- **Single-method interfaces.** Java uses an interface with one method in place of a call operator[^1][^9].
- **Function to object.** Go `http.HandlerFunc` turns a function into a value of an interface type[^14].

## Pitfalls

### A C++ algorithm works on a copy

The C++ algorithms take the function object by value. `std::for_each` is declared as `UnaryFunc for_each( InputIt first, InputIt last, UnaryFunc f );`[^15], so it changes a copy of the object:

```cpp wrong id=cpp-for-each-copy
#include <algorithm>
#include <iostream>
#include <vector>

struct Counter {
    int calls = 0;
    void operator()(int) { ++calls; }
};

int main() {
    std::vector<int> v = {1, 2, 3};
    Counter c;
    std::for_each(v.begin(), v.end(), c);
    std::cout << c.calls << '\n';
}
```

```output for=cpp-for-each-copy
0
```

The copy counted 3 calls, and the program dropped it. `std::for_each` returns the copy: its return value is `f`[^15]. Write `c = std::for_each(v.begin(), v.end(), c);`, and `c.calls` is 3.

### A mutating closure is not an Fn in Rust

A Rust closure that changes a captured variable implements `FnMut` but not `Fn`[^8]. A function with an `Fn` bound does not accept it. `Fn` is for code that will "call it repeatedly and without mutating state (e.g., when calling it concurrently)"[^7]. The same page says: "If you do not need such strict requirements, use FnMut or FnOnce as bounds."[^7]

## Cost

A call to a function object is a call to a method. When the compiler knows the type of the object, it knows which method runs. Wikipedia notes for D that function objects in the C++ style exist "To allow the compiler to inline the code"[^1]. A C++ `std::function` is "a general-purpose polymorphic function wrapper"[^6], so one type holds targets of many types.

## References
