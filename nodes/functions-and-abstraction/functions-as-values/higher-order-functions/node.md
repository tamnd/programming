---
tag: NEW-higher-order-functions
kind: concept
slug: higher-order-functions
name: Higher-order function
aliases: [higher-order procedure]
area: functions-and-abstraction/functions-as-values
level: intro
summary: >-
  A higher-order function takes a function as an argument or returns a function as its result. Map, filter, fold, sort with a key, composition and callbacks are the common forms.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1960
  people: [John McCarthy]
  note: McCarthy defined maplist and search, functions with functions as arguments, in his paper on the recursive functions of Lisp.
see_also: [closures, recursion, iterators, parametric-polymorphism, immutability, subtyping]
same_as:
  wikidata: Q1474542
  wikipedia: Higher-order_function
sources:
  - url: https://kotlinlang.org/docs/lambdas.html#higher-order-functions
    title: Kotlin documentation, Higher-order functions and lambdas
    locator: Higher-order functions
  - paper: doi:10.1023/A:1010000313106
    title: Christopher Strachey, Fundamental Concepts in Programming Languages, Higher-Order and Symbolic Computation 13(1-2), 2000 (lecture notes from August 1967)
    locator: 3.5.1 First and second class objects
  - url: https://docs.python.org/3.14/reference/datamodel.html#user-defined-functions
    title: The Python Language Reference, Data model, Python 3.14
    locator: 3.2.8.1 User-defined functions
  - url: https://port70.net/~nsz/c/c11/n1570.html#6.3.2.1p4
    title: ISO/IEC 9899:201x, Programming languages, C (N1570, the C11 draft)
    locator: 6.3.2.1, paragraph 4 (function designators)
  - url: https://port70.net/~nsz/c/c11/n1570.html#6.5.2.2p1
    title: ISO/IEC 9899:201x, Programming languages, C (N1570, the C11 draft)
    locator: 6.5.2.2, paragraph 1 (function calls)
  - url: https://doc.rust-lang.org/reference/types/function-pointer.html
    title: The Rust Reference
    locator: Function pointer types, coercion from function items and non-capturing closures
  - url: https://doc.rust-lang.org/reference/types/trait-object.html
    title: The Rust Reference
    locator: Trait objects, the layout of a pointer to a trait object
  - url: https://timsong-cpp.github.io/cppwp/n4950/expr.prim.lambda.closure
    title: Working Draft, Programming Languages, C++ (N4950, the C++23 draft)
    locator: "[expr.prim.lambda.closure], paragraphs 1 and 8"
  - url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-9.html#jls-9.8
    title: The Java Language Specification, Java SE 25 Edition
    locator: 9.8 Functional Interfaces
  - url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-15.html#jls-15.13
    title: The Java Language Specification, Java SE 25 Edition
    locator: 15.13 Method Reference Expressions
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/programming-guide/delegates/
    title: C# programming guide, Delegates
    locator: Overview of delegates
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/operators/lambda-expressions
    title: C# language reference, Lambda expressions
    locator: the conversion of a lambda expression to the Func and Action delegate types
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.func-2
    title: Microsoft .NET API reference, Func<T,TResult> Delegate
    locator: Definition
  - url: https://docs.python.org/3.14/library/functions.html#map
    title: The Python Standard Library, Built-in Functions, Python 3.14
    locator: map(), filter() and sorted()
  - url: https://docs.python.org/3.14/library/functools.html#functools.reduce
    title: The Python Standard Library, functools, Python 3.14
    locator: functools.reduce()
  - url: https://tc39.es/ecma262/2026/#sec-array.prototype.map
    title: ECMA-262, ECMAScript 2026 Language Specification
    locator: 23.1.3.21 Array.prototype.map
  - url: https://tc39.es/ecma262/2026/#sec-array.prototype.filter
    title: ECMA-262, ECMAScript 2026 Language Specification
    locator: 23.1.3.8 Array.prototype.filter
  - url: https://tc39.es/ecma262/2026/#sec-array.prototype.reduce
    title: ECMA-262, ECMAScript 2026 Language Specification
    locator: 23.1.3.24 Array.prototype.reduce
  - url: https://port70.net/~nsz/c/c11/n1570.html#7.22.5.2
    title: ISO/IEC 9899:201x, Programming languages, C (N1570, the C11 draft)
    locator: 7.22.5.2 The qsort function
  - url: https://pkg.go.dev/slices#SortFunc
    title: Go standard library, package slices
    locator: func SortFunc
  - url: https://timsong-cpp.github.io/cppwp/n4950/alg.transform
    title: Working Draft, Programming Languages, C++ (N4950, the C++23 draft)
    locator: "[alg.transform]"
  - url: https://doc.rust-lang.org/book/ch13-02-iterators.html#methods-that-produce-other-iterators
    title: The Rust Programming Language, chapter 13.2
    locator: Methods That Produce Other Iterators
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.linq.enumerable.select
    title: Microsoft .NET API reference, Enumerable.Select Method
    locator: Remarks (deferred execution)
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/function/package-summary.html
    title: Java SE 25 API, package java.util.function
    locator: "package description: function shapes and primitive specializations"
  - url: https://go.dev/ref/spec#Function_types
    title: The Go Programming Language Specification
    locator: Function types
  - url: https://go.dev/ref/spec#Comparison_operators
    title: The Go Programming Language Specification
    locator: Comparison operators
  - url: https://doc.rust-lang.org/book/ch13-01-closures.html#moving-captured-values-out-of-closures-and-the-fn-traits
    title: The Rust Programming Language, chapter 13.1
    locator: Moving Captured Values Out of Closures and the Fn Traits
  - url: https://www.typescriptlang.org/docs/handbook/2/functions.html#function-type-expressions
    title: TypeScript Handbook, More on Functions
    locator: Function Type Expressions; Optional Parameters in Callbacks
  - url: https://kotlinlang.org/docs/lambdas.html#function-types
    title: Kotlin documentation, Higher-order functions and lambdas
    locator: Function types
  - url: https://learn.microsoft.com/en-us/powershell/module/microsoft.powershell.core/about/about_script_blocks?view=powershell-7.5
    title: PowerShell documentation, about_Script_Blocks
    locator: Long description; Using the call operator
  - url: https://learn.microsoft.com/en-us/powershell/module/microsoft.powershell.core/foreach-object?view=powershell-7.5
    title: PowerShell documentation, ForEach-Object
    locator: Description, script block syntax
  - url: https://www.gnu.org/software/bash/manual/bash.html#Command-Search-and-Execution
    title: Bash Reference Manual
    locator: 3.7.2 Command Search and Execution
  - url: https://plato.stanford.edu/archives/fall2025/entries/lambda-calculus/#BriHisLCal
    title: Jesse Alama and Johannes Korbmacher, The Lambda Calculus, Stanford Encyclopedia of Philosophy (Fall 2025 Edition)
    locator: 3. Brief history of the lambda calculus
  - url: https://tc39.es/ecma262/2026/#sec-function.prototype.bind
    title: ECMA-262, ECMAScript 2026 Language Specification
    locator: 20.2.3.2 Function.prototype.bind
  - paper: doi:10.1145/359576.359579
    title: John Backus, Can Programming Be Liberated from the von Neumann Style? A Functional Style and Its Algebra of Programs, Communications of the ACM 21(8), 1978
    locator: 5.2 A Functional Program for Inner Product; 11.2.4 Functional forms
  - url: https://tc39.es/ecma262/2026/#sec-parseint-string-radix
    title: ECMA-262, ECMAScript 2026 Language Specification
    locator: 19.2.5 parseInt ( string, radix )
  - url: https://docs.python.org/3.14/glossary.html#term-iterator
    title: Python Glossary, Python 3.14
    locator: iterator
  - url: https://tc39.es/ecma262/2026/#sec-ordinarycallbindthis
    title: ECMA-262, ECMAScript 2026 Language Specification
    locator: 10.2.1.2 OrdinaryCallBindThis
  - url: https://tc39.es/ecma262/2026/#sec-strict-mode-code
    title: ECMA-262, ECMAScript 2026 Language Specification
    locator: 11.2.2 Strict Mode Code
  - url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-11.html#jls-11.2.3
    title: The Java Language Specification, Java SE 25 Edition
    locator: 11.2.3 Exception Checking
  - url: https://go.dev/ref/spec#Calls
    title: The Go Programming Language Specification
    locator: Calls
  - url: https://timsong-cpp.github.io/cppwp/n4950/func.wrap.func.inv
    title: Working Draft, Programming Languages, C++ (N4950, the C++23 draft)
    locator: "[func.wrap.func.inv], paragraph 2"
  - url: https://port70.net/~nsz/c/c11/n1570.html#6.5p5
    title: ISO/IEC 9899:201x, Programming languages, C (N1570, the C11 draft)
    locator: 6.5, paragraph 5 (exceptional conditions)
  - url: https://port70.net/~nsz/c/c11/n1570.html#7.22.5p4
    title: ISO/IEC 9899:201x, Programming languages, C (N1570, the C11 draft)
    locator: 7.22.5, paragraph 4 (consistent results of the comparison function)
  - url: https://doc.rust-lang.org/book/ch10-01-syntax.html#performance-of-code-using-generics
    title: The Rust Programming Language, chapter 10.1
    locator: Performance of Code Using Generics
  - url: https://doc.rust-lang.org/book/ch18-02-trait-objects.html#trait-objects-perform-dynamic-dispatch
    title: The Rust Programming Language, chapter 18.2
    locator: Trait Objects Perform Dynamic Dispatch
  - url: https://kotlinlang.org/docs/inline-functions.html
    title: Kotlin documentation, Inline functions
    locator: introduction (runtime penalties of function objects)
  - paper: doi:10.1145/367177.367199
    title: John McCarthy, Recursive Functions of Symbolic Expressions and Their Computation by Machine, Part I, Communications of the ACM 3(4), 1960
    locator: 3g Functions with Functions as Arguments (maplist and search)
  - url: https://docs.python.org/3.14/whatsnew/3.0.html#views-and-iterators-instead-of-lists
    title: What's New In Python 3.0
    locator: Views And Iterators Instead Of Lists; Builtins
  - url: https://openjdk.org/jeps/126
    title: "JEP 126: Lambda Expressions and Virtual Extension Methods"
    locator: header (Release 8, JSR 335)
  - url: https://openjdk.org/projects/jdk8/
    title: OpenJDK, JDK 8 project page
    locator: General Availability date
