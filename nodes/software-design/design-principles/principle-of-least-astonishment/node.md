---
tag: HXQD41
kind: practice
slug: principle-of-least-astonishment
name: Principle of least astonishment
aliases: [POLA, principle of least surprise, POLS, law of least astonishment]
area: software-design/design-principles
level: core
summary: >-
  A component should behave the way most of its users expect, so that its behaviour does not surprise them; names, defaults and conventions should match what the user already knows.
history:
  year: 1967
  people: []
  note: An early reference to the Law of Least Astonishment appeared in the PL/I Bulletin in 1967; the law first appeared in print in 1972.
see_also: [keep-it-simple, separate-query-from-modifier, dont-repeat-yourself]
sources:
  - url: https://en.wikipedia.org/wiki/Principle_of_least_astonishment
    title: Wikipedia, Principle of least astonishment
    locator: the whole article
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Array/sort
    title: MDN Web Docs, Array.prototype.sort()
    locator: the description of the default sort order
  - url: https://docs.python.org/3/tutorial/controlflow.html
    title: The Python Tutorial, 4. More Control Flow Tools
    locator: section 4.9.1 Default Argument Values, the important warning
  - url: https://go.dev/blog/loopvar-preview
    title: David Chase and Russ Cox, Fixing For Loops in Go 1.22, The Go Blog, 19 September 2023
    locator: the sections The Problem and the start of the post
  - url: https://go.dev/doc/go1.22
    title: Go 1.22 Release Notes
    locator: Changes to the language, the first item
---

## Statement

The principle of least astonishment "proposes that a component of a system should behave in a way that most users will expect it to behave, and therefore not astonish or surprise users."[^1] A corollary: "If a necessary feature has a high astonishment factor, it may be necessary to redesign the feature."[^1]

## Meaning

The user of a function is a programmer, and the user of a command is often a person at a terminal. Each user comes with expectations from names, from conventions and from other programs. The principle asks the designer to meet those expectations.

- **Names match behaviour.** In an API, "the expectation that function or method names intuitively match their behavior" is an example of the principle.[^1] A function called getUser should not create a user.
- **Follow the conventions of the platform.** Unix command line programs are expected to follow certain conventions for their switches, and Windows programs follow conventions for keyboard shortcuts.[^1]
- **Sensible defaults.** A default should be the value that most callers want. A standard library function that parses an integer conventionally uses base 10 unless another base is given.[^1]
- **Think as the user, not as the builder.** A programmer "should try to think of the behavior that will least surprise someone who uses the program, rather than that behavior that is natural from knowing the inner workings of the program."[^1]

The 1972 statement of the law for programming languages says: "every construct in the system should behave exactly as its syntax suggests. Widely accepted conventions should be followed whenever possible, and exceptions to previously established rules of the language should be minimal."[^1]

## Examples

Each language has well-known surprises. In JavaScript, the default order of sort is "built upon converting the elements into strings, then comparing their sequences of UTF-16 code unit values."[^2] An array of numbers is sorted as text. A designer who knows this gives the comparison explicitly, and does not copy the surprise into their own API.

```javascript run id=surprise
const numbers = [10, 9, 1];

// The surprise: elements are compared as strings.
console.log([...numbers].sort());

// No surprise: the comparison is explicit.
console.log([...numbers].sort((a, b) => a - b));
```

```output
[ 1, 10, 9 ]
[ 1, 9, 10 ]
```

| Language | What the example shows |
|---|---|
| JavaScript | the default string order of sort, and the fix with a comparator, as above |
| Python | a list as a default argument, which is "evaluated only once", and the fix with None[^3] |
| Go | closures in a loop, which print 1, 2 and 3 because the Go 1.22 release gave each iteration its own variable[^4][^5] |

## Criticism and limits

- **Surprise depends on the user.** "The choice of least surprising behavior can depend on the expected audience (for example, end users, programmers, or system administrators)."[^1] Behaviour that is natural for a system administrator can surprise an end user.
- **Expectations belong to a platform.** "User expectations in this respect may be closely related to a particular computing platform or tradition."[^1] A program that runs on many platforms must choose whose conventions it follows.
- **Fixing a surprise can break code.** A change to remove a surprise can change the behaviour of programs that already depend on it. JavaScript once read a string that begins with 0 in base 8 by default. This was discouraged in ECMAScript 3 and dropped in ECMAScript 5.[^1] The Go team changed the scope of loop variables in Go 1.22 to remove "one of the most common Go mistakes".[^4]

## History

"An early reference to the Law of Least Astonishment appeared in the PL/I Bulletin in 1967". By the late 1960s, PL/I was infamous for breaking the law: because of its precision conversion rules, 25 + 1/3 and 1/3 + 25 gave a fatal error or 5.33333333333 instead of 25.33333333333.[^1] The law first appeared in print in 1972, in a statement about systems programming languages.[^1] The principle has been used for computer interaction since at least the 1970s, and communities such as FreeBSD use it as a guideline.[^1]

## References
