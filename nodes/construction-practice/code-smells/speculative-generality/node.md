---
tag: 5C308A
kind: pattern
slug: speculative-generality
name: Speculative Generality
aliases: [presumptive feature]
area: construction-practice/code-smells
level: core
summary: >-
  A code smell: code has hooks, parameters, abstract classes and special cases for needs that the program does not have, and they make the code that it does need harder to read and to change.
family: code-smell
polarity: negative
intent: "Find the generality that no current requirement uses, and remove it, so that the code says only what the program does."
also_known_as: []
refactorings: [collapse-hierarchy, inline-function, inline-class, change-function-declaration, remove-dead-code]
history:
  year: 1999
  people: [Kent Beck, Martin Fowler]
  note: Speculative Generality is in the first edition of Refactoring (1999), and keeps its name in the second edition (2018).
see_also: [code-smell, lazy-element, middle-man, unit-testing]
sources:
  - book: isbn:9780134757599
    title: Martin Fowler, with Kent Beck, Refactoring, Improving the Design of Existing Code, second edition, Addison-Wesley, 2018
    locator: Chapter 3, Bad Smells in Code, Speculative Generality
  - url: https://en.wikipedia.org/w/index.php?title=You_aren%27t_gonna_need_it&oldid=1374076145
    title: Wikipedia, You aren't gonna need it, revision 1374076145
    locator: the introduction
  - url: https://martinfowler.com/bliki/Yagni.html
    title: Martin Fowler, Yagni, bliki
    locator: the introduction, the costs of a presumptive feature, and the section on abstractions
  - url: https://refactoring.com/catalog/
    title: Martin Fowler, Catalog of Refactorings
    locator: the cards Collapse Hierarchy, Inline Function, Inline Class, Change Function Declaration (alias Remove Parameter) and Remove Dead Code
  - book: isbn:9780201485677
    title: Martin Fowler, Refactoring, Improving the Design of Existing Code, Addison-Wesley, 1999
    locator: Chapter 3, Bad Smells in Code, Speculative Generality
---

## Intent

Find the generality that the program does not use: a parameter that every caller passes the same value for, a plugin point with one plugin, an abstract class with one subclass, a case for a format that nobody asks for. Remove it, so that the code says only what the program does, and add generality back when a real need comes.

## Problem

Programmers try to see ahead. A function that exports CSV now will surely need JSON one day, so it takes a format parameter. A class that talks to one database might talk to another, so it gets an interface and a factory. *Refactoring* describes the smell by the sentence that justifies it: someone says that the code will need the ability one day, and adds hooks and special cases for things that are not required.[^1]

The principle that the smell breaks has a name. Wikipedia describes YAGNI ("You aren't gonna need it") as "a principle which arose from extreme programming (XP) that states a programmer should not add functionality until deemed necessary."[^2] Fowler's article on it uses the term "presumptive feature" for "any code that supports a feature that isn't yet being made available for use."[^3]

The forces are these:

- The future need may never come. Fowler cites a study at Microsoft which found that only a third of carefully planned features improved the metrics they were meant to improve.[^3]
- Even when it comes, the guess about its shape is often wrong. Fowler writes that "your current ideas of what abstractions you'll need will not match what you learn when you do actually need them."[^3]
- The generality has a cost while it waits. Fowler calls it the cost of carry: the code for the presumptive feature "adds some complexity to the software, this complexity makes it harder to modify and debug that software".[^3]
- *Refactoring* gives a sign of the smell that is quick to check: the only users of a function or class are its tests.[^1]

## Solution

*Refactoring* names a refactoring for each form of the smell.[^1] The catalogue at refactoring.com has a card for each, and gives Remove Parameter as one of the names of Change Function Declaration.[^4]

1. **An abstract class that does little:** [[collapse-hierarchy|Collapse Hierarchy]].
2. **A layer of delegation that is not needed:** [[inline-function|Inline Function]] and [[inline-class|Inline Class]].
3. **A parameter that no caller uses:** [[change-function-declaration|Change Function Declaration]], to remove it.
4. **A function or class that only tests use:** delete the tests, then [[remove-dead-code|Remove Dead Code]].

