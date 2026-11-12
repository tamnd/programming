---
tag: 9BZ2PT
kind: technique
slug: encapsulate-collection
name: Encapsulate collection
aliases: [read-only collection view]
area: construction-practice/refactoring-encapsulation
level: core
summary: >-
  Stop a getter from handing out the collection that an object owns; give the object methods to add and remove elements, and return a copy or a read-only view.
family: refactoring
polarity: positive
intent: "Make the owner of a collection the only code that changes which elements it holds."
also_known_as: []
history:
  year: 1999
  people: [Martin Fowler]
  note: Encapsulate Collection is in the first edition of Refactoring (1999) and in the chapter on encapsulation of the second edition (2018).
see_also: [encapsulate-record, encapsulate-variable, mutable-data, encapsulation-and-visibility, code-smell]
sources:
  - url: https://refactoring.com/catalog/encapsulateCollection.html
    title: Martin Fowler, Catalog of Refactorings, Encapsulate Collection
    locator: the entry
  - book: isbn:9780201485677
    title: Martin Fowler, Refactoring, Improving the Design of Existing Code, Addison-Wesley, 1999
    locator: Chapter 8, Organizing Data, Encapsulate Collection
  - book: isbn:9780134757599
    title: Martin Fowler, Refactoring, Improving the Design of Existing Code, second edition, Addison-Wesley, 2018
    locator: Chapter 7, Encapsulation, Encapsulate Collection, motivation and mechanics
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Object/freeze
    title: MDN Web Docs, Object.freeze()
    locator: the description, and the note that freeze is shallow
  - url: https://docs.python.org/3/library/types.html
    title: Python documentation, types, MappingProxyType
    locator: class types.MappingProxyType
  - url: https://docs.oracle.com/en/java/javase/21/docs/api/java.base/java/util/List.html
    title: Java SE 21 API, Interface List
    locator: Unmodifiable Lists
  - url: https://pkg.go.dev/slices
    title: Go standard library, package slices
    locator: func Clone
---

## Purpose

An object that owns a list, a set or a map usually has rules about it: no duplicates, a maximum size, an order, a count kept in another field. A getter that returns the collection itself breaks all of them. The caller gets a reference, calls `push` or `remove` on it, and the owner never knows.

Encapsulate collection keeps the changes inside the owner. The getter returns a copy or a read-only view, and the owner gets methods such as `addCourse` and `removeCourse` for each change it allows. The catalogue card shows a `Person` whose `courses` getter returns `this._courses` before and `this._courses.slice()`, a copy, after, with `addCourse` and `removeCourse` added.[^1] The first edition has the refactoring under the same name.[^2]

This is the collection case of [[encapsulate-record|Encapsulate Record]] and [[encapsulate-variable|Encapsulate Variable]]. Encapsulating the variable that holds the collection is not enough. The variable can be private while its contents are open to everyone who called the getter.[^3]

## Steps

1. Run the tests and see them pass.
2. If the field that holds the collection is not yet behind accessors, apply [[encapsulate-variable|Encapsulate Variable]].[^3]
3. Add methods to add and to remove an element, and to do any other change that callers need.
4. If there is a setter for the whole collection, remove it, or make it store a copy of its argument.[^3]
5. Run the static checks.
6. Find each caller that changes the collection through the getter, and change it to call the methods of step 3. Test after each.
7. Change the getter to return a copy or a read-only view. Test.[^3]

## Example

A team allows at most three members. Before the refactoring, a caller can add a fourth through the getter. After it, the team checks each change and hands out only copies.

```javascript run id=owner-decides
import assert from "node:assert/strict";

// Before: the getter returns the list that the team owns.
class TeamBefore {
  #members = [];
  get members() { return this.#members; }
}
const leaky = new TeamBefore();
leaky.members.push("ana", "bo", "cy", "dee"); // the rule cannot run
assert.equal(leaky.members.length, 4);

// After: changes go through the owner; readers get a frozen copy.
class Team {
  static MAX = 3;
  #members = [];
  get members() { return Object.freeze([...this.#members]); }
  addMember(name) {
    if (this.#members.includes(name)) return;
    if (this.#members.length >= Team.MAX) throw new RangeError("team is full");
    this.#members.push(name);
  }
  removeMember(name) {
    this.#members = this.#members.filter((m) => m !== name);
  }
}

const team = new Team();
for (const n of ["ana", "bo", "cy"]) team.addMember(n);
assert.throws(() => team.addMember("dee"), RangeError);
assert.throws(() => team.members.push("dee"), TypeError); // frozen copy
team.removeMember("bo");
team.addMember("dee");
assert.deepEqual(team.members, ["ana", "cy", "dee"]);
console.log("encapsulate collection: tests pass");
```

```output
encapsulate collection: tests pass
```

`Object.freeze` makes the copy read-only, so a caller that tries to change it gets an error at once, not a change that has no effect. A frozen array is shallow: the elements, if they are objects, can still change.[^4]

| Language | What the example shows |
|---|---|
| JavaScript | a frozen copy from the getter, and add and remove methods, as above |
| Python | a tuple from the getter, and a `MappingProxyType` view of a dictionary |
| Java | `List.copyOf` in the getter, and the `UnsupportedOperationException` on a change |
| Go | `slices.Clone` in the getter, because a slice shares its array |

## When to use

- **The owner has rules about the collection.** A size, an order, no duplicates, a total kept in step.
- **Callers change the collection through a getter.** Each such caller is a place where a rule can be broken.
- **As part of a fix for [[mutable-data|Mutable Data]].** A collection that many callers can change is shared mutable data.

## Pitfalls

**A copy is not a view.** A copy is a snapshot: later changes by the owner do not show in it. A view shows them. Python's `MappingProxyType` is a view: the documentation calls it a "Read-only proxy of a mapping" that "provides a dynamic view on the mapping’s entries".[^5] Choose by what the caller needs.

**Shallow protection.** A read-only list of mutable objects lets the caller change the objects. The Java documentation says so of its unmodifiable lists: "if the contained elements are themselves mutable, this may cause the List's contents to appear to change".[^6]

**A silent failure.** A copy that the caller can change, such as `slice()` without a freeze, accepts the change and throws it away. The caller's code looks right and does nothing. A read-only view or a frozen copy fails at once.

**The cost of a copy.** A copy on each read of a large collection can be slow. A read-only view costs little, and an iterator over the elements can be enough for many readers.

**Shared backing arrays.** In Go, a slice returned from a getter shares its array with the field. The caller can change the elements in place. `slices.Clone` "returns a copy of the slice".[^7]

## Tools that do it

This refactoring is a sequence of steps, not one action of a tool. A tool can generate the accessors of a field; the add and remove methods, and the choice of a copy or a view in the getter, are the programmer's steps.

## References
