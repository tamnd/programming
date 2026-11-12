---
tag: A135SR
kind: pattern
slug: mutable-data
name: Mutable Data
aliases: [shared mutable state, aliasing bug]
area: construction-practice/code-smells
level: core
summary: >-
  A code smell: data is changed in place while other code still holds a reference to it, so a change in one part of the program shows up as a surprise in another.
family: code-smell
polarity: negative
intent: "Find the data that is changed in place without need, and limit the changes: copy before a change, compute values on demand, or make the data immutable."
also_known_as: []
refactorings: [encapsulate-variable, split-variable, slide-statements, extract-function, separate-query-from-modifier, remove-setting-method, replace-derived-variable-with-query, combine-functions-into-class, combine-functions-into-transform, change-reference-to-value]
history:
  year: 2018
  people: [Kent Beck, Martin Fowler]
  note: Mutable Data is one of the four smells that the second edition of Refactoring (2018) adds to the list of the first edition.
see_also: [code-smell, immutability, side-effect, global-data, data-races]
sources:
  - book: isbn:9780134757599
    title: Martin Fowler, with Kent Beck, Refactoring, Improving the Design of Existing Code, second edition, Addison-Wesley, 2018
    locator: Chapter 3, Bad Smells in Code, Mutable Data
  - url: https://martinfowler.com/bliki/AliasingBug.html
    title: Martin Fowler, Aliasing Bug, bliki
    locator: the first paragraph, the Date example and the last paragraph
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Array/sort
    title: MDN Web Docs, Array.prototype.sort()
    locator: the introduction and Return value
  - url: https://docs.python.org/3.14/howto/sorting.html
    title: Python documentation, Sorting Techniques, Python 3.14
    locator: Sorting Basics
  - url: https://pkg.go.dev/slices
    title: Go standard library, package slices
    locator: func Clone and func Sort
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Statements/const
    title: MDN Web Docs, const
    locator: Description
  - url: https://refactoring.com/catalog/
    title: Martin Fowler, Catalog of Refactorings
    locator: the cards Split Variable, Separate Query from Modifier, Remove Setting Method, Replace Derived Variable with Query and Change Reference to Value
  - url: https://martinfowler.com/bliki/CommandQuerySeparation.html
    title: Martin Fowler, Command Query Separation, bliki
    locator: the definitions of queries and commands
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Array/toSorted
    title: MDN Web Docs, Array.prototype.toSorted()
    locator: the introduction
  - url: https://doc.rust-lang.org/book/ch03-01-variables-and-mutability.html
    title: The Rust Programming Language, Variables and Mutability
    locator: the introduction
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Object/freeze
    title: MDN Web Docs, Object.freeze()
    locator: the introduction and Description
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/List.html#unmodifiable
    title: Java SE 25 API, java.util.List
    locator: Unmodifiable Lists
  - url: https://docs.python.org/3.14/faq/programming.html#why-are-default-values-shared-between-objects
    title: Python Programming FAQ, Python 3.14
    locator: Why are default values shared between objects?
---

## Intent

Find the data that the program changes in place when it does not need to, and limit the changes. Make a copy before a change, compute a derived value when it is asked for, or make the data immutable, so that a change in one place cannot surprise the code in another place.

## Problem

A program that changes data in place is normal: a counter goes up, a buffer fills, a record gets a new address. The smell is a change that other code does not expect. *Refactoring* gives the reason: a change to data in one place can have consequences in another place that the programmer did not foresee, and the resulting bugs are hard to find.[^1]

The most common form is aliasing. Fowler defines it: "Aliasing occurs when the same memory location is accessed through more than one reference."[^2] His example is in Java. A variable `partyDate` is set to `retirementDate`, so both names refer to one `Date` object. A call to `partyDate.setDate(5)` moves the party to Saturday, and it also moves the retirement.[^2]

The forces are these:

- In place changes are often faster and use less memory than copies.
- Many library functions change their argument. In JavaScript, `sort()` "sorts the elements of an array in place and returns the reference to the same array".[^3] In Python, `list.sort()` "modifies the list in-place".[^4] In Go, `slices.Sort` sorts the slice that it is given.[^5]
- A `const` or `final` declaration protects the name, not the value. MDN states it for JavaScript: `const` "creates an immutable reference to a value. It does not mean the value it holds is immutable".[^6]
- The wider the scope of the data, the worse the smell. A local variable that one function changes is easy to follow. A field that many functions change, or a [[global-data|global variable]], is not.

## Solution

*Refactoring* names a group of refactorings, each for one form of the smell.[^1] The catalogue at refactoring.com has a card for each.[^7]

