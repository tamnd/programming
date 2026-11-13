---
tag: J59DAF
kind: task
slug: parse-a-markdown-text
name: Parse a Markdown text
aliases: [convert Markdown to HTML, render Markdown, Markdown to HTML]
area: data-and-persistence/data-formats
level: core
summary: Convert a text in a subset of CommonMark (headings, paragraphs, lists, fenced code, emphasis, code spans and links) to the same HTML that a CommonMark converter gives.
contract: contract.yaml
category: data-formats
languages: [python, javascript, typescript, csharp, go]
sources:
- url: https://spec.commonmark.org/0.31.2/
  title: "J. MacFarlane, CommonMark Spec, Version 0.31.2, January 2024"
  locator: 2.1 Characters and lines; 4.2 ATX headings; 4.5 Fenced code blocks; 5.3 Lists; 6.1 Backslash escapes; 6.1 Code spans; 6.2 Emphasis and strong emphasis; 6.7 Hard line breaks; Appendix A parsing strategy
- url: https://docs.python.org/3/library/unicodedata.html
  title: The Python Standard Library, unicodedata
  locator: unicodedata.category
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Regular_expressions/Unicode_character_class_escape
  title: "MDN Web Docs, Character class escape: \\p{...}, \\P{...}"
  locator: Description
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/String/Symbol.iterator
  title: "MDN Web Docs, String.prototype[Symbol.iterator]()"
  locator: Return value
- url: https://pkg.go.dev/unicode
  title: Go standard library, package unicode
  locator: IsPunct; IsSymbol; Zs
- url: https://learn.microsoft.com/en-us/dotnet/api/system.text.rune
  title: .NET API reference, Rune Struct
  locator: Remarks (When to use the Rune type); IsPunctuation; IsSymbol
- url: https://github.com/commonmark/commonmark.js
  title: commonmark.js, the CommonMark reference implementation in JavaScript (README)
  locator: Usage (the safe option); A note on security
- url: https://github.com/commonmark/cmark
  title: cmark, the CommonMark reference implementation in C (README)
  locator: Security
---

## Task

Convert a Markdown text to HTML, with the rules of CommonMark 0.31.2. The text uses a subset of Markdown: ATX headings (`#` to `######`), paragraphs, bullet lists with `- `, fenced code with backticks, emphasis and strong emphasis with `*`, code spans, inline links, backslash escapes and hard line breaks. The output must be the same HTML that a CommonMark converter gives for the text, character for character.

For example, this text:

````markdown
# Release notes

