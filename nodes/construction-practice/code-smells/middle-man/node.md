---
tag: DCBNAH
kind: pattern
slug: middle-man
name: Middle Man
aliases: []
area: construction-practice/code-smells
level: core
summary: >-
  A code smell: a class does little but pass calls on to another object, so clients pay for an extra layer that adds no rule of its own.
family: code-smell
polarity: negative
intent: "Find the classes whose methods mostly forward to one delegate, and let the clients talk to the delegate, or fold the class into it."
also_known_as: []
refactorings: [remove-middle-man, inline-function, replace-superclass-with-delegate, replace-subclass-with-delegate]
history:
  year: 1999
  people: [Kent Beck, Martin Fowler]
  note: Middle Man is in the first edition of Refactoring (1999), and keeps its name in the second edition (2018).
see_also: [code-smell, message-chains, lazy-element, encapsulation-and-visibility]
sources:
  - book: isbn:9780134757599
    title: Martin Fowler, with Kent Beck, Refactoring, Improving the Design of Existing Code, second edition, Addison-Wesley, 2018
    locator: Chapter 3, Bad Smells in Code, Middle Man
  - url: https://en.wikipedia.org/w/index.php?title=Law_of_Demeter&oldid=1306566666
    title: Wikipedia, Law of Demeter, revision 1306566666
    locator: Disadvantages
  - url: https://refactoring.com/catalog/
    title: Martin Fowler, Catalog of Refactorings
    locator: the cards Remove Middle Man, Inline Function, Replace Superclass with Delegate and Replace Subclass with Delegate
  - book: isbn:9780201485677
    title: Martin Fowler, Refactoring, Improving the Design of Existing Code, Addison-Wesley, 1999
    locator: Chapter 3, Bad Smells in Code, Middle Man
  - url: https://go.dev/ref/spec
    title: The Go Programming Language Specification
    locator: Struct types, promoted fields and methods
---

## Intent

Find the classes that mostly pass calls on to one other object. If the class adds no rule to the calls, let the clients call the delegate directly. If it adds a little, fold that little into the delegate or into the clients. Keep the layer only where it hides something that the clients should not know.

## Problem

Delegation is a normal part of design. A class hides an object behind its own methods, so that the clients do not depend on the object. That is the point of [[hide-delegate|Hide Delegate]], the fix for [[message-chains|Message Chains]]. *Refactoring* notes that the same tool can go too far: when half of a class's methods do nothing but delegate to another class, the class is a middle man.[^1]

The forces are these:

- Each forwarding method is small and seems harmless. A class gets one each time a client needs a feature of the delegate.
- The forwarding methods must grow with the delegate. Each feature of the delegate that a client wants needs one more method in the middle.
- A reader who follows a call must step through the middle man to find the code that does the work.
- Wikipedia notes the same cost for the Law of Demeter, which "may result in having to write many wrapper methods to propagate calls to components".[^2]

## Solution

*Refactoring* gives three moves, by how much the middle man does.[^1] The catalogue at refactoring.com has a card for each refactoring.[^3]

1. **The class mostly forwards:** [[remove-middle-man|Remove Middle Man]]. Give the clients access to the delegate, and change them to call it directly. Then delete the forwarding methods.
2. **Only a few methods forward:** [[inline-function|Inline Function]] on each one, into its callers.
3. **The middle man also has some behaviour:** [[replace-superclass-with-delegate|Replace Superclass with Delegate]] or [[replace-subclass-with-delegate|Replace Subclass with Delegate]], to fold the middle man into the real object.

The first edition of *Refactoring* names the same smell, and Remove Middle Man as its main fix.[^4]

*Refactoring* treats Hide Delegate and Remove Middle Man as a pair that a programmer applies back and forth as the code changes.[^1] Neither one is right for good. The question is how much of the delegate the clients need.

## Example

The class `Playlist` holds an array of songs. Four of its five methods only forward to the array, and each feature of arrays that a client wants needs one more of them. After Remove Middle Man, the clients use the array, and the one method with a rule of its own stays.

```javascript run id=remove-the-middle
import assert from "node:assert/strict";

const songs = [
  { title: "Intro", seconds: 90 },
  { title: "Theme", seconds: 200 },
  { title: "Outro", seconds: 60 },
];

// Before: four methods forward, and one has a rule.
class PlaylistBefore {
  #songs = [];
  add(song) { this.#songs.push(song); }
  size() { return this.#songs.length; }
  at(i) { return this.#songs.at(i); }
  some(f) { return this.#songs.some(f); }
  totalSeconds() { return this.#songs.reduce((t, s) => t + s.seconds, 0); }
}

// After: the clients use the array; the rule is a function.
const totalSeconds = (list) => list.reduce((t, s) => t + s.seconds, 0);

const before = new PlaylistBefore();
for (const s of songs) before.add(s);
const after = [...songs];

assert.equal(before.size(), after.length);
assert.equal(before.at(-1).title, after.at(-1).title);
assert.equal(before.some((s) => s.seconds > 180), after.some((s) => s.seconds > 180));
assert.equal(before.totalSeconds(), totalSeconds(after));
assert.equal(totalSeconds(after), 350);
console.log("playlist: tests pass");
```

```output
playlist: tests pass
```

The array already has `length`, `at`, `some` and many more methods, and the clients can use all of them without a change to any class. The total is the only rule, so it is the only code that remains.

| Language | What the example shows |
|---|---|
| JavaScript | forwarding methods removed, as above |
| Python | the same class, removed, with the rule kept as a function |
| Java | a class that forwards to a `List`, removed; the rule moves to a record |
| Go | forwarding methods replaced by an embedded field, whose methods the struct gets for free |

## Consequences

The clients reach the delegate in one step, and they can use all its features. There is no layer to keep in step with the delegate.

The layer may have been doing a job. A private array behind a class cannot be changed by the clients, and a class can keep a rule, such as no duplicates, on each change. When the clients get the array, they can break the rule. If the class guards a rule on writes, the class is not a middle man, even if most of its methods forward. Keep the methods that guard the rule, and remove the others.

Some languages remove the cost of the forwarding methods. In Go, a field that is declared with a type and no name is an embedded field. The Go specification calls its fields and methods promoted, says that "Promoted fields act like ordinary fields of a struct", and includes the promoted methods in the method set of the outer struct.[^5] The outer struct gets the methods of the delegate with no code, and can still add rules of its own.

## How to fix it

- **Half or more of the methods only forward:** [[remove-middle-man|Remove Middle Man]].[^1]
- **A few methods only forward:** [[inline-function|Inline Function]].[^1]
- **The middle man also adds behaviour:** [[replace-superclass-with-delegate|Replace Superclass with Delegate]] or [[replace-subclass-with-delegate|Replace Subclass with Delegate]].[^1]
- **The layer guards a rule:** keep the methods that guard it, and remove the rest.

## Variants and related patterns

**Message Chains.** The opposite smell. Hide Delegate fixes it, and too much Hide Delegate makes a middle man. See [[message-chains|Message Chains]].

**Lazy Element.** A middle man is a lazy element whose one job is to forward. See [[lazy-element|Lazy Element]].

**Proxy and decorator.** Patterns that forward calls on purpose, to add a rule such as access control or caching on each call. A class that forwards and adds a rule is one of these, not a middle man.

## References
