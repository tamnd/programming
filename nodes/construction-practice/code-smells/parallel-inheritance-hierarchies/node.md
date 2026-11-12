---
tag: R317YJ
kind: pattern
slug: parallel-inheritance-hierarchies
name: Parallel Inheritance Hierarchies
aliases: [parallel hierarchies]
area: construction-practice/code-smells
level: core
summary: >-
  A code smell: two class hierarchies mirror each other, so each subclass added to one needs a matching subclass in the other, and the two must change together.
family: code-smell
polarity: negative
intent: "Find the hierarchies whose subclasses come in pairs, and move the behaviour of one into the other, so that one hierarchy is left."
also_known_as: []
refactorings: [move-function, move-field]
history:
  year: 1999
  people: [Kent Beck, Martin Fowler]
  note: Parallel Inheritance Hierarchies is a smell of the first edition of Refactoring (1999), as a special case of Shotgun Surgery. The second edition (2018) does not list it on its own.
see_also: [code-smell, shotgun-surgery, class-inheritance, bridge-pattern, visitor-pattern, divergent-change]
sources:
  - book: isbn:9780201485677
    title: Martin Fowler, Refactoring, Improving the Design of Existing Code, Addison-Wesley, 1999
    locator: Chapter 3, Bad Smells in Code, Parallel Inheritance Hierarchies
  - book: isbn:9780134757599
    title: Martin Fowler, with Kent Beck, Refactoring, Improving the Design of Existing Code, second edition, Addison-Wesley, 2018
    locator: Chapter 3, Bad Smells in Code, Shotgun Surgery
  - url: https://refactoring.com/catalog/
    title: Martin Fowler, Catalog of Refactorings
    locator: the cards Move Function (alias Move Method) and Move Field
  - url: https://en.wikipedia.org/w/index.php?title=Bridge_pattern&oldid=1383733369
    title: Wikipedia, Bridge pattern, revision 1383733369
    locator: the introduction
---

## Intent

Find two hierarchies whose subclasses come in pairs: `Circle` and `CircleRenderer`, `Square` and `SquareRenderer`. Each time someone adds a subclass to one, they must add its twin to the other. Move the behaviour of one hierarchy into the classes of the other, until the second hierarchy has nothing left and can be deleted.

## Problem

The first edition of *Refactoring* names this smell and calls it a special case of [[shotgun-surgery|Shotgun Surgery]]: each time a programmer makes a subclass of one class, they must also make a subclass of another. It gives a sign that is quick to check: the class names in one hierarchy have the same prefixes as the names in the other.[^1]

The second edition does not list the smell on its own. The problem is still there, inside Shotgun Surgery: one change, one more kind of shape, forces edits in several classes.[^2]

The forces are these:

- Each hierarchy was a sensible split when it was made. Shapes know geometry, and renderers know a drawing format.
- The pairs are linked by a convention, not by the compiler. A missing twin shows up at run time, often as a failed lookup in a table from one class to the other.
- The two hierarchies change together, so a reader must keep both in mind to make any change.
- Some parallel hierarchies are on purpose. When both sides vary on their own, the pairing is the design, and the fix is not to merge them.

## Solution

The first edition gives the general strategy: make the instances of one hierarchy refer to the instances of the other, then use Move Method and Move Field until the hierarchy that refers disappears.[^1] The catalogue at refactoring.com has cards for both refactorings, and gives Move Method as another name for Move Function.[^3]

1. **Link the pairs:** give each object of one hierarchy a reference to its twin, or a method that finds it. The table that maps one class to the other is now in one place.
2. **Move the behaviour:** [[move-function|Move Function]] for each method of the twin, into the class that it serves.
3. **Move the data:** [[move-field|Move Field]] for each field of the twin.
4. **Delete the empty hierarchy.**

Before merging, ask whether the two sides really vary together. If there are four shapes and three output formats, and each format must draw each shape, merging gives twelve methods spread over four classes, and a fifth format means editing every shape. The bridge pattern is the design for that case. Wikipedia describes it as a pattern meant to "decouple an abstraction from its implementation so that the two can vary independently".[^4] The [[visitor-pattern|Visitor pattern]] is another design for a fixed set of classes and a growing set of operations.

