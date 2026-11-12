---
tag: 68AC90
kind: pattern
slug: global-data
name: Global Data
aliases: [global variable, global state, global mutable state]
area: construction-practice/code-smells
level: core
summary: >-
  A code smell: data that any part of the program can read and change, so a reader cannot find which code changed it, and a change in one place breaks code in another.
family: code-smell
polarity: negative
intent: "Find the data that any code can change, and put it behind a small interface that controls and shows each change, or pass it to the code that needs it."
also_known_as: []
refactorings: [encapsulate-variable]
history:
  year: 2018
  people: [Kent Beck, Martin Fowler]
  note: Global Data is one of the four smells that the second edition of Refactoring (2018) adds to the list of the first edition.
see_also: [code-smell, side-effect, lexical-scope, singleton-pattern, dependency-injection, data-races]
sources:
  - url: https://en.wikipedia.org/w/index.php?title=Global_variable&oldid=1383619991
    title: Wikipedia, Global variable, revision 1383619991
    locator: the introduction and Use
  - book: isbn:9780134757599
    title: Martin Fowler, with Kent Beck, Refactoring, Improving the Design of Existing Code, second edition, Addison-Wesley, 2018
    locator: Chapter 3, Bad Smells in Code, Global Data
  - book: isbn:9780201485677
    title: Martin Fowler, Refactoring, Improving the Design of Existing Code, Addison-Wesley, 1999
    locator: Chapter 3, Bad Smells in Code, the list of 22 smells, which has no entry for global data
  - url: https://refactoring.com/catalog/
    title: Martin Fowler, Catalog of Refactorings
    locator: the card Encapsulate Variable, with the aliases Encapsulate Field and Self-Encapsulate Field
  - url: https://docs.python.org/3.14/faq/programming.html#what-are-the-rules-for-local-and-global-variables-in-python
    title: Python Programming FAQ, Python 3.14
    locator: What are the rules for local and global variables in Python?
  - url: https://docs.python.org/3.14/reference/simple_stmts.html#the-global-statement
    title: The Python Language Reference, Simple statements, Python 3.14
    locator: 7.12 The global statement
  - url: https://pylint.readthedocs.io/en/stable/user_guide/messages/warning/global-statement.html
    title: Pylint documentation, global-statement / W0603
    locator: Description
  - url: https://eslint.org/docs/latest/rules/no-implicit-globals
    title: ESLint documentation, Rules, no-implicit-globals
    locator: Rule Details
  - url: https://golangci-lint.run/docs/linters/
    title: golangci-lint documentation, Linters
    locator: gochecknoglobals
  - url: https://doc.rust-lang.org/reference/items/static-items.html#mutable-statics
    title: The Rust Reference, Static items
    locator: Mutable statics, items.static.mut.intro and items.static.mut.safety
---

## Intent

Find the data that any code in the program can change, and limit who can change it. The first step is to put the data behind functions, so that each change goes through one place that a reader can find. The next step is often to pass the data as a parameter to the code that needs it.

## Problem

A global variable is visible to the whole program. Wikipedia defines it as "a variable with global scope, meaning that it is visible (hence accessible) throughout the program, unless shadowed."[^1] Any function can read it, and in most languages any function can write it.

The problem is the write. When a global value is wrong, the cause can be in any function of the program, and nothing in the code shows which one. *Refactoring* gives this as the reason for the smell: the data can be changed from anywhere in the code base, and there is no mechanism to find the code that changed it.[^2]

The forces are these:

- Global data is convenient. A value that many functions need, such as a configuration setting, a logger or a cache, is available without a parameter in each signature.
- A function that reads a global has an input that is not in its signature. Two calls with the same arguments can give different results, and a test must set the global before it calls the function and reset it after.
- Tests that share a global can pass alone and fail together, because one test leaves the global in a state that the next one does not expect.
- Wikipedia notes that "Without proper locking (such as with a mutex), code using global variables will not be thread-safe except for read only values in protected memory."[^1] Two threads that write the same global without a lock make a [[data-races|data race]].

The smell includes more than variables at the top level of a file. *Refactoring* also names class variables and singletons as forms of global data.[^2] A static field that any class can set is global data with a class name in front of it.

The first edition of *Refactoring* has no smell for global data. The second edition adds it to the list.[^3][^2]

## Solution

*Refactoring* names one main refactoring: [[encapsulate-variable|Encapsulate Variable]].[^2] Put the data behind a function to read it and a function to write it, and make the variable itself private to a module or a class. In the online catalogue, this refactoring also has the names Encapsulate Field and Self-Encapsulate Field.[^4]

The wrapper does not make the data less global. It gives the data a door:

1. Each write goes through one function, so a search for that function finds all the code that changes the data.
2. The write function can check the value, and reject a value that is out of range.
3. The write function can be removed, or kept private, once it is clear that only the module itself needs to write.

