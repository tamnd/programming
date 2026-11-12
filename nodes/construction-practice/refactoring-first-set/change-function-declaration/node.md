---
tag: NRRZP1
kind: technique
slug: change-function-declaration
name: Change function declaration
aliases: [Rename Function, Rename Method, Add Parameter, Remove Parameter, Change Signature]
area: construction-practice/refactoring-first-set
level: core
summary: >-
  Change the name or the parameters of a function and update every caller, in one step when the callers are few and in stages when they are not.
family: refactoring
polarity: positive
intent: "Give a function the name and the parameters that say what it does and what it needs."
also_known_as: [Rename Function, Rename Method, Add Parameter, Remove Parameter, Change Signature]
history:
  year: 2018
  people: [Martin Fowler]
  note: The first edition of Refactoring (1999) had separate refactorings for Rename Method, Add Parameter and Remove Parameter. The second edition (2018) joins them as Change Function Declaration.
see_also: [extract-function, inline-function, mysterious-name, code-smell]
sources:
  - book: isbn:9780134757599
    title: Martin Fowler, Refactoring, Improving the Design of Existing Code, second edition, Addison-Wesley, 2018
    locator: Chapter 6, A First Set of Refactorings, Change Function Declaration, motivation and mechanics; Chapter 3, Bad Smells in Code, Mysterious Name
  - url: https://refactoring.com/catalog/changeFunctionDeclaration.html
    title: Martin Fowler, Catalog of Refactorings, Change Function Declaration
    locator: the entry; aliases Rename Function, Rename Method, Add Parameter, Remove Parameter, Change Signature
  - book: isbn:9780201485677
    title: Martin Fowler, Refactoring, Improving the Design of Existing Code, Addison-Wesley, 1999
    locator: Chapter 10, Making Method Calls Simpler, Rename Method, Add Parameter and Remove Parameter
  - url: https://go.dev/gopls/features/transformation
    title: gopls documentation, Code transformation features
    locator: Remove unused parameter, and Move function parameters
  - url: https://www.jetbrains.com/help/idea/change-signature.html
    title: IntelliJ IDEA documentation, Change signature
    locator: the introduction and the section on default values
  - url: https://code.visualstudio.com/docs/typescript/typescript-refactoring
    title: Visual Studio Code documentation, Refactoring TypeScript and JavaScript
    locator: Rename, and Convert parameters to destructured object
---

## Purpose

A function declaration is the joint where the rest of the program plugs in. The name tells a caller what the function does, and the parameters tell it what the function needs. When either is wrong, every call site carries the error. A function named `circum` makes each reader decode the name; a function that takes a whole `customer` when it needs only an address ties every caller to the customer type.

Change function declaration fixes the name, adds a parameter, removes one or changes what a parameter is, and updates each caller to match.[^1] The catalogue card shows the rename of `circum(radius)` to `circumference(radius)`, and lists the aliases Rename Function, Rename Method, Add Parameter, Remove Parameter and Change Signature.[^2] The first edition had separate refactorings for the rename and for each kind of parameter change.[^3]

The refactoring changes the interface and keeps the behaviour. A good name is often found only after the code is in use, so the refactoring is also how a team writes down what it has learned about a function.

## Steps

The second edition gives two ways to make the change.[^1]

**In one step**, when every caller can be found and changed at the same time:

1. Run the tests and see them pass.
2. If you remove a parameter, check that the body does not use it.
3. Change the declaration to the one you want.
4. Find each reference to the old declaration and update it.
5. Test.

**In stages (migration)**, when the callers are many, are hard to find, or are in code that you do not own:

1. If needed, refactor the body so that the next step is clean.
2. Use [[extract-function|Extract Function]] on the body to make a function with the declaration you want. If the name must stay the same, give it a temporary name.
3. If the change adds a parameter, decide what the old function passes for it.
4. Test.
5. Use [[inline-function|Inline Function]] on the old function, one caller at a time, testing after each, until it has no callers. Then delete it.
6. If the extracted function had a temporary name, rename it with the one-step way.

