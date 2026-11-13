---
tag: 4XJ9RQ
kind: standard
slug: commonmark
name: CommonMark
aliases: [CommonMark Spec, CommonMark specification]
area: data-and-persistence/data-formats
level: core
summary: >-
  CommonMark is an exact specification of Markdown with more than 600 examples that double as tests. It defines how a parser divides a text into blocks and inlines, and GFM builds on it.
see_also: [markdown, cross-site-scripting]
sources:
- url: https://commonmark.org/
  title: CommonMark, the project page
  locator: Why is CommonMark needed?; Who are you?
- url: https://spec.commonmark.org/0.31.2/
  title: CommonMark Spec, Version 0.31.2 (2024-01-28)
  locator: 1.2 Why is a spec needed?; 1.3 About this document; 2.2 Tabs; 3 Blocks and inlines; 4.6 HTML blocks; 4.7 Link reference definitions; 5.1 Block quotes; 5.2 List items; 5.3 Lists; 6.3 Links; Appendix A parsing strategy
- url: https://github.com/commonmark/commonmark-spec
  title: CommonMark, commonmark-spec README
  locator: Running tests against the spec; The spec
- url: https://github.github.com/gfm/
  title: GitHub Flavored Markdown Spec, Version 0.29-gfm (2019-04-06)
  locator: 1.1 What is GitHub Flavored Markdown?
- url: https://spec.commonmark.org/
  title: CommonMark Spec, the list of versions
  locator: All versions
- url: https://github.com/commonmark/commonmark-spec/blob/master/changelog.txt
  title: CommonMark, commonmark-spec changelog
  locator: "[0.31.2], [0.31.1], [0.30]"
- url: https://github.com/commonmark/cmark
  title: CommonMark, cmark README
  locator: the introduction
- url: https://github.com/commonmark/commonmark.js
  title: CommonMark, commonmark.js README
  locator: the introduction
- url: https://github.com/commonmark/commonmark-spec/wiki/List-of-CommonMark-Implementations
  title: CommonMark, List of CommonMark Implementations
  locator: the whole page
---

## What it is

CommonMark is a specification of [[markdown]]. The project page calls it "A strongly defined, highly compatible specification of Markdown".[^1] John MacFarlane is the author of the specification.[^2] A group of Markdown fans started the project in 2014. The project page says that they "either work at companies with industrial scale deployments of Markdown, have written Markdown parsers, have extensive experience supporting Markdown with end users".[^1]

The project exists because the original description of Markdown is not exact. "John Gruber’s canonical description of Markdown’s syntax does not specify the syntax unambiguously."[^1] Converters had to guess the answers, and their guesses did not agree. The specification gives some of the questions that the description does not answer, for example "How much indentation is needed for a sublist?" and "Is a blank line needed before a block quote or heading?"[^2] Its goal is stated in one sentence: "This document attempts to specify Markdown syntax unambiguously."[^2]

The specification is a list of rules with examples. Each example is a Markdown text and the HTML that it must give. "It contains many examples with side-by-side Markdown and HTML. These are intended to double as conformance tests."[^2] Version 0.31.2 has 652 examples.[^2] So a converter can show that it follows CommonMark: it runs all of the examples and compares its output with the expected HTML.

The specification describes the result and not a program. Its README says that the specification "is not an algorithm". It is "a declarative description of what counts as a block quote, a code block, and each of the other structural elements that can make up a Markdown document."[^3] An appendix gives one parsing strategy, which the reference implementations use.[^2]

GitHub Flavored Markdown is built on CommonMark. "GFM is a strict superset of CommonMark."[^4] It adds tables, task lists, strikethrough, extended autolinks and a filter of raw HTML as extensions.[^4] CommonMark itself has none of these.

## Syntax or structure

The specification has six parts and an appendix:[^2]

| Part | Contents |
|---|---|
| 1 Introduction | why a specification is needed, and how to use the tests |
| 2 Preliminaries | characters and lines, tabs, insecure characters, backslash escapes, entity and numeric character references |
| 3 Blocks and inlines | precedence, container blocks and leaf blocks |
| 4 Leaf blocks | thematic breaks, ATX headings, setext headings, indented code blocks, fenced code blocks, HTML blocks, link reference definitions, paragraphs, blank lines |
| 5 Container blocks | block quotes, list items, lists |
| 6 Inlines | code spans, emphasis, links, images, autolinks, raw HTML, hard line breaks, soft line breaks, textual content |
| Appendix | a parsing strategy in two phases |

The central idea is the difference between blocks and inlines. "We can divide blocks into two types: container blocks, which can contain other blocks, and leaf blocks, which cannot."[^2] A block quote and a list item are containers. A paragraph, a heading and a code block are leaves. Inlines, such as emphasis and links, are inside leaf blocks. "Indicators of block structure always take precedence over indicators of inline structure."[^2] So in the text below, the two lines are two list items, although a pair of backticks seems to make one code span:[^2]

