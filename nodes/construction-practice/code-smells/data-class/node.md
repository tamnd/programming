---
tag: YNY5BB
kind: pattern
slug: data-class
name: Data Class
aliases: [anemic class, dumb data holder]
area: construction-practice/code-smells
level: core
summary: >-
  A code smell: a class has fields and accessors and no behaviour, while the rules about its data live in the classes that use it, often copied in several places.
family: code-smell
polarity: negative
intent: "Find the code that other classes run on a data class, and move it into the data class, so that the data and the rules about it are in one place."
also_known_as: []
refactorings: [encapsulate-record, remove-setting-method, move-function, extract-function, split-phase]
history:
  year: 1999
  people: [Kent Beck, Martin Fowler]
  note: Data Class is in the first edition of Refactoring (1999), and keeps its name in the second edition (2018). Fowler's article Anemic Domain Model (2003) describes the same problem at the scale of a whole domain model.
see_also: [code-smell, feature-envy, encapsulation-and-visibility, immutability, classes-and-instances]
sources:
  - book: isbn:9780134757599
    title: Martin Fowler, with Kent Beck, Refactoring, Improving the Design of Existing Code, second edition, Addison-Wesley, 2018
    locator: Chapter 3, Bad Smells in Code, Data Class
  - url: https://martinfowler.com/bliki/AnemicDomainModel.html
    title: Martin Fowler, Anemic Domain Model, bliki, 2003
    locator: the whole article
  - url: https://refactoring.com/catalog/
    title: Martin Fowler, Catalog of Refactorings
    locator: the cards Encapsulate Record, Remove Setting Method, Move Function, Extract Function and Split Phase
  - book: isbn:9780201485677
    title: Martin Fowler, Refactoring, Improving the Design of Existing Code, Addison-Wesley, 1999
    locator: Chapter 3, Bad Smells in Code, Data Class
  - url: https://openjdk.org/jeps/395
    title: JEP 395, Records
    locator: Summary, Goals and Motivation
  - url: https://docs.python.org/3/library/dataclasses.html
    title: Python documentation, dataclasses
    locator: the introduction and frozen instances
---

## Intent

Find the classes that only hold data, with getters and setters and nothing else. Look at the code that uses those getters and setters in other classes. Move that code into the data class, so that the class owns the rules about its own fields, and remove the setters that the rules make unnecessary.

## Problem

A class that only holds data is not wrong in itself. It becomes a smell when the rules about the data are somewhere else: in a service, in a controller, or in three places that each work out the same thing from the same fields.

*Refactoring* describes data classes as classes with fields and accessors and nothing else, which other classes manipulate in too much detail. It notes that such a class is often a sign that behaviour is in the wrong place.[^1] Fowler's article on the Anemic Domain Model describes the same problem for a whole design: objects named after the business concepts that turn out to be "little more than bags of getters and setters", while "a set of service objects" holds the logic. He calls the result "really just a procedural style design".[^2]

The forces are these:

- A data class is quick to write and quick to map to a database row or a JSON document. Its behaviour has to go somewhere, and the first caller is the closest place.
- Each caller that needs the same rule writes it again. The copies drift, and the result is [[duplicated-code|Duplicated Code]] and [[shotgun-surgery|Shotgun Surgery]].
- Public setters let any code change any field at any time. The class cannot keep a rule across its fields, such as an end date that is after the start date.
- Each method elsewhere that reads several getters of the data class is a case of [[feature-envy|Feature Envy]].

## Solution

*Refactoring* gives the moves.[^1] The catalogue at refactoring.com has a card for each refactoring.[^3]

1. **Close the fields:** [[encapsulate-record|Encapsulate Record]] for public fields, so that all access goes through the class.
2. **Remove the setters that should not exist:** [[remove-setting-method|Remove Setting Method]] for each field that must not change after construction.
3. **Find the users:** look for the code in other classes that reads the getters and setters.
4. **Move the behaviour in:** [[move-function|Move Function]] to bring that code into the data class. If only part of a function belongs there, use [[extract-function|Extract Function]] first, then move the part.

The first edition gives the same steps, and compares data classes to children: a fine place to start, but they must take on some responsibility to grow up.[^4]

*Refactoring* also names an exception. A record that holds the result of one phase for the next phase, as made by [[split-phase|Split Phase]], is a data class by design, and it is fine when it is immutable.[^1] Languages now have a construct for this case. JEP 395 describes Java records as "classes that act as transparent carriers for immutable data", and the Python dataclasses module adds "generated special methods such as `__init__()` and `__repr__()`" to such classes.[^5][^6] A record with no behaviour is not a smell when its job is to carry values.

