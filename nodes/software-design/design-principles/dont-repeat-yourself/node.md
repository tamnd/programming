---
tag: 3RKB1Q
kind: practice
slug: dont-repeat-yourself
name: Don't repeat yourself
aliases: [DRY]
area: software-design/design-principles
level: core
summary: >-
  Give each piece of knowledge in a system one authoritative representation, so that a change to that knowledge is made in one place.
history:
  year: 1999
  people: [Andy Hunt, Dave Thomas]
  note: Hunt and Thomas stated the principle in the book The Pragmatic Programmer, published by Addison-Wesley in 1999.
see_also: [separation-of-concerns, duplicated-code, speculative-generality]
sources:
  - url: https://www.artima.com/articles/orthogonality-and-the-dry-principle
    title: Bill Venners, Orthogonality and the DRY Principle, a conversation with Andy Hunt and Dave Thomas, Artima, 2003
    locator: the sections on DRY and on code generators
  - url: https://en.wikipedia.org/wiki/Don%27t_repeat_yourself
    title: Wikipedia, Don't repeat yourself
    locator: the lead and the sections Single choice principle and Alternatives
  - url: https://sandimetz.com/blog/2016/1/20/the-wrong-abstraction
    title: Sandi Metz, The Wrong Abstraction, 2016
    locator: the whole post
---

## Statement

"Every piece of knowledge must have a single, unambiguous, authoritative representation within a system."[^2] Andy Hunt and Dave Thomas gave this rule in The Pragmatic Programmer.

## Meaning

The rule is about knowledge, not about text. Dave Thomas said: "Most people take DRY to mean you shouldn't duplicate code. That's not its intention. The idea behind DRY is far grander than that."[^1]

The knowledge of a system is more than its code. Thomas said that it "refers to database schemas, test plans, the build system, even documentation."[^1] When the same fact has two representations, "at some point the two or three different representations will most likely fall out of step with each other."[^1]

The rule asks for these actions:

- **In code, use functions and named values.** Put a rule, such as a tax rate or a validation check, in one function or constant, and call it from every place that needs it.
- **For other artifacts, generate them.** Hunt and Thomas use code generators, automatic build systems and scripting languages. These let a team keep "single, authoritative representations that then generate non-authoritative work products, like code or DDLs".[^1]
- **Do not merge text that is not the same knowledge.** Two lines can look the same but state two different facts. If the facts can change for different reasons, they are two pieces of knowledge, and the rule does not ask for one function.

A special case is the single choice principle of Bertrand Meyer: "Whenever a software system must support a set of alternatives, one and only one module in the system should know their exhaustive list."[^2]

## Examples

The VAT rate appears in two functions. A change to the rate must be made in both places, and it is easy to miss one. After the change, the rate is one named value.

```javascript run id=one-rate
import assert from "node:assert/strict";

// Before: the same fact in two places.
const priceWithTaxBefore = (net) => net * 1.2;
const taxOnlyBefore = (net) => net * 0.2;

// After: one authoritative representation.
const VAT_RATE = 0.2;
const taxOnly = (net) => net * VAT_RATE;
const priceWithTax = (net) => net + taxOnly(net);

assert.equal(priceWithTax(50), priceWithTaxBefore(50));
assert.equal(taxOnly(50), taxOnlyBefore(50));

// Two facts that only look the same stay apart.
const MAX_USERNAME_LENGTH = 20;
const MAX_LOGIN_ATTEMPTS = 20;
console.log(priceWithTax(50), taxOnly(50), MAX_USERNAME_LENGTH === MAX_LOGIN_ATTEMPTS);
```

```output
60 10 true
```

The two limits have the same value, 20, but they are different knowledge. A change to the length of a name must not change the number of login attempts, so they stay two constants.

| Language | What the example shows |
|---|---|
| JavaScript | one VAT constant used by two functions, as above |
| Python | the same, with a module constant |
| Go | the same, with a package constant |

## Criticism and limits

- **The wrong abstraction.** Sandi Metz wrote that "duplication is far cheaper than the wrong abstraction" and advised: "prefer duplication over the wrong abstraction".[^3] She described a pattern. A programmer extracts duplicated code into one function. Later requirements are almost the same but not quite, so other programmers "alter the code to take a parameter, and then add logic to conditionally do the right thing based on the value of that parameter." The function becomes "a condition-laden procedure which interleaves a number of vaguely associated ideas."[^3] Her fix is to inline the function back into each caller and start again.
- **The cost of the tools.** A code generator removes duplication but must itself be built and explained. Hunt said: "Creating a code generator is an investment."[^1] Thomas said that he often makes the products by hand first and builds the generator only when he comes back to the task.[^1]
- **Other names for the opposite view.** WET, often read as write everything twice, names the opposite approach. AHA, avoid hasty abstractions, says to wait until the duplication is a real problem and the shape of the abstraction is known.[^2]

See [[duplicated-code|Duplicated code]] for the code smell, and [[speculative-generality|Speculative generality]] for the risk of an abstraction made too early.

## History

Hunt and Thomas named the principle in The Pragmatic Programmer, published by Addison-Wesley in 1999.[^1] They explained it again in a 2003 interview with Bill Venners, which separated the principle from the simple rule against copied code.[^1] The term WET has been used for the opposite view since at least 2002.[^2] Metz wrote The Wrong Abstraction in 2016.[^3]

## References
