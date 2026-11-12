---
tag: 44T824
kind: practice
slug: you-arent-gonna-need-it
name: You aren't gonna need it
aliases: [YAGNI, Yagni]
area: software-design/design-principles
level: core
summary: >-
  Do not build a capability for a need that you only foresee; build it when the need is real.
history:
  year: 1999
  people: [Kent Beck, Chet Hendrickson, Ron Jeffries]
  note: The phrase comes from a conversation between Kent Beck and Chet Hendrickson on the C3 project. The first edition of Extreme Programming Explained, 1999, used it for the practice of simple design.
see_also: [keep-it-simple, speculative-generality, dont-repeat-yourself]
sources:
  - url: https://martinfowler.com/bliki/Yagni.html
    title: Martin Fowler, Yagni, 26 May 2015
    locator: the whole post and its notes
  - url: https://en.wikipedia.org/wiki/You_aren%27t_gonna_need_it
    title: Wikipedia, You aren't gonna need it
    locator: the lead and the section Context
  - book: isbn:9780201616415
    title: Kent Beck, Extreme Programming Explained, first edition, Addison-Wesley, 1999
    locator: the first edition, October 1999
---

## Statement

Ron Jeffries, a co-founder of extreme programming (XP), stated the rule: "Always implement things when you actually need them, never when you just foresee that you [will] need them."[^2]

## Meaning

Martin Fowler calls a capability that is built before it is used a presumptive feature. YAGNI says that "some capability we presume our software needs in the future should not be built now".[^1] Fowler gives four costs of a presumptive feature:

- **The cost of build.** If the need never comes, all the work to analyze, program and test the feature is lost.
- **The cost of delay.** The time spent on the presumptive feature is not spent on a feature that the users need now.
- **The cost of carry.** The extra code "adds some complexity to the software, this complexity makes it harder to modify and debug that software, thus increasing the cost of other features."[^1]
- **The cost of repair.** Even the right feature can be built wrong, because the team learns more before the feature is used.

The rule also applies to abstractions. Fowler wrote that YAGNI "doesn't mean to forego all abstractions, but it does mean any abstraction that makes it harder to understand the code for current requirements is presumed guilty."[^1]

The rule has limits that are part of its meaning:

- **It is not about code health.** YAGNI "does not apply to effort to make the software easier to modify."[^1] Refactoring, self-testing code and continuous delivery are not violations.
- **It applies only to extra complexity.** "If you do something for a future need that doesn't actually increase the complexity of the software, then there's no reason to invoke yagni."[^1]
- **It needs other practices.** YAGNI is meant to be used with continuous refactoring, automated unit tests and continuous integration. Without refactoring, it can lead to disorganized code and technical debt.[^2]

## Examples

A team needs to send a receipt by email. One design adds a channel parameter for SMS and push messages that nobody has asked for. The YAGNI design does the one job, and it can grow when a second channel is a real need.

```javascript run id=receipt-email
import assert from "node:assert/strict";

const sent = [];

// Presumptive: channels that no caller uses yet.
function sendReceiptGeneral(order, channel = "email", options = {}) {
  if (channel === "email") sent.push(`email to ${order.email}: ${order.total}`);
  else if (channel === "sms") sent.push(`sms to ${options.phone}: ${order.total}`);
  else if (channel === "push") sent.push(`push to ${options.device}: ${order.total}`);
  else throw new Error(`unknown channel ${channel}`);
}

// YAGNI: the one capability that is needed now.
function sendReceipt(order) {
  sent.push(`email to ${order.email}: ${order.total}`);
}

const order = { email: "ana@example.org", total: "12.50" };
sendReceiptGeneral(order);
sendReceipt(order);
assert.equal(sent[0], sent[1]);
console.log(sent[1]);
```

```output
email to ana@example.org: 12.50
```

The SMS and push branches have no callers and no tests that matter to a user. They must still be read and kept correct at each change of the order data.

| Language | What the example shows |
|---|---|
| JavaScript | the general function and the simple function, as above |
| Python | the simple function |
| Go | the simple function, with an Order struct |

## Criticism and limits

- **Some later changes cost more.** Fowler wrote that "there are times when applying yagni does cause a problem, and you are faced with an expensive change when an earlier change would have been much cheaper." He adds that these cases are hard to spot in advance and are easier to remember than the cases where YAGNI saved effort.[^1]
- **The code must be easy to change.** "Yagni is only a viable strategy if the code is easy to change".[^1] A team without tests or refactoring pays a high price for each later change.
- **A cheap step now can be right.** Fowler suggests imagining the refactoring that a later need would require. Sometimes this shows a step that is "easy to do now, adds minimal complexity, yet significantly reduces the later cost", such as a lookup table for error messages that makes translations easier later.[^1]

See [[speculative-generality|Speculative generality]] for the code smell that YAGNI prevents.

## History

The phrase comes from a conversation on the C3 project. Chet Hendrickson gave Kent Beck a series of capabilities that the system would soon need, and "to each one Kent replied you aren't going to need it".[^1] The rule was then discussed on Ward's Wiki. In the first edition of Extreme Programming Explained, published in 1999, YAGNI is a way to refer to the practice of simple design; the second edition uses the related idea of incremental design.[^1] It is a principle behind the XP practice "do the simplest thing that could possibly work".[^2] Fowler wrote his article on it in 2015.[^1]

## References
