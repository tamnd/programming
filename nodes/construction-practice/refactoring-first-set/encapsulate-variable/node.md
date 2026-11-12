---
tag: D2ZYSQ
kind: technique
slug: encapsulate-variable
name: Encapsulate variable
aliases: [Encapsulate Field, Self-Encapsulate Field]
area: construction-practice/refactoring-first-set
level: core
summary: >-
  Put a widely used variable behind a function that reads it and a function that updates it, so that every access goes through one place that you can change.
family: refactoring
polarity: positive
intent: "Turn the access to data with a wide scope into calls of functions, so that the data can be watched, checked and changed in one place."
also_known_as: [Encapsulate Field, Self-Encapsulate Field]
history:
  year: 1999
  people: [Martin Fowler]
  note: The first edition of Refactoring (1999) had Self Encapsulate Field and Encapsulate Field. The second edition (2018) names the refactoring Encapsulate Variable.
see_also: [encapsulation-and-visibility, global-data, mutable-data, change-function-declaration, code-smell]
sources:
  - book: isbn:9780134757599
    title: Martin Fowler, Refactoring, Improving the Design of Existing Code, second edition, Addison-Wesley, 2018
    locator: Chapter 6, A First Set of Refactorings, Encapsulate Variable, motivation and mechanics; Chapter 3, Bad Smells in Code, Global Data and Mutable Data
  - url: https://refactoring.com/catalog/encapsulateVariable.html
    title: Martin Fowler, Catalog of Refactorings, Encapsulate Variable
    locator: the entry; aliases Self-Encapsulate Field and Encapsulate Field
  - book: isbn:9780201485677
    title: Martin Fowler, Refactoring, Improving the Design of Existing Code, Addison-Wesley, 1999
    locator: Chapter 8, Organizing Data, Self Encapsulate Field and Encapsulate Field
  - url: https://www.jetbrains.com/help/idea/encapsulate-fields.html
    title: IntelliJ IDEA documentation, Encapsulate Fields
    locator: the introduction
  - url: https://code.visualstudio.com/docs/typescript/typescript-refactoring
    title: Visual Studio Code documentation, Refactoring TypeScript and JavaScript
    locator: Generate get and set accessors
  - url: https://rust-analyzer.github.io/book/assists.html
    title: rust-analyzer manual, Assists
    locator: generate_getter, generate_setter
---

## Purpose

A function can move or change its name in stages: keep the old one as a wrapper and the callers keep working. Data does not work like that. Every reference to a variable must change at the same moment as the variable, and for data with a wide scope, such as a module variable or a public field, the references are everywhere.[^1]

Encapsulate variable puts the data behind two functions, one that reads it and one that updates it, and changes each reference to call them. After that, the reorganization of the data is a reorganization of functions, which the other refactorings handle.[^1] The functions also give one place to check a value that is set, to log who changes it, or to return a copy so that a caller cannot change the contents.

The catalogue card shows a module variable `defaultOwner` that becomes the private `defaultOwnerData` with the functions `defaultOwner()` and `setDefaultOwner(arg)`.[^2] The first edition had Self Encapsulate Field, which routes the access inside a class through accessors, and Encapsulate Field, which makes a public field private.[^3]

## Steps

1. Run the tests and see them pass.
2. Write a function that returns the value and a function that sets it.
3. Run the static checks that you have.
4. Replace each reference to the variable with a call of the right function, and test after each.
5. Restrict the visibility of the variable, so that only the functions can reach it. If a reference remains, the compiler or the tests find it.
6. Test.
7. If the value is a record, decide whether the reader may change its contents. If not, return a copy or a read-only view, or go on to Encapsulate Record.[^1]

## Example

A tax rate is a module value that any code can read and change. The encapsulated version checks each update and returns a copy of the record.

```javascript run id=one-way-in
import assert from "node:assert/strict";

// Before: shared data that anyone can change.
let taxSettings = { rate: 0.2, region: "EU" };
function priceWithTaxBefore(net) {
  return net * (1 + taxSettings.rate);
}

// After: the data is private to the module; two functions guard it.
function makeTaxModule() {
  let settings = { rate: 0.2, region: "EU" };
  return {
    taxSettings() {
      return { ...settings }; // a copy, so a reader cannot change ours
    },
    setTaxSettings(next) {
      if (!(next.rate >= 0 && next.rate < 1)) {
        throw new RangeError(`bad tax rate ${next.rate}`);
      }
      settings = { ...next };
    },
  };
}

const tax = makeTaxModule();
const priceWithTax = (net) => net * (1 + tax.taxSettings().rate);

assert.equal(priceWithTax(100), priceWithTaxBefore(100)); // same behaviour

// A reader that changes its copy no longer changes the module.
const copy = tax.taxSettings();
copy.rate = 0.9;
assert.equal(tax.taxSettings().rate, 0.2);

// Every update goes through one check.
assert.throws(() => tax.setTaxSettings({ rate: 1.5, region: "EU" }), RangeError);
tax.setTaxSettings({ rate: 0.1, region: "EU" });
assert.equal(Math.round(priceWithTax(100)), 110);
console.log("encapsulate variable: tests pass");
```

```output
encapsulate variable: tests pass
```

The callers now name the operation, `taxSettings()` or `setTaxSettings(...)`, not the storage. The module can store the settings in another form, read them from a file, or count the updates, and no caller changes.

| Language | What the example shows |
|---|---|
| JavaScript | a module value behind two functions, with a copy and a check, as above |
| Python | a module value behind two functions, and a class attribute turned into a `property` |
| Go | an unexported package variable with exported functions, and a mutex for concurrent use |

## When to use

- **The data has a wide scope.** A global variable, a module variable or a public field that many parts of the program touch. The book names the refactoring as a fix for [[global-data|Global Data]] and for [[mutable-data|Mutable Data]].[^1]
- **Before you change the data.** To rename it, change its type or move it, first put it behind functions, then change it behind them.
- **To find who changes it.** One setter is one place for a breakpoint or a log line.

A local variable in a short function does not need this. Its scope is already small. Data that is immutable needs less of it too, because no caller can change it.[^1]

## Pitfalls

**The getter returns a reference.** If the value is an object or a list and the getter returns it as it is, a caller can still change the contents, and the encapsulation is only of the variable, not of the data. Return a copy or a read-only view when that matters.

**A copy that costs too much.** A copy on each read of a large record can be slow. Measure before you choose, or keep the reference and document that readers must not change it.

**Accessors with no reason.** Wrapping every field of every class in a getter and a setter adds code and hides nothing, because the setter still lets any caller set any value. The refactoring pays when the data is widely shared or when the functions add a check, a copy or a change of representation.

**Concurrency.** Two functions are also the place to add a lock. Without one, a reader and a writer on different threads can still race; the encapsulation does not make the access safe by itself.

## Tools that do it

| Tool | Languages | What it automates |
|---|---|---|
| IntelliJ IDEA | Java, Kotlin | Encapsulate Fields, which "lets you hide your data and create the necessary accessors"[^4] |
| VS Code | TypeScript, JavaScript | "Generate get and set accessors", which encapsulates "a selected class property by generating a getter and setter for it"[^5] |
| rust-analyzer | Rust | `generate_getter` and `generate_setter`, which generate the accessor methods for a field[^6] |

The tools write the accessors and, in IntelliJ IDEA, change the uses. The decision of what the getter returns, a reference or a copy, is the programmer's.

## References
