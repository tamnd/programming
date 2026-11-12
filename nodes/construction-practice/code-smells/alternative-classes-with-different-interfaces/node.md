---
tag: QJW6R9
kind: pattern
slug: alternative-classes-with-different-interfaces
name: Alternative Classes with Different Interfaces
aliases: [alternative classes]
area: construction-practice/code-smells
level: core
summary: >-
  A code smell: two classes do the same job but have different method names and parameters, so a client cannot swap one for the other and must know which one it has.
family: code-smell
polarity: negative
intent: "Find the classes that do one job under two interfaces, and change their declarations until they match, so that a client can use either one."
also_known_as: []
refactorings: [change-function-declaration, move-function, extract-superclass]
history:
  year: 1999
  people: [Kent Beck, Martin Fowler]
  note: Alternative Classes with Different Interfaces is in the first edition of Refactoring (1999), and keeps its name in the second edition (2018).
see_also: [code-smell, interfaces, subtyping, adapter-pattern, duplicated-code, repeated-switches]
sources:
  - book: isbn:9780134757599
    title: Martin Fowler, with Kent Beck, Refactoring, Improving the Design of Existing Code, second edition, Addison-Wesley, 2018
    locator: Chapter 3, Bad Smells in Code, Alternative Classes with Different Interfaces
  - url: https://refactoring.com/catalog/
    title: Martin Fowler, Catalog of Refactorings
    locator: the cards Change Function Declaration (alias Rename Function), Move Function and Extract Superclass
  - book: isbn:9780201485677
    title: Martin Fowler, Refactoring, Improving the Design of Existing Code, Addison-Wesley, 1999
    locator: Chapter 3, Bad Smells in Code, Alternative Classes with Different Interfaces
  - url: https://en.wikipedia.org/w/index.php?title=Adapter_pattern&oldid=1340717395
    title: Wikipedia, Adapter pattern, revision 1340717395
    locator: the introduction and Overview
---

## Intent

Find two classes that do the same job in different words: one has `send(to, subject, body)`, the other has `dispatch({ phone, text })`. Rename the methods and reshape the parameters until the two classes have the same interface. Then a client can take either one, and the code that chooses between them is in one place.

## Problem

Much of the value of classes comes from substitution. A client that asks an object to do a job should not need to know which class does it. That works only when the classes answer the same messages.

*Refactoring* makes this point: one of the great benefits of using classes is that one can be substituted for another, and that benefit is lost when two classes for the same job have different interfaces.[^1] The two classes often come from different authors or different times. One was written for email, the other was added for text messages a year later, and nobody aligned them.

The forces are these:

- Each class was designed alone, and its names fit its own case. `subject` makes sense for email and not for a text message.
- The clients must branch on the class. The branch is repeated wherever both classes are used, which is close to [[repeated-switches|Repeated Switches]].
- The two classes often share code under different names, which is [[duplicated-code|Duplicated Code]] that a search does not find.
- A class that the team cannot change, such as one from a library, cannot be renamed. It needs a wrapper.

## Solution

*Refactoring* gives the moves.[^1] The catalogue at refactoring.com has a card for each refactoring, and lists Rename Function as one of the names of Change Function Declaration.[^2]

1. **Align the names and the parameters:** [[change-function-declaration|Change Function Declaration]]. Pick one name for the job and one shape for the arguments, and change each class to match.
2. **Move the behaviour into place:** [[move-function|Move Function]]. If one class does part of the job in its clients, move that part into the class, until the two classes do the same amount of work.
3. **Share what is the same:** [[extract-superclass|Extract Superclass]], if the aligned classes now hold duplicated code.

The first edition gives the same advice, with Rename Method and Move Method as the names of the first two refactorings.[^3]

When a class cannot be changed, the fix is a wrapper with the common interface. Wikipedia describes the adapter pattern as one that "allows the interface of an existing class to be used as another interface", and notes that it is used "to make existing classes work with others without modifying their source code."[^4]

## Example

An email sender and a text message gateway both deliver a message to a person. Their methods have different names and different arguments, so the function that alerts a user must check which one it has.

```javascript run id=align-the-senders
import assert from "node:assert/strict";

// Before: two classes for one job, with two interfaces.
class EmailSenderBefore {
  constructor() { this.sent = []; }
  send(to, subject, body) { this.sent.push(`email to ${to}: ${subject} | ${body}`); }
}
class SmsGatewayBefore {
  constructor() { this.sent = []; }
  dispatch({ phone, text }) { this.sent.push(`sms to ${phone}: ${text.slice(0, 40)}`); }
}
function alertBefore(channel, user, message) {
  if (channel instanceof EmailSenderBefore) channel.send(user.email, "Alert", message);
  else channel.dispatch({ phone: user.phone, text: message });
}

// After: one interface, deliver(user, message), in both classes.
class EmailSender {
  constructor() { this.sent = []; }
  deliver(user, message) { this.sent.push(`email to ${user.email}: Alert | ${message}`); }
}
class SmsGateway {
  constructor() { this.sent = []; }
  deliver(user, message) { this.sent.push(`sms to ${user.phone}: ${message.slice(0, 40)}`); }
}
const alert = (channel, user, message) => channel.deliver(user, message);

const ana = { email: "ana@example.org", phone: "+31600000000" };
const msg = "Your backup finished with 2 warnings.";
const pairs = [[new EmailSenderBefore(), new EmailSender()], [new SmsGatewayBefore(), new SmsGateway()]];
for (const [old, now] of pairs) {
  alertBefore(old, ana, msg);
  alert(now, ana, msg);
  assert.deepEqual(now.sent, old.sent);
}
console.log("deliver: tests pass");
```

```output
deliver: tests pass
```

In the second version `alert` has no branch. A third channel, such as a chat bot, needs only a `deliver` method, and no client changes.

| Language | What the example shows |
|---|---|
| JavaScript | two classes aligned to `deliver(user, message)`, as above |
| Python | the same alignment, with a `Protocol` that names the interface |
| Java | the same alignment, with an interface that both classes implement |
| Go | the same alignment, with an interface that both types satisfy without declaring it |

## Consequences

The clients depend on one interface. They can take either class, and a test can pass a fake that has the same method. The code that chooses a class moves to one place, such as the code that reads the user's settings.

The work can be larger than it looks. Each caller of the old methods must change, and if the classes are part of a published interface, the old names need a deprecation period. An adapter avoids that cost and keeps both names, at the price of one more class for each wrapped one.

Two classes that look alike may not be alternatives. If they do different jobs that happen to share a word, forcing one interface on them makes a client think it can swap them when it cannot. The test is whether a client could use either one and be correct.

## How to fix it

- **Methods with different names for one job:** [[change-function-declaration|Change Function Declaration]] to rename them.[^1]
- **Methods with different parameters for one job:** [[change-function-declaration|Change Function Declaration]] to reshape them.[^1]
- **One class that leaves part of the job to its clients:** [[move-function|Move Function]] into the class.[^1]
- **Aligned classes with duplicated code:** [[extract-superclass|Extract Superclass]].[^1]
- **A class that cannot be changed:** an adapter. See [[adapter-pattern|Adapter pattern]].[^4]

## Variants and related patterns

**Interfaces.** A language feature that names the common interface and lets the compiler check it. See [[interfaces|Interfaces]].

**Adapter pattern.** The fix when one of the classes cannot be edited. See [[adapter-pattern|Adapter pattern]].

**Repeated Switches.** The branch on the class in each client is a switch that the aligned interface removes. See [[repeated-switches|Repeated Switches]].

## References