---

## Idea

A higher-order function is a function that takes a function as an argument, returns a function as its result, or does both.[^1] A language can have higher-order functions only when functions are values. Code must be able to pass a function, return it and store it in a variable, as it does with a number. Strachey called such values first class. He called the procedures of ALGOL second class, because no ALGOL expression can have a procedure as its result.[^2]

Higher-order functions separate the shape of a computation from its details. A loop that changes each element of a list has the same shape for every change. A higher-order function holds the loop once and takes the change as an argument. In Python, `apply_to_all` below takes a function, and `make_adder` returns one.

```python run id=idea
def apply_to_all(f, items):
    result = []
    for item in items:
        result.append(f(item))
    return result


def make_adder(n):
    return lambda x: x + n


print(apply_to_all(abs, [-2, 3, -5]))
add_ten = make_adder(10)
print(apply_to_all(add_ten, [1, 2, 3]))
print(apply_to_all(str.upper, ["a", "b"]))
```

```output for=idea
[2, 3, 5]
[11, 12, 13]
['A', 'B']
```

The built-in function `abs`, the method `str.upper` and the result of `make_adder` are all function objects, so `apply_to_all` accepts each of them. The function that `make_adder` returns uses `n` after `make_adder` has returned. [[closures]] explains how it keeps `n`.

