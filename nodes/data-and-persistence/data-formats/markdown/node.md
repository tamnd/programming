---
tag: WY2HQS
kind: format
slug: markdown
name: Markdown
aliases: [md, text/markdown, Markdown.pl]
area: data-and-persistence/data-formats
level: core
summary: >-
  Markdown is a plain text syntax for documents that a program converts to HTML. The original description is not exact, so CommonMark and GitHub Flavored Markdown give exact specifications of it.
see_also: [commonmark, cross-site-scripting, csv]
sources:
- url: https://daringfireball.net/projects/markdown/
  title: John Gruber, Daring Fireball, Markdown
  locator: Introduction; Download
- url: https://commonmark.org/
  title: CommonMark, the project page
  locator: Why is a spec needed?; Who are you?
- url: https://daringfireball.net/projects/markdown/syntax
  title: John Gruber, Daring Fireball, Markdown Syntax Documentation
  locator: Overview; Block Elements; Span Elements; Backslash Escapes
- url: https://spec.commonmark.org/0.31.2/
  title: CommonMark Spec, Version 0.31.2 (2024-01-28)
  locator: 1 Introduction; 2.3 Insecure characters; 4.5 Fenced code blocks; 4.6 HTML blocks; 5.2 List items; 5.3 Lists; 6.2 Emphasis and strong emphasis; 6.7 Hard line breaks
- url: https://www.rfc-editor.org/rfc/rfc7763.html
  title: "S. Leonard, RFC 7763: The text/markdown Media Type, March 2016"
  locator: 2 Markdown Media Type Registration Application; 6.1 Markdown Variants
- url: https://github.github.com/gfm/
  title: GitHub Flavored Markdown Spec, Version 0.29-gfm (2019-04-06)
  locator: 1.1 What is GitHub Flavored Markdown?; 4.10 Tables; 6.11 Disallowed Raw HTML
- url: https://www.rfc-editor.org/rfc/rfc7764.html
  title: "S. Leonard, RFC 7764: Guidance on Markdown: Design Philosophies, Stability Strategies, and Select Registrations, March 2016"
  locator: 3 Registration Templates for Common Markdown Syntaxes
- url: https://github.com/commonmark/cmark
  title: CommonMark, cmark README
  locator: the introduction; Security
- url: https://github.com/commonmark/commonmark.js
  title: CommonMark, commonmark.js README
  locator: A note on security
- url: https://github.com/commonmark/commonmark-spec/wiki/List-of-CommonMark-Implementations
  title: CommonMark, List of CommonMark Implementations
  locator: the whole page
---

## What it is

Markdown is a plain text syntax for documents. A program converts it to HTML. John Gruber wrote the first description of the syntax and a converter: "Markdown is a text-to-HTML conversion tool for web writers."[^1] The CommonMark project says: "It was developed in 2004 by John Gruber in collaboration with Aaron Swartz."[^2]

The name has two meanings. "Thus, “Markdown” is two things: (1) a plain text formatting syntax; and (2) a software tool, written in Perl, that converts the plain text formatting to HTML."[^1] The tool is `Markdown.pl`. Its download page gives its last version, Markdown 1.0.1, with the date 17 Dec 2004.[^1]

The main goal is a file that people can read before any conversion. "The overriding design goal for Markdown’s formatting syntax is to make it as readable as possible."[^1] The model is plain text email: "the single biggest source of inspiration for Markdown’s syntax is the format of plain text email."[^1] So a heading is a line with `#` at the start, a list item starts with `-` or `1.`, a quotation starts with `>`, and emphasis is a word between asterisks.

Markdown does not try to cover all of HTML. "Markdown is not a replacement for HTML, or even close to it."[^3] "For any markup that is not covered by Markdown’s syntax, you simply use HTML itself."[^3] Thus a Markdown document can hold raw HTML, and the converter copies it to the output. This rule is the cause of a security problem that the pitfalls describe.

The original description is short and not exact, and many programs have their own converter. The CommonMark project says: "John Gruber’s canonical description of Markdown’s syntax does not specify the syntax unambiguously."[^2] It also says: "By 2014 there were dozens of implementations in many languages."[^2] The [[commonmark]] specification, which started in 2014, defines one exact syntax and a test suite.[^2]

## Syntax or structure

