---
tag: DEHPTN
kind: pattern
slug: mysterious-name
name: Mysterious Name
aliases: [unclear name, bad name]
area: construction-practice/code-smells
level: core
summary: >-
  A code smell: a function, variable, field or module has a name that does not say what it does or what it holds, so the reader must read the code behind the name to learn it.
family: code-smell
polarity: negative
intent: "Find the names that hide the purpose of the code, and rename them so that the name tells the reader what the code does."
also_known_as: []
refactorings: [change-function-declaration, rename-variable, rename-field]
history:
  year: 2018
  people: [Kent Beck, Martin Fowler]
  note: Mysterious Name is the first smell of the second edition of Refactoring (2018). The first edition had no smell for names.
see_also: [code-smell, identifier, comment, extract-function]
sources:
  - url: https://martinfowler.com/bliki/TwoHardThings.html
    title: Martin Fowler, Two Hard Things, bliki
    locator: the saying attributed to Phil Karlton
  - url: https://go.dev/wiki/CodeReviewComments#variable-names
    title: Go Wiki, Go Code Review Comments
    locator: Variable Names
  - book: isbn:9780134757599
    title: Martin Fowler, with Kent Beck, Refactoring, Improving the Design of Existing Code, second edition, Addison-Wesley, 2018
    locator: Chapter 3, Bad Smells in Code, Mysterious Name
  - url: https://refactoring.com/catalog/
    title: Martin Fowler, Catalog of Refactorings
    locator: the cards Change Function Declaration (aliases Rename Function, Rename Method), Rename Variable and Rename Field
  - url: https://www.jetbrains.com/help/idea/rename-refactorings.html
    title: IntelliJ IDEA documentation, Rename refactorings
    locator: the introduction
  - url: https://code.visualstudio.com/docs/editing/refactoring#_rename-symbol
    title: Visual Studio Code documentation, Refactoring
    locator: Rename symbol
  - url: https://go.dev/gopls/features/transformation#rename
    title: gopls documentation, Code transformation features
    locator: Rename
  - url: https://pylint.readthedocs.io/en/stable/user_guide/messages/convention/disallowed-name.html
    title: Pylint documentation, disallowed-name / C0104
    locator: the description and the configuration bad-names
  - url: https://eslint.org/docs/latest/rules/id-length
    title: ESLint documentation, Rules, id-length
    locator: the rule summary and the default minimum of 2
  - url: https://peps.python.org/pep-0008/#names-to-avoid
    title: PEP 8, Style Guide for Python Code
    locator: Naming Conventions, Names to Avoid
---

## Intent

Find the names that hide what the code does, and change them so that the name says it. A good name lets a reader skip the body of a function, or the history of a variable. A mysterious name forces the reader to open it.

## Problem

Code is read many more times than it is written, and names are most of what a reader sees. A function call shows its name and its arguments, not its body. A variable that is used 40 lines after it is set shows only its name at the place where it is used. When the name does not say what the thing is, the reader must find out, and must keep the answer in mind.

The forces are these:

- A name is chosen when the code is first written, often before its purpose is clear. Later the purpose changes, and the name stays.
- A good name is hard to find. Fowler keeps on his site the saying attributed to Phil Karlton: "There are only two hard things in Computer Science: cache invalidation and naming things."[^1]
- Short names are right in some places. The Go code review guide prefers `c` to `lineCount` for a local variable with a small scope, and states the rule: "the further from its declaration that a name is used, the more descriptive the name must be."[^2] So the length of a name is not the test. The test is whether the reader at each use knows what the name means.
- Some names are part of an interface: a public function, a field of a JSON document, a column of a table. Changing them breaks the code of other people.

*Refactoring* puts this smell first in its list of 24.[^3]

## Solution

The smell has two parts: a name, and the purpose of the thing behind it. The name is mysterious when the two do not match. There are three common forms:

1. **A name that says nothing**: `data`, `tmp`, `x`, `calc`, `process`, `handle`, `manager`, `info`.
2. **A name that says how, not what**: `loopOverList`, `stringToInt2`, `checkFlags`.
3. **A name that lies**: `getUser` that also writes to the database, `isValid` that returns a count, `total` that holds the total without tax in one branch and with tax in another.

The fix is a rename. *Refactoring* names three refactorings for it: Change Function Declaration for a function, Rename Variable for a variable, and Rename Field for a field of a record or a class.[^3] In the online catalogue, Change Function Declaration has the aliases Rename Function and Rename Method, from the first edition.[^4]

