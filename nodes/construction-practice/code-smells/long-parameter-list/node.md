---
tag: B3W15R
kind: pattern
slug: long-parameter-list
name: Long Parameter List
aliases: [too many parameters, too many arguments]
area: construction-practice/code-smells
level: core
summary: >-
  A code smell: a function takes so many parameters that a caller cannot tell which argument is which, and a swapped pair of arguments of the same type is a bug that the compiler does not catch.
family: code-smell
polarity: negative
intent: "Find the functions with more parameters than a caller can keep in mind, and pass fewer values that each mean more."
also_known_as: []
refactorings: [replace-parameter-with-query, preserve-whole-object, introduce-parameter-object, remove-flag-argument, combine-functions-into-class]
history:
  year: 1999
  people: [Kent Beck, Martin Fowler]
  note: Long Parameter List is in the first edition of Refactoring (1999) and keeps its name in the second edition (2018).
see_also: [code-smell, parameter-passing, long-function, constructors, encapsulation-and-visibility]
sources:
  - book: isbn:9780134757599
    title: Martin Fowler, with Kent Beck, Refactoring, Improving the Design of Existing Code, second edition, Addison-Wesley, 2018
    locator: Chapter 3, Bad Smells in Code, Long Parameter List
  - url: https://pylint.readthedocs.io/en/stable/user_guide/messages/refactor/too-many-positional-arguments.html
    title: Pylint documentation, too-many-positional-arguments / R0917
    locator: Description, Correct code and Additional details
  - url: https://refactoring.com/catalog/
    title: Martin Fowler, Catalog of Refactorings
    locator: the cards Replace Parameter with Query, Preserve Whole Object, Introduce Parameter Object, Remove Flag Argument and Combine Functions into Class
  - book: isbn:9780201485677
    title: Martin Fowler, Refactoring, Improving the Design of Existing Code, Addison-Wesley, 1999
    locator: Chapter 3, Bad Smells in Code, Long Parameter List, the exception for a dependency on the larger object
  - url: https://docs.python.org/3.14/tutorial/controlflow.html#special-parameters
    title: The Python Tutorial, More Control Flow Tools, Python 3.14
    locator: 4.9.3 Special parameters, 4.9.3.3 Keyword-Only Arguments and 4.9.3.5 Recap
  - url: https://pylint.readthedocs.io/en/stable/user_guide/messages/refactor/too-many-arguments.html
    title: Pylint documentation, too-many-arguments / R0913
    locator: Description and Correct code
  - url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-8.html#jls-8.10
    title: The Java Language Specification, Java SE 25 Edition
    locator: 8.10 Record Classes
  - url: https://eslint.org/docs/latest/rules/max-params
    title: ESLint documentation, Rules, max-params
    locator: Options, max (default 3)
  - url: https://pylint.readthedocs.io/en/stable/user_guide/configuration/all-options.html
    title: Pylint documentation, All configuration options
    locator: Design checker, --max-args (default 5) and --max-positional-arguments (default 5)
  - url: https://checkstyle.org/checks/sizes/parameternumber.html
    title: Checkstyle documentation, ParameterNumber
    locator: Properties, max (default 7)
---

## Intent

Find the functions that take more parameters than a caller can keep in mind, and change them so that they take fewer values, each with a clear meaning.

## Problem

A call such as `stayPrice(10, 13, 8000, 1500, 2, true)` shows six values and no names. The reader of the call must open the definition to learn that the third value is the price of a night and the fourth is the price of a breakfast. The writer of the call must get the order right. When two parameters have the same type, a swapped pair is still a valid call, and the program computes a wrong result.

The forces are these:

- *Refactoring* says that early programmers were taught to pass everything a function needs as parameters, because the alternative was global data. That advice is still sound, but the parameter lists then grow, and long lists are confusing in their own right.[^1]
- Many parameters often go together: a start and an end, an x and a y, a currency and an amount. Each pair is a concept that has no name in the code.
- A boolean parameter makes one function do two things, and the call site does not say which one. `true` means nothing at the place where it is written.
- Each added parameter changes every call. A function with many callers and a growing list of parameters is expensive to change.

The Pylint documentation for `too-many-positional-arguments` puts it in one line: "For almost all interfaces, comprehensibility suffers beyond a handful of arguments." It also names a fair exception, `rgba(1.0, 0.5, 0.3, 1.0)`, where the order follows a convention that all readers know.[^2]

## Solution

*Refactoring* gives five refactorings, each for a different cause of the long list.[^1] The catalogue at refactoring.com has a card for each.[^3]

1. **A parameter that the function can compute from another one.** [[replace-parameter-with-query|Replace Parameter with Query]] removes it, and the function asks for the value itself.
2. **Several fields taken out of one object and passed one by one.** [[preserve-whole-object|Preserve Whole Object]] passes the object.
3. **Parameters that always travel together.** [[introduce-parameter-object|Introduce Parameter Object]] makes a type for them, such as a date range.
4. **A boolean that selects the behaviour.** [[remove-flag-argument|Remove Flag Argument]] makes one function for each value of the flag.
5. **Several functions that share the same parameters.** [[combine-functions-into-class|Combine Functions into Class]] stores the shared values as fields, and the functions read them from there.