## Example

A subscription is a class with a start, an end and setters. Billing and reminders each compute from the fields whether the subscription is active and how many days are left, and a renewal sets the end date from outside.

```javascript run id=move-the-rules-in
import assert from "node:assert/strict";

const DAY = 24 * 60 * 60 * 1000;
const d = (s) => Date.parse(`${s}T00:00:00Z`);

// Before: a data class, and the rules in the code that uses it.
class SubscriptionBefore {
  constructor(start, end) { this.start = start; this.end = end; }
  getStart() { return this.start; }
  getEnd() { return this.end; }
  setEnd(end) { this.end = end; }
}
const billingActive = (s, day) => s.getStart() <= day && day < s.getEnd();
const reminderDaysLeft = (s, day) => Math.max(0, Math.ceil((s.getEnd() - day) / DAY));
function renewBefore(s, days) { s.setEnd(s.getEnd() + days * DAY); }

// After: the rules are methods, and a renewal makes a value.
class Subscription {
  #start;
  #end;
  constructor(start, end) {
    if (end <= start) throw new RangeError("end must be after start");
    this.#start = start;
    this.#end = end;
  }
  isActive(day) { return this.#start <= day && day < this.#end; }
  daysLeft(day) { return Math.max(0, Math.ceil((this.#end - day) / DAY)); }
  renewed(days) { return new Subscription(this.#start, this.#end + days * DAY); }
}

const old = new SubscriptionBefore(d("2026-01-01"), d("2026-02-01"));
const sub = new Subscription(d("2026-01-01"), d("2026-02-01"));
for (const day of [d("2025-12-31"), d("2026-01-20"), d("2026-02-01")]) {
  assert.equal(sub.isActive(day), billingActive(old, day));
  assert.equal(sub.daysLeft(day), reminderDaysLeft(old, day));
}
renewBefore(old, 30);
const next = sub.renewed(30);
assert.equal(next.daysLeft(d("2026-02-01")), reminderDaysLeft(old, d("2026-02-01")));
assert.equal(sub.daysLeft(d("2026-02-01")), 0); // the old value is unchanged
assert.throws(() => new Subscription(d("2026-02-01"), d("2026-01-01")), RangeError);
console.log("subscription: tests pass");
```

```output
subscription: tests pass
```

In the second version billing and reminders call `isActive` and `daysLeft`, and the rule for each is in one place. The class can check that the end is after the start, because no setter can break that rule after construction. A renewal returns a value, so code that holds the old subscription is not surprised by a change.

| Language | What the example shows |
|---|---|
| JavaScript | the rules moved in, the setter removed, as above |
| Python | a frozen dataclass with methods, and `dataclasses.replace` for the renewal |
| Java | a record with a compact constructor that checks the rule, and methods |
| Go | a struct with unexported fields, a constructor function and methods |

## Consequences

The rules about the data are next to the data, and each one is written once. The class can protect its own rules, and a reader who wants to know what a subscription can do finds it in one file.

The class grows, and it must take care not to grow into a [[large-class|Large Class]]. Only the rules about the class's own data belong in it. Code that coordinates several objects, or that talks to a database or a screen, belongs elsewhere; Fowler's article makes the same point about keeping persistence and presentation in their own layers.[^2]

Some data classes are right as they are. A message between services, a row read from a file, or a result passed from one phase to the next is data on purpose, and the code that uses it is in the phase that reads it. Make such classes immutable, so that their lack of behaviour cannot become a lack of control.

## How to fix it

- **Public fields:** [[encapsulate-record|Encapsulate Record]].[^1]
- **Setters for fields that should not change:** [[remove-setting-method|Remove Setting Method]].[^1]
- **Code elsewhere that works on the data:** [[move-function|Move Function]] into the class, after [[extract-function|Extract Function]] if only part of it belongs.[^1]
- **A record that carries the result of one phase:** keep it, and make it immutable.[^1]

## Variants and related patterns

**Feature Envy.** The methods that use a data class's getters are envious of it. Moving them in fixes both smells. See [[feature-envy|Feature Envy]].

**Anemic Domain Model.** The same problem across a whole model, where all the logic is in services. Fowler's article describes it.[^2]

**Immutability.** A data class that cannot change is safe to share, which makes it the correct form for the cases that should stay data. See [[immutability|Immutability]].

## References