## How it works

### Functions as values

In Python, a `def` statement or a `lambda` expression makes a function object when it runs.[^3] The name of the function is a variable like any other, and its value is the object. In C, a function name in an expression converts to a pointer to the function, except as the operand of `sizeof`, `_Alignof` or `&`.[^4] The call operator takes an expression of pointer-to-function type, so `f(x)` and `(*f)(x)` mean the same thing.[^5]

### The machine view

At the machine level, a function value has one of two forms:

- **A code pointer.** A function that uses no outer variables needs only the address of its code. A C function pointer and a Rust `fn` type are code pointers.[^6]
- **A code pointer and an environment.** A function that uses outer variables also needs those values. The value is a pair: the address of the code and a record of the captured values.

A call of a named function is a direct call: the address of the target is fixed in the instruction. A call through a function value is an indirect call. The program loads the address from the value and then jumps to it. The Rust program below shows the sizes of the forms on a 64-bit target.

```rust run id=sizes
use std::mem::size_of_val;

fn add_one(x: i32) -> i32 {
    x + 1
}

fn main() {
    let k = 10;
    let pointer: fn(i32) -> i32 = add_one;
    let closure = move |x: i32| x + k;
    let erased: &dyn Fn(i32) -> i32 = &closure;
    let boxed: Box<dyn Fn(i32) -> i32> = Box::new(closure);
    println!("fn pointer: {} bytes", size_of_val(&pointer));
    println!("closure: {} bytes", size_of_val(&closure));
    println!("&dyn Fn: {} bytes", size_of_val(&erased));
    println!("Box<dyn Fn>: {} bytes", size_of_val(&boxed));
    println!("{} {} {} {}", pointer(1), closure(1), erased(1), boxed(1));
}
```