Version **2.1** adds one *small* feature:
- a `--dry-run` flag
- a [manual](https://example.com/manual)

```sh
tool --dry-run
```
````

gives this HTML:

```html
<h1>Release notes</h1>
<p>Version <strong>2.1</strong> adds one <em>small</em> feature:</p>
<ul>
<li>a <code>--dry-run</code> flag</li>
<li>a <a href="https://example.com/manual">manual</a></li>
</ul>
<pre><code class="language-sh">tool --dry-run
</code></pre>
```

The subset leaves out block quotes, ordered lists, indented code, setext headings, thematic breaks, raw HTML, autolinks, images, reference links, character references and `_` emphasis. The file `contract.yaml` lists the exact rules of the input. In the subset, each construct has a clear start, so a small parser can give the same output as a full one.

## Solutions

All the solutions use the two phases of the CommonMark parsing strategy. In the first phase, "lines of input are consumed and the block structure of the document" is built.[^1] "In the second phase, the raw text contents of paragraphs and headings are parsed into sequences of Markdown inline elements".[^1]

The block phase reads the lines in order. A line that starts with three or more backticks opens a fenced code block, which runs to a closing fence, which "must be at least as long as the opening fence", or to the end of the text.[^1] The lines in it are escaped and are not parsed. A heading line gives one `<h1>` to `<h6>` element, and a closing sequence of `#` characters is removed. A `- ` line starts a list, and a list is loose if "any of its constituent list items are separated by blank lines".[^1] The items of a loose list are wrapped in `<p>`. All other lines make a paragraph, which runs until a blank line or the start of a fence, a heading or a list.

The inline phase reads the text of each heading, paragraph and list item from left to right:

- **Backslash escapes.** "Any ASCII punctuation character may be backslash-escaped".[^1] A backslash at the end of a line gives `<br />`.
- **Code spans.** "A code span begins with a backtick string and ends with a backtick string of equal length."[^1] In the content, "line endings are converted to spaces".[^1] Then, if the content "both begins and ends with a space character, but does not consist entirely of space characters, a single space character is removed from the front and back."[^1] A backtick string with no closing string of the same length is plain text.
- **Links.** `[text](url)` gives `<a href="url">`. The text is parsed again for emphasis and code spans.
- **Line breaks.** A line ending "that is preceded by two or more spaces and does not occur at the end of a block is parsed as a hard line break".[^1] The spaces at the end of every line are removed.
- **Emphasis.** Each run of `*` becomes a delimiter. The parser looks at the characters before and after the run, and the start or the end of the text counts as white space. A run is left-flanking if it is "not followed by Unicode whitespace", and either not followed by a Unicode punctuation character, or "followed by a Unicode punctuation character and preceded by Unicode whitespace or a Unicode punctuation character".[^1] A right-flanking run is the mirror of this. A left-flanking run can open emphasis, and a right-flanking run can close it. After the scan, the parser matches each closer with the nearest opener before it, as in the "process emphasis" procedure of the spec. Two characters from each run give `<strong>`, and one gives `<em>`. Each other character is written with `&`, `<`, `>` and `"` escaped.

The flanking rules need two classes of characters. "A Unicode whitespace character is a character in the Unicode Zs general category, or a tab ( U+0009 ), line feed ( U+000A ), form feed ( U+000C ), or carriage return ( U+000D )."[^1] A Unicode punctuation character is a character in the P (punctuation) or S (symbol) general categories.[^1] Each language has a function for the general category:

- **Python.** `unicodedata.category` "Returns the general category assigned to the character chr as string."[^2] The code tests whether the category starts with `P` or `S`, or is `Zs`.
- **JavaScript and TypeScript (Node.js).** The regular expressions `[\p{P}\p{S}]` and `\p{Zs}` test the category. `\p` is "only supported in Unicode-aware mode", so the expressions have the `u` flag.[^3] `Array.from(text)` splits the text into an array of code points, because the string iterator "yields the Unicode code points of the string value as individual strings".[^4] The TypeScript file adds types only.
- **Go.** `unicode.IsPunct` "reports whether the rune is a Unicode punctuation character (category P )".[^5] `unicode.IsSymbol` "reports whether the rune is a symbolic character", and `unicode.Zs` is the set of characters "in category Zs (Separator, space)".[^5] The text is converted to a `[]rune`, and Go's `regexp` package handles the block patterns.
- **C#.** `string.EnumerateRunes` gives the text as `Rune` values. `Rune.IsPunctuation` and `Rune.IsSymbol` return whether a rune "is categorized as a punctuation mark" or "as a symbol character".[^6]

Java, Rust, C, C++ and Bash have no Markdown parser in their standard library, so this page has no solution in those languages. The file `matrix.yaml` names a CommonMark library for each one.

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`. Each case gives the Markdown text and the HTML that the solution must return. The HTML of each case comes from commonmark.js 0.31.2.

- `heading-and-paragraph`, `paragraph-lines`, `bullet-list`, `fenced-code`, `code-span` and `link`: the common shapes. The lines of a paragraph stay on separate lines in the HTML.
- `empty`, `blank-lines-only` and `no-final-newline`: an empty text or a text of blank lines gives an empty string, and a text with no LF at the end gives the same HTML as one with it.
- `paragraph-ends-at-block`: a heading or a fence ends a paragraph with no blank line before it.
- `html-chars`: `a < b & "c" > d` becomes `a &lt; b &amp; &quot;c&quot; &gt; d`.
- `backslash-escape`: `\*not emphasis\*` gives `*not emphasis*`.
- `intraword-star` and `intraword-underscore`: `foo*bar*baz` gives emphasis in a word, but `snake_case_name` stays as it is. The subset has no `_` emphasis, and in CommonMark "Emphasis with _ is not allowed inside words".[^1]
- `star-with-spaces`: in `a * b * c`, each `*` has a space on both sides, so it cannot open or close emphasis.
- `triple-star`, `unmatched-star` and `rule-of-three`: `***both***` gives `<em><strong>both</strong></em>`, `**foo*` gives `*<em>foo</em>`, and `*foo**bar**baz*` gives `<em>foo<strong>bar</strong>baz</em>`. The last case needs the rule that "the sum of the lengths of the delimiter runs containing the opening and closing delimiters must not be a multiple of 3 unless both lengths are multiples of 3".[^1]
- `punctuation-flanking`: `*"quoted"*` gives emphasis, but in `a*"b"*` the first `*` has a letter before it and a quotation mark after it, so it cannot open.
- `code-span-backticks`, `code-span-precedence` and `unclosed-backtick`: `` `` a ` b `` `` holds a backtick. In `` *a `*` b* `` the `*` in the code span is not a delimiter, because "Code span backticks have higher precedence than any other inline constructs except HTML tags and autolinks."[^1]
- `hard-break-spaces`, `hard-break-backslash` and `trailing-spaces`: two spaces or a backslash at the end of a line give `<br />`, but not at the end of a paragraph.
- `heading-closing`, `heading-empty` and `not-a-heading`: `## Title ##` gives `<h2>Title</h2>`, `#` alone gives an empty `<h1>`, and `#hashtag` and seven `#` characters give paragraphs.
- `list-interrupts-paragraph` and `loose-list`: a `- ` line ends a paragraph, and a blank line between items makes the list loose.
- `unclosed-fence`, `longer-fence`, `fence-info-words` and `fence-keeps-markup`: a fence with no end runs to the end of the text, a fence of four backticks holds a line of three, only the first word of the info string becomes the class, and `#` and `*` in the code stay as they are.
- `link-with-emphasis`, `not-a-link` and `link-ampersand`: emphasis can be inside a link or around it, `[a] (u)` with a space is not a link, and the `&` in a URL becomes `&amp;`.
- `non-ascii`: `Zürich *東京* 🙂` keeps each character.

## Pitfalls

- **A regular expression for each construct gives the wrong HTML.** A converter can replace `**...**` and then `*...*` with regular expressions. Such a converter does not use the flanking rules, they change the text inside code spans, and they do not escape HTML:

```python wrong id=naive-regex
import re


def naive(text):
    text = re.sub(r"\*\*(.+?)\*\*", r"<strong>\1</strong>", text)
    return re.sub(r"\*(.+?)\*", r"<em>\1</em>", text)


for text in ["a * b * c", "*a `*` b*", "2 < 3"]:
    print(naive(text))
```

```output for=naive-regex
a <em> b </em> c
<em>a `</em>` b*
2 < 3
```

CommonMark gives `a * b * c`, `<em>a <code>*</code> b</em>` and `2 &lt; 3`. The solutions scan each inline text once and decide about emphasis only after all the code spans and links are known.

- **A parser that reads each line alone breaks code blocks.** A line in a fenced code block is code, also if it starts with `#` or `- `. The parser must know whether a fence is open before it looks at the line. The case `fence-keeps-markup` tests this.

- **The text in the HTML must be escaped.** If the converter writes `<`, `>` and `&` as they are, a text such as `<script>` runs in the browser of each reader. See [[cross-site-scripting]]. The solutions escape all text, also the code and the URLs.

- **A full converter passes raw HTML through.** CommonMark allows raw HTML in the text. The README of commonmark.js says: "The library does not attempt to sanitize link attributes or raw HTML."[^7] Its `safe` option replaces raw HTML with comments, and it replaces "potentially unsafe URLs in links and images (those beginning with `javascript:`, `vbscript:`, `file:`, and with a few exceptions `data:`)" with empty strings.[^7] The README of cmark says: "By default, the library will scrub raw HTML and potentially dangerous links".[^8] For its unsafe option, the README says "we recommend you use a HTML sanitizer specific to your needs".[^8] If the text comes from users, use the safe mode of the library or a sanitizer.

- **A JavaScript string index is a UTF-16 code unit, not a character.** A character outside the BMP, such as an emoji, is two code units, and each one alone is not punctuation:

```javascript wrong id=js-code-units
const isPunct = (c) => /^[\p{P}\p{S}]$/u.test(c);
const text = "🙂*";
console.log(text.length, Array.from(text).length);
console.log(isPunct(text[text.length - 2]), isPunct(Array.from(text)[0]));
```

```output for=js-code-units
3 2
false true
```

The second line shows that the emoji matches `[\p{P}\p{S}]`, so it is a Unicode punctuation character for the flanking rules. A parser that reads `text[i - 1]` sees half of it and decides differently about the `*` after it. The JavaScript and TypeScript solutions use `Array.from`, and the C# solution uses `Rune`, because "some of the char methods won't work correctly on char instances that are in the surrogate range."[^6] The contract keeps such characters away from `*` runs, so each case has one HTML result.

## Related tasks

- [[markdown]]: the Markdown format and its history.
- [[commonmark]]: the CommonMark spec and its reference implementations.
- [[cross-site-scripting]]: why HTML output must escape the text.

## References
