---
tag: 3PD0MH
kind: concept
slug: code-smell
name: Code smell
aliases: [smell, bad smell in code]
area: construction-practice/code-smells
level: core
summary: >-
  A code smell is a sign in the source that is quick to see and that often points to a deeper problem in the design. It is a hint to look closer and maybe refactor, not proof of a bug.
history:
  year: 1999
  people: [Kent Beck, Martin Fowler]
  note: Kent Beck coined the term. Beck and Fowler wrote the chapter Bad Smells in Code of Refactoring (1999), and a revised list of 24 smells in the second edition (2018).
see_also: [extract-function, source-code, unit-testing, test-driven-development, expression-problem]
sources:
  - url: https://martinfowler.com/bliki/CodeSmell.html
    title: Martin Fowler, Code Smell, bliki, 9 February 2006
    locator: the definition and the two points that follow it
  - url: https://c2.com/wiki/remodel/pages/CodeSmell
    title: WikiWikiWeb, Code Smell, page text of 12 December 2014
    locator: the first three paragraphs
  - book: isbn:9780134757599
    title: Martin Fowler, with Kent Beck, Refactoring, Improving the Design of Existing Code, second edition, Addison-Wesley, 2018
    locator: Chapter 3, Bad Smells in Code (Kent Beck and Martin Fowler), the 24 smells and the refactorings named under each
  - url: https://refactoring.com/catalog/
    title: Martin Fowler, Catalog of Refactorings
    locator: the cards, with the second edition name and the aliases of each refactoring
  - url: https://doc.rust-lang.org/reference/items/static-items.html#mutable-statics
    title: The Rust Reference, Static items
    locator: Mutable statics, items.static.mut.safety
  - url: https://docs.python.org/3.14/tutorial/controlflow.html#keyword-only-arguments
    title: The Python Tutorial, More Control Flow Tools, Python 3.14
    locator: 4.9.3 Special parameters and 4.9.3.3 Keyword-Only Arguments
  - url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-8.html#jls-8.10
    title: The Java Language Specification, Java SE 25 Edition
    locator: 8.10 Record Classes
  - url: https://go.dev/ref/spec#Type_definitions
    title: The Go Programming Language Specification
    locator: Type definitions
  - url: https://doc.rust-lang.org/book/ch06-02-match.html#matches-are-exhaustive
    title: The Rust Programming Language, The match Control Flow Construct
    locator: Matches Are Exhaustive
  - book: isbn:9780201485677
    title: Martin Fowler, Refactoring, Improving the Design of Existing Code, Addison-Wesley, 1999
    locator: Chapter 3, Bad Smells in Code (Kent Beck and Martin Fowler), the 22 smells
  - url: https://en.wikipedia.org/w/index.php?title=Code_smell&oldid=1379558939
    title: Wikipedia, Code smell, revision 1379558939
    locator: the introduction, Definition, and Common code smells
  - url: https://eslint.org/docs/latest/rules/max-params
    title: ESLint documentation, Rules, max-params
    locator: Options, max (default 3)
  - url: https://checkstyle.org/checks/sizes/parameternumber.html
    title: Checkstyle documentation, ParameterNumber
    locator: Properties, max (default 7)
  - url: https://pylint.readthedocs.io/en/stable/user_guide/configuration/all-options.html
    title: Pylint documentation, All configuration options
    locator: Design checker, --max-args (default 5)
---

## Idea

A code smell is a property of source code that a reader can see quickly and that often means a deeper problem in the design. Fowler's short definition: "A code smell is a surface indication that usually corresponds to a deeper problem in the system."[^1] The word "usually" is important. A smell does not prove a problem. It tells the reader where to look.

The program below runs, and it gives the right answers. It smells anyway.

```javascript run id=smelly-but-correct
function calc(a, b, c, d) {
  let t = 0;
  for (let i = 0; i < a.length; i++) {
    t = t + a[i].p * a[i].q;
  }
  if (b) t = t * 0.9;
  if (c === "DE") t = t * 1.19;
  if (c === "FR") t = t * 1.2;
  if (d) t = t + 4.99;
  return Math.round(t * 100) / 100;
}

const cart = [{ p: 10, q: 2 }, { p: 5.5, q: 1 }];
console.log(calc(cart, true, "DE", false));
console.log(calc(cart, false, "FR", true));
```

```output for=smelly-but-correct
27.31
35.59
```

