---
tag: 8NZ7GD
kind: practice
slug: uniform-access-principle
name: Uniform access principle
aliases: [UAP]
area: software-design/design-principles
level: advanced
summary: >-
  Give each service of a module one notation, so that a client cannot tell whether the value is stored or computed, and the module can change between the two without changes to its clients.
history:
  year: 1988
  people: [Bertrand Meyer]
  note: Bertrand Meyer put the principle forward in his book Object-Oriented Software Construction; Open Library dates the first edition of the book, from Prentice-Hall, to 1988.
see_also: [encapsulation-and-visibility, command-query-separation, program-to-an-interface, design-principle]
sources:
  - url: https://en.wikipedia.org/wiki/Uniform_access_principle
    title: Wikipedia, Uniform access principle
    locator: the lead and the sections Explanation, Problems and JavaScript
  - url: https://openlibrary.org/isbn/9780136290315
    title: Open Library, Object-oriented software construction, Bertrand Meyer, Prentice-Hall, 1988
    locator: the edition record
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Functions/get
    title: MDN Web Docs, get
    locator: the description
  - url: https://docs.python.org/3/library/functions.html
    title: Python documentation, Built-in Functions
    locator: class property
  - url: https://go.dev/doc/effective_go
    title: Effective Go
    locator: section Getters
---

## Statement

"All services offered by a module should be available through a uniform notation, which does not betray whether they are implemented through storage or through computation."[^1]

## Meaning

The principle is about the cost of a change. A class can start with a stored attribute and later need to compute the value. When a language has one notation for an attribute and another for a method call, "e.g., object.something versus object.something()", the change "would require, in popular programming languages of the day, changing the source code in all the places where the attribute was used."[^1] If the class is in a library, each customer must "find and change all the places the attribute was used in their own code and recompile their programs."[^1]

With a uniform notation, the client writes the same thing in both cases, so the class can move between storage and computation without breaking its clients. The opposite change was never a problem: "one can always just keep the function and have it simply return the attribute value."[^1]

Most languages do not follow the principle strictly. Properties address the problem "in a different way. Instead of providing a single uniform notation, properties provide a way to invoke a method of an object while using the same notation as is used for attribute access."[^1]

## Examples

In the example below, the first version of a rectangle stores its area, and the second version computes it with a getter. The function report reads rect.area in the same way for both. The example then changes the width of each rectangle.

```javascript run id=rect-area
class StoredRect {
  constructor(width, height) {
    this.width = width;
    this.height = height;
    this.area = width * height; // stored once
  }
}

class ComputedRect {
  constructor(width, height) {
    this.width = width;
    this.height = height;
  }

  get area() {
    return this.width * this.height; // computed on each read
  }
}

const report = (name, rect) => console.log(`${name}: ${rect.area}`);

const a = new StoredRect(2, 3);
const b = new ComputedRect(2, 3);
report("stored", a);
report("computed", b);

a.width = 4;
b.width = 4;
report("stored", a);
report("computed", b);
```

```output
stored: 6
computed: 6
stored: 6
computed: 12
```

The client code is the same for both classes, so a change from the first class to the second does not break report. The stored area is stale after the change of width; the computed area is not.

| Language | Uniform notation |
|---|---|
| JavaScript | "The get syntax binds an object property to a function that will be called when that property is looked up."[^3] |
| Python | With property, "If c is an instance of C, c.x will invoke the getter, c.x = value will invoke the setter, and del c.x the deleter."[^4] |
| Go | There are no properties. "Go doesn't provide automatic support for getters and setters." A getter for a field owner "should be called Owner (upper case, exported), not GetOwner."[^5] A uniform notation in Go means that clients call a method, such as Area(), from the start. |

The Python example uses property; the Go example uses an Area method for both versions.

## Criticism and limits

- **Hidden cost.** The principle "can lead to problems, if used in places where the differences between access methods are not negligible, such as when the returned value is expensive to compute or will trigger cache operations."[^1] A client that reads rect.area in a loop cannot see that each read runs code.
- **Writes are harder.** Most examples are about reading a value. Meyer showed that "the 'write' implications (i.e., modifying a value) of the principle are harder to deal with", in his monthly column on the official website of the Eiffel language.[^1]

## History

The principle "was put forth by Bertrand Meyer (originally in his book Object-Oriented Software Construction)."[^1] Open Library lists a Prentice-Hall edition of the book from 1988.[^2] In JavaScript, support for computed properties "via getters and setters since ECMAScript 5 in 2009" let the language follow the principle for reads.[^1]

## References