Then reduce the scope. A function that reads the global can take the value as a parameter. Code that sets up the program can pass the values down, which is the idea of [[dependency-injection]]. A value that never changes after start-up can be made a constant, and constant global data is much less of a problem than mutable global data. *Refactoring* makes the same point about the dose: a small amount of global data can be managed, and a large amount is hard to work with.[^2]

## Example

The price function reads a global tax rate. The export function sets the rate to zero for a tax-free order, and does not put it back. After one export, every domestic price is wrong.

```javascript run id=encapsulate-global
import assert from "node:assert/strict";

// Before: any code can write the global.
let taxRate = 0.2;
function priceBefore(cents) {
  return Math.round(cents * (1 + taxRate));
}
function exportOrderBefore(cents) {
  taxRate = 0; // the export is tax free
  return priceBefore(cents); // and nothing puts the rate back
}

assert.equal(priceBefore(1000), 1200);
assert.equal(exportOrderBefore(1000), 1000);
assert.equal(priceBefore(1000), 1000); // the domestic price has changed
console.log("before: the export changed the domestic price");

// After: the rate is encapsulated, and the export passes its own rate.
const taxSettings = (() => {
  let rate = 0.2;
  return {
    rate: () => rate,
    setRate(r) {
      if (!(r >= 0 && r <= 1)) throw new RangeError(`tax rate ${r}`);
      rate = r;
    },
  };
})();

function price(cents, rate = taxSettings.rate()) {
  return Math.round(cents * (1 + rate));
}
const exportOrder = (cents) => price(cents, 0);

assert.equal(price(1000), 1200);
assert.equal(exportOrder(1000), 1000);
assert.equal(price(1000), 1200);
assert.throws(() => taxSettings.setRate(20), RangeError);
console.log("after: tests pass");
```

```output
before: the export changed the domestic price
after: tests pass
```

The "after" version has two changes. The rate lives in a closure, and the only way to change it is `setRate`, which also rejects a rate of 20 (a percentage passed where a fraction is expected). The export no longer changes shared data at all: it passes its rate as an argument. The global is now read in one place, the default value of the parameter.

| Language | What the example shows |
|---|---|
| JavaScript | a closure as the private home of the rate, and a parameter with a default |
| Python | the `global` statement in the "before" version, and a small class in the "after" version |
| Java | a public static field, then a private static field with a getter and a checked setter |
| Go | an exported package variable, then an unexported one with accessor functions |

## Consequences

Encapsulation makes each write visible, and makes it possible to check the values. It does not remove the hidden input: a function that calls `taxSettings.rate()` still depends on global state. Only passing the value as a parameter removes that.

Passing values has a cost. A setting that is needed deep in the call tree must be passed through each function above it, and the signatures grow. That is the trade-off with [[long-parameter-list|Long Parameter List]]. A parameter object or a context object that holds the settings keeps the lists short.

Languages and tools push in the same direction:

- Python requires the `global` statement to assign to a global from inside a function. The Python FAQ explains why: "requiring global for assigned variables provides a bar against unintended side-effects."[^5] The language reference states the rule: "It would be impossible to assign to a global variable without global".[^6] Pylint reports `global-statement` (W0603) "when you use the "global" statement to update a global variable".[^7]
- In JavaScript, ES modules give each file its own scope. The ESLint rule `no-implicit-globals` is for browser scripts, where top-level `var` and `function` declarations become global variables on the `window` object.[^8]
- golangci-lint has the linter `gochecknoglobals`, which checks "that no global variables exist".[^9]
- Rust requires an `unsafe` block to read or write a `static mut`. The Rust Reference calls mutable statics "a very large source of race conditions or other bugs".[^10]

## How to fix it

- **A global variable with many writers:** [[encapsulate-variable|Encapsulate Variable]]. Then search for the calls to the write function, and move each write to the module that owns the data.[^2]
- **A function that reads a global:** pass the value as a parameter, and let the caller decide where it comes from.
- **A setting that never changes after start-up:** make it a constant, or a read-only value that is set once.
- **A singleton that many classes reach for:** pass the instance to the classes that need it. See [[dependency-injection]].
- **Shared data between threads:** protect it with a lock, or replace it with a message between the threads. See [[data-races]].

## Variants and related patterns

**Mutable Data.** The general smell: data that changes, in any scope. Global data is the worst case, because its scope is the whole program. See [[mutable-data|Mutable Data]].

**Singleton.** A class with one instance and a global access point. The pattern gives a name and a type to global data, but the data is still reachable from anywhere. See [[singleton-pattern]].

**Side effects.** A function that writes a global has a [[side-effect]] that its signature does not show.

**Lexical scope.** The rule that a name is visible only in the block that declares it. Each step from global to module to function scope reduces the code that can reach a value. See [[lexical-scope]].

## References