A Markdown document is a sequence of blocks. A block is a paragraph, a heading, a list, a quotation, a code block, a thematic break or an HTML block. A blank line separates two blocks. Some blocks, such as lists and quotations, hold other blocks. Inside a paragraph or a heading, the converter parses inline elements: emphasis, code spans, links, images and line breaks.

The CommonMark specification says that "Indicators of block structure always take precedence over indicators of inline structure."[^4] So "parsing can proceed in two steps: first, the block structure of the document can be discerned; second, text lines inside paragraphs, headings, and other block constructs can be parsed for inline structure."[^4] The task [[parse-a-markdown-text]] uses the same two phases.

The main elements are these:

| Element | Markdown | HTML |
|---|---|---|
| Heading, level 1 to 6 | `# Title` to `###### Title` | `<h1>` to `<h6>` |
| Heading, level 1 or 2, underlined | a line, then `===` or `---` | `<h1>` or `<h2>` |
| Paragraph | lines of text with a blank line before and after | `<p>` |
| Quotation | `> text` | `<blockquote>` |
| Bullet list | `- item`, `* item` or `+ item` | `<ul>` and `<li>` |
| Ordered list | `1. item` or `1) item` | `<ol>` and `<li>` |
| Code block, indented | each line indented by four spaces | `<pre><code>` |
| Code block, fenced | a line of three backticks or tildes before and after | `<pre><code>` |
| Thematic break | `***`, `---` or `___` on a line | `<hr />` |
| Emphasis | `*text*` or `_text_` | `<em>` |
| Strong emphasis | `**text**` or `__text__` | `<strong>` |
| Code span | `` `code` `` | `<code>` |
| Link | `[text](url)` or `[text][label]` | `<a href="url">` |
| Image | `![alt](url)` | `<img src="url" alt="alt" />` |

Gruber's description gives most of these. "Markdown supports two styles of headers, Setext and atx."[^3] "Markdown uses email-style > characters for blockquoting."[^3] A bullet list can use asterisks, pluses or hyphens as list markers, and the three are equivalent.[^3] "To produce a code block in Markdown, simply indent every line of the block by at least 4 spaces or 1 tab."[^3] "Text wrapped with one * or _ will be wrapped with an HTML <em> tag; double * ’s or _ ’s will be wrapped with an HTML <strong> tag."[^3]

Fenced code blocks are not in Gruber's description. CommonMark added them: "A code fence is a sequence of at least three consecutive backtick characters ( ` ) or tildes ( ~ )."[^4] The text after the opening fence is the info string.[^4] "The first word of the info string is typically used to specify the language of the code sample, and rendered in the class attribute of the code tag."[^4] So the info string `python` gives the class name `language-python` in the example below.

A link can give its URL in the text or in a separate definition. "Markdown supports two style of links: inline and reference."[^3] A reference link uses a label, and a line elsewhere in the document defines the label:

```markdown
See [the spec][cm] for the rules.

[cm]: https://spec.commonmark.org/ "CommonMark Spec"
```

A backslash before a punctuation character makes the character literal: "To produce a literal asterisk or underscore at a position where it would otherwise be used as an emphasis delimiter, you can backslash escape it".[^3] In CommonMark, "A backslash at the end of the line is a hard line break".[^4]

Markdown has no syntax errors. Each input gives some output. The CommonMark specification says: "Any sequence of characters is a valid CommonMark document."[^4] If a construct is not complete, the converter shows its characters as text. For example, `*foo bar *` stays text, because the second asterisk follows a space.[^4]

## Examples

This document uses most of the block elements:

````markdown
# Release notes

Version **2.1** fixes two bugs and adds one *small* feature.

## Changes

- The parser accepts `CRLF` line endings.
- The `--quiet` option hides the progress bar.

> Note: the old option `-q` still works.

```python
parse(text, strict=True)
```