```markdown
- `one
- two`
```

```html
<ul>
<li>`one</li>
<li>two`</li>
</ul>
```

The parsing strategy of the appendix follows from this rule. "Parsing has two phases".[^2] In the first phase, the parser reads the lines and builds the block structure: the paragraphs, block quotes, list items and so on.[^2] "In the second phase, the raw text contents of paragraphs and headings are parsed into sequences of Markdown inline elements (strings, code spans, links, emphasis, and so on), using the map of link references constructed in phase 1."[^2] The map of link references is the reason for two phases. A link can come before the definition of its label, so the parser must read all of the blocks first.

In the first phase, the parser keeps a tree of open blocks. For each line, "we iterate through the open blocks, starting with the root document, and descending through last children down to the last open block. Each block imposes a condition that the line must satisfy if the block is to remain open. For example, a block quote requires a > character. A paragraph requires a non-blank line."[^2] Then it looks for the start of new blocks, and it adds the rest of the line to the deepest open block. "Once a line has been incorporated into the tree in this way, it can be discarded, so input can be read in a stream."[^2]

In the second phase, the parser finds emphasis with a delimiter stack. When it finds a run of `*` or `_`, or a `[` or `![`, it adds a text node, "and we add a pointer to this text node to the delimiter stack."[^2] When it finds a closing delimiter, it looks back in the stack "for the first matching potential opener".[^2] Whether a run of `*` or `_` can open or close emphasis depends on the characters before and after it. The specification calls these runs left-flanking and right-flanking.[^2]

The rules of the specification are exact about whitespace. Two examples:

- Tabs: "in contexts where spaces help to define block structure, tabs behave as if they were replaced by spaces with a tab stop of 4 characters."[^2] A tab inside a line stays a tab.
- List items: the content of a list item starts at a column, and the lines after it must be indented to that column. If M is a list marker of width W followed by N spaces, then the following lines are indented "by W + N spaces".[^2]

## Examples

The tests of the specification are in its source file `spec.txt`. The README shows the form of an example: a fence of 32 backticks with the word `example`, the Markdown, a line with only a period, the HTML and a closing fence.[^3] "In the examples, the → character is used to represent tabs."[^2] The code below reads examples in this form. It builds the fence in code, so that this page does not need a fence of 32 backticks:

```python run id=spec-examples
FENCE = "`" * 32

spec = "\n".join([
    "A list item can hold two paragraphs:",
    "",
    FENCE + " example",
    "- one",
    "",
    "  two",
    ".",
    "<ul>",
    "<li>",
    "<p>one</p>",
    "<p>two</p>",
    "</li>",
    "</ul>",
    FENCE,
    "",
    "A tab is shown as an arrow:",
    "",
    FENCE + " example",
    "→foo",
    ".",
    "<pre><code>foo",
    "</code></pre>",
    FENCE,
    "",
])


def examples(text):
    tests = []
    lines = text.split("\n")
    i = 0
    while i < len(lines):
        if lines[i] == FENCE + " example":
            dot = lines.index(".", i + 1)
            end = lines.index(FENCE, dot + 1)
            markdown = "\n".join(lines[i + 1:dot]) + "\n"
            html = "\n".join(lines[dot + 1:end]) + "\n"
            tests.append({
                "markdown": markdown.replace("→", "\t"),
                "html": html.replace("→", "\t"),
                "number": len(tests) + 1,
            })
            i = end
        i += 1
    return tests


for test in examples(spec):
    print(test["number"], repr(test["markdown"]), repr(test["html"]))
```

```output for=spec-examples
1 '- one\n\n  two\n' '<ul>\n<li>\n<p>one</p>\n<p>two</p>\n</li>\n</ul>\n'
2 '\tfoo\n' '<pre><code>foo\n</code></pre>\n'
```

The first example is example 256 of the specification, and the second is a short form of example 1.[^2] The repository has a script that does this work and runs the tests against a converter. "To run the tests using an executable `$PROG`: python3 test/spec_tests.py --program $PROG".[^3] The option `--dump-tests` gives all of the tests in JSON, and the npm package `commonmark-spec` exports them as an array of objects with the members `markdown`, `html`, `section` and `number`.[^3] A converter in any language can read this JSON and run the tests in its own test suite.

A link reference definition shows how the two phases work together. The definition can come after the link, and the label matches without regard to case:[^2]

```markdown
[Foo]