```output for=sizes
fn pointer: 8 bytes
closure: 4 bytes
&dyn Fn: 16 bytes
Box<dyn Fn>: 16 bytes
2 11 11 11
```

The function pointer is one address. The closure holds only the captured `k`, because its type tells the compiler where its code is. The `dyn` forms erase the type, so they hold two pointers: one to the captured data and one to a vtable. The vtable is a table of function pointers for the methods of the trait.[^7]

Other languages use the same two forms behind an object. A C++ lambda expression makes an object of a unique class with a call operator. A lambda that captures nothing also converts to a plain function pointer.[^8] A Java lambda or method reference makes an object of a functional interface, an interface with one abstract method. The call is a call of that method.[^9][^10] A C# lambda converts to a delegate type such as `Func<int, int>`. A delegate is a type that represents references to methods with a given parameter list and return type.[^11][^12][^13]

### The common operations

Most standard libraries supply the same small set of higher-order functions for collections:

| Operation | What it does | Python | JavaScript | C# | Rust |
|---|---|---|---|---|---|
| map | applies `f` to each element and keeps the results | `map` | `map` | `Select` | `Iterator::map` |
| filter | keeps the elements for which `p` returns true | `filter` | `filter` | `Where` | `Iterator::filter` |
| fold | combines the elements with `f` and a start value | `functools.reduce` | `reduce` | `Aggregate` | `Iterator::fold` |
| sort | orders the elements by a key or a comparator | `sorted(key=...)` | `sort(compare)` | `OrderBy` | `sort_by_key` |

In Python, `map` and `filter` return iterators, and `reduce` applies a function of two arguments cumulatively from the left.[^14][^15] In JavaScript, `map` and `filter` make a fresh array, and `reduce` passes the accumulator and the element to the callback.[^16][^17][^18] The example files show the same operations in each language.

A sort takes a function in one of two shapes. A key function takes one element and returns a value that the sort compares, as in Python `sorted(words, key=len)`.[^14] A comparator takes two elements and returns a negative number, zero or a positive number. The C function `qsort` and the Go function `slices.SortFunc` take comparators.[^19][^20] A key function is often shorter. A comparator can express any order, but it must follow the contract of the sort (see Pitfalls).

C++ names the map operation `std::transform` and the fold `std::accumulate`. The ranges forms of the algorithms also take a projection, a function that the algorithm applies to each element before it compares or uses it.[^21]

