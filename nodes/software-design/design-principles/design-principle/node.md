---
tag: Y34WG8
kind: concept
slug: design-principle
name: Design principle
aliases: [software design principle, design rule]
area: software-design/design-principles
level: intro
summary: >-
  A general rule that guides the choices of a design, such as how to divide a program into parts and what each part may know about the others.
history:
  year: 1972
  people: [David Parnas, Edsger W. Dijkstra]
  note: Parnas described information hiding in 1972, and Dijkstra named the separation of concerns in 1974; later lists include the rules of the Pragmatic Programmer, extreme programming and SOLID.
see_also: [separation-of-concerns, dont-repeat-yourself, keep-it-simple, you-arent-gonna-need-it, simple-design, orthogonality, code-smell, dependency-injection]
sources:
  - url: https://en.wikipedia.org/wiki/Software_design
    title: Wikipedia, Software design
    locator: the sections Design principles and Design concepts
  - url: https://www.cs.utexas.edu/~EWD/transcriptions/EWD04xx/EWD447.html
    title: Edsger W. Dijkstra, On the role of scientific thought, EWD 447, 1974
    locator: the paragraphs that begin "Let me try to explain to you"
  - url: https://en.wikipedia.org/wiki/Information_hiding
    title: Wikipedia, Information hiding
    locator: the lead and the section History
  - url: https://en.wikipedia.org/wiki/SOLID
    title: Wikipedia, SOLID
    locator: the lead
  - url: https://martinfowler.com/bliki/BeckDesignRules.html
    title: Martin Fowler, Beck Design Rules, 2 March 2015
    locator: the quote from Kent Beck at the end
  - url: https://sandimetz.com/blog/2016/1/20/the-wrong-abstraction
    title: Sandi Metz, The Wrong Abstraction, 2016
    locator: the whole post
  - url: https://go.dev/ref/spec
    title: The Go Programming Language Specification
    locator: section Exported identifiers
  - url: https://docs.python.org/3/tutorial/classes.html
    title: The Python Tutorial, 9. Classes
    locator: section 9.6 Private Variables
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Statements/export
    title: MDN Web Docs, export
    locator: the lead
  - url: https://www.artima.com/articles/orthogonality-and-the-dry-principle
    title: Bill Venners, Orthogonality and the DRY Principle, a conversation with Andy Hunt and Dave Thomas, Artima, 2003
    locator: the introduction and the sections on DRY and orthogonality
---

## Idea

A design principle is a general rule that guides the choices of a design. "Design principles enable a software engineer to navigate the design process."[^1] A principle does not give a design. It gives a test that a programmer can apply to a design, and a reason for the test.

Most principles answer one of a few questions. How do we divide the program into parts? What may each part know about the others? When do we add a part, and when do we remove one? Examples are [[separation-of-concerns|Separation of concerns]], [[dont-repeat-yourself|Don't repeat yourself]], [[keep-it-simple|Keep it simple]], [[you-arent-gonna-need-it|You aren't gonna need it]] and [[orthogonality|Orthogonality]].

The smallest example shows why a principle exists. A shipping rule is written in two places. A change is made in one place and missed in the other, and the program gives two answers to one question.

```javascript run id=two-copies
// The rule "orders of 50 or more ship free" is written twice.
const shippingCost = (total) => (total >= 50 ? 0 : 5);
const bannerText = (total) => (total >= 50 ? "Free shipping" : "Shipping: 5");

// The business changes the limit to 40. Only one copy is updated.
const shippingCostV2 = (total) => (total >= 40 ? 0 : 5);

console.log(shippingCostV2(45), bannerText(45));
```

```output
0 Shipping: 5
```

The customer is told that shipping costs 5, but the order is charged 0. Don't repeat yourself says that the limit is one piece of knowledge and must have one representation.

## How it works

A principle works as a constraint on a set of possible designs. A programmer makes a design, applies the test of the principle, and changes the design where it fails. The same test can be used to review the code of other people, and to name a problem: this function has two concerns, this fact is written twice.

