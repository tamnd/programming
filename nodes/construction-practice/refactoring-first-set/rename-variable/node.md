---
tag: C17WE0
kind: technique
slug: rename-variable
name: Rename variable
aliases: [rename local variable]
area: construction-practice/refactoring-first-set
level: core
summary: >-
  Give a variable a name that says what it holds, and change every reference in its scope, with care for names that are shadowed and for references that a tool cannot see.
family: refactoring
polarity: positive
intent: "Replace a variable name that does not say what the variable holds with one that does."
also_known_as: []
history:
  year: 2018
  people: [Martin Fowler]
  note: The second edition of Refactoring (2018) puts Rename Variable in its first set of refactorings, next to Change Function Declaration.
see_also: [change-function-declaration, encapsulate-variable, mysterious-name, extract-variable, code-smell]
sources:
  - url: https://refactoring.com/catalog/renameVariable.html
    title: Martin Fowler, Catalog of Refactorings, Rename Variable
    locator: the entry
  - book: isbn:9780134757599
    title: Martin Fowler, Refactoring, Improving the Design of Existing Code, second edition, Addison-Wesley, 2018
    locator: Chapter 6, A First Set of Refactorings, Rename Variable, motivation and mechanics; Chapter 3, Bad Smells in Code, Mysterious Name
  - url: https://go.dev/gopls/features/transformation
    title: gopls documentation, Code transformation features
    locator: Rename
  - url: https://www.jetbrains.com/help/idea/rename-refactorings.html
    title: IntelliJ IDEA documentation, Rename refactorings
    locator: the introduction
  - url: https://code.visualstudio.com/docs/typescript/typescript-refactoring
    title: Visual Studio Code documentation, Refactoring TypeScript and JavaScript
    locator: Rename
---

## Purpose

A name is the first thing a reader sees of a variable, and often the only thing. If the name is `a`, the reader must find the assignment to know what `a` holds. If the name is `area`, the reader knows at the use. The catalogue card shows this case: `let a = height * width;` becomes `let area = height * width;`.[^1]

Rename variable changes the name at the declaration and at each reference. A good name is often found only after the code is written and read, so a rename is a normal part of the work, not a sign that the first name was careless. The book treats it as one of the first fixes for [[mysterious-name|Mysterious Name]].[^2]

How much care a rename needs depends on the scope. A variable in a one-line function is seen whole, and a short name can be enough. A field of a record that is stored, sent or used across many modules needs a careful name and a careful rename.[^2]

## Steps

For a variable with a small scope:

1. Run the tests and see them pass.
2. Change the name at the declaration.
3. Change each reference in the scope. A tool that knows the language does this in one step.
4. Test.

For a variable with a wide scope:

1. If many parts of the program use it, first apply [[encapsulate-variable|Encapsulate Variable]]. Then the rename is a rename of the functions and the variable behind them.[^2]
2. If the variable does not change after it is set, copy it to a variable with the name you want, move the references one at a time, testing after each, and delete the old name.[^2]
3. If the name is part of a published interface, such as a field in a stored file or an API, the rename is a change of format. Keep both names for a time, or leave the old one.

## Example

A rename by search and replace changes text, not names. A rename that knows the scope changes only the references.

```javascript run id=rename-the-name
import assert from "node:assert/strict";

// Before: the reader must find the assignment to know what a is.
function roomBefore(height, width) {
  const a = height * width;
  return `room of ${a} square metres`;
}

// After: the name says what the value is.
function room(height, width) {
  const area = height * width;
  return `room of ${area} square metres`;
}
assert.equal(room(3, 4), roomBefore(3, 4));

// A rename by text, not by scope: count -> total also changes the string.
const source = 'const count = 2; console.log("count: " + count);';
const byText = source.replaceAll("count", "total");
assert.equal(byText, 'const total = 2; console.log("total: " + total);');
// The message that users see has changed; a rename by scope leaves strings alone.
const byScope = 'const total = 2; console.log("count: " + total);';
assert.notEqual(byText, byScope);
console.log("rename variable: tests pass");
```

```output
rename variable: tests pass
```

The second part shows why the tools work on the syntax tree. A text replace cannot tell the variable `count` from the word "count" in a message, from a comment, or from another variable in another scope with the same name.

| Language | What the example shows |
|---|---|
| JavaScript | a rename, and a text replace that changes a string, as above |
| Python | a rename that a tool cannot finish, because a name is used as a string in `getattr` |
| Go | a rename that would capture an outer variable, which gopls refuses |

## When to use

- **The name does not say what the variable holds**, or says something that is no longer true.
- **The name is the type, not the role.** `list` or `str` says less than `pendingOrders` or `customerName`.
- **After you learn more.** A first name often comes from the first use of the value. When other uses appear, a better name may too.
- **Before an extraction.** A good name for a variable is often the name for the function that [[extract-function|Extract Function]] makes from the code around it.

## Pitfalls

**Shadowing and capture.** A rename can make two variables share a name, so that a reference that meant one now reaches the other. gopls checks for this: "changing a name may cause a symbol to become “shadowed”, so that some existing references are no longer in scope", and it reports an error.[^3]

**References that the tool cannot see.** A name in a string, in a reflection call, in a template, in a configuration file or in a serialized record is not a reference in the syntax tree. The tool does not find it.

**Wide renames.** IntelliJ IDEA notes that renaming local variables "can be done easily inline since only the limited scope is affected", while renaming classes or public methods "could potentially impact a lot of files", and it shows a preview of the changes.[^4]

**A name that is worse.** A long name is not always clearer. In a two-line loop, `i` is a known convention; `currentIndexIntoTheArray` makes the loop harder to read.

## Tools that do it

| Tool | Languages | What it automates |
|---|---|---|
| IntelliJ IDEA | Java, Kotlin, and others | Rename, to "change names of symbols, files, directories, packages, modules and all the references to them throughout code"[^4] |
| gopls | Go | the LSP rename, with a check that "takes great care to detect situations in which renaming might introduce a compilation error"[^3] |
| VS Code | TypeScript, JavaScript | Rename with F2, "across your TypeScript project"[^5] |

Each of these tools renames by scope, not by text. None of them can see the references that are only strings.

## References