### Eager and lazy results

A higher-order function can do its work at once or when the consumer asks for the results. JavaScript `Array.prototype.map` is eager: it calls the callback for each element and returns a complete array.[^16] Python `map` and `filter`, the Rust iterator adapters and C# `Select` and `Where` are lazy. They return an object that calls the function only when code asks for the next item.[^14][^22][^23] A lazy pipeline of several steps makes no intermediate collections. It also means that the function runs later, and perhaps more than once or not at all. [[iterators]] covers the protocol that lazy results use.

### Composition and returned functions

A function that returns a function can build behaviour from parts. The function `compose(f, g)` returns a function that applies `g` and then `f`. In Java, the interface `Function` has the default methods `andThen` and `compose` for this.[^24] A returned function usually needs the arguments of the call that made it, so it is a closure. [[closures]] describes how the captured variables stay alive.

### Callbacks

A callback is a function that one part of a program gives to another, so that the other part can call it later. An event handler, a completion function and a comparator are callbacks. The caller gives control of the timing to the receiver. The C# guide says that the ability to refer to a method as a parameter makes delegates ideal for callback methods.[^11]

## In languages

| Language | Verdict | Form |
|---|---|---|
| Python | native | function objects from `def` and `lambda`; `map`, `filter`, `functools.reduce`, `key=` |
| JavaScript | native | function objects; `map`, `filter`, `reduce`, `sort` on arrays |
| TypeScript | native | as JavaScript, with function types such as `(x: T) => U` |
| Java | native | lambda expressions and method references that implement a functional interface |
| C | native, limited | function pointers; no lambda expressions and no captured state |
| C++ | native | lambda expressions, function objects, function templates and `std::function` |
| C# | native | delegates such as `Func<T, TResult>`; LINQ methods |
| Go | native | function types such as `func(T) U`; function literals |
| Rust | native | closures with the traits `Fn`, `FnMut` and `FnOnce`; `fn` pointers |
| Kotlin | native | function types such as `(Int) -> Int`; lambdas; `inline` functions |
| PowerShell | native | script blocks; `&`; `ForEach-Object`, `Where-Object`, `Sort-Object` |
| Bash | idiom | the name of a function in a variable |
| SQL | n/a | a query has no function values |
| Regex | n/a | a pattern has no functions |
| HTML | n/a | markup has no functions |
| CSS | n/a | style rules have no function values |

Each language with a native form also has its own type for function values. A Go function type denotes all functions with the same parameter and result types, and its zero value is `nil`.[^25] Go function values cannot be compared, except with `nil`.[^26] Rust gives each closure a unique type, and a generic parameter names it through the traits `Fn`, `FnMut` and `FnOnce`.[^27] A TypeScript function type expression has the form `(a: string) => void`.[^28] A Kotlin function type has the form `(Int) -> Int`, and `((Int) -> Int)?` is its nullable form.[^29]

A PowerShell script block is a collection of statements that can be used as a single unit. A variable can hold a script block. The call operator `&` runs it.[^30] `ForEach-Object` runs a script block for each input object and sets `$_` to the current object.[^31]

Bash functions are not values, but a command name can come from an expansion. When the first word of a command has no slash and names a shell function, the shell calls that function.[^32] So a variable that holds the name of a function acts as a function argument.

```bash run id=bash-name
shout() { echo "$1!"; }
whisper() { echo "($1)"; }

apply_to_all() {
  local f=$1
  shift
  for item in "$@"; do
    "$f" "$item"
  done
}

apply_to_all shout pear fig
apply_to_all whisper banana
```

```output for=bash-name
pear!
fig!
(banana)
```

The name is a string, so the shell finds out whether the function exists only at the call.

## Variations and related ideas

