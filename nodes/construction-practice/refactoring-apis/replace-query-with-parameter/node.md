---
tag: MJJ808
kind: technique
slug: replace-query-with-parameter
name: Replace query with parameter
area: construction-practice/refactoring-apis
level: core
summary: >-
  Move a value that a function reads from a global or another hidden source into a parameter, so that the caller supplies it.
family: refactoring
polarity: positive
intent: "Remove a hidden dependency from a function."
history:
  year: 2018
  people: [Martin Fowler]
  note: The refactoring is new in the second edition of Refactoring (2018), in the chapter on refactoring APIs. It is the inverse of Replace Parameter with Query.
see_also: [replace-parameter-with-query, global-data, mutable-data, side-effect, change-function-declaration]
sources:
  - url: https://refactoring.com/catalog/replaceQueryWithParameter.html
    title: Martin Fowler, Catalog of Refactorings, Replace Query with Parameter
    locator: the entry, the targetTemperature sketch; inverse of Replace Parameter with Query
  - url: https://refactoring.com/catalog/
    title: Martin Fowler, Catalog of Refactorings, index
    locator: the entry replaceQueryWithParameter, its books and tags
  - book: isbn:9780134757599
    title: Martin Fowler, Refactoring, Improving the Design of Existing Code, second edition, Addison-Wesley, 2018
    locator: Chapter 11, Refactoring APIs, Replace Query with Parameter
  - url: https://www.jetbrains.com/help/idea/extract-parameter.html
    title: JetBrains, IntelliJ IDEA documentation, Extract parameter
    locator: the first paragraph
---

## Purpose

A function that reads a global variable or a shared object gives a result that depends on more than its arguments. Two calls with the same arguments can give different results. A test must set the global before each call.

Replace query with parameter moves the read to the caller. The function gets the value as a parameter. The catalogue card has `targetTemperature(aPlan)`, which reads `thermostat.currentTemperature`. After the refactoring, the call is `targetTemperature(aPlan, thermostat.currentTemperature)`.[^1]

The card names the inverse, Replace Parameter with Query.[^1] The two refactorings move one decision in opposite directions: who finds the value, the function or its caller.

## Steps

1. Use Extract Variable on the query in the body, so that the body reads the value once into a local variable.
2. Use Extract Function on the rest of the body. Give the new function the local variable as a parameter.
3. Inline the variable into the call of the new function. The old function now only calls the query and the new function.
4. Inline the old function at each caller, and give the new function the old name. Test after each step.

## Example

`targetTemperature` reads the current temperature from a shared thermostat object. The result of a call depends on that object, so the test must change it.

```javascript run id=parameter-not-query
import assert from "node:assert/strict";

const thermostat = { currentTemperature: 19 };
const plan = { target: 21, maxStep: 1 };

function targetTemperatureBefore(aPlan) {
  const currentTemperature = thermostat.currentTemperature;
  return Math.min(aPlan.target, currentTemperature + aPlan.maxStep);
}

function targetTemperature(aPlan, currentTemperature) {
  return Math.min(aPlan.target, currentTemperature + aPlan.maxStep);
}

assert.equal(targetTemperature(plan, thermostat.currentTemperature), targetTemperatureBefore(plan));

// The new function needs no shared state for a test.
assert.equal(targetTemperature(plan, 20.5), 21);
assert.equal(targetTemperature(plan, 15), 16);
console.log(targetTemperature(plan, thermostat.currentTemperature));
```

```output
20
```

| Language | What the example shows |
|---|---|
| JavaScript | the read of `thermostat.currentTemperature` moved from `targetTemperature` to its caller, as above |
| Python | the same change, with a module-level thermostat object |
| Go | the same change, with a package-level variable; the new function has no access to it |

## When to use

- **A function reads a global or a shared mutable object.** See [[global-data|Global data]] and [[mutable-data|Mutable data]]. The parameter makes the dependency visible in the signature.
- **A function that must be testable alone.** With the value as a parameter, a test calls the function with any value, and no setup of shared state is necessary.
- **A function that must give the same result for the same arguments.** This property is referential transparency. It lets a caller cache the result or call the function in any order.
- **A module that must not depend on another.** The caller, not the function, then depends on the source of the value.

## Pitfalls

**Longer parameter lists.** Each moved query adds a parameter, and each caller must now find the value. If many callers read the same source, the duplication moves to them. Then [[replace-parameter-with-query|Replace Parameter with Query]] can be the better choice.

**The caller must get it right.** A caller can now pass a stale or a wrong value. If only one value is correct, the function can be safer when it reads the value itself.

**Objects.** In a class, a field of the object is not a hidden dependency in the same way. The refactoring is mostly useful for globals, singletons and objects that the function reaches through other objects.

## Tools that do it

No IDE has one action for this refactoring. The steps use Extract Variable, Extract Function, Inline Variable and Inline Function, and most IDEs can do each of these. IntelliJ IDEA also has Extract Parameter: "The Extract Parameter refactoring lets you extract a new parameter to a method."[^4] Select the query in the body, and the action makes it a parameter.

The catalogue lists the refactoring in the second edition only, with the tags refactoring-apis and parameters.[^2] In the second edition it is in Chapter 11, Refactoring APIs.[^3]

## References
