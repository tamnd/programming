---
tag: Y4GEGK
kind: practice
slug: robustness-principle
name: Robustness principle
aliases: [Postel's law, be conservative in what you send, be liberal in what you accept]
area: software-design/design-principles
level: core
summary: >-
  Send only what the specification allows, and accept any input whose meaning is clear; a guideline from early Internet protocols that later work says can make protocols less robust over time.
history:
  year: 1980
  people: [Jon Postel, Bob Braden, Martin Thomson, David Schinazi]
  note: Jon Postel wrote the principle into the DoD standard Internet Protocol, RFC 760, January 1980; RFC 1122 restated it in 1989, and RFC 9413 of June 2023 described its harmful consequences.
see_also: [fail-fast, principle-of-least-astonishment, design-principle]
sources:
  - url: https://www.rfc-editor.org/rfc/rfc760.txt
    title: J. Postel, editor, RFC 760, DoD Standard Internet Protocol, January 1980
    locator: section 3.2, Discussion
  - url: https://www.rfc-editor.org/rfc/rfc1122.txt
    title: R. Braden, editor, RFC 1122, Requirements for Internet Hosts, Communication Layers, October 1989
    locator: section 1.2.2, Robustness Principle; section 3.1
  - url: https://www.rfc-editor.org/rfc/rfc9413.txt
    title: M. Thomson and D. Schinazi, RFC 9413, Maintaining Robust Protocols, June 2023
    locator: the Abstract; sections 2, 4.1 and 5.1
  - url: https://en.wikipedia.org/wiki/Robustness_principle
    title: Wikipedia, Robustness principle
    locator: the lead and the sections Interpretation and Criticism
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/parseInt
    title: MDN Web Docs, parseInt()
    locator: the description
  - url: https://docs.python.org/3/library/functions.html
    title: Python documentation, Built-in Functions
    locator: class int
  - url: https://pkg.go.dev/strconv
    title: Go standard library, package strconv
    locator: func ParseInt
---

## Statement

RFC 760, the Internet Protocol standard of 1980, says: "In general, an implementation should be conservative in its sending behavior, and liberal in its receiving behavior."[^1] RFC 1122 of 1989 gives the short form: "Be liberal in what you accept, and conservative in what you send".[^2] The principle is "also known as Postel's law, after Jon Postel".[^4]

## Meaning

The principle has two halves. On the sending side, a program follows the specification exactly. On the receiving side, a program accepts input that does not follow the specification, as long as it can find the meaning. RFC 760 explains: an implementation "should be careful to send well-formed datagrams, but should accept any datagram that it can interpret".[^1]

The reason is that a specification can be read in different ways. RFC 760 says: "While the goal of this specification is to be explicit about the protocol there is the possibility of differing interpretations."[^1] A tolerant receiver works with senders that read the specification differently.

RFC 1122 adds a defensive reading. "Software should be written to deal with every conceivable error, no matter how unlikely". It also says: "In general, it is best to assume that the network is filled with malevolent entities that will send in packets designed to have the worst possible effect."[^2]

## Examples

The same choice occurs in the number parsers of most languages. A lenient parser reads a number from text that is not exactly a number; a strict parser rejects it. In the example below, a setting has the text 8080x.

```javascript run id=port-setting
const text = "8080x";

const lenient = parseInt(text, 10); // reads digits until it cannot
const strict = Number(text); // the whole text must be a number

console.log(`lenient: ${lenient}`);
console.log(`strict: ${Number.isNaN(strict) ? "rejected" : strict}`);
```

```output
lenient: 8080
strict: rejected
```

The lenient parser accepts the text, so the sender never learns that the text is wrong. The strict parser rejects it at once. The libraries of each language take a position:

| Language | Behaviour of the standard parser |
|---|---|
| JavaScript | "If parseInt encounters a character in the input string that is not a valid numeral in the specified radix, it ignores it and all succeeding characters and returns the integer value parsed up to that point."[^5] |
| Python | int accepts a string that has a sign, leading zeros, "be surrounded by whitespace, and have single underscores interspersed between digits."[^6] Other characters raise an error. |
| Go | strconv.ParseInt: "If s is empty or contains invalid digits, err.Err = ErrSyntax and the returned value is 0".[^7] |

The Python and Go examples of this page parse the same text with a lenient and a strict function.

## Criticism and limits

- **Protocol decay.** RFC 9413 says that the principle "has been interpreted in a variety of ways. While some interpretations help ensure the health of the Internet, others can negatively affect interoperability over time."[^3] Tolerance "defers the effort of dealing with interoperability problems and prioritizes progress. However, this deferral can amplify the ultimate cost of handling interoperability problems."[^3] An implementation that follows the principle "enters a pathological feedback cycle", in which "Errors in implementations or confusion about semantics are permitted or ignored."[^3]
- **Virtuous intolerance.** As the alternative, RFC 9413 says: "Choosing to generate fatal errors for unspecified conditions instead of attempting error recovery can ensure that faults receive attention." When a new implementation meets an intolerant peer, "it receives strong feedback that allows the problem to be discovered quickly."[^3] This is the [[fail-fast|fail fast]] practice applied to protocols.
- **Late discovery.** In 2001, Marshall Rose described a defective implementation that works with tolerant peers until, "possibly several years later, it is connected with a less tolerant application that rejects its messages." He recommended "explicit consistency checks in a protocol ... even if they impose implementation overhead".[^4]

## History

Jon Postel edited RFC 760, the DoD standard Internet Protocol of January 1980, which contains the principle in its discussion of the protocol.[^1] According to Wikipedia, Postel also used the wording in an early specification of TCP.[^4] In October 1989, RFC 1122, edited by Bob Braden, called it "The Robustness Principle" and said that it "is particularly important in the Internet layer, where one misbehaving host can deny Internet service to many other hosts."[^2] In June 2023, the Internet Architecture Board published RFC 9413 by Martin Thomson and David Schinazi, which recommends active protocol maintenance in place of tolerance.[^3]

## References
