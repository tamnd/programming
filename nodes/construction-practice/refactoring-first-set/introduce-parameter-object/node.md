---
tag: VAE1EY
kind: technique
slug: introduce-parameter-object
name: Introduce parameter object
aliases: [Extract Parameter Object]
area: construction-practice/refactoring-first-set
level: core
summary: >-
  Replace a group of parameters that travel together with one object that holds them, so that the group has a name and a place for the behaviour that belongs to it.
family: refactoring
polarity: positive
intent: "Give a group of values that are always passed together one name and one type."
also_known_as: [Extract Parameter Object]
history:
  year: 1999
  people: [Martin Fowler]
  note: Introduce Parameter Object is in the first edition of Refactoring (1999) and in the second edition (2018).
see_also: [data-clumps, long-parameter-list, primitive-obsession, change-function-declaration, combine-functions-into-class, code-smell]
sources:
  - url: https://refactoring.com/catalog/introduceParameterObject.html
    title: Martin Fowler, Catalog of Refactorings, Introduce Parameter Object
    locator: the entry
  - book: isbn:9780134757599
    title: Martin Fowler, Refactoring, Improving the Design of Existing Code, second edition, Addison-Wesley, 2018
    locator: Chapter 6, A First Set of Refactorings, Introduce Parameter Object, motivation and mechanics; Chapter 3, Bad Smells in Code, Long Parameter List and Data Clumps
  - book: isbn:9780201485677
    title: Martin Fowler, Refactoring, Improving the Design of Existing Code, Addison-Wesley, 1999
    locator: Chapter 10, Making Method Calls Simpler, Introduce Parameter Object
  - url: https://www.jetbrains.com/help/idea/extract-into-class-refactorings.html
    title: IntelliJ IDEA documentation, Extract into class refactorings
    locator: Extract parameter object
  - url: https://code.visualstudio.com/docs/typescript/typescript-refactoring
    title: Visual Studio Code documentation, Refactoring TypeScript and JavaScript
    locator: Convert parameters to destructured object
---

## Purpose

Some values go everywhere together: a start date and an end date, a minimum and a maximum, an x and a y. When each function takes them as separate parameters, every signature repeats the pair, every caller passes it in the right order, and the rule that joins them, such as the rule that the start is not after the end, lives nowhere.

Introduce parameter object puts the group in one structure and passes the structure. The catalogue card shows three functions, `amountInvoiced`, `amountReceived` and `amountOverdue`, that each take `(startDate, endDate)` and after the refactoring take `(aDateRange)`.[^1] The parameter lists get shorter, the names are the same in every function, and the relation between the values is explicit.[^2]

The larger gain comes after. Once the group is a type, behaviour that uses the group can move onto it: a range can answer whether it contains a value. That can change the design of the code around it, because a concept that had no name now has one.[^2] The refactoring has the same name in the first edition.[^3]

## Steps

1. Run the tests and see them pass.
2. If no suitable structure exists, create one. A class or a record type with value semantics is a good choice, because it can later hold behaviour.
3. Test.
4. Use [[change-function-declaration|Change Function Declaration]] to add a parameter for the structure.
5. Test.
6. Change each caller to pass an instance of the structure, and test after each.
7. For each value in the structure, replace the use of the old parameter in the body with the field of the structure, and remove the old parameter. Test after each.
8. Look for behaviour that uses the values together, and consider moving it onto the structure.

## Example

Functions that check meetings against office hours take the hours as two numbers, the minutes from midnight. After the refactoring they take a window object, and the check itself moves onto the window.

```javascript run id=name-the-group
import assert from "node:assert/strict";

const meetings = [
  { title: "standup", start: 8 * 60 },
  { title: "review", start: 10 * 60 },
  { title: "planning", start: 16 * 60 },
  { title: "retro", start: 18 * 60 },
];

// Before: the pair (open, close) travels as two parameters.
function outsideHoursBefore(list, open, close) {
  return list.filter((m) => m.start < open || m.start >= close);
}

// After: one value with a name, a rule and a behaviour.
class TimeWindow {
  #open;
  #close;
  constructor(open, close) {
    if (open >= close) throw new RangeError(`open ${open} is not before close ${close}`);
    this.#open = open;
    this.#close = close;
  }
  get open() { return this.#open; }
  get close() { return this.#close; }
  includes(minute) { return minute >= this.#open && minute < this.#close; }
}

function outsideHours(list, hours) {
  return list.filter((m) => !hours.includes(m.start));
}

const office = new TimeWindow(9 * 60, 17 * 60);
assert.deepEqual(outsideHours(meetings, office), outsideHoursBefore(meetings, 9 * 60, 17 * 60));
assert.deepEqual(outsideHours(meetings, office).map((m) => m.title), ["standup", "retro"]);

// The rule that joins the values now has one home.
assert.throws(() => new TimeWindow(17 * 60, 9 * 60), RangeError);
console.log("introduce parameter object: tests pass");
```

```output
introduce parameter object: tests pass
```

Before the refactoring, a caller could pass the two numbers in the wrong order and get every meeting back as outside the hours, with no error. After it, the window checks its own rule once, at the place where it is made.

| Language | What the example shows |
|---|---|
| JavaScript | a window class with a check and an `includes` method, as above |
| Python | a frozen dataclass for the window |
| Go | a struct with a constructor that returns an error, and a `Contains` method |

## When to use

- **The same group of parameters appears in several functions.** This is the smell [[data-clumps|Data Clumps]], and the book names this refactoring as one of its fixes.[^2]
- **A parameter list is long.** Grouping values that belong together is one fix for [[long-parameter-list|Long Parameter List]].[^2]
- **The values obey a rule together.** A start before an end, a total that is the sum of parts. A type can check the rule once.
- **Primitive values stand for a concept.** A range, a money amount or a coordinate as bare numbers is a case of [[primitive-obsession|Primitive Obsession]].

## Pitfalls

**A bag with no meaning.** Putting unrelated parameters into one object, such as `options` with ten fields that no two functions use the same way, hides the parameter list without making it clearer. Group the values that belong together, not all of them.

**Mutable parameter objects.** If the callee can change the object, a caller that reuses it sees the change. Make the object immutable, as a value, when it stands for a value.

**Optional fields.** A structure with many optional fields can make each function check which fields are set. A separate type for each meaningful group is often clearer.

**Order of the refactoring.** Change the callers before you remove the old parameters. If you remove them first, the program does not build until every caller changes, and you lose the small steps.

## Tools that do it

| Tool | Languages | What it automates |
|---|---|---|
| IntelliJ IDEA | Java | Extract Parameter Object, which lets you "select a set of parameters to a method, and either create a wrapper class for those parameters, or use an existing compatible wrapper class"; the calls are wrapped and the uses are replaced[^4] |
| VS Code | TypeScript, JavaScript | "Convert parameters to destructured object", which rewrites "a function that takes a long list of arguments to take a single arguments object"[^5] |

The VS Code action makes one object for all the parameters, which is a different result from a type for a group that belongs together. It is a step towards the refactoring, not the whole of it.

## References
