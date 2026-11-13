---
tag: JNQMC0
kind: practice
slug: deep-modules
name: Deep modules
aliases: [deep module, deep methods, deep interfaces, shallow modules]
area: software-design/design-principles
level: core
summary: >-
  Prefer modules that give a lot of functionality through a simple interface; a module whose interface is almost as complex as its implementation is shallow and adds little.
history:
  year: 2018
  people: [John Ousterhout]
  note: John Ousterhout described deep and shallow modules in his book A Philosophy of Software Design, first published in April 2018; the second edition of July 2021 expanded the chapter General-Purpose Modules are Deeper.
see_also: [separation-of-concerns, keep-it-simple, design-principle, encapsulation-and-visibility]
sources:
  - url: https://github.com/johnousterhout/aposd-vs-clean-code
    title: John Ousterhout and Robert Martin, A Philosophy of Software Design vs Clean Code, September 2024 to February 2025
    locator: sections Introductions and Method Length
  - url: https://web.stanford.edu/~ouster/cgi-bin/aposd.php
    title: John Ousterhout, A Philosophy Of Software Design, the book page
    locator: the notes on the Second Edition
  - url: https://openlibrary.org/isbn/9781732102200
    title: Open Library, A Philosophy of Software Design, John Ousterhout, Yaknyam Press, 6 April 2018
    locator: the edition record
---

## Statement

"The best methods are those that provide a lot of functionality but have a very simple interface: they replace a large cognitive load (reading the detailed implementation) with a much smaller cognitive load (learning the interface). I call these methods 'deep'."[^1]

## Meaning

John Ousterhout judges a design idea by one question: "When I'm evaluating an idea related to software design, I ask whether it will reduce complexity. This usually means either reducing the amount of information a developer has to know, or making the required information more obvious."[^1]

A module helps when its users can work with it "without learning the details of how the method is implemented; they only need to learn its interface."[^1] The benefit is the difference between the two costs. A deep module hides much behind a small interface. A shallow module hides little: its interface is almost as large as the code behind it.

Ousterhout uses the idea to judge decomposition. "As methods get smaller and smaller there is less and less benefit to further subdivision. The amount of functionality hidden behind each interface drops, while the interfaces often become more complex. I call these interfaces 'shallow': they don't help much in terms of reducing what the programmer needs to know."[^1] In the end, "someone using the method needs to understand every aspect of its implementation. Such methods are usually pointless."[^1]

## Examples

The example below reads a small configuration text in two ways. In the shallow version, each step is its own function, and the caller must know all four and their order. In the deep version, one function with one parameter does the same work.

```javascript run id=config-reader
// Shallow: each step is its own function; the caller must know them all.
const splitLines = (text) => text.split("\n");
const dropComments = (lines) => lines.filter((l) => !l.startsWith("#"));
const dropBlank = (lines) => lines.filter((l) => l.trim() !== "");
const toPair = (line) => line.split("=").map((s) => s.trim());

// Deep: one simple interface hides the same steps.
function parseConfig(text) {
  const result = {};
  for (const line of text.split("\n")) {
    const s = line.trim();
    if (s === "" || s.startsWith("#")) continue;
    const [key, value] = s.split("=").map((x) => x.trim());
    result[key] = value;
  }
  return result;
}

const text = "# server\nhost = example.org\n\nport = 8080\n";
const shallow = Object.fromEntries(dropBlank(dropComments(splitLines(text))).map(toPair));
console.log(`shallow: ${JSON.stringify(shallow)}`);
console.log(`deep: ${JSON.stringify(parseConfig(text))}`);
```

```output
shallow: {"host":"example.org","port":"8080"}
deep: {"host":"example.org","port":"8080"}
```

Both give the same result. A user of the deep version learns one function. A user of the shallow version learns four, and must also know that comments are dropped before blank lines and before the split at the equals sign.

| Language | What the example shows |
|---|---|
| JavaScript | four small functions against one parseConfig, as above |
| Python | the same two versions, with a dict as the result |
| Go | the same two versions, with a map as the result and strings.Cut |

## Criticism and limits

- **The opposite advice.** Robert Martin's Clean Code says "The first rule of functions is that they should be small. The second rule of functions is that *they should be smaller than that*."[^1] Ousterhout answers that "decomposition can be taken too far", and that the advice "encourages programmers to create teeny-tiny methods that suffer from both shallow interfaces and entanglement."[^1] Martin replies: "if I must err, I'd rather err on the side of decomposition."[^1]
- **Entanglement.** Small methods can also be entangled: "Two methods are entangled (or 'conjoined' in APOSD terminology) if, in order to understand how one of them works internally, you also need to read the code of the other."[^1] Ousterhout's fix is to combine them, "so that all the code is in one place."[^1]
- **A judgement, not a rule.** Ousterhout uses the deep and shallow view because "it captures both sides of the tradeoff; it will tell you when a decomposition is good and also when decomposition makes things worse."[^1] It gives no size limit for a method or a module.

## History

Open Library dates the first edition of A Philosophy of Software Design, published by Yaknyam Press, to 6 April 2018.[^3] Ousterhout released the second edition in July 2021, and in it he "reworked and expanded Chapter 6 ('General-Purpose Modules are Deeper')".[^2] From September 2024 to February 2025, Ousterhout and Robert Martin held a series of discussions on the differences between the two books, which they published on GitHub.[^1]

## References
