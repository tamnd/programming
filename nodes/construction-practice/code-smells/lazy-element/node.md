---
tag: DYFV9F
kind: pattern
slug: lazy-element
name: Lazy Element
aliases: [lazy class]
area: construction-practice/code-smells
level: core
summary: >-
  A code smell: a class, a function or another program element does so little that it costs more to read and to keep than it gives, and the code is clearer without it.
family: code-smell
polarity: negative
intent: "Find the classes and functions that add a name and a layer but no meaning, and fold them into the code that uses them."
also_known_as: [Lazy Class]
refactorings: [inline-function, inline-class, collapse-hierarchy]
history:
  year: 1999
  people: [Kent Beck, Martin Fowler]
  note: The first edition of Refactoring (1999) calls the smell Lazy Class. The second edition (2018) renames it Lazy Element, so that it covers functions as well as classes.
see_also: [code-smell, extract-function, class-inheritance, speculative-generality]
sources:
  - book: isbn:9780134757599
    title: Martin Fowler, with Kent Beck, Refactoring, Improving the Design of Existing Code, second edition, Addison-Wesley, 2018
    locator: Chapter 3, Bad Smells in Code, Lazy Element
  - book: isbn:9780201485677
    title: Martin Fowler, Refactoring, Improving the Design of Existing Code, Addison-Wesley, 1999
    locator: Chapter 3, Bad Smells in Code, Lazy Class
  - url: https://refactoring.com/catalog/
    title: Martin Fowler, Catalog of Refactorings
    locator: the cards Inline Function (alias Inline Method), Inline Class and Collapse Hierarchy
---

## Intent

Find the program elements that do too little to earn their place: a class with one trivial method, a function whose body says the same thing as its name, a subclass that adds nothing to its parent. Fold each one into the code that uses it, so that a reader has one less name to learn and one less layer to step through.

## Problem

Structure has a cost. Each class and each function is a name to learn, a file or a block to open, and a step in a call chain that a reader must follow. That cost is worth paying when the element gives something back: a name that explains, a place that hides a decision, or a piece that is used in many places.

The second edition of *Refactoring* describes the smell as an element that is not needed: a function whose name reads the same as its body, or a class that is in effect one function.[^1] The first edition gave the smell the name Lazy Class, and spoke only of classes.[^2]

The forces are these:

- An element can start lazy. It was made for growth that never came, which is close to [[speculative-generality|Speculative Generality]].
- An element can become lazy. Refactoring moves behaviour out of a class, and what is left is a shell. *Refactoring* gives both causes.[^1]
- Small elements are not lazy in themselves. A one-line function with a good name, such as `isOverdue(invoice)`, says what a comparison of dates means. The smell is an element whose name adds nothing to its body.
- A layer that only passes a call through makes the reader step through it to learn that it does nothing.

## Solution

*Refactoring* names the refactorings by the kind of element.[^1] The catalogue at refactoring.com has a card for each, and gives Inline Method as another name for Inline Function.[^3]

1. **A function:** [[inline-function|Inline Function]]. Replace each call with the body, then delete the function.
2. **A class:** [[inline-class|Inline Class]]. Move its fields and methods into the class that uses it, then delete it.
3. **A subclass or a superclass that adds nothing:** [[collapse-hierarchy|Collapse Hierarchy]]. Merge it with its parent or its child.

Inline Function is the inverse of [[extract-function|Extract Function]]. The test for which way to go is the name: if the name tells the reader more than the body, keep the function; if the body is as clear as the name, inline it.

## Example

The code to show a person's name goes through a class with one method and a function that only calls that method. The two layers add no rule and no name that the body does not already say.

```javascript run id=inline-the-layers
import assert from "node:assert/strict";

// Before: a class with one method, and a function that only forwards to it.
class NameFormatter {
  format(person) {
    return `${person.first} ${person.last}`;
  }
}
const formatter = new NameFormatter();
function fullNameBefore(person) {
  return formatter.format(person);
}
function greetingBefore(person) {
  return `Hello, ${fullNameBefore(person)}`;
}

// After: one function, with the body inline.
function fullName(person) {
  return `${person.first} ${person.last}`;
}
const greeting = (person) => `Hello, ${fullName(person)}`;

const ada = { first: "Ada", last: "Lovelace" };
assert.equal(greetingBefore(ada), "Hello, Ada Lovelace");
assert.equal(greeting(ada), greetingBefore(ada));
assert.equal(fullName(ada), fullNameBefore(ada));
console.log("greeting: tests pass");
```

```output
greeting: tests pass
```

The second version keeps `fullName`, because the name says what the template means and three callers can share it. It removes the class, because a class with one stateless method and one instance is a function with extra steps. A reader who follows `greeting` now reaches the template in one step, not three.

| Language | What the example shows |
|---|---|
| JavaScript | a one-method class and a forwarding function, inlined, as above |
| Python | the same layers, inlined to one function |
| Java | a subclass that adds nothing, collapsed into its parent, and a forwarding method inlined |
| Go | an interface with one implementation and one method, replaced by a function |

## Consequences

The code has fewer names and fewer layers. A reader reaches the logic in fewer steps, and a search for a rule finds it in one place.

Inlining removes a seam. A class or an interface with one implementation can be the place where a test puts a fake, or where a second implementation is planned for the next release. If the second implementation is real and close, keep the seam. If it is only possible, the element is lazy now, and it can be extracted again when the need is real.

The smell is relative. A function that is lazy in a small program can be a useful name in a large one, where the same template is used in many places. The question is what the element gives to its readers now, not what it might give later.

## How to fix it

- **A function whose body is as clear as its name:** [[inline-function|Inline Function]].[^1]
- **A class that does almost nothing:** [[inline-class|Inline Class]].[^1]
- **A subclass or superclass that adds nothing:** [[collapse-hierarchy|Collapse Hierarchy]].[^1]
- **A layer that only forwards calls:** see [[middle-man|Middle Man]] and Remove Middle Man.

## Variants and related patterns

**Speculative Generality.** An element made for a need that never came is often lazy as well. See [[speculative-generality|Speculative Generality]].

**Middle Man.** A class whose methods mostly forward to another class is a lazy element of a particular shape. See [[middle-man|Middle Man]].

**Long Function.** The opposite problem: too much in one element. Extract Function fixes it, and Inline Function undoes an extraction that went too far. See [[long-function|Long Function]].

## References
