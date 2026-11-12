---
tag: 2QAJS8
kind: pattern
slug: callback-pattern
name: Callback
aliases: [callback, callback function, call-after function]
area: software-design/behavioral-patterns
level: intro
summary: >-
  A callback is a function that one part of a program passes to another part, so that the other part can call it, now or later.
family: behavioral-design
polarity: positive
intent: Pass a function reference from a consumer to a provider, so that the provider can call the function back.
also_known_as: [callback function]
see_also: [higher-order-functions, closures, continuation-passing-style, futures, async-await, observer-pattern, event-driven-programming, command-pattern]
sources:
  - url: https://developer.mozilla.org/en-US/docs/Glossary/Callback_function
    title: Callback function, MDN Web Docs glossary
    locator: whole page
  - url: https://en.wikipedia.org/wiki/Callback_(computer_programming)
    title: Callback (computer programming), Wikipedia
    locator: lead; Use; Event handling; Asynchronous action; Implementation; Example code, C
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Guide/Using_promises
    title: Using promises, MDN Web Docs
    locator: Chaining; Error handling
  - url: https://github.com/nodejs/node/blob/v24.0.0/doc/api/errors.md
    title: Node.js v24.0.0 documentation source, doc/api/errors.md
    locator: Error propagation and interception, lines 83 to 86; ERR_MULTIPLE_CALLBACK, lines 2273 to 2279
  - url: https://github.com/golang/go/blob/go1.25.0/src/sort/slice.go#L12-L24
    title: Go go1.25.0 source, src/sort/slice.go
    locator: func Slice and its comment, lines 12 to 24
---

## Intent

A callback is a function passed into another function as an argument, which is then invoked inside the outer function to complete some kind of routine or action.[^1]

## Problem

A general function often needs one step that only its caller knows. A sort routine needs to know the order of two items. A window system needs to tell the program about a mouse click. A slow operation needs to say when it is done.[^2]

The general function cannot contain this step, because the step is different for each caller.

## Solution

The consumer passes a function reference to the provider, and the provider calls the function. If the function uses state or functionality of the consumer, then the call goes back to the consumer, opposite to the normal flow of control.[^2]

```text
   consumer                          provider
      |   call provider(callback)       |
      |-------------------------------->|
      |                                 |  ... does its work ...
      |        callback(arguments)      |
      |<--------------------------------|  calls back, now or later
```

The provider can call the callback before it returns to its caller. More typically, the provider stores the callback reference and calls the function later, as a deferred callback.[^2] Wikipedia compares a callback to instructions that you give to a tailor for what to do when a suit is ready, such as a phone number to call.[^2]

MDN names two ways to call a callback.[^1]

- **Synchronous.** The callback is called immediately after the invocation of the outer function, with no intervening asynchronous tasks. The callbacks passed to `Array.prototype.map()` and `Array.prototype.forEach()` are examples.
- **Asynchronous.** The callback is called at some point later, after an asynchronous operation has completed. The callbacks passed to `setTimeout()` and `Promise.prototype.then()` are examples.

## Example

The example files show one program in each language. First, `for_each` calls the callback `shout` once for each word, before it returns. Then a queue stores a second callback, and the program continues. The queue calls the stored callback only when it runs. The program prints:

```text
HELLO
WORLD
request sent
saved
```

## Consequences

The benefits:

- One general function can serve many callers. In C, `qsort()` and `bsearch()` take a comparator function that determines the collation order.[^2]
- A program can react to events. The consuming code registers a callback for a type of event, and the callback is called when that event occurs.[^2]
- A caller can request a slow action and continue. The callback is called when the action completes, which might be long after the request.[^2]

The costs:

- The caller must know whether the callback is called synchronously or asynchronously. This is particularly important for side effects.[^1]
- Steps in a sequence give nested callbacks. Several asynchronous operations in a row lead to the classic "callback hell", and the error callback is repeated at each level.[^3]
- The provider must call the callback the correct number of times. In Node.js, a callback is almost always meant to be called only once, and a second call is the error `ERR_MULTIPLE_CALLBACK`.[^4]

Here the program reads a value before a deferred callback has run. The callback changes `value` to 2, but the first `print` shows the old value:

```python wrong id=too-early
import asyncio


async def main():
    value = 1

    def set_value():
        nonlocal value
        value = 2

    asyncio.get_running_loop().call_soon(set_value)  # deferred
    print("value:", value)  # the callback has not run yet
    await asyncio.sleep(0)
    print("later:", value)


asyncio.run(main())
```

```output for=too-early
value: 1
later: 2
```

## When not to use it

- The operation is a sequence of asynchronous steps. Promises let each step attach its callback to a chain, so the error handler appears only once, at the end of the chain.[^3] With `async` and `await`, the code resembles synchronous code.[^3]
- The provider must call several related functions. Then an object with an interface can be clearer than several separate callbacks.

## Variants and related patterns

- **Error-first callbacks.** Most asynchronous methods of Node.js that accept a `callback` function pass an `Error` object as the first argument to that function. If that argument is not `null` and is an instance of `Error`, an error occurred that should be handled.[^4] The convention looks like this in Python:

```python run id=error-first
def read_config(name, callback):
    if name != "app.toml":
        callback(FileNotFoundError(name), None)  # error first
        return
    callback(None, "debug = true")


def done(err, data):
    if err is not None:
        print("error:", err)
    else:
        print("data:", data)


read_config("app.toml", done)
read_config("missing.toml", done)
```

```output for=error-first
data: debug = true
error: missing.toml
```

- **Function pointers, functors and lambdas.** In C, C++ and Pascal, a callback can be a function pointer. In C++, a functor, an object with `operator()`, can also be a callback. Many languages have lambda expressions, which often act as callbacks.[^2]
- **Observer and Command.** An observer registers a callback for an event. A command object is a callback with its arguments, in the form of an object.

## Known uses

- **C sorting and searching.** The C standard library functions `qsort()`, `lsearch()` and `bsearch()` take a comparator function as an argument.[^2]
- **Go `sort.Slice`.** The function `Slice(x any, less func(i, j int) bool)` sorts the slice `x` given the provided less function.[^5]
- **Node.js file system.** In the Node.js documentation, `fs.readFile` takes a callback `(err, data) =>` that gets the error first.[^4]
- **JavaScript arrays and timers.** `Array.prototype.map()` and `Array.prototype.forEach()` take synchronous callbacks, and `setTimeout()` takes an asynchronous callback.[^1]

## References