A reader of the call `calc(cart, true, "DE", false)` cannot tell what `true` and `false` mean. The names `calc`, `a`, `t`, `p` and `q` say nothing about prices, discounts or tax ([[mysterious-name|Mysterious Name]]). Two of the four parameters are flags that select behaviour ([[boolean-parameter-smell|Boolean parameter]]). The tax rates are literals in a chain of `if` statements, and the next function that needs a tax rate will copy the chain ([[repeated-switches|Repeated Switches]]). Each of these is a smell. None of them is a bug yet, but each one makes the next change slower and more likely to be wrong.

A smell has a name, and the name comes with a set of refactorings that usually remove it. That is the practical value of the idea. A programmer who knows the names can say "this is Feature Envy" in a review, and the team knows both the problem and the usual fix.

## How it works

A smell is a starting point for a decision, not a rule. The work has four steps.

1. **See the sign.** Smells are chosen to be quick to spot. Fowler calls them "sniffable" and gives a long method as the example: he notices a Java method of more than a dozen lines.[^1]
2. **Look deeper.** Then he adds that "smells don't always indicate a problem."[^1] Some long methods are fine. The reader asks whether the sign comes from a real problem: does the long function do several things, or one thing in many steps? The WikiWikiWeb page says the same thing: a smell is a hint that something might be wrong, "not a certainty".[^2]
3. **Choose a refactoring.** Each smell in *Refactoring* lists the refactorings that usually fix it.[^3] The table below gives the main ones.
4. **Refactor in small steps, with tests.** After each step, run the tests and look again. A fix often shows the next smell, or shows that the first smell was not the real problem.

The 24 smells of the second edition of *Refactoring*, with the main refactorings that the book names for each, are these.[^3] The names of the refactorings are the names of the online catalogue.[^4]

| Smell | Main refactorings |
|---|---|
| [[mysterious-name]] | Change Function Declaration, Rename Variable, Rename Field |
| [[duplicated-code]] | Extract Function, Slide Statements, Pull Up Method |
| [[long-function]] | Extract Function, Replace Temp with Query, Introduce Parameter Object, Decompose Conditional, Split Loop |
| [[long-parameter-list]] | Replace Parameter with Query, Preserve Whole Object, Introduce Parameter Object, Remove Flag Argument |
| [[global-data]] | Encapsulate Variable |
| [[mutable-data]] | Encapsulate Variable, Split Variable, Separate Query from Modifier, Replace Derived Variable with Query |
| [[divergent-change]] | Split Phase, Move Function, Extract Class |
| [[shotgun-surgery]] | Move Function, Move Field, Combine Functions into Class, Inline Class |
| [[feature-envy]] | Move Function, Extract Function |
| [[data-clumps]] | Extract Class, Introduce Parameter Object, Preserve Whole Object |
| [[primitive-obsession]] | Replace Primitive with Object, Replace Type Code with Subclasses |
| [[repeated-switches]] | Replace Conditional with Polymorphism |
| [[loops-smell]] | Replace Loop with Pipeline |
| [[lazy-element]] | Inline Function, Inline Class, Collapse Hierarchy |
| [[speculative-generality]] | Collapse Hierarchy, Inline Function, Change Function Declaration, Remove Dead Code |
| [[temporary-field]] | Extract Class, Introduce Special Case |
| [[message-chains]] | Hide Delegate, Extract Function, Move Function |
| [[middle-man]] | Remove Middle Man, Inline Function |
| [[insider-trading]] | Move Function, Move Field, Hide Delegate |
| [[large-class]] | Extract Class, Extract Superclass |
| [[alternative-classes-with-different-interfaces]] | Change Function Declaration, Move Function, Extract Superclass |
| [[data-class]] | Encapsulate Record, Remove Setting Method, Move Function |
| [[refused-bequest]] | Push Down Method, Push Down Field, Replace Subclass with Delegate |
| [[comments]] | Extract Function, Change Function Declaration, Introduce Assertion |

The table is a map, not a recipe. The same smell can need a different fix in a different place, and two smells can pull in opposite directions: Hide Delegate removes a Message Chain and can make a Middle Man, and Remove Middle Man is its inverse.[^4]

## In languages

Smells are mostly independent of the language. But the features of a language change how often a smell appears and how cheap the fix is.

