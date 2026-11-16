---
tag: VVCCP6
kind: concept
slug: callback
name: Callback
aliases: [call-back, synchronous callback, asynchronous callback, deferred callback, callback argument]
area: functions-and-abstraction/functions-as-values
level: intro
summary: >-
  A callback is a function that a program gives to another function, so that the other function can call it now or later. Sort comparators, event handlers and timer functions are callbacks.
applies_to: core-languages
viz: optional
formal: none
see_also: [callback-pattern, first-class-function, higher-order-functions, function-pointer-and-delegate, anonymous-function, closures, async-await, event-driven-programming]
sources:
- url: https://developer.mozilla.org/en-US/docs/Glossary/Callback_function
  title: Callback function, MDN Web Docs glossary
  locator: the whole page
- url: https://en.wikipedia.org/wiki/Callback_(computer_programming)
  title: Callback (computer programming), Wikipedia
  locator: the introduction; Use; Event handling; Asynchronous action; Implementation
- url: https://en.cppreference.com/w/c/algorithm/qsort.html
  title: qsort, qsort_s, C reference, cppreference.com
  locator: the declaration; the description
- url: https://docs.python.org/3/library/functions.html#sorted
  title: Built-in Functions, The Python Standard Library
  locator: sorted
- url: https://docs.python.org/3/library/atexit.html
  title: atexit, Exit handlers, The Python Standard Library
  locator: the introduction; atexit.register
- url: https://developer.mozilla.org/en-US/docs/Web/API/Window/setTimeout
  title: Window, setTimeout() method, MDN Web Docs
  locator: the introduction; Parameters
- url: https://www.typescriptlang.org/docs/handbook/2/functions.html#function-type-expressions
  title: More on Functions, The TypeScript Handbook
  locator: Function Type Expressions
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/List.html#sort(java.util.Comparator)
  title: Interface List, Java SE 25 API
  locator: sort
- url: https://learn.microsoft.com/en-us/dotnet/csharp/programming-guide/delegates/
  title: Delegates, C# Programming Guide
  locator: the introduction; Delegates overview
- url: https://en.cppreference.com/w/cpp/utility/functional/function.html
  title: std::function, C++ reference, cppreference.com
  locator: the introduction
- url: https://pkg.go.dev/sort#Slice
  title: Package sort, Go standard library
  locator: func Slice
- url: https://doc.rust-lang.org/book/ch13-01-closures.html
  title: Closures, The Rust Programming Language
  locator: Capturing the Environment with Closures
- url: https://www.gnu.org/software/bash/manual/html_node/Bourne-Shell-Builtins.html
  title: Bourne Shell Builtins, Bash Reference Manual
  locator: trap
- url: https://developer.mozilla.org/en-US/docs/Learn_web_development/Extensions/Async_JS/Introducing
  title: Introducing asynchronous JavaScript, MDN Web Docs
  locator: Callbacks
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Array/forEach
  title: Array.prototype.forEach(), MDN Web Docs
  locator: Parameters; Description
---

## Idea

A callback is a function that one part of a program gives to another part, and the other part calls it. MDN gives the definition: "A callback function is a function passed into another function as an argument, which is then invoked inside the outer function to complete some kind of routine or action."[^1]

The function that receives the callback can call it at once, or it can keep it and call it later.

```python run id=idea
import atexit


def each(items, callback):
    for item in items:
        callback(item)


def goodbye():
    print("goodbye")


atexit.register(goodbye)
each([1, 2, 3], lambda x: print("item", x))
print("end of main")
```

```output for=idea
item 1
item 2
item 3
end of main
goodbye
```

`each` calls its callback three times before it returns. `atexit.register` keeps the function `goodbye`, and Python calls it when the program ends, after the last line of the main code.

## How it works

1. **The consumer writes a function.** "The consumer of a callback-based API writes a function that is passed into the API."[^1]
2. **The consumer gives it to the provider.** Wikipedia says that "a function reference is passed from one context (consumer) to another (provider) such that the provider can call the function."[^2] The function is an argument, or the consumer registers it, as with an event handler[^2].
3. **The provider calls it.** "The provider of the API (called the caller) takes the function and calls back (or executes) the function at some point inside the caller's body."[^1] The provider gives the arguments: "The caller is responsible for passing the right parameters into the callback function."[^1]
4. **The result.** "The caller may also expect a particular return value from the callback function, which is used to instruct further behavior of the caller."[^1] In `qsort`, the result of the comparison callback tells the sort the order of two items[^3].

### Now or later

MDN gives two kinds: "Synchronous callbacks are called immediately after the invocation of the outer function, with no intervening asynchronous tasks, while asynchronous callbacks are called at some point later, after an asynchronous operation has completed."[^1] Wikipedia says the same in other words: "A function that accepts a callback parameter may be designed to call back before returning to its caller. But, more typically, a callback reference is stored by the provider so that it can call the function later (as deferred)."[^2]

## In languages