## Example

Each shape has a renderer subclass that draws it as SVG, and a table maps each shape class to its renderer class. Adding a triangle needs a `Triangle` and a `TriangleRenderer`, and an entry in the table.

```javascript run id=merge-the-twins
import assert from "node:assert/strict";

// Before: a shape hierarchy and a renderer hierarchy that mirrors it.
class CircleBefore { constructor(r) { this.r = r; } }
class SquareBefore { constructor(side) { this.side = side; } }
class TriangleBefore { constructor(base, height) { Object.assign(this, { base, height }); } }
class CircleRenderer { svg(c) { return `<circle r="${c.r}"/>`; } }
class SquareRenderer { svg(s) { return `<rect width="${s.side}" height="${s.side}"/>`; } }
const renderers = new Map([[CircleBefore, new CircleRenderer()], [SquareBefore, new SquareRenderer()]]);
function svgBefore(shape) {
  const renderer = renderers.get(shape.constructor);
  if (!renderer) throw new Error(`no renderer for ${shape.constructor.name}`);
  return renderer.svg(shape);
}

// After: each shape draws itself, and the renderer hierarchy is gone.
class Circle {
  constructor(r) { this.r = r; }
  svg() { return `<circle r="${this.r}"/>`; }
}
class Square {
  constructor(side) { this.side = side; }
  svg() { return `<rect width="${this.side}" height="${this.side}"/>`; }
}
class Triangle {
  constructor(base, height) { Object.assign(this, { base, height }); }
  svg() { return `<polygon points="0,${this.height} ${this.base},${this.height} ${this.base / 2},0"/>`; }
}

assert.equal(new Circle(5).svg(), svgBefore(new CircleBefore(5)));
assert.equal(new Square(4).svg(), svgBefore(new SquareBefore(4)));

// The twin is missing for the triangle, and only the run finds out.
assert.throws(() => svgBefore(new TriangleBefore(6, 3)), /no renderer for TriangleBefore/);
assert.equal(new Triangle(6, 3).svg(), '<polygon points="0,3 6,3 3,0"/>');
console.log("svg: tests pass");
```

```output
svg: tests pass
```

In the second version a triangle is one class, with no table to update. A programmer who adds it sees that every other shape has an `svg` method, and in a typed language an interface requires it.

| Language | What the example shows |
|---|---|
| JavaScript | the renderer hierarchy moved into the shapes, as above |
| Python | the same merge, with an abstract base class that requires `svg` |
| Java | the same merge, with a sealed interface whose `permits` list names the shapes |
| Go | the same merge, with an interface that each shape type satisfies |

## Consequences

There is one hierarchy, and an added subclass is one class. The compiler or a base class can require each subclass to have the methods it needs, so a missing method is found before the program runs.

The merged classes know more. A shape now knows SVG, which mixes geometry with one output format. With one format that is a fair trade. With several formats that change for their own reasons, the merge produces [[divergent-change|Divergent Change]] in each shape, and the parallel hierarchies, or a bridge, were the better design.

The decision depends on which side grows. If shapes are added often and formats rarely, put the drawing in the shapes. If formats are added often and shapes rarely, keep a separate hierarchy per format, as a visitor does, and accept that an added shape touches each format.

## How to fix it

- **Two hierarchies whose subclasses come in pairs:** link each pair, then [[move-function|Move Function]] and [[move-field|Move Field]] from one side to the other, then delete the empty side.[^1]
- **Two hierarchies that really vary on their own:** keep them, and make the link explicit with a bridge. See [[bridge-pattern|Bridge pattern]].[^4]

## Variants and related patterns

**Shotgun Surgery.** The general smell: one change, many edits. Parallel hierarchies are the form it takes with inheritance. See [[shotgun-surgery|Shotgun Surgery]].

**Bridge pattern.** The design for two hierarchies that should vary on their own. See [[bridge-pattern|Bridge pattern]].

**Visitor pattern.** A deliberate parallel structure: one class per operation, with one method per element class. See [[visitor-pattern|Visitor pattern]].

## References