- **Rust.** A mutable global needs `static mut`, and the Rust Reference requires an `unsafe` block "when either reading or writing a mutable static variable".[^5] So [[global-data|Global Data]] that can change is visible in the code.
- **Python.** A `*` in a parameter list makes the parameters after it keyword-only.[^6] The caller must then write `discount=True`, not `True`, which removes the worst part of a [[boolean-parameter-smell|Boolean parameter]]. The list is still long.
- **Java.** A record class is "a simple aggregate of values".[^7] A record that holds data and no behaviour is the stated purpose of the type, so [[data-class|Data Class]] asks a different question for it: does code in other classes do work that belongs with this data?
- **Go.** A type definition "creates a new, distinct type with the same underlying type and operations as the given type".[^8] A `type Cents int64` costs one line, so the first step out of [[primitive-obsession|Primitive Obsession]] is cheap.
- **Rust, and other languages with checked pattern matching.** The arms of a `match` "must cover all possibilities", so the compiler finds each `match` that misses an added variant.[^9] [[repeated-switches|Repeated Switches]] is then less risky, because a forgotten case does not compile. The choice between a switch and polymorphism is the [[expression-problem]].
- **JavaScript.** The second edition of *Refactoring* uses JavaScript for its examples.[^3] The smell pages of this site show each smell in JavaScript first, then in Python, Java and Go.

## Variations and related ideas

**Catalogues.** The first edition of *Refactoring* has 22 smells.[^10] The second edition has 24. It adds Mysterious Name, Global Data, Mutable Data and Loops. It renames Long Method to Long Function, Switch Statements to Repeated Switches, Lazy Class to Lazy Element and Inappropriate Intimacy to Insider Trading. It drops [[parallel-inheritance-hierarchies|Parallel Inheritance Hierarchies]] and [[incomplete-library-class|Incomplete Library Class]].[^3] Other names in common use are [[downcasting-smell|Downcasting]], magic numbers and too many parameters; Wikipedia lists them with the Fowler smells.[^11]

**Design smells.** A smell can be at the level of the design, not of one function: an abstraction that does too much, a hierarchy that is too deep, a module that depends on everything. Wikipedia quotes a definition of smells as structures that "indicate violation of fundamental design principles and negatively impact design quality".[^11] See [[design-smell|Design smell]].

**Anti-patterns.** An anti-pattern is a named solution that looks right and is a bad choice. A smell is a symptom; an anti-pattern is a cause with a name. The page [[god-object|God object]] is an anti-pattern; [[large-class|Large Class]] is the smell that it produces.

**Test smells.** Test code has smells of its own, such as tests that depend on each other or on a shared fixture. See [[test-smells|Test smells]].

## Pitfalls

**A smell is not a defect.** Wikipedia: code smells "are usually not bugs; they are not technically incorrect and do not prevent the program from functioning".[^11] A review that blocks a change only because a function has 30 lines treats the hint as proof. Ask what the smell costs in this code.

**A threshold is a policy, not a fact.** Linters turn some smells into numbers, and the numbers differ. The ESLint rule `max-params` allows 3 parameters by default.[^12] Checkstyle's `ParameterNumber` allows 7.[^13] Pylint's `max-args` allows 5.[^14] None of the three numbers is right. A team picks one, and then looks at each report.

**A fix of the sign, not the cause.** A long function split into five functions that pass ten variables between them still has the problem. It is now a [[long-parameter-list|Long Parameter List]]. The fix for a smell must make the code easier to understand, or it is not a fix.

**Refactoring without tests.** A smell gives a reason to change code that works. Without tests, the change can break it. Write the tests first; see [[unit-testing]].

## Cost

A smell costs nothing when the program runs. The run block above gives the same answers before and after a refactoring. The cost is in the work of people: a smell makes the code slower to read and to change, and makes a wrong change more likely. Wikipedia describes smells as weaknesses in design "that may slow down development or increase the risk of bugs or failures in the future".[^11]

The cost of a fix is small when the refactoring is small and the tests are good. Most of the refactorings in the table are automated in IDEs, such as rename, extract and inline (see [[extract-function]]). The cost is large when the smell is in a published interface, because every caller must change too.

## History

Kent Beck coined the term while he helped Fowler with the book *Refactoring*.[^1] The WikiWikiWeb page says that Beck seems to have first used the phrase on its page OnceAndOnlyOnce, and that the chapter "Bad Smells in Code" was an essay by Beck and Fowler, published as chapter 3 of *Refactoring*.[^2] The book appeared in 1999 with 22 smells.[^10] Wikipedia notes that use of the term grew after the book.[^11]

Fowler's bliki entry on the term dates from 9 February 2006. It adds that inexperienced people can spot smells before they can judge them, and it tells of lead developers who choose a "smell of the week" for the team to look for.[^1]

The second edition of *Refactoring* (2018) rewrote the examples in JavaScript and revised the list to 24 smells.[^3]

## References