- **Currying.** A function of two arguments can be a function of one argument that returns a function of the other. Frege observed that functions of one argument are enough in the study of functions, for this reason. The transformation is called currying.[^33]
- **Partial application.** A function that fixes some arguments of another function returns a function of the rest. JavaScript `Function.prototype.bind` fixes the `this` value and leading arguments.[^34]
- **Combinators and point-free style.** Backus wrote programs as combinations of functions with no variables. His inner product program composes the functional forms Insert, ApplyToAll and Composition.[^35]
- **Generic higher-order functions.** A function such as `map` works for any element type only when the language can express it with type parameters. [[parametric-polymorphism]] covers this.
- **Function types and subtypes.** A function that accepts more kinds of arguments can stand in for a function that accepts fewer. The TypeScript checker allows a callback with fewer parameters than the function type.[^28] [[subtyping]] gives the general rule.
- **Recursion.** A fold replaces an explicit recursive walk of a list. [[recursion]] shows the recursive form.
- **Immutability.** `map` and `filter` return fresh collections and leave the input alone, while JavaScript `sort` changes the array in place. [[immutability]] discusses the difference.

## Pitfalls

### A callback receives more arguments than it expects

JavaScript `map` calls the callback with three arguments: the element, the index and the array.[^16] `parseInt` takes a string and an optional radix, so the index becomes the radix.

```javascript wrong id=parseint
console.log(["1", "2", "3"].map(parseInt));
```

```output for=parseint
[ 1, NaN, NaN ]
```

For the first element, the radix is 0, and `parseInt` uses base 10. For the second, the radix is 1, and a radix less than 2 gives `NaN`. For the third, the radix is 2, and "3" is not a digit in base 2.[^36] The fix is a callback that passes only the element.

```javascript run id=parseint-fix
console.log(["1", "2", "3"].map((s) => parseInt(s, 10)));
console.log(["1", "2", "3"].map(Number));
```

```output for=parseint-fix
[ 1, 2, 3 ]
[ 1, 2, 3 ]
```

### A lazy result is used twice

Python `map` returns an iterator, and an iterator gives its items only once. A second pass sees an exhausted iterator, which looks like an empty container.[^37]

```python wrong id=iterator-twice
lengths = map(len, ["pear", "fig", "banana"])
print(max(lengths))
print(sum(lengths))
```

```output for=iterator-twice
6
0
```

`max` takes all the items, so `sum` gets none and returns 0. The fix is `lengths = list(map(len, words))`, which stores the results once. C# queries have the opposite trap: each enumeration runs the query again, and so calls the function again.[^23]

### A method loses its object

In JavaScript, a method that a program passes as a callback is a plain function. The receiver calls it with no object, so `this` is `undefined` in strict code.[^38] All code inside a class is strict code.[^39]

```javascript fails expect=exception:TypeError id=lost-this
class Counter {
  constructor() {
    this.count = 0;
  }
  add(n) {
    this.count += n;
  }
}

const counter = new Counter();
[1, 2, 3].forEach(counter.add);
console.log(counter.count);
```

The run stops with `TypeError: Cannot read properties of undefined (reading 'count')`. The fix is an arrow function, `(n) => counter.add(n)`, or a bound method, `counter.add.bind(counter)`.[^34]

### A Java lambda cannot throw a checked exception

The method `apply` of `java.util.function.Function` declares no checked exceptions. A lambda body that can throw a checked exception that the function type does not declare is a compile-time error.[^40]

```java fails expect=compile id=checked
import java.io.IOException;
import java.util.List;

public class Main {
    static String load(String name) throws IOException {
        if (name.isEmpty()) {
            throw new IOException("empty name");
        }
        return name.toUpperCase();
    }

    public static void main(String[] args) {
        List<String> names = List.of("a", "b");
        System.out.println(names.stream().map(n -> load(n)).toList());
    }
}
```

`javac` stops with `unreported exception IOException; must be caught or declared to be thrown`. One fix is a `try` block inside the lambda that wraps the exception in an unchecked one. Another fix is a separate functional interface whose method declares `throws IOException`.

### A function value is nil or empty

A function variable can hold no function. In Go, the zero value of a function type is `nil`, and a call of a nil function value causes a run-time panic.[^25][^41]