If the old function is part of a published interface, step 5 may never finish. Then mark the old function as deprecated and keep it as a call to the extracted one until its callers are gone.

## Example

A function has a short name and takes a whole object when it needs one field. The migration keeps the old function as a thin wrapper while the callers move.

```javascript run id=rename-in-stages
import assert from "node:assert/strict";

// Before: a cryptic name, and a parameter wider than the need.
function circum(circle) {
  return 2 * Math.PI * circle.radius;
}

// Step 2: the body moves to a function with the declaration we want.
function circumference(radius) {
  return 2 * Math.PI * radius;
}

// Step 3: the old function becomes a wrapper and passes the field.
/** @deprecated use circumference(radius) */
function circumOld(circle) {
  return circumference(circle.radius);
}

const circles = [{ radius: 0 }, { radius: 1 }, { radius: 2.5 }];
for (const c of circles) {
  assert.equal(circumOld(c), circum(c)); // the wrapper keeps the behaviour
  assert.equal(circumference(c.radius), circum(c)); // a caller moved over
}

// Adding a parameter: a default value keeps the old calls valid.
function price(quantity, unitPrice, discount = 0) {
  return quantity * unitPrice * (1 - discount);
}
assert.equal(price(3, 10), 30); // an old call, unchanged
assert.equal(price(3, 10, 0.5), 15); // a call that passes the discount
console.log("change function declaration: tests pass");
```

```output
change function declaration: tests pass
```

Each step leaves the program working. A caller that still uses the old function gets the same result, and a caller that has moved uses the clearer declaration. The default value in `price` is the same idea for an added parameter: the old calls stay valid while the callers that need the parameter pass it.

| Language | What the example shows |
|---|---|
| JavaScript | a rename in stages and a parameter with a default, as above |
| Python | the same rename, and a keyword argument with a default |
| Go | the same rename; Go has no default values, so the old function supplies the value |

## When to use

- **The name does not say what the function does.** If you must read the body to know what a call means, the name is wrong.[^1]
- **The parameters tie the function to more than it needs.** Passing one field, not the whole record, lets the function serve more callers. The opposite also holds: passing the record can be right when the function needs several of its fields. The choice depends on how the callers use the function.
- **A parameter is not used.** It makes each caller supply a value for nothing.
- **Before other refactorings.** A function that will move to another module or class often needs its parameters changed first.
- **To treat a smell.** The rename is the first fix for [[mysterious-name|Mysterious Name]].[^1]

## Pitfalls

**Callers that the tools cannot see.** A rename by a tool finds the calls that the tool can resolve. Calls by reflection, by a string name, from another repository or from a client of a published API are not found. Use the migration way for these.

**Overloading and dispatch.** In a language with overloading or with methods that override, a rename that changes one declaration and not the others can change which function a call reaches. Change the whole family of declarations together.

**A removed argument with a side effect.** If a caller passes an expression that does something, deleting the argument deletes the effect. gopls takes care of this: its remove-parameter fix preserves "the behavior of all existing calls" and does not delete an argument such as `chargeCreditCard()` that may have side effects.[^4]

**Defaults that hide a wrong call.** A default value keeps the old calls valid, which is the point during a migration, and also hides a caller that should have passed a value. Remove the default when the migration is done if every caller should choose.

## Tools that do it

| Tool | Languages | What it automates |
|---|---|---|
| IntelliJ IDEA | Java, Kotlin | Change Signature can "change the method name, add, remove, reorder, and rename parameters and exceptions", and it offers a default value for an added parameter.[^5] |
| gopls | Go | removes an unused parameter "using a ChangeSignature command, updating all callers", and moves a parameter left or right, "updating all callers accordingly"[^4] |
| VS Code | TypeScript, JavaScript | Rename (F2) "across your TypeScript project", and "Convert parameters to destructured object"[^6] |

A tool does the one-step way. The migration way, with a wrapper and a deprecation, is a sequence of tool steps (extract, then inline at each caller) that the programmer plans.

## References