- **Python.** Any function or callable object. The `key` argument of `sorted` "specifies a function of one argument that is used to extract a comparison key from each element in iterable"[^4]. `atexit.register` keeps a function for later: "Register func as an exit handler."[^5]
- **JavaScript.** Any function. "Examples of synchronous callbacks include the callbacks passed to Array.prototype.map(), Array.prototype.forEach(), etc. Examples of asynchronous callbacks include the callbacks passed to setTimeout() and Promise.prototype.then()."[^1] The parameter of `setTimeout` is "A Function to be executed after the timer expires."[^6]
- **TypeScript.** The same as JavaScript, and a function type expression gives the type of the callback. In the handbook example `function greeter(fn: (a: string) => void)`, the type `(a: string) => void` means "a function with one parameter, named a, of type string, that doesn’t have a return value"[^7].
- **Java.** An object of a functional interface, often made with a lambda. `List.sort` "Sorts this list according to the order induced by the specified Comparator"[^8].
- **C#.** Delegates. "The ability to refer to a method as a parameter makes delegates ideal for defining callback methods."[^9]
- **C.** Function pointers. "In assembly, C, C++, Pascal, Modula-2 and other languages, a callback function is stored internally as a function pointer"[^2]. In `qsort`, the "Function pointed to by comp is used for object comparison."[^3] See [[function-pointer-and-delegate]].
- **C++.** A function pointer, a lambda or another function object. A function that keeps a callback often stores it in a `std::function`, "a general-purpose polymorphic function wrapper"[^10].
- **Go.** Function values. "Slice sorts the slice x given the provided less function."[^11]
- **Rust.** Closures and functions. The method `unwrap_or_else` of `Option<T>` "takes one argument: a closure without any arguments that returns a value T"[^12]. It calls the closure only when the value is `None`[^12].
- **Bash.** The name of a function in a variable or an argument. The `trap` builtin keeps a command for later: "The action is a command that is read and executed when the shell receives any of the signals sigspec."[^13] "If a sigspec is 0 or EXIT, action is executed when the shell exits."[^13]

## Variations and related ideas

- **Blocking and deferred callbacks.** "A blocking callback runs in the execution context of the function that passes the callback. A deferred callback can run in a different context such as during interrupt or from a thread."[^2]
- **Event handlers.** "Often, consuming code registers a callback for a particular type of event. When that event occurs, the callback is called."[^2] MDN says: "An event handler is a particular type of callback."[^14] See [[event-driven-programming]].
- **Context data.** A C callback gets only its arguments. A library can take one more pointer for the data of the caller: C11 `qsort_s` passes its `context` parameter to the comparison function[^3]. In languages with closures, the callback captures the data. See [[closures]].
- **Promises and async functions.** "For these reasons, most modern asynchronous APIs don't use callbacks. Instead, the foundation of asynchronous programming in JavaScript is the Promise"[^14]. See [[futures]] and [[async-await]].
- **The design pattern.** The page [[callback-pattern]] tells when to design an API around callbacks.

## Pitfalls

### An async callback that nobody waits for

`forEach` calls its callback and does not use the result: the return value of the callback "is discarded"[^15]. An `async` function returns a promise, so `forEach` does not wait for it. "forEach() expects a synchronous function — it does not wait for promises."[^15] This is the example from MDN:

```javascript wrong id=javascript-async-foreach
const ratings = [5, 4, 5];
let sum = 0;

const sumFunction = async (a, b) => a + b;

ratings.forEach(async (rating) => {
  sum = await sumFunction(sum, rating);
});

console.log(sum);
```

```output for=javascript-async-foreach
0
```

The program prints `sum` before any callback adds a rating. MDN gives the next step: "To run a series of asynchronous operations sequentially or concurrently, see promise composition."[^15] A `for...of` loop with `await` in an async function runs the steps one after another.

### Not knowing when the callback runs

Code after a call can see the effects of the callback or not, as a function of the time of the call. MDN says: "Understanding whether the callback is synchronously or asynchronously called is particularly important when analyzing side effects."[^1] In its example, the program sets a variable in a callback and then prints the variable, and the output is 1 or 2 as a function of the kind of callback[^1]. Read the documentation of the provider, and do not depend on the order when it does not give one.

### Callbacks in callbacks

Steps that each take a callback nest inside each other. "Because we have to call callbacks inside callbacks, we get a deeply nested doOperation() function, which is much harder to read and debug."[^14] MDN calls this "callback hell" or the "pyramid of doom"[^14]. Errors are hard to handle too: you often must handle errors at each level, "instead of having error handling only once at the top level."[^14] Use promises or `async` and `await` when the language has them.

## Cost

A call to a callback is a call through a function value. In C and similar languages the provider holds "a function pointer"[^2], so the call is an indirect call. See [[function-pointer-and-delegate]]. A deferred callback keeps its data alive until the call. Python `atexit` keeps the arguments too: "Any optional arguments that are to be passed to func must be passed as arguments to register()."[^5]

## References
