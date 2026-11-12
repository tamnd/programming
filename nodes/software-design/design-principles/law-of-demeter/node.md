---
tag: SB5E36
kind: practice
slug: law-of-demeter
name: Law of Demeter
aliases: [LoD, principle of least knowledge, only talk to your friends]
area: software-design/design-principles
level: core
summary: >-
  A method should call only the methods of objects it knows directly, its own object, its parameters, the objects it creates and its fields, and not reach through them to objects further away.
history:
  year: 1987
  people: [Ian Holland, Karl Lieberherr]
  note: Ian Holland proposed the rule at Northeastern University in the fall of 1987, on the Demeter Project; Lieberherr and Holland published it in IEEE Software in 1989.
see_also: [message-chains, middle-man, feature-envy, facade-pattern, orthogonality, encapsulation-and-visibility]
sources:
  - url: https://www.ccs.neu.edu/home/lieber/LoD.html
    title: Karl Lieberherr, Law of Demeter, Principle of Least Knowledge, Northeastern University
    locator: the whole page
  - url: https://en.wikipedia.org/wiki/Law_of_Demeter
    title: Wikipedia, Law of Demeter
    locator: the lead and the sections In object-oriented programming, Advantages and Disadvantages
---

## Statement

The Law of Demeter (LoD) "is a simple style rule for designing object-oriented systems. Only talk to your friends is the motto."[^1]

In its formal form, the Law of Demeter for functions says that a method m of an object a may only call the methods of these objects:[^2]

- a itself;
- the parameters of m;
- the objects that m creates;
- the attributes of a;
- the global variables that a can access in the scope of m.

## Meaning

An object a can ask an object b for a service, but it "should not reach through object b to access yet another object, c, to request its services."[^2] If it does, a depends on the internal structure of b. A change to how b holds c then breaks a.

The fix is to give b a method that does the job, so that b passes the request to its own parts. "If the law is followed, only object b knows its own internal structure."[^2] Wikipedia gives an analogy: "when one wants a dog to walk, one does not command the dog's legs to walk directly; instead, one commands the dog which then commands its own legs."[^2]

In languages that use a dot to reach a field or a method, the law is often stated as use only one dot: "the code a.m().n() breaks the law where a.m() does not."[^2] This short form is a guide, not the rule. A chain of calls on one value, such as a builder or a list pipeline, returns the same kind of object at each step and does not reach into a stranger.

The law is a specific case of loose coupling.[^2] It follows the idea of information hiding: an object "should assume as little as possible about the structure or properties of anything else".[^2]

## Examples

A shop takes money from a customer. The first version reaches through the customer into the wallet. The second version asks the customer to pay, and only the customer knows that it has a wallet.

```javascript run id=paying-customer
import assert from "node:assert/strict";

class Wallet {
  constructor(cash) { this.cash = cash; }
  take(amount) {
    if (amount > this.cash) throw new Error("not enough cash");
    this.cash -= amount;
  }
}

class Customer {
  constructor(cash) { this.wallet = new Wallet(cash); }
  // The customer talks to its own part.
  pay(amount) { this.wallet.take(amount); }
  get balance() { return this.wallet.cash; }
}

// Breaks the law: the shop talks to a stranger, the wallet.
function chargeReaching(customer, amount) { customer.wallet.take(amount); }

// Follows the law: the shop talks only to its parameter.
function charge(customer, amount) { customer.pay(amount); }

const a = new Customer(20);
const b = new Customer(20);
chargeReaching(a, 7);
charge(b, 7);
assert.equal(a.balance, b.balance);
console.log(b.balance);
```

```output
13
```

If the customer later pays by card, only Customer.pay changes. Every function like chargeReaching would have to change too.

| Language | What the example shows |
|---|---|
| JavaScript | the reaching and the lawful charge, as above |
| Python | the lawful version, with a pay method |
| Go | the lawful version, with a Pay method on a Customer struct |

## Criticism and limits

- **Many wrapper methods.** The law "may result in having to write many wrapper methods to propagate calls to components; in some cases, this can add noticeable time and space overhead."[^2] Too many such methods give the [[middle-man|Middle man]] smell.
- **Wide class interfaces.** At the method level the law gives narrow interfaces. At the class level, if it is not used correctly, it can give wide interfaces with many auxiliary methods. Wikipedia says that this is "due to poor design rather than a consequence of the LoD per se."[^2]
- **Mixed results on bugs.** Basili and others published results in 1996 that suggest that a lower Response For a Class (RFC) can reduce bugs, and the law can lower RFC. The same results suggest that a higher Weighted Methods per Class (WMC) can increase bugs, and the law can raise WMC.[^2]
- **Not a dot count.** Phil Haack wrote an article with the title The Law of Demeter is not a Dot Counting Exercise.[^2] The rule is about which objects a method knows, not about the syntax.

In the other direction, a study from the University of Zurich in 2011 concluded: "we conclude that obeying the Law of Demeter is a straight-forward approach for developers to reduce the number of bugs in their software."[^1] David E. Smyth of the Jet Propulsion Laboratory wrote that on Mars Pathfinder "the integration costs of the law breaking parts of the system were at least an order of magnitude higher".[^1]

See [[message-chains|Message chains]] for the code smell that the law prevents.

## History

The rule "was first proposed at Northeastern University in the fall of 1987 by Ian Holland".[^1] It is named after the Demeter Project, where it was found. The project was named after Demeter, a sister of Zeus, because the team was building a tool for a hardware description language named Zeus.[^2] Karl Lieberherr and Ian Holland published it as Assuring Good Style for Object-Oriented Programs in IEEE Software in September 1989.[^1] In 2003 and 2004, Lieberherr refined the motto to "Only talk to your friends who share your concerns", the Law of Demeter for Concerns.[^1]

## References
