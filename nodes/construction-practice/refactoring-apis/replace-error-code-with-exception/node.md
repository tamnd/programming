---
tag: N3VZ77
kind: technique
slug: replace-error-code-with-exception
name: Replace error code with exception
area: construction-practice/refactoring-apis
level: core
summary: >-
  Replace a special return value that signals an error with a thrown exception, in languages where exceptions are the normal way to report errors.
family: refactoring
polarity: positive
intent: "Keep the error out of the return value, and stop a caller from ignoring it."
history:
  year: 1999
  people: [Martin Fowler]
  note: The refactoring is in the first edition of Refactoring (1999), the Ruby edition and the second edition (2018), where it is in the chapter on refactoring APIs.
see_also: [exception-handling, errors-as-values, panics, replace-exception-with-precheck, introduce-assertion]
sources:
  - url: https://refactoring.com/catalog/replaceErrorCodeWithException.html
    title: Martin Fowler, Catalog of Refactorings, Replace Error Code with Exception
    locator: the entry, the ShippingRules sketch
  - url: https://refactoring.com/catalog/
    title: Martin Fowler, Catalog of Refactorings, index
    locator: the entry replaceErrorCodeWithException, its books and tags
  - book: isbn:9780134757599
    title: Martin Fowler, Refactoring, Improving the Design of Existing Code, second edition, Addison-Wesley, 2018
    locator: Chapter 11, Refactoring APIs, Replace Error Code with Exception
  - url: https://go.dev/doc/faq
    title: The Go Programming Language, Frequently Asked Questions
    locator: Why does Go not have exceptions?
  - url: https://go.dev/blog/error-handling-and-go
    title: Andrew Gerrand, Error handling and Go, The Go Blog, 12 July 2011
    locator: the first section
  - url: https://doc.rust-lang.org/book/ch09-00-error-handling.html
    title: The Rust Programming Language, Chapter 9, Error Handling
    locator: the first paragraphs
  - url: https://doc.rust-lang.org/book/ch09-03-to-panic-or-not-to-panic.html
    title: The Rust Programming Language, Chapter 9.3, To panic! or Not to panic!
    locator: the first paragraphs and Guidelines for Error Handling
---

## Purpose

An error code is a special value, such as `-1` or `-23`, that a function returns in place of its normal result. Error codes have three problems:

- The caller can ignore the code, and the program goes on with a wrong value.
- The code shares the return value with real results, so a valid result cannot use that value.
- Each caller up the stack must check the code and pass it on, even when only one function far away can handle it.

In a language with exceptions, Replace error code with exception throws an exception in place of the code. The catalogue card has `else return -23;`. After the refactoring, it is `else throw new OrderProcessingError(-23);`.[^1] The exception goes up the stack to the nearest handler. A caller that does not handle it does not need any code for it. See [[exception-handling|Exception handling]].

### The language decides

This refactoring is right for JavaScript, Python, Java, C# and other languages where the standard library throws exceptions. It is wrong for Go and Rust. Do not apply it there.

- **Go.** The Go FAQ says: "We believe that coupling exceptions to a control structure, as in the try-catch-finally idiom, results in convoluted code. It also tends to encourage programmers to label too many ordinary errors, such as failing to open a file, as exceptional."[^4] The Go Blog says: "Go code uses error values to indicate an abnormal state."[^5] The Go form of this refactoring is to replace the special value with a second return value of type `error`.
- **Rust.** The Rust book says: "Rust doesn’t have exceptions. Instead, it has the type Result<T, E> for recoverable errors and the panic! macro that stops execution when the program encounters an unrecoverable error."[^6] It adds that "when failure is expected, it’s more appropriate to return a Result than to make a panic! call."[^7] The Rust form is to return `Result<T, E>`.

In both languages, the change removes the special value from the normal result, as the exception does, and the type of the result shows that the call can fail. See [[errors-as-values|Errors as values]] and [[panics|Panics]].

## Steps

1. Make an exception class for the error, or choose one that exists. Do not use the most general class, so that a handler can catch only this error.
2. Find each caller that checks the code. Change each one: put the call in a `try` block, and move the code that handles the error into the `catch` block. Callers that only passed the code on need no handler.
3. Change the function to throw the exception in place of the code.
4. Remove the checks of the code that are now dead. Test.

## Example

`localShippingRules` returns a code when the country has no shipping data. Each caller must check for the code. After the refactoring, the function throws an `OrderProcessingError`, and only the top level catches it.

```javascript run id=code-to-exception
import assert from "node:assert/strict";

const countryData = { shippingRules: { PT: { flat: 5 } } };

function localShippingRulesBefore(country) {
  const data = countryData.shippingRules[country];
  if (data) return data;
  else return -23;
}

class OrderProcessingError extends Error {
  constructor(errorCode) {
    super(`order processing error ${errorCode}`);
    this.code = errorCode;
  }
  get name() { return "OrderProcessingError"; }
}

function localShippingRules(country) {
  const data = countryData.shippingRules[country];
  if (data) return data;
  else throw new OrderProcessingError(-23);
}

// Before: each caller must check the code.
assert.equal(localShippingRulesBefore("XX"), -23);

// After: the code that does not handle the error has no checks.
function shippingCost(country) {
  return localShippingRules(country).flat;
}

const status = [];
for (const country of ["PT", "XX"]) {
  try {
    status.push(shippingCost(country));
  } catch (e) {
    if (e instanceof OrderProcessingError) status.push(e.code);
    else throw e;
  }
}
assert.deepEqual(status, [5, -23]);
console.log(status);
```

```output
[ 5, -23 ]
```

The handler catches only `OrderProcessingError`, and it throws any other error again.

| Language | What the example shows |
|---|---|
| JavaScript | the code `-23` replaced with `throw new OrderProcessingError(-23)`, as above |
| Python | the same change; the function raises a subclass of `Exception`, and the handler catches only that subclass |
| Go | not an exception: the code is replaced with a second return value of type `error`, and the caller checks it with `errors.As` |

## When to use

- **A language with exceptions**, where callers expect them.
- **An error that most callers cannot handle.** The exception passes through them with no code.
- **A special value that a valid result could also have.** For example, `-1` from a function that can also return negative numbers.

## Pitfalls

**Expected results are not errors.** If a caller must check a case at each call, such as a key that is not in a map, a special result or an optional value can be clearer than an exception. For a case that the caller can check before the call, see [[replace-exception-with-precheck|Replace Exception with Precheck]].

**Catching too much.** A handler that catches every exception also hides bugs, such as a `TypeError` from a wrong name. Catch the class of the error, as the example does.

**Exceptions are invisible in the signature.** In JavaScript, Python and C#, the signature does not list the exceptions that a function can throw. Document them, or the callers do not know what to catch.

**Go and Rust.** See the section above. An exception-style `panic` in Go or Rust for an ordinary error goes against the advice of both languages.[^4][^7]

## Tools that do it

No IDE has one action for this refactoring. The steps change each caller, so they are done by hand.

The catalogue lists the refactoring in the first edition, the second edition and the Ruby edition, with the tags refactoring-apis and errors.[^2] In the second edition it is in Chapter 11, Refactoring APIs.[^3]

## References