[FOO]: /url
```

```html
<p><a href="/url">Foo</a></p>
```

To compare labels, a parser normalizes them: "To normalize a label, strip off the opening and closing brackets, perform the Unicode case fold, strip leading and trailing spaces, tabs, and line endings, and collapse consecutive internal spaces, tabs, and line endings to a single space."[^2] If two definitions have the same label, "the first is used".[^2]

## Versions

CommonMark has not reached version 1.0. The list of versions starts with 0.5 on 2014-10-25, and the latest version is 0.31.2 on 2024-01-28.[^5] The recent versions are these:

| Version | Date | Notes |
|---|---|---|
| 0.31.2 | 2024-01-28 | "Fix packaging bug (date not updated in spec.txt)."[^6] |
| 0.31.1 | not in the list of versions | "Fix packaging bug (spec version not updated in spec.txt)."[^6] |
| 0.31 | | for example, adds `search` to the known block elements and removes `source`[^6] |
| 0.30 | 2021-06-19 | for example, adds `textarea` to the literal HTML block tags[^5][^6] |
| 0.29 | 2019-04-06 | the same date as GFM 0.29-gfm[^5][^4] |
| 0.28 | 2017-08-01 | |
| 0.27 | 2016-11-18 | |

Each version has its own URL, such as spec.commonmark.org/0.31.2/, and its own set of test cases.[^5] A change between versions can change the output for some texts. For example, version 0.30 made `textarea` a literal HTML block tag, so that its blocks "can contain blank lines without the contents being interpret as commonmark."[^6] Thus a statement that a library follows CommonMark is not complete without the version.

## Pitfalls

- **A line without `>` can continue a block quote.** This is a lazy continuation line. If a line is paragraph text, it continues the paragraph of an open block quote or list item, even without the marker.[^2] Example 247 of the specification:

```markdown
> bar
baz
```

```html
<blockquote>
<p>bar
baz</p>
</blockquote>
```

A blank line ends the block quote. A parser cannot close a block quote at the first line without `>`. The appendix says: "we cannot close unmatched blocks yet, because we may have a lazy continuation line."[^2]

- **A second paragraph must be indented to the content of the list item.** In example 255, the line `two` has one space of indentation. The content of the item `- one` starts in column 3, so `two` is a new paragraph after the list:[^2]

```markdown
- one

 two
```

```html
<ul>
<li>one</li>
</ul>
<p>two</p>
```

With two spaces, as in example 256 and the code above, `two` is a second paragraph in the item.[^2] Other converters can need a different indentation. The specification says about sublists: "It is natural to think that they, too, must be indented four spaces, but Markdown.pl does not require that."[^2] It adds that "divergences between implementations on this issue often lead to surprises for users in real documents."[^2]

- **A blank line inside a list makes all items loose.** "A list is loose if any of its constituent list items are separated by blank lines, or if any of its constituent list items directly contain two block-level elements with a blank line between them."[^2] "The difference in HTML output is that paragraphs in a loose list are wrapped in <p> tags, while paragraphs in a tight list are not."[^2] So one blank line changes the spacing of the whole list.

- **Markdown inside an HTML block needs blank lines.** Most kinds of HTML block, among them a block that starts with `<del>`, end at a line that is "followed by a blank line".[^2] In example 166, the emphasis is not parsed, because the text is part of the HTML block. In example 167, blank lines end the block, so the text is a paragraph:[^2]

```markdown
<del>

*foo*

</del>
```

```html
<del>
<p><em>foo</em></p>
</del>
```

An HTML block "is treated as raw HTML (and will not be escaped in HTML output)."[^2] If the text comes from users, raw HTML is a risk of [[cross-site-scripting]]. See the pitfalls of [[markdown]].

- **The HTML of the tests is not all required.** The specification says: "Note that not every feature of the HTML samples is mandated by the spec. For example, the spec says what counts as a link destination, but it doesn’t mandate that non-ASCII characters in the URL be percent-encoded."[^2] A converter that prints different but equivalent HTML can follow the specification and still fail a test. The test script compares normalized HTML. Its changelog mentions that it can "show diffs of normalized HTML".[^6] Normalize the HTML in your own tests too.

- **A test for case folding must use Unicode.** A parser that only changes labels to upper case passes many tests, but it is not correct. The changelog of 0.30 says: "Use better example to test unicode case fold for reference links (#582). The earlier test could be passed by implementations that just uppercase."[^6] Use the full Unicode case fold, as the rule of normalization says.

## Tools and libraries

The CommonMark project has two reference implementations. "`cmark` is the C reference implementation of CommonMark".[^7] The README of commonmark.js says: "This repository contains the JavaScript reference implementation."[^8] The project also keeps a list of other implementations.[^9] Some of them are markdown-it for JavaScript, markdown-it-py for Python, goldmark for Go, commonmark-java for Java, Markdig for C#, and pulldown-cmark and comrak for Rust.[^9]

The library of cmark parses "CommonMark documents to an abstract syntax tree (AST)", and it can render the tree as "HTML, groff man, LaTeX, CommonMark, or an XML representation of the AST."[^7] The CommonMark dingus at spec.commonmark.org/dingus is a "Live testing tool powered by the reference implementation."[^1] It is a quick way to see what a text gives.

To check a converter, run `spec_tests.py` with the `--program` option, or read the JSON of `--dump-tests` in a test of your own.[^3] Use the tests of the version that the converter claims.

## References