A rename changes no behaviour, so the tests that pass before it pass after it. But the work of a rename is to understand the code well enough to name it. *Refactoring* makes the point that when no good name comes to mind, the code often has a deeper problem in its design.[^3] A function that cannot be named often does two things. Then the fix is to split it, with [[extract-function]], and to name each part.

## Example

The function `calc` takes a list of phone calls and a number of free minutes, and returns the hours to bill. Nothing in its name or its parameters says so. The example files show it in four languages, with the same tests on the mysterious version and the renamed one.

```javascript run id=rename-in-place
import assert from "node:assert/strict";

function calc(l, x) {
  let n = 0;
  for (const e of l) {
    if (e.minutes > x) n += e.minutes;
  }
  return n / 60;
}

function billableHours(calls, freeMinutes) {
  let billedMinutes = 0;
  for (const call of calls) {
    if (call.minutes > freeMinutes) billedMinutes += call.minutes;
  }
  return billedMinutes / 60;
}

const calls = [{ minutes: 5 }, { minutes: 30 }, { minutes: 45 }];
for (const f of [calc, billableHours]) {
  assert.equal(f(calls, 10), 1.25);
  assert.equal(f([], 10), 0);
  console.log(`${f.name}: tests pass`);
}
```

```output
calc: tests pass
billableHours: tests pass
```

The renamed version shows a business rule that `calc` hid. A call longer than the free minutes is billed in full, not only for the minutes over the limit. With the names `l`, `x` and `n`, a reader cannot tell if that is the intent or a bug. With `freeMinutes` and `billedMinutes`, the question is clear, and a reader can ask it.

| Language | What the example shows |
|---|---|
| JavaScript | the rename above, with `node:assert` |
| Python | the same rename; PEP 8 also forbids `l` as a one-letter name |
| Java | a rename of a static method and its parameters, with a record for the call |
| Go | a rename of an unexported function; the short loop variable `c` stays, because its scope is two lines |

## Consequences

Renames are cheap when the name is local. An IDE does them across the whole project. IntelliJ IDEA's Rename refactoring changes a symbol "and all the references to them throughout code".[^5] Visual Studio Code has the command Rename Symbol (F2), and some languages support it across files.[^6] For Go, the language server gopls implements the rename request of the Language Server Protocol.[^7] A tool rename is safer than search and replace, because the tool knows the scopes.

Renames are expensive when the name is published. A public function has callers outside the project. A field name can be in stored data, in a JSON API or in a database column. Change Function Declaration has a migration form for this case: add the function with the good name, make the old name call it, mark the old name as deprecated, and remove it when no caller is left.[^3]

Dynamic languages make renames harder for tools. A name that is built from a string, or a field that is read with `obj[key]`, is not found by the rename tool. The tests must cover these uses.

## How to fix it

- **A function or method:** [[change-function-declaration|Change Function Declaration]]. Rename it, and use the migration form when it is published.[^3]
- **A local variable or a parameter:** [[rename-variable|Rename Variable]]. The scope decides the effort: a local variable needs one function changed, a module-level one needs all its readers changed.[^3]
- **A field of a record or a class:** [[rename-field|Rename Field]]. Encapsulate the field first when many places use it directly.[^3]
- **A name that does not come:** split the code with [[extract-function]], and name each part.

Linters can find some mysterious names, but not most of them. Pylint reports `disallowed-name` for names in its `bad-names` list, such as `foo`, `bar` and `baz`.[^8] The ESLint rule `id-length` enforces a minimum length, two characters by default.[^9] A rule can find `x`. It cannot know that `total` holds the wrong total. That needs a person who reads the code.

## Variants and related patterns

**Magic numbers.** A literal such as `0.19` with no name is a mysterious name of another kind: the value has no name at all. See [[magic-number|Magic number]] and the refactoring [[replace-magic-literal|Replace Magic Literal]].

**Comments.** A comment that explains what a variable holds is often a sign that the variable needs a better name. See [[comments|Comments]] (the smell) and [[comment]] (the language construct).

**Boolean parameter.** A call such as `send(msg, true)` has an argument with no name at the call site. See [[boolean-parameter-smell|Boolean parameter]].

**Primitive Obsession.** A type can have a mysterious name too. A parameter of type `string` that holds a phone number says less than a parameter of type `PhoneNumber`. See [[primitive-obsession|Primitive Obsession]].

**Names to avoid.** Some style guides forbid names that a reader can confuse with other characters. PEP 8 says: "Never use the characters ‘l’ (lowercase letter el), ‘O’ (uppercase letter oh), or ‘I’ (uppercase letter eye) as single character variable names."[^10] The reason is that some fonts show them like the digits one and zero.

## References
