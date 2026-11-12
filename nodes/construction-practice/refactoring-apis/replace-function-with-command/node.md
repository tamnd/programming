---
tag: 4QMAXW
kind: technique
slug: replace-function-with-command
name: Replace function with command
aliases: [replace method with method object]
area: construction-practice/refactoring-apis
level: core
summary: >-
  Turn a long or complex function into an object whose fields hold the arguments and the temporary values, and whose one method runs the work.
family: refactoring
polarity: positive
intent: "Give a complex function room to be split up."
also_known_as: [Replace Method with Method Object]
history:
  year: 1999
  people: [Martin Fowler]
  note: The first edition of Refactoring (1999) has it as Replace Method with Method Object. The second edition (2018) calls it Replace Function with Command, in the chapter on refactoring APIs, and adds the inverse, Replace Command with Function.
see_also: [replace-command-with-function, command-pattern, extract-function, long-function, replace-temp-with-query]
sources:
  - url: https://refactoring.com/catalog/replaceFunctionWithCommand.html
    title: Martin Fowler, Catalog of Refactorings, Replace Function with Command
    locator: the entry, the score and Scorer sketch; inverse of Replace Command with Function; aliases Replace Method with Method Object
  - url: https://refactoring.com/catalog/
    title: Martin Fowler, Catalog of Refactorings, index
    locator: the entries replaceFunctionWithCommand and replaceCommandWithFunction, their books and tags
  - book: isbn:9780134757599
    title: Martin Fowler, Refactoring, Improving the Design of Existing Code, second edition, Addison-Wesley, 2018
    locator: Chapter 11, Refactoring APIs, Replace Function with Command
---

## Purpose

A long function with many local variables is hard to split. Each piece that Extract Function takes out needs the locals as parameters, and the new functions get long parameter lists.

Replace function with command moves the function into its own class. The arguments become fields, set in the constructor. The body moves to one method, often called `execute`. The local variables can then become fields too, and each piece of the body can become a method with no parameters. The catalogue card turns `function score(candidate, medicalExam, scoringGuide)` into `class Scorer`, with the three arguments as fields, and `result` and `healthLevel` as fields that `execute` sets.[^1]

The object is a command in the sense of the [[command-pattern|Command pattern]]: an object that holds one request. The card names the old name, Replace Method with Method Object, and the inverse, Replace Command with Function.[^1]

## Steps

1. Make an empty class. Name it after the function.
2. Move the function into the class as one method, `execute`. Keep the old function and make it call the class, so that the callers do not change.
3. Make a field for each argument, and set it in the constructor. Change `execute` to read the fields.
4. Test.
5. Make the local variables fields, one at a time, and test after each one.
6. Now use Extract Function on the body. The new methods read the fields, so they need no parameters. See [[extract-function|Extract Function]].

## Example

`score` gives points to a candidate. The body has three parts, and each one changes the local `result`. As a command, each part becomes a method.

```javascript run id=function-to-command
import assert from "node:assert/strict";

const candidate = { originState: "CA" };
const medicalExam = { isSmoker: true };
const scoringGuide = { stateWithLowCertification: (s) => s === "CA" };

function score(candidate, medicalExam, scoringGuide) {
  let result = 0;
  let healthLevel = 0;
  if (medicalExam.isSmoker) { healthLevel += 10; }
  let certificationGrade = "regular";
  if (scoringGuide.stateWithLowCertification(candidate.originState)) {
    certificationGrade = "low";
    result -= 5;
  }
  result -= Math.max(healthLevel - 5, 0);
  return { result, certificationGrade };
}

class Scorer {
  constructor(candidate, medicalExam, scoringGuide) {
    this._candidate = candidate;
    this._medicalExam = medicalExam;
    this._scoringGuide = scoringGuide;
  }
  execute() {
    this._result = 0;
    this._healthLevel = 0;
    this.scoreSmoking();
    this.scoreCertification();
    this._result -= Math.max(this._healthLevel - 5, 0);
    return { result: this._result, certificationGrade: this._certificationGrade };
  }
  scoreSmoking() {
    if (this._medicalExam.isSmoker) { this._healthLevel += 10; }
  }
  scoreCertification() {
    this._certificationGrade = "regular";
    if (this._scoringGuide.stateWithLowCertification(this._candidate.originState)) {
      this._certificationGrade = "low";
      this._result -= 5;
    }
  }
}

const after = new Scorer(candidate, medicalExam, scoringGuide).execute();
assert.deepEqual(after, score(candidate, medicalExam, scoringGuide));
console.log(after);
```

```output
{ result: -10, certificationGrade: 'low' }
```

The rules of the score are made up for the example. The shape follows the catalogue card.[^1]

| Language | What the example shows |
|---|---|
| JavaScript | `score` turned into `class Scorer` with `execute`, as above |
| Python | the same class; a nested function with `nonlocal` is shown as the lighter way to share the locals |
| Go | a `scorer` struct with an `execute` method; the helper methods take a pointer receiver because they change the fields |

## When to use

- **A long function that is hard to split.** The locals become fields, so the extracted pieces need no parameters. See [[long-function|Long function]].
- **The call needs more than a call.** A command object can be stored, put in a queue, logged, or undone. It can also have methods to set options before it runs.
- **Subclasses must change one step.** A command can have a subclass that overrides one of its methods.

## Pitfalls

**More code.** A class with a constructor, fields and `execute` is longer than a function. Use the refactoring only when the function needs it. Most functions do not.

**Fields that live too long.** The temporary values now live as long as the object. If the same object runs twice, an old value can stay in a field. Set each temporary field at the start of `execute`, as the example does.

**First-class functions.** In a language with closures, a nested function can often share the locals with less code. The refactoring is most useful in languages without them, or when the command must be stored or changed.

## Tools that do it

IDEs do not have one action for this refactoring under this name. The steps use Extract Function, Change Function Declaration and the field and variable refactorings of the IDE.

The catalogue lists Replace Function with Command in the first edition, the second edition and the Ruby edition, and its inverse in the second edition only. Both have the tag refactoring-apis.[^2] In the second edition it is in Chapter 11, Refactoring APIs.[^3]

## References
