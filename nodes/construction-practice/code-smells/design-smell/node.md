---
tag: AVWV70
kind: concept
slug: design-smell
name: Design smell
aliases: [design smells, architecture smell]
area: construction-practice/code-smells
level: advanced
summary: >-
  A design smell is a structure in the design of a program, such as a missing abstraction or a cycle between modules, that breaks a basic design principle and makes the program harder to change.
history:
  year: 2014
  people: [Girish Suryanarayana, Ganesh Samarthyam, Tushar Sharma]
  note: The term follows code smell, from the Refactoring book of 1999. Suryanarayana, Samarthyam and Sharma named and grouped a catalogue of design smells in Refactoring for Software Design Smells, Managing Technical Debt (Morgan Kaufmann, 2014).
see_also: [code-smell, primitive-obsession, data-clumps, repeated-switches, refused-bequest, alternative-classes-with-different-interfaces, large-class]
sources:
  - url: https://en.wikipedia.org/w/index.php?title=Design_smell&oldid=1234564304
    title: Wikipedia, Design smell, revision 1234564304
    locator: the introduction, Details and Common design smells
  - book: isbn:9780128013977
    title: Girish Suryanarayana, Ganesh Samarthyam and Tushar Sharma, Refactoring for Software Design Smells, Managing Technical Debt, Morgan Kaufmann, 2014
    locator: the catalogue of design smells, grouped by design principle
  - url: https://go.dev/ref/spec
    title: The Go Programming Language Specification
    locator: Import declarations and Exported identifiers
  - url: https://martinfowler.com/bliki/CodeSmell.html
    title: Martin Fowler, Code Smell, bliki, 9 February 2006
    locator: the definition
---

## Idea

A [[code-smell|code smell]] is a sign in a few lines of code. A design smell is the same kind of sign one level up: in how the classes, modules and packages of a program divide the work and depend on each other. Wikipedia defines a design smell as "a structure in a design that indicates a violation of fundamental design principles, and which can negatively impact the project's quality."[^1]

Two classes that each know about the other, a type that stands for three ideas at once, a concept with no type at all, a subclass that is not a kind of its parent: each of these makes some changes hard, and none of them shows in any one function.

## How it works

The catalogue that Wikipedia follows comes from the book *Refactoring for Software Design Smells* by Suryanarayana, Samarthyam and Sharma.[^1][^2] It names each smell by the design principle that the smell breaks. The four principles are abstraction, encapsulation, modularization and hierarchy. A selection from Wikipedia's list:[^1]

| Principle | Smell | What it is |
|---|---|---|
| Abstraction | Missing abstraction | "clumps of data or encoded strings are used instead of creating an abstraction" |
| Abstraction | Multifaceted abstraction | "an abstraction has multiple responsibilities assigned to it" |
| Abstraction | Duplicate abstraction | two or more abstractions have the same name, the same implementation, or both |
| Encapsulation | Deficient encapsulation | a member is more accessible than it needs to be |
| Encapsulation | Unexploited encapsulation | client code checks the type of an object in place of using the variation in a hierarchy |
| Modularization | Broken modularization | data and methods that belong together are spread over several abstractions |
| Modularization | Cyclically dependent modularization | two or more abstractions depend on each other, directly or indirectly |
| Hierarchy | Cyclic hierarchy | "a supertype in a hierarchy depends on any of its subtypes" |
| Hierarchy | Broken hierarchy | a supertype and its subtype do not share an is-a relationship |

Many of these are code smells seen at the scale of the design. Wikipedia notes that missing abstraction is also known as primitive obsession and data clumps, and that duplicate abstraction is also known as alternative classes with different interfaces, which are the names of three smells in *Refactoring*.[^1] See [[primitive-obsession|Primitive Obsession]], [[data-clumps|Data Clumps]] and [[alternative-classes-with-different-interfaces|Alternative Classes with Different Interfaces]]. Unexploited encapsulation is the design form of [[repeated-switches|Repeated Switches]] and [[downcasting-smell|Downcasting]], and broken hierarchy is the design form of [[refused-bequest|Refused Bequest]].

Some design smells can be found by a program. A cycle between modules is a cycle in the graph of their imports, and a search of the graph finds it:

```javascript run id=find-the-cycle
import assert from "node:assert/strict";

// Each module and the modules it imports.
const imports = {
  orders: ["customers", "pricing"],
  customers: ["orders"], // customers needs orders, and orders needs customers
  pricing: ["catalog"],
  catalog: [],
};

// A depth-first search that returns the first cycle it finds, or null.
function findCycle(graph) {
  const state = new Map(); // undefined: not seen, 1: on the path, 2: done
  const path = [];
  function visit(node) {
    state.set(node, 1);
    path.push(node);
    for (const next of graph[node]) {
      if (state.get(next) === 1) return [...path.slice(path.indexOf(next)), next];
      if (state.get(next) === undefined) {
        const found = visit(next);
        if (found) return found;
      }
    }
    path.pop();
    state.set(node, 2);
    return null;
  }
  for (const node of Object.keys(graph)) {
    if (state.get(node) === undefined) {
      const found = visit(node);
      if (found) return found;
    }
  }
  return null;
}

assert.deepEqual(findCycle(imports), ["orders", "customers", "orders"]);

// The fix: move what both need into a module that depends on neither.
const fixed = { ...imports, customers: ["accounts"], orders: ["customers", "pricing", "accounts"], accounts: [] };
assert.equal(findCycle(fixed), null);
console.log("cycle check: tests pass");
```

