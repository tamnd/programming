---
tag: 5D3PMD
kind: practice
slug: fail-fast
name: Fail fast
aliases: [fail-fast, failing fast]
area: software-design/design-principles
level: core
summary: >-
  When a problem occurs, stop and report it at once and in a visible way, instead of working around it and failing in strange ways later.
history:
  year: 1985
  people: [Jim Gray, Jim Shore]
  note: Wikipedia credits a 1985 report by Jim Gray, Why Do Computers Stop and What Can Be Done About It?, with introducing the term; Jim Shore's column Fail Fast in IEEE Software, 2004, applied it to everyday code.
see_also: [principle-of-least-astonishment, keep-it-simple, design-principle]
sources:
  - url: https://www.martinfowler.com/ieeeSoftware/failFast.pdf
    title: Jim Shore, Fail Fast, IEEE Software, September/October 2004, Design column edited by Martin Fowler
    locator: the sections Immediate and visible failure and Robust failure
  - url: https://en.wikipedia.org/wiki/Fail-fast_system
    title: Wikipedia, Fail-fast system
    locator: the lead, the section Hardware and software, and the references
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Map/get
    title: MDN Web Docs, Map.prototype.get()
    locator: the description and the return value
  - url: https://docs.python.org/3/library/stdtypes.html
    title: Python documentation, Built-in Types
    locator: Mapping Types, dict, d[key] and get
  - url: https://go.dev/ref/spec
    title: The Go Programming Language Specification
    locator: Index expressions, for a of map type M
---

## Statement

"A system that fails fast does exactly the opposite: when a problem occurs, it fails immediately and visibly."[^1] The opposite that Jim Shore means is the advice to make software robust "by working around problems automatically", which he says "results in the software 'failing slowly.'"[^1]

## Meaning

A program that works around a problem continues, but "fails in strange ways later on."[^1] The place where the program fails is then far from the place where the problem started, and the stack trace does not lead to the cause. A program that fails fast stops at the first sign of the problem, so the error report points at the cause.

Shore wrote that the technique sounds wrong: "'failing immediately and visibly' sounds like it would make your software more fragile, but it actually makes it more robust. Bugs are easier to find and fix, so fewer go into production."[^1] He also noted that it does not make fewer bugs at first, "but it'll make most defects much easier to find."[^1]

In systems design, "a fail-fast system is one that immediately reports at its interface any condition that is likely to indicate a failure."[^2] Such systems "are usually designed to stop normal operation rather than attempt to continue a possibly flawed process", and "The responsibility of a fail-fast module is detecting errors, and then letting the next-highest level of the system handle them."[^2]

## Examples

Shore's example reads a property from a configuration file. A common approach returns a default value when the property is missing; a program that fails fast throws an exception.[^1] In the example below, the file has a typo in the name of the property.

```javascript run id=missing-setting
const config = new Map([["maxConections", "50"]]); // a typo in the file

function maxConnectionsSlow() {
  const value = config.get("maxConnections");
  return value === undefined ? 10 : Number(value);
}

function maxConnectionsFast() {
  const value = config.get("maxConnections");
  if (value === undefined) throw new Error("maxConnections not found in config");
  return Number(value);
}

console.log(`slow: ${maxConnectionsSlow()}`);
try {
  console.log(`fast: ${maxConnectionsFast()}`);
} catch (e) {
  console.log(`fast: ${e.message}`);
}
```

```output
slow: 10
fast: maxConnections not found in config
```

The slow version runs with 10 connections, and nothing tells the operator that the setting of 50 was ignored. In Shore's story, "everything will seem fine" at first, but customers then see slowdowns, and the search for the cause "could take days of hair pulling."[^1] The fast version stops with a message that names the missing property.

The lookup of each language decides which behaviour you get by default:

| Language | Lookup of a missing key |
|---|---|
| JavaScript | Map.prototype.get returns "the value corresponding to the key in this Map, or undefined if there is none."[^3] |
| Python | d[key] "Raises a KeyError if key is not in the map." get returns a default, "so that this method never raises a KeyError."[^4] |
| Go | "if the map is nil or does not contain such an entry, a[x] is the zero value for the element type of M". With the comma-ok form, "The value of ok is true if the key x is present in the map, and false otherwise."[^5] |

The Python and Go examples of this page show the same configuration lookup.

## Criticism and limits

- **A crash at the customer's site.** Shore asked: "what about when you deploy the software to customers? We don't want the application to crash just because there's a typo in a configuration file." He rejected the answer of turning assertions off in the field: "Don't do that!" His reason is that such an error "made it through your testing process", so it is hard to reproduce.[^1]
- **The middle ground.** Shore also wrote that "a crash is never appropriate." His answer is a "global exception handler to gracefully handle unexpected exceptions, such as assertions, and bring them to the developers' attention." A GUI program can show an error dialog, and a batch system "might page a system administrator and continue with the next transaction".[^1]
- **Fault tolerance.** "In some sense, the error-handling policy is the opposite of that used in a fault-tolerant system."[^2] Failing fast moves the handling of an error to a higher level; it does not remove the need to handle it.

## History

The Wikipedia article on fail-fast systems lists a 1985 report by Jim Gray, Why Do Computers Stop and What Can Be Done About It?, as the work that introduced the term.[^2] In 2004, Jim Shore wrote the column Fail Fast for the Design department of IEEE Software, which Martin Fowler edited. The column applied the idea to the error handling of everyday code.[^1]

## References