The first edition has the same smell, and names Remove Parameter for the parameters that no caller uses.[^5]

Fowler gives a test for what to keep. Ask what the refactoring would cost later, when the need is real; "Often that thought experiment is enough to convince them that it won't be significantly more expensive to add it later."[^3] He also states the rule for abstractions: "any abstraction that makes it harder to understand the code for current requirements is presumed guilty."[^3]

## Example

The export function takes a format and an options object, and looks up a writer in a registry. The program only ever exports CSV with a comma. Each caller must pass the format, and a reader must follow the registry to find the code that runs.

```javascript run id=remove-the-hooks
import assert from "node:assert/strict";

// Before: a registry, a format parameter and options that no caller uses.
const writers = new Map();
function registerWriter(format, writer) {
  writers.set(format, writer);
}
registerWriter("csv", (rows, options) =>
  rows.map((r) => r.join(options.delimiter ?? ",")).join(options.lineEnd ?? "\n"),
);
function exportRowsBefore(rows, format = "csv", options = {}) {
  const writer = writers.get(format);
  if (!writer) throw new Error(`no writer for ${format}`);
  return writer(rows, options);
}

// After: the one format that the program uses.
const toCsv = (rows) => rows.map((r) => r.join(",")).join("\n");

const rows = [["id", "name"], [1, "Ada"], [2, "Grace"]];
assert.equal(exportRowsBefore(rows), "id,name\n1,Ada\n2,Grace");
assert.equal(toCsv(rows), exportRowsBefore(rows, "csv", {}));
console.log("toCsv: tests pass");
```

```output
toCsv: tests pass
```

The second version is one line, and its name says what it produces. If a JSON export is needed later, the programmer adds `toJson` next to it, and if many formats appear, a registry can come back with the real formats as a guide to its shape.

| Language | What the example shows |
|---|---|
| JavaScript | a writer registry and unused options, removed, as above |
| Python | the same registry and options, removed |
| Java | an interface and an abstract base class with one subclass, collapsed |
| Go | an options struct whose fields no caller sets, removed |

## Consequences

The code is shorter and says what the program does. A reader does not need to learn hooks that nothing uses, and a change to the real behaviour does not need to keep the unused paths working.

Fowler names the limits of the rule. YAGNI is about features, not about the health of the code: "expending effort on refactoring isn't a violation of yagni because refactoring makes the code more malleable."[^3] He also says that a choice for the future that "doesn't actually increase the complexity of the software" needs no YAGNI argument.[^3] His example is a lookup table for error messages, which costs little now and makes translation easier later.[^3]

Some generality is required by the context, not guessed. A library with outside users must keep a stable interface, and a parameter that its own code does not use may be used by its clients. A public API is not the place to remove a parameter without a deprecation period.

YAGNI works together with other practices. Wikipedia notes that it "is meant to be used in combination with several other practices, such as continuous refactoring, continuous automated unit testing, and continuous integration."[^2] Without tests and refactoring, the cost of adding the generality later is real, and the argument for removing it is weaker.

## How to fix it

- **An abstract class with one subclass:** [[collapse-hierarchy|Collapse Hierarchy]].[^1]
- **A layer that only forwards:** [[inline-function|Inline Function]] or [[inline-class|Inline Class]].[^1]
- **A parameter that every caller passes the same way, or not at all:** [[change-function-declaration|Change Function Declaration]] to remove it.[^1]
- **Code whose only callers are tests:** delete the tests and [[remove-dead-code|Remove Dead Code]].[^1]

## Variants and related patterns

**Lazy Element.** Generality that was never used often leaves behind elements that do nothing. See [[lazy-element|Lazy Element]].

**Dead code.** Code that the program never runs. Speculative code that was never called is one kind of it. See [[dead-code-smell|Dead code]].

**Middle Man.** A layer of delegation added for a future that did not come. See [[middle-man|Middle Man]].

## References