```go fails expect=panic id=nil-func
package main

import "fmt"

func apply(f func(int) int, x int) int {
	return f(x)
}

func main() {
	var double func(int) int
	fmt.Println(apply(double, 3))
}
```

The program stops with `panic: runtime error: invalid memory address or nil pointer dereference`. In C++, a call of an empty `std::function` throws `std::bad_function_call`.[^42]

```cpp fails expect=exception:bad_function_call id=empty-function
#include <functional>
#include <print>

int main() {
    std::function<int(int)> double_it;
    std::println("{}", double_it(3));
}
```

With libc++, the program stops with `terminating due to uncaught exception of type std::__1::bad_function_call`. A function parameter that can be absent needs a check before the call. A type that cannot be empty also prevents the error, for example a Rust closure or a non-nullable Kotlin function type.

### A comparator overflows

A common C comparator subtracts the two values. The subtraction can overflow, and signed overflow is undefined behaviour.[^43] The sort then sees a wrong sign, or the program fails in another way.

```c fails expect=sanitizer:undefined id=comparator
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

static int compare_ints(const void *a, const void *b) {
    return *(const int *)a - *(const int *)b;
}

int main(void) {
    int items[] = {1, INT_MIN, 3};
    qsort(items, 3, sizeof items[0], compare_ints);
    printf("%d %d %d\n", items[0], items[1], items[2]);
    return 0;
}
```

UndefinedBehaviorSanitizer reports `runtime error: signed integer overflow: 1 - -2147483648 cannot be represented in type 'int'`. The fix is `return (x > y) - (x < y);`, which gives -1, 0 or 1 with no arithmetic on the values, as in the C example. A comparator must also give a consistent order. The results for the same objects must be consistent with one another and define a total order on the array.[^44]

## Cost

- **Indirect calls.** A call through a function pointer or a vtable is an indirect call. `qsort` calls the comparator through a pointer for each comparison.[^19] The compiler usually cannot inline a call when it does not know the target.
- **Monomorphization.** A generic function that takes the function type as a type parameter gets a separate copy for each closure type. Rust does this for generic code, and the result runs as fast as code written for the concrete types.[^45] Each C++ lambda has its own class, so a function template such as `std::ranges::sort` gets a copy for each lambda.[^8]
- **Dynamic dispatch.** `dyn Fn` in Rust hides the type of the closure. A call looks up the target in the vtable at run time, and this prevents inlining.[^46]
- **Function objects.** On the JVM, each Kotlin function value is an object, and its call is a virtual call. The `inline` modifier copies the function and its lambda arguments into the call site, which can remove this overhead.[^47]
- **Boxing.** The generic Java interfaces take reference types, so `Function<Integer, Integer>` boxes each `int`. The package `java.util.function` has primitive specializations, such as `IntUnaryOperator` and `ToIntFunction`, that avoid the boxing.[^24]
- **Intermediate collections.** Each eager `map` or `filter` step in JavaScript makes a complete array.[^16] A chain of lazy steps makes none, but it keeps the work for the time when the consumer asks for items.

## History

- **1893 to 1932.** Frege observed that functions of one argument are enough to describe functions of several arguments. In the 1920s, Schönfinkel studied combinators. In 1932, Church introduced the notation `λx[M]` for a function.[^33]
- **1960.** McCarthy defined `maplist[x; f]` and `search[x; p; f; u]` in his paper on Lisp, in a section named "Functions with Functions as Arguments". His `maplist` applies `f` to the list and to each of its tails.[^48]
- **1967.** Strachey gave lectures that named first class and second class objects. He used the procedures of ALGOL as his example of second class values. The notes appeared as a paper in 2000.[^2]
- **1978.** Backus described FP systems, where programs are combinations of functions with no variables. He called the combining operations functional forms.[^35]
- **2008.** Python 3.0 changed `map` and `filter` to return iterators, and it moved `reduce` to the module `functools`.[^49]
- **2014.** JDK 8 reached General Availability on 18 March 2014. JDK 8 delivered JEP 126, Lambda Expressions and Virtual Extension Methods, which belongs to JSR 335.[^50][^51]

## References
