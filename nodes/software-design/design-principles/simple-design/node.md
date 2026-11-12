---
tag: Z1545A
kind: practice
slug: simple-design
name: Simple design
aliases: [Four rules of simple design, Beck design rules]
area: software-design/design-principles
level: core
summary: >-
  Kent Beck's four rules for a simple design, in priority order: the code passes the tests, reveals intention, has no duplication, and has the fewest elements.
history:
  year: 1999
  people: [Kent Beck]
  note: Beck formed the rules while he developed extreme programming in the late 1990s; the first edition of Extreme Programming Explained states them on page 57.
see_also: [you-arent-gonna-need-it, dont-repeat-yourself, keep-it-simple, duplicated-code]
sources:
  - url: https://martinfowler.com/bliki/BeckDesignRules.html
    title: Martin Fowler, Beck Design Rules, 2 March 2015
    locator: the whole post and note 1
  - url: https://martinfowler.com/bliki/Yagni.html
    title: Martin Fowler, Yagni, 26 May 2015
    locator: the second paragraph
---

## Statement

Kent Beck gave four rules of simple design while he developed extreme programming (XP) in the late 1990s. Martin Fowler expresses them like this:[^1]

1. Passes the tests.
2. Reveals intention.
3. No duplication.
4. Fewest elements.

"The rules are in priority order, so passes the tests takes priority over reveals intention".[^1]

## Meaning

Each rule is a test that a programmer can apply to the code now.

- **Passes the tests.** Fowler calls this the most important rule: "whatever else you do with the software, the primary aim is that it works as intended and tests are there to ensure that happens."[^1]
- **Reveals intention.** The code is easy to understand. Names and structure show what the writer meant, so that a reader can see the purpose.
- **No duplication.** Everything is said once and only once. This is the same idea as [[dont-repeat-yourself|Don't repeat yourself]]. Beck's own wording warns of "hidden duplication like parallel class hierarchies".[^1]
- **Fewest elements.** The last rule says that "anything that doesn't serve the three prior rules should be removed."[^1] When Beck formed the rules, much design advice said to add elements to an architecture for flexibility in the future. Fowler notes that the extra complexity usually made the system harder to change.[^1]

The rules are part of the XP practice of simple design. Fowler writes that [[you-arent-gonna-need-it|YAGNI]] is a way to refer to the same practice.[^2]

## Examples

The first version passes its test, but the names do not reveal intention, the discount rule is written twice, and a class exists only to hold one constant. Each step applies one rule and keeps the test green.

```javascript run id=four-rules
import assert from "node:assert/strict";

// Version 1: passes the test, nothing else.
class Config { static RATE = 0.1; }
function calc(a, m) {
  if (m) return a - a * Config.RATE;
  if (a > 100) return a - a * Config.RATE;
  return a;
}

// Version 2: reveals intention, no duplication, fewest elements.
const MEMBER_DISCOUNT = 0.1;
function priceAfterDiscount(amount, isMember) {
  const qualifies = isMember || amount > 100;
  return qualifies ? amount * (1 - MEMBER_DISCOUNT) : amount;
}

// Rule 1 stays first: both versions must pass the same tests.
for (const [amount, member] of [[50, false], [50, true], [200, false]]) {
  assert.equal(priceAfterDiscount(amount, member), calc(amount, member));
}
console.log(priceAfterDiscount(50, false), priceAfterDiscount(50, true), priceAfterDiscount(200, false));
```

```output
50 45 180
```

| Language | What the example shows |
|---|---|
| JavaScript | the two versions and one set of tests, as above |
| Python | the second version, checked with assert |
| Go | the second version, checked in main |

## Criticism and limits

- **Clarity and duplication can conflict.** People often argue about the order of rules 2 and 3. Fowler sees the order as unimportant, because the two rules feed each other.[^1] Beck said: "In the rare case they are in conflict (in tests are the only examples I can recall), empathy wins over some strictly technical metric."[^1]
- **Many wordings.** Beck stated the rules in many places, and other authors phrase them their own way. The first edition of Extreme Programming Explained has another list on page 109 that omits the tests and splits the last rule in two.[^1]
- **Not a full measure of quality.** Beck wrote that the real criterion, "minimizes cost (including the cost of delay) and maximizes benefit over the lifetime of the software," can only be evaluated afterwards. He said of the four rules: "These criteria aren't perfect, but they serve to sort out some of the obvious crap and (importantly) you can evaluate them right now."[^1]

## History

Beck formed the rules while he developed XP in the late 1990s. Fowler says the most authoritative form is on page 57 of the first edition of Extreme Programming Explained, in the section on the practice of simple design.[^1] The rules were first discussed and refined on Ward's Wiki.[^1] Fowler wrote his summary in 2015.[^1]

## References
