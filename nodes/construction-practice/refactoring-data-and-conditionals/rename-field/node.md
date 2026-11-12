---
tag: 9PRVAX
kind: technique
slug: rename-field
name: Rename field
aliases: []
area: construction-practice/refactoring-data-and-conditionals
level: core
summary: >-
  Change the name of a field of a record or class, and of its accessors, so that the data says what it means; when the name is also stored or sent, rename in stages.
family: refactoring
polarity: positive
intent: "Make the names of data match what the data means."
also_known_as: []
history:
  year: 2018
  people: [Martin Fowler]
  note: Rename Field is in the chapter on organizing data of the second edition of Refactoring (2018).
see_also: [rename-variable, change-function-declaration, encapsulate-record, mysterious-name, move-field]
sources:
  - url: https://refactoring.com/catalog/renameField.html
    title: Martin Fowler, Catalog of Refactorings, Rename Field
    locator: the entry
  - book: isbn:9780134757599
    title: Martin Fowler, Refactoring, Improving the Design of Existing Code, second edition, Addison-Wesley, 2018
    locator: Chapter 9, Organizing Data, Rename Field, motivation and mechanics
  - url: https://www.jetbrains.com/help/idea/rename-refactorings.html
    title: IntelliJ IDEA documentation, Rename refactorings
    locator: the introduction
  - url: https://go.dev/gopls/features/transformation
    title: gopls documentation, Code transformation features
    locator: Rename
  - url: https://pkg.go.dev/encoding/json
    title: Go standard library, package encoding/json
    locator: func Marshal, struct field tags
---

## Purpose

Fields are read more often than they are written, and by more people. A field named `name` that holds an organization's display title, or a field `d` that holds a duration in days, makes every reader stop and check.

Rename field gives the field a name that fits. The catalogue card renames the `name` of an organization to `title`, with its getter.[^1] The book stresses that data structures are central to understanding a program, so their names matter as much as the names of functions.[^2] The same reasons apply as for [[rename-variable|Rename Variable]], but the reach is wider: a field is visible wherever the record is.

## Steps

1. If the record is used only in a small scope, rename the field and every access to it, test, and stop.[^2]
2. Otherwise, if the record is not encapsulated, encapsulate it first with [[encapsulate-record|Encapsulate Record]].[^2]
3. Rename the private field inside the class, and change the methods inside the class that use it. Test.
4. If the constructor takes the field by its old name, rename that parameter with [[change-function-declaration|Change Function Declaration]].
5. Rename the accessors to the changed name, and test.

When the field name is also part of a stored or sent format, such as a JSON key or a column, the rename is a change to that format too. Do it in stages: accept both names, write the changed name, migrate the data, then stop accepting the old name.

## Example

An organization's `name` is shown as a title on every page, and the team wants the code to say so. The class keeps a reader for the old key during the migration of stored data.

```javascript run id=say-what-it-is
import assert from "node:assert/strict";

class Organization {
  #title;
  #country;
  constructor(data) {
    // During the migration, read the old key if the changed key is missing.
    this.#title = data.title ?? data.name;
    this.#country = data.country;
  }
  get title() { return this.#title; }
  set title(t) { this.#title = t; }
  get country() { return this.#country; }
  toJSON() { return { title: this.#title, country: this.#country }; }
}

const fromOld = new Organization({ name: "Harbour Light Trust", country: "NO" });
const fromNew = new Organization({ title: "Harbour Light Trust", country: "NO" });
assert.equal(fromOld.title, fromNew.title);
assert.equal(JSON.stringify(fromOld), '{"title":"Harbour Light Trust","country":"NO"}');
console.log("rename field: tests pass");
```

```output
rename field: tests pass
```

When all stored records have the `title` key, the `?? data.name` fallback goes, and the rename is done.

| Language | What the example shows |
|---|---|
| JavaScript | a renamed private field and accessors, with a fallback for the old key during a data migration, as above |
| Python | the same rename on a dataclass, with a property under the old name kept for a time and marked deprecated |
| Go | the same rename of a struct field, where a `json` struct tag keeps the stored key while the Go name changes |

## When to use

- **The name does not say what the data means.** That is the [[mysterious-name|Mysterious Name]] smell applied to data.
- **The meaning changed.** The field was right once, but the program now uses it for something more specific.
- **Two names for one thing.** Different records call the same data by different names. Pick one.

## Pitfalls

**Names outside the code.** A search and an IDE rename find the references in code. They do not find JSON keys, database columns, configuration files, templates or clients in other repositories. Treat those as a format change, in stages.

**Reflection and strings.** Code that reads fields by name from a string, as serializers and templating engines do, breaks without a compile error. Test the paths that serialize.

**Public fields.** If other modules read the field directly, encapsulate it first, so that the rename happens in one class.

## Tools that do it

IDE rename actions change a field and the references to it. IntelliJ IDEA's Rename can "change names of symbols, files, directories, packages, modules and all the references to them throughout code".[^3] For Go, the gopls rename "takes great care to detect situations in which renaming might introduce a compilation error".[^4] In Go, a struct field tag keeps the stored key apart from the field name: "The encoding of each struct field can be customized by the format string stored under the "json" key in the struct field's tag."[^5]

## References