Principles are general, so they work at many sizes. Separation of concerns applies to one function, to a module and to the layers of a system. Edsger W. Dijkstra described it first as a way of thinking: to study "an aspect of one's subject matter in isolation for the sake of its own consistency".[^2]

Many principles are about change. Information hiding is "the principle of segregation of the design decisions in a computer program that are most likely to change, thus protecting other parts of the program from extensive modification if the design decision is changed."[^3] One list of software design principles includes this rule: "The design should be structured to accommodate change".[^1]

Principles can conflict. Don't repeat yourself asks for one representation of each fact, so it tends to add abstractions. Keep it simple and You aren't gonna need it ask for fewer parts. The programmer must weigh them for each case. Kent Beck wrote that his own design rules "aren't perfect, but they serve to sort out some of the obvious crap and (importantly) you can evaluate them right now."[^5]

## In languages

Most design principles do not depend on the language. But a language can make a principle cheap or expensive to follow. Information hiding is a good example, because each language has its own way to say which names are visible outside a module.

- **Go.** Visibility is part of the name. "An identifier may be exported to permit access to it from another package." It is exported when "the first character of the identifier's name is a Unicode uppercase letter".[^7] The compiler enforces the rule.
- **Python.** The language has no private names. "“Private” instance variables that cannot be accessed except from inside an object don’t exist in Python." By convention, "a name prefixed with an underscore (e.g. _spam) should be treated as a non-public part of the API".[^8] The rule depends on the discipline of the programmer.
- **JavaScript.** A module shares only what it names in an export declaration: export "is used to export values from a JavaScript module."[^9] The other names of the module stay inside it.

The pages of each principle show it in JavaScript, Python and Go.

## Variations and related ideas

- **Lists of principles.** Several authors have collected principles into named lists. The Pragmatic Programmer, by Andy Hunt and Dave Thomas, gives DRY and orthogonality.[^10] Extreme programming gives the four rules of [[simple-design|Simple design]] and YAGNI. Robert C. Martin introduced the basic principles of SOLID in his 2000 paper Design Principles and Design Patterns, and Michael Feathers coined the SOLID acronym around 2004.[^4]
- **Design concepts.** Abstraction, modularity, information hiding and refinement are often called design concepts. They are the ideas that principles use.[^1] Grady Booch "mentions abstraction, encapsulation, modularization, and hierarchy as fundamental software design principles."[^1]
- **Patterns.** A design pattern is a named solution to a problem that occurs again and again. A pattern usually follows one or more principles. See [[creational-pattern|Creational pattern]] and [[structural-pattern|Structural pattern]].
- **Smells.** A [[code-smell|code smell]] is a sign in the code that a principle is broken, for example duplicated code for DRY.

## Pitfalls

- **Rules applied without their reason.** A principle applied without the problem it solves can make the code worse. Sandi Metz describes code where duplication was removed too early: each new requirement added a parameter and a conditional, until the shared code became "a condition-laden procedure which interleaves a number of vaguely associated ideas."[^6] Her advice: "prefer duplication over the wrong abstraction".[^6]
- **One principle at the cost of all others.** A design that follows one rule to the end can break another. Many small parts can satisfy separation of concerns but make the program hard to follow.
- **Principles as a checklist.** A principle helps a programmer think. It does not replace thought about the problem, the users and the cost of change.

## Cost

A principle has no cost at run time by itself. The cost is in the design: an extra function, module or interface to separate two concerns, a code generator to keep one representation of a fact, or a wrapper method to hide the structure of an object. These parts must be read, tested and kept up to date. The benefit is a lower cost of change later. Beck named the real goal of a design as to minimize cost "(including the cost of delay)" and maximize benefit "over the lifetime of the software".[^5]

## History

Modularity was discussed by Richard Gauthier and Stephen Pont in their 1970 book Designing Systems Programs. David Parnas first described information hiding in 1972.[^3] Dijkstra named the separation of concerns in 1974.[^2] Later lists include the rules of simple design, which Kent Beck formed in the late 1990s while he developed extreme programming, and SOLID, from 2000 and 2004.[^5][^4]

## References
