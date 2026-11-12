---
tag: D35XSR
kind: technique
slug: replace-parameter-with-query
name: Replace parameter with query
aliases: [replace parameter with method]
area: construction-practice/refactoring-apis
level: core
summary: >-
  Remove a parameter whose value the function can get by itself, for example from another parameter, and let the function compute it.
family: refactoring
polarity: positive
intent: "Do not make the caller pass a value that the function can find."
also_known_as: [Replace Parameter with Method]
history:
  year: 1999
  people: [Martin Fowler]
  note: The first edition of Refactoring (1999) has it as Replace Parameter with Method. The second edition (2018) uses the name Replace Parameter with Query, in the chapter on refactoring APIs, and adds the inverse, Replace Query with Parameter.
see_also: [replace-query-with-parameter, long-parameter-list, preserve-whole-object, replace-temp-with-query, change-function-declaration, global-data]
sources:
  - url: https://refactoring.com/catalog/replaceParameterWithQuery.html
    title: Martin Fowler, Catalog of Refactorings, Replace Parameter with Query
    locator: the entry, the availableVacation sketch; inverse of Replace Query with Parameter; aliases Replace Parameter with Method
  - url: https://refactoring.com/catalog/
    title: Martin Fowler, Catalog of Refactorings, index
    locator: the entries replaceParameterWithQuery and replaceQueryWithParameter, their books and tags
  - book: isbn:9780134757599
    title: Martin Fowler, Refactoring, Improving the Design of Existing Code, second edition, Addison-Wesley, 2018
    locator: Chapter 11, Refactoring APIs, Replace Parameter with Query
  - url: https://www.jetbrains.com/help/idea/change-signature.html
    title: JetBrains, IntelliJ IDEA documentation, Change signature
    locator: the first paragraphs
---

## Purpose

A parameter list is a part of the contract between a function and its callers. Each parameter is a value that each caller must find. When the function can get the value itself, the parameter is duplication: the callers and the function compute the same thing.

Replace parameter with query removes the parameter. The function gets the value from its other parameters or from its own object. The catalogue card turns `availableVacation(anEmployee, anEmployee.grade)` into `availableVacation(anEmployee)`, and the function reads `anEmployee.grade` itself.[^1]

The card names the inverse, Replace Query with Parameter, and the old name of the refactoring, Replace Parameter with Method.[^1]

## Steps

1. If needed, use Extract Function on the computation of the parameter value, so that the function has a query to call.
2. In the body of the function, replace each use of the parameter with the query.
3. Test.
4. Remove the parameter with Change Function Declaration, and remove the argument at each call. See [[change-function-declaration|Change Function Declaration]].

## Example

`availableVacation` takes an employee and the grade of the employee. Each caller reads the grade from the employee that it already passes.

```javascript run id=query-not-parameter
import assert from "node:assert/strict";

const ana = { name: "Ana", grade: 3 };

function availableVacationBefore(anEmployee, grade) {
  return 20 + 2 * grade;
}

function availableVacation(anEmployee) {
  const grade = anEmployee.grade;
  return 20 + 2 * grade;
}

assert.equal(availableVacation(ana), availableVacationBefore(ana, ana.grade));
// The old signature let a caller pass a grade that is not the grade of the employee.
assert.notEqual(availableVacationBefore(ana, 5), availableVacation(ana));
console.log(availableVacation(ana));
```

```output
26
```

The vacation rule (20 days and 2 for each grade) is a value made up for the example.

| Language | What the example shows |
|---|---|
| JavaScript | the grade argument removed from `availableVacation`, as above |
| Python | the same change; the grade becomes a property of the employee class |
| Go | the same change; the grade becomes a method `Grade()` on the employee type |

## When to use

- **A caller computes the argument from another argument.** The function can do the same computation, so the caller does not need to.
- **The argument can be wrong.** With the parameter, a caller can pass a grade that does not belong to the employee. Without it, the function always uses the right one.
- **A long parameter list.** Each parameter that the function can find makes the list shorter. See [[long-parameter-list|Long parameter list]].

## Pitfalls

**A new dependency.** If the query reads a global or a mutable object, the function now depends on it. The function is then harder to test and to reason about. This is the case for the inverse, [[replace-query-with-parameter|Replace Query with Parameter]], which moves the choice of value back to the caller.

**Referential transparency.** A function whose result depends only on its parameters gives the same result for the same arguments. Do not remove a parameter if the query makes the result depend on hidden state.

**The caller had a reason.** Some callers may pass a different value on purpose, for example a grade for a planned promotion. Find each caller before you remove the parameter.

## Tools that do it

IntelliJ IDEA has Change Signature. For a method, its documentation says that it "can change the method name, add, remove, reorder, and rename parameters and exceptions".[^4] It removes the parameter and the arguments at the calls. The change of the body is done by hand.

The catalogue lists Replace Parameter with Query in the first edition, the second edition and the Ruby edition, and its inverse, Replace Query with Parameter, in the second edition only. Both have the tags refactoring-apis and parameters.[^2] In the second edition it is in Chapter 11, Refactoring APIs.[^3]

## References