See the [change log](https://example.com/changes) for details.
````

A CommonMark converter gives this HTML:

```html
<h1>Release notes</h1>
<p>Version <strong>2.1</strong> fixes two bugs and adds one <em>small</em> feature.</p>
<h2>Changes</h2>
<ul>
<li>The parser accepts <code>CRLF</code> line endings.</li>
<li>The <code>--quiet</code> option hides the progress bar.</li>
</ul>
<blockquote>
<p>Note: the old option <code>-q</code> still works.</p>
</blockquote>
<pre><code class="language-python">parse(text, strict=True)
</code></pre>
<p>See the <a href="https://example.com/changes">change log</a> for details.</p>
```

The converter escapes special characters of HTML. "Within a code block, ampersands ( & ) and angle brackets ( < and > ) are automatically converted into HTML entities."[^3] In text, Gruber gives the example of `AT&T`, which becomes `AT&amp;T` in the HTML.[^3]

A file of Markdown has the extension `.md`, and its media type is `text/markdown`. RFC 7763 registers the type: "This document registers the text/markdown media type for use with Markdown, a family of plain-text formatting syntaxes that optionally can be converted to formal markup languages such as HTML."[^5] The `charset` parameter is required.[^5] An optional `variant` parameter names the syntax, for example `Content-Type: text/markdown; charset=UTF-8; variant=CommonMark`. The RFC says that it is "An optional identifier of the specific Markdown variant that the author intended."[^5]

## Versions

Markdown has no single version. The main definitions are these:

| Definition | Year | Status |
|---|---|---|
| `Markdown.pl` 1.0.1 and the syntax description by John Gruber[^1][^3] | 2004 | The original. Not exact. |
| CommonMark[^2][^4] | 2014, version 0.31.2 in 2024 | An exact specification with tests. |
| GitHub Flavored Markdown (GFM)[^6] | version 0.29-gfm in 2019 | CommonMark with five extensions. |
| Other variants, for example MultiMarkdown, Pandoc and Markdown Extra[^7] | various | Each adds its own extensions. |

The CommonMark specification gives the history: "In the next decade, dozens of implementations were developed in many languages. Some extended the original Markdown syntax with conventions for footnotes, tables, and other document elements."[^4] It also says: "In the absence of a spec, early implementers consulted Markdown.pl to resolve these ambiguities."[^4] The results did not agree. The CommonMark project adds: "because nothing in Markdown counts as a “syntax error,” the divergence often isn’t discovered right away."[^2]

GFM is the syntax of GitHub. "GitHub Flavored Markdown, often shortened as GFM, is the dialect of Markdown that is currently supported for user content on GitHub."[^6] "GFM is a strict superset of CommonMark."[^6] Its extensions are tables, task list items, strikethrough, extended autolinks and the filter of disallowed raw HTML.[^6] A GFM table looks like this:

```markdown
| Name | Age |
|------|----:|
| Ada  |  36 |
| Alan |  41 |
```

The delimiter row "consists of cells whose only content are hyphens ( - ), and optionally, a leading or trailing colon ( : ), or both, to indicate left, right, or center alignment respectively."[^6] So the column `Age` is aligned to the right.

RFC 7763 starts the IANA registry "Markdown Variants". Its first entry is `Original`, with the description "Gruber's original Markdown syntax."[^5] RFC 7764 adds more entries, among them `MultiMarkdown`, `GFM`, `pandoc`, `CommonMark` and `Extra`.[^7] It says: "Compared to "Original", CommonMark is much longer and in a few instances contradicts "Original" based on seasoned experience."[^7]

## Pitfalls

- **A number at the start of a line makes a list.** Gruber's description gives this example: "It’s worth noting that it’s possible to trigger an ordered list by accident, by writing something like this: 1986. What a great season."[^3] The fix is a backslash: "To avoid this, you can backslash-escape the period: 1986\. What a great season."[^3] The output also differs between versions. Gruber says that "the actual numbers you use to mark the list have no effect on the HTML output Markdown produces."[^3] In CommonMark, the list starts at the first number. Its example `123456789. ok` gives `<ol start="123456789">`.[^4]

```markdown
1986. What a great season.

1986\. What a great season.
```

```html
<ol start="1986">
<li>What a great season.</li>
</ol>
<p>1986. What a great season.</p>
```

CommonMark lowers the risk in the middle of a paragraph. "In order to solve the problem of unwanted lists in paragraphs with hard-wrapped numerals, we allow only lists starting with 1 to interrupt paragraphs."[^4] So a line that starts with `14.` inside a paragraph stays text, but a line that starts with `1.` still starts a list.[^4]

- **An underscore in a word does not always make emphasis.** Gruber's description says that "Emphasis can be used in the middle of a word: un*frigging*believable".[^3] It does not say whether `_` works in the same way, and converters differ. The CommonMark specification notes: "Many implementations have also restricted intraword emphasis to the * forms, to avoid unwanted emphasis in words containing internal underscores."[^4] CommonMark does the same. Its example `foo_bar_` stays text, and `5_6_78` stays text too.[^4] So `snake_case_name` is safe in CommonMark, but an old converter can show part of the word in italics. Put code and names in a code span.

- **Two spaces at the end of a line are a hard line break.** "When you do want to insert a <br /> break tag using Markdown, you end a line with two or more spaces, then type return."[^3] The spaces are not visible. If an editor removes trailing spaces, the break is gone and nothing warns you. CommonMark allows a backslash instead: "For a more visible alternative, a backslash before the line ending may be used instead of two or more spaces".[^4] Use the backslash.

- **Raw HTML goes to the output.** A converter copies an HTML block to the output: "An HTML block is a group of lines that is treated as raw HTML (and will not be escaped in HTML output)."[^4] If a site converts Markdown from its users and shows the HTML to other users, a user can put a `<script>` element in the text. That is [[cross-site-scripting]]. RFC 7763 says: "Markdown interpreted as a precursor to other formats, such as HTML, carries all of the security considerations as the target formats."[^5] The default of each library differs:
  - The C reference implementation cmark is safe by default: "By default, the library will scrub raw HTML and potentially dangerous links (`javascript:`, `vbscript:`, `data:`, `file:`)."[^8]
  - The JavaScript reference implementation is not: "The library does not attempt to sanitize link attributes or raw HTML."[^9] Its documentation says that for "untrusted user input, you should either enable the `safe` option (see above) or run the output through an HTML sanitizer".[^9]
  - The tagfilter of GFM is not a sanitizer. It filters nine tags, among them `<script>`, `<style>` and `<iframe>`, and it says: "All other HTML tags are left untouched."[^6] So `<img src="x" onerror="...">` passes the filter.

  For text from users, turn off raw HTML in the converter, or run the output through an HTML sanitizer.

- **A Markdown file does not give its variant.** A file with the extension `.md` can use GFM tables, the extensions of another variant or only the original syntax. A table looks right on GitHub, but a converter that only follows CommonMark shows it as a paragraph of pipes, because tables are an extension of GFM and not part of CommonMark.[^6] The `variant` parameter of `text/markdown` can name the syntax, but "The value serves as a "hint" to the recipient".[^5] Tell the readers which variant a project uses, and use one converter for all of the files.

- **The output of two converters is often different.** The CommonMark specification lists questions that the original description does not answer. The first is "How much indentation is needed for a sublist?", and it adds that "divergences between implementations on this issue often lead to surprises for users in real documents."[^4] The CommonMark project says: "There’s no standard test suite for Markdown; MDTest is the closest thing we had, and is now obsolete."[^2] Check that a library follows CommonMark or GFM, and test the documents that matter with it.

## Tools and libraries

No language has a Markdown converter in its standard library. The CommonMark project keeps a list of implementations.[^10] Some of them are these:

| Language | Library | Notes |
|---|---|---|
| C | cmark | "the C reference implementation of CommonMark"[^8] |
| C | cmark-gfm | "GitHub's fork of cmark with extensions for tables, etc."[^10] |
| JavaScript | commonmark.js | the JavaScript reference implementation[^9][^10] |
| JavaScript | markdown-it | "high speed pluggable implementation"[^10] |
| Python | markdown-it-py | "A pure Python port of the JS markdown-it library"[^10] |
| Go | goldmark | "pure Go implementation"[^10] |
| Rust | pulldown-cmark, comrak | listed for Rust[^10] |
| Java | commonmark-java | "pure Java"[^10] |
| C# | Markdig | "available on NuGet"[^10] |

The command-line program `cmark` converts Markdown to "HTML, groff man, LaTeX, CommonMark, or an XML representation of the AST."[^8] Its README says that "cmark is 10,000 times faster than the original `Markdown.pl`".[^8] The CommonMark dingus at spec.commonmark.org/dingus is a "Live testing tool powered by the reference implementation."[^2] It is a quick way to check a pitfall.

## References