A parameter object is often the first step to a better design. Once the start and the end are one `DateRange`, the code that computes the number of nights can move into it.

## Example

The function computes the price of a hotel stay. It takes six parameters: two days, two prices, the number of guests and a flag for breakfast. The test shows that a call with the two prices swapped runs and gives a wrong price.

```javascript run id=parameter-object
import assert from "node:assert/strict";

// Before: six positional parameters.
function stayPriceBefore(startDay, endDay, nightlyRate, breakfastRate, guests, withBreakfast) {
  const nights = endDay - startDay;
  let price = nights * nightlyRate;
  if (withBreakfast) price += nights * guests * breakfastRate;
  return price;
}

// After: a date range, the room as a whole, and a stay that holds the rest.
const nights = (range) => range.end - range.start;

function stayPrice(stay, room) {
  const n = nights(stay.range);
  const breakfast = stay.breakfast ? n * stay.guests * room.breakfastRate : 0;
  return n * room.nightlyRate + breakfast;
}

const room = { nightlyRate: 8000, breakfastRate: 1500 };
const stay = { range: { start: 10, end: 13 }, guests: 2, breakfast: true };

assert.equal(stayPriceBefore(10, 13, 8000, 1500, 2, true), 33000);
assert.equal(stayPriceBefore(10, 13, 1500, 8000, 2, true), 52500); // swapped, and no error
console.log("stayPriceBefore: tests pass");

assert.equal(stayPrice(stay, room), 33000);
assert.equal(stayPrice({ ...stay, breakfast: false }, room), 24000);
console.log("stayPrice: tests pass");
```

```output
stayPriceBefore: tests pass
stayPrice: tests pass
```

Three nights at 80.00 cost 240.00, and breakfast for two guests on three mornings at 15.00 costs 90.00, so the price is 330.00. With the prices swapped, the old function returns 525.00. In the second version the caller writes `nightlyRate` and `breakfastRate` by name, once, in the room object, so that mistake has no place to happen.

| Language | What the example shows |
|---|---|
| JavaScript | the parameter objects above, as object literals |
| Python | the same objects as frozen dataclasses, and keyword-only parameters as a lighter fix |
| Java | records for the range, the room and the stay |
| Go | structs, and a composite literal with field names at the call site |

## Consequences

The calls become shorter and they name their values. The parameter objects are types that the compiler can check: in Java and Go, a `Room` cannot be passed where a `DateRange` is expected. Behaviour that belongs to the group, such as the number of nights, gets a home.

The cost is more types. A parameter object for one call is overhead, and a reader must look in two places to see what a function receives. Preserve Whole Object also adds a dependency: the function now depends on the type of the whole object, not on two numbers, so it cannot be called without one. The first edition of *Refactoring* names this as the exception to Preserve Whole Object: the case where the called code should not depend on the larger object.[^4]

Some languages reduce the problem without an added type. Python can make parameters keyword-only with a `*` in the definition, so that each call must name its arguments. The Python tutorial advises keyword-only parameters "when names have meaning and the function definition is more understandable by being explicit with names".[^5] The Pylint example for `too-many-positional-arguments` uses this fix.[^2] The example for `too-many-arguments` uses the other fix: a dataclass that groups the coordinates of a chess piece.[^6] In Java, a record is a short way to declare a parameter object with its fields, constructor and accessors.[^7]

Linters set a limit on the count. Their defaults differ widely:

| Tool | Rule | Default limit |
|---|---|---|
| ESLint | `max-params` | 3 parameters[^8] |
| Pylint | `too-many-arguments` (`max-args`) | 5 arguments[^9] |
| Pylint | `too-many-positional-arguments` (`max-positional-arguments`) | 5 positional arguments[^9] |
| Checkstyle | `ParameterNumber` | 7 parameters[^10] |

## How to fix it

- **A value the function can get for itself:** [[replace-parameter-with-query|Replace Parameter with Query]].[^1]
- **Fields pulled out of one object:** [[preserve-whole-object|Preserve Whole Object]].[^1]
- **A group that always travels together:** [[introduce-parameter-object|Introduce Parameter Object]].[^1]
- **A boolean or an enum that selects the path:** [[remove-flag-argument|Remove Flag Argument]].[^1]
- **The same values passed to many functions:** [[combine-functions-into-class|Combine Functions into Class]].[^1]
- **In Python, a list that must stay:** make the parameters keyword-only with `*`, so that each call names them.[^5]

## Variants and related patterns

**Data Clumps.** The same group of values in many parameter lists and many classes. A parameter object is the fix for both smells. See [[data-clumps|Data Clumps]].

**Flag argument.** A boolean parameter that selects one of two behaviours. See [[boolean-parameter-smell|Boolean parameter]].

**Primitive Obsession.** Six numbers in a row are often six primitives that should be two or three types. See [[primitive-obsession|Primitive Obsession]].

**Builder.** When a constructor has many optional parameters, the Builder pattern sets them one by one with named methods. See [[builder-pattern|Builder]].

**Global Data.** The old alternative to many parameters, with worse problems. See [[global-data|Global Data]].

## References
