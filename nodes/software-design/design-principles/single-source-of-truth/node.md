---
tag: 56ES6S
kind: practice
slug: single-source-of-truth
name: Single source of truth
aliases: [SSOT, single point of truth, SPOT rule]
area: software-design/design-principles
level: core
summary: >-
  Master each piece of data or knowledge in one place only, and make every other use read it or derive it from that place, so that copies cannot disagree.
history:
  year: 2003
  people: [Eric S. Raymond, Brian Kernighan]
  note: Eric S. Raymond named the Single Point Of Truth or SPOT rule in The Art of Unix Programming, 2003, following a suggestion by Brian Kernighan, as another name for the Don't Repeat Yourself rule.
see_also: [dont-repeat-yourself, orthogonality, command-query-separation, design-principle]
sources:
  - url: https://en.wikipedia.org/wiki/Single_source_of_truth
    title: Wikipedia, Single source of truth
    locator: the lead
  - url: http://www.catb.org/esr/writings/taoup/html/ch04s02.html
    title: Eric S. Raymond, The Art of Unix Programming, chapter 4, Compactness and Orthogonality
    locator: section The SPOT Rule
  - url: http://www.catb.org/esr/writings/taoup/html/index.html
    title: Eric S. Raymond, The Art of Unix Programming, table of contents
    locator: the copyright line, 2003
---

## Statement

Single source of truth (SSOT) "is the practice of structuring information models and associated data schemas such that every data element is mastered (or edited) in only one place".[^1] In code, Eric S. Raymond calls it the SPOT rule, from the Don't Repeat Yourself rule of The Pragmatic Programmer: "every piece of knowledge must have a single, unambiguous, authoritative representation within a system."[^2]

## Meaning

When a fact is in two places, a change must reach both. "Repetition leads to inconsistency and code that is subtly broken, because you changed only some repetitions when you needed to change all of them."[^2] With one source, there is nothing to keep in step.

A single source does not forbid copies. Wikipedia lists the cases: the data is "never copied and instead only references to it are made"; or it is "copied but the copies are only read and only the master data is updated"; or the copies are also updated, which "needs a reconciliation mechanism when there are concurrent updates."[^1] The advantages "include easier prevention of mistaken inconsistencies (such as a duplicate value/copy somewhere being forgotten), and greatly simplified version control."[^1]

Raymond gives the rule for code: "Constants, tables, and metadata should be declared and initialized once and imported elsewhere."[^2] When two representations seem necessary, he asks: "can you write a function, tool or code generator to make one representation from the other, or both from a common source?"[^2]

## Examples

In the example below, a program has a list of statuses for its menu and a second list in its validator. A developer adds the status archived to the menu and forgets the validator. The second version uses one list for both.

```javascript run id=status-list
// Two copies of the same knowledge.
const menuStatuses = ["open", "closed", "archived"];
const isValidCopy = (s) => ["open", "closed"].includes(s); // not updated

// One source of truth.
const STATUSES = ["open", "closed", "archived"];
const menu = STATUSES;
const isValid = (s) => STATUSES.includes(s);

console.log(`copies: menu ${menuStatuses.length}, archived valid ${isValidCopy("archived")}`);
console.log(`single source: menu ${menu.length}, archived valid ${isValid("archived")}`);
```

```output
copies: menu 3, archived valid false
single source: menu 3, archived valid true
```

With two copies, the user can choose a status that the validator then rejects. With one source, a new status reaches the menu and the validator at the same time.

| Language | What the example shows |
|---|---|
| JavaScript | one array that the menu and the validator share, as above |
| Python | one tuple that the menu and the validator share |
| Go | one slice that the menu and the validator share, with slices.Contains |

## Criticism and limits

- **Caches are copies.** Raymond: "Are you duplicating data because you're caching intermediate results of some computation or lookup? Consider carefully whether this is premature optimization; stale caches (and the layers of code needed to keep caches synchronized) are a fertile source of bugs".[^2]
- **The cost of no single source.** "Without a SSOT, dealing with inconsistencies implies either complex and error-prone consensus algorithms, or using a simpler architecture that's liable to lose data in the face of inconsistency".[^1] A system with copies that are updated must choose one of these.
- **Generated copies.** When two representations are necessary, for example the code and its documentation, Raymond suggests generating one from the other.[^2] The generator is then a new part of the build that must be kept working.

## History

Raymond's book The Art of Unix Programming is copyright 2003.[^3] It takes the Don't Repeat Yourself rule from The Pragmatic Programmer and says: "In this book we prefer, following a suggestion by Brian Kernighan, to call this the Single Point Of Truth or SPOT rule."[^2] Raymond notes that in the Unix world, "the SPOT Rule as a unifying idea has seldom been explicit", but that the heavy use of code generators to implement it is part of the tradition.[^2] See [[dont-repeat-yourself|Don't repeat yourself]].

## References