1. **Data that many functions change:** [[encapsulate-variable|Encapsulate Variable]], so that each change goes through one function that can be found and watched.
2. **A variable that holds different things at different times:** [[split-variable|Split Variable]], so that each variable is set once.
3. **Code that changes data mixed with code that only reads it:** [[slide-statements|Slide Statements]] and [[extract-function|Extract Function]] to move the changes away from the reads, and [[separate-query-from-modifier|Separate Query from Modifier]] so that a function either returns a value or changes state, not both. Fowler's article on command query separation states the idea: queries "Return a result and do not change the observable state of the system", and commands "Change the state of a system but do not return a value".[^8]
4. **A setter that nothing should call after construction:** [[remove-setting-method|Remove Setting Method]].
5. **A stored value that can be computed from other data:** [[replace-derived-variable-with-query|Replace Derived Variable with Query]]. A value that is computed when asked for cannot get out of date.
6. **Functions that update a shared structure step by step:** [[combine-functions-into-class|Combine Functions into Class]] or [[combine-functions-into-transform|Combine Functions into Transform]], so that the changes happen in one place.
7. **A small object that is shared and changed:** [[change-reference-to-value|Change Reference to Value]]. Make it immutable, and replace it as a whole in place of changing its fields.

Fowler sums up the last point: "A good way to avoid shared updates of value objects is to make value objects immutable."[^2]

## Example

The function `median` sorts its argument to find the middle value. The caller passes the list of readings in the order they were taken, and after the call the list is in a different order. The function returns the right median, so a test of the result alone passes.

```javascript run id=sort-a-copy
import assert from "node:assert/strict";

// Before: sort() changes the caller's array.
function medianBefore(values) {
  values.sort((a, b) => a - b);
  return values[Math.floor(values.length / 2)];
}

// After: toSorted() returns a sorted copy.
function median(values) {
  const sorted = values.toSorted((a, b) => a - b);
  return sorted[Math.floor(sorted.length / 2)];
}

let readings = [30, 10, 20];
assert.equal(medianBefore(readings), 20);
assert.deepEqual(readings, [10, 20, 30]); // the first reading is lost
console.log("medianBefore: the caller's array was changed");

readings = [30, 10, 20];
assert.equal(median(readings), 20);
assert.deepEqual(readings, [30, 10, 20]);
console.log("median: tests pass");
```

```output
medianBefore: the caller's array was changed
median: tests pass
```

The fix is one line, but it changes the contract of the function. The first version has a [[side-effect]] that its name does not show. The second version is a query: it returns a value and leaves its input as it was. MDN describes `toSorted()` as "the copying version of the sort() method".[^9]

| Language | What the example shows |
|---|---|
| JavaScript | `sort()` against `toSorted()`, as above |
| Python | `list.sort()` against `sorted()`, and the mutable default argument |
| Java | `Collections.sort` on the caller's list, against a sorted stream; `List.of` rejects the change |
| Go | `slices.Sort` on the caller's slice, against `slices.Clone` first |

## Consequences

Data that does not change can be shared without fear. It can be passed to any function, kept in a cache and read from many threads, and no copy is needed to protect it. Fowler notes that immutability "makes it harder to create several kinds of bugs".[^2]

The cost is copies and more allocation. Sorting a copy uses memory for a second array. For a large data set in a tight loop, an in place change can be the right choice, and then the function name and its documentation must say that it changes its argument. Fowler also warns that immutability is "by no means a free breakfast": when things do need to change, it can add complexity.[^2]

Some changes are meant to be shared. When a person's phone number changes, every reference to that person must see the new number. Fowler calls these reference objects, and contrasts them with value objects, which no code should change in place.[^2] The smell is a change to data that the other holders expect to stay the same.

Languages give different defaults:

- Rust variables are immutable unless declared with `mut`. The Rust book says that `mut` "conveys intent to future readers of the code by indicating that other parts of the code will be changing this variable’s value."[^10]
- In JavaScript, `Object.freeze()` makes an object immutable, but MDN notes that "freeze is shallow": an object inside a frozen object can still change.[^11]
- In Java, `List.of` and `List.copyOf` return unmodifiable lists, and "Calling any mutator method on the List will always cause UnsupportedOperationException to be thrown."[^12]
- In Python, a mutable default argument is shared by all calls. The Python FAQ says that this "type of bug commonly bites neophyte programmers."[^13]

## How to fix it

- **A function that changes its argument without need:** copy first, or use the copying form of the operation (`toSorted`, `sorted`, `slices.Clone`).[^9][^4][^5]
- **A field that many functions set:** [[encapsulate-variable|Encapsulate Variable]].[^1]
- **A variable that is set many times for different purposes:** [[split-variable|Split Variable]].[^1]
- **A function that returns a value and also changes state:** [[separate-query-from-modifier|Separate Query from Modifier]].[^1]
- **A field that can be computed:** [[replace-derived-variable-with-query|Replace Derived Variable with Query]].[^1]
- **A setter that is only used to build the object:** [[remove-setting-method|Remove Setting Method]], and set the field in the constructor.[^1]
- **A small object that is shared and changed:** [[change-reference-to-value|Change Reference to Value]].[^1]

## Variants and related patterns

**Global Data.** Mutable data with the largest scope. See [[global-data|Global Data]].

**Immutability.** The language feature that removes the smell at its root. See [[immutability]].

**Data races.** Mutable data that two threads change at the same time without a lock. See [[data-races]].

**Value objects.** Small objects, such as a date or an amount of money, that are compared by value and are not changed in place. See [[value-object|Value object]].

**Temporary Field.** A field that is set only in some cases, which is a form of mutable state in an object. See [[temporary-field|Temporary Field]].

## References