```output
cycle check: tests pass
```

Other design smells need judgement. Whether an abstraction has one responsibility or three, or whether a subclass is truly a kind of its parent, depends on what the program means, which no graph shows.

## In languages

The language decides which design smells the compiler refuses and which only a review finds.

- **Go.** The specification makes a cycle of packages an error: "It is illegal for a package to import itself, directly or indirectly".[^3] Cyclically dependent modularization between packages cannot compile, but a cycle between types in one package can.
- **Go, encapsulation.** An identifier "may be exported to permit access to it from another package", and it is exported if its name starts with an uppercase letter.[^3] Deficient encapsulation in Go is a field or a function with an uppercase name that no other package needs.
- **Java, C#, JavaScript and Python.** These allow cycles between classes, and between modules within limits. A tool or a review must find them.
- **Languages with sum types and checked pattern matching.** A type test over a closed set of cases is a design choice there, not unexploited encapsulation. See [[sum-types|Sum types]].

## Variations and related ideas

**Code smell.** The smaller scale. Fowler describes a code smell as "a surface indication that usually corresponds to a deeper problem in the system."[^4] A design smell is often that deeper problem.

**Architecture smell.** The same idea at the scale of services and layers: a layer that calls the layer above it, or two services that must always be deployed together.

**Anti-pattern.** A named, recurring bad solution, with a story of why it is chosen. A design smell is a sign; an anti-pattern is a diagnosis.

**Technical debt.** Wikipedia notes that design smells indicate "the accumulated design debt (one of the prominent dimensions of technical debt)."[^1]

## Pitfalls

**Treating every smell as a defect.** Wikipedia notes that the context "plays an important role to decide whether a certain structure or decision should be considered as a design smell", and that "Generally, it is appropriate to live with design smells due to constraints imposed by the context."[^1] A cycle between two small classes that always change together may cost less than the extra module that breaks it.

**Counting in place of reading.** A tool can count the methods of a class or the edges of a graph. It cannot say whether the class has one idea. A list of tool warnings is a list of places to read, not a list of things to fix.

**Mixing smells with bugs.** Wikipedia is explicit: "Bugs or unimplemented features are not accounted as design smells."[^1] A design smell makes the program harder to change; it does not make it wrong today.

## Cost

A design smell costs more to fix than a code smell, because the fix moves responsibilities between classes or modules and changes the code that uses them. Breaking a cycle between two modules may touch every file that imports either one. The cost of leaving it grows in the same way: each module added on top of a cycle depends on both sides of it.

For this reason, Wikipedia adds that design smells "should be tracked and managed as technical debt because they degrade the overall system quality over time."[^1] Record them, decide which to live with, and fix the others in small steps, each one a refactoring that keeps the tests passing.

## History

The term comes from code smell, which Kent Beck coined while helping Martin Fowler with *Refactoring*.[^4] Wikipedia traces the origin of the term design smell to that book.[^1] Different authors then defined the word in their own ways; Wikipedia quotes Moha and others: "Code and design smells are poor solutions to recurring implementation and design problems."[^1]

In 2014, Suryanarayana, Samarthyam and Sharma published *Refactoring for Software Design Smells*, which names a catalogue of design smells, groups them by the design principle that each one breaks, and gives the refactorings for each.[^2]

## Theory

A design is a graph: the nodes are modules or types, and the edges are dependencies, calls, inheritance and the sharing of data. Several design smells are properties of this graph. Cyclically dependent modularization is a cycle. A cyclic hierarchy is an edge from a supertype to one of its subtypes. A hub, a module that many others depend on and that depends on many others, is a node with a high degree in both directions.

These properties can be computed, which is why tools find them. A depth-first search finds a cycle, as the example above does. The strongly connected components of the graph give all the cycles at once, as groups of modules that depend on each other.

The other smells are about meaning, not structure. Whether two abstractions duplicate each other, or a hierarchy is broken, depends on what the types stand for. These need a reader.

## Exercises

1. Draw the import graph of a project that you know, with one node for each package or directory. Find one cycle, and say which module you would move a piece of code into to break it.
2. Find a class in a codebase that you know whose name has "and" or "Manager" in it. List the responsibilities it has. Is it a multifaceted abstraction, and what would the parts be called?
3. Take the `imports` graph of the example, add a module `reports` that imports `orders` and is imported by `catalog`, and run `findCycle`. Which cycle does it find, and why that one first?

## References
