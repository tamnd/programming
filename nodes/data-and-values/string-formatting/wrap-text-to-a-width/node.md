---
tag: KBZ8FA
kind: task
slug: wrap-text-to-a-width
name: Wrap text to a width
aliases: [word wrap, line wrap, wrap a paragraph, fill a paragraph, line breaking, greedy word wrap, textwrap, break text into lines, wrap at column, fold text]
area: data-and-values/string-formatting
level: core
summary: >-
  Break a text into lines of at most a given width with the greedy method: put as many words as fit on a line, then start the next one. A word longer than the width gets a line of its own.
contract: contract.yaml
category: text-and-strings
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
- url: https://github.com/acmeism/RosettaCodeData/blob/main/Task/Word-wrap/00-TASK.txt
  title: Rosetta Code, Word wrap (task text, from acmeism/RosettaCodeData)
  locator: task text
- url: https://en.wikipedia.org/wiki/Wrapping_(text)
  title: Wrapping (text), Wikipedia
  locator: Algorithm; Minimum number of lines
- url: https://docs.python.org/3/library/textwrap.html
  title: textwrap, Text wrapping and filling, Python documentation
  locator: textwrap.wrap; TextWrapper.break_long_words; TextWrapper.replace_whitespace
---

## Task

Break a text into lines that are at most `width` characters long, for a terminal, a commit message, a comment or a plain text email. The Rosetta Code task Word wrap asks to "wrap a paragraph of text in a simple way in your language"[^1], with a function from the standard library if there is one, and otherwise with the "minimum length greedy algorithm"[^1].

The function `wrap(text, width)` returns the list of lines:

1. Split the text into words at runs of white space: spaces, tabs and newlines.
2. Put a word on the current line if the line stays at most `width` characters long, with one space between two words.
3. Otherwise, start a new line with the word.
4. A word longer than `width` is not cut. It is a line of its own.

For example, `wrap("The quick brown fox jumps over the lazy dog", 10)` is `["The quick", "brown fox", "jumps over", "the lazy", "dog"]`. An empty text, or a text with only white space, gives no lines.

Wikipedia says that "Word wrapping is an optimization problem"[^2]. The greedy method "puts as many words on a line as possible, then moving on to the next line to do the same until there are no more words left to place"[^2]. It "always uses the minimum possible number of lines but may lead to lines of widely varying lengths"[^2]. A typesetting system such as TeX looks at the whole paragraph to make the lines more even, but the greedy method is the usual one for plain text.

## Solutions

Python has the module `textwrap`. In the other languages of this site, the standard library has no function that wraps text, so the solution is a loop of about 15 lines.

| Language | Split into words | Wrap |
|---|---|---|
| Python | `text.split()` | `textwrap.wrap(text, width, break_long_words=False, break_on_hyphens=False)`, or a loop |
| JavaScript, TypeScript | `text.split(/\s+/).filter(Boolean)` | a loop |
| Java | `text.trim().split("\\s+")` | a loop with a `StringBuilder` |
| C# | `text.Split(separators, StringSplitOptions.RemoveEmptyEntries)` | a loop with a `StringBuilder` |
| Go | `strings.Fields(text)` | a loop |
| Rust | `text.split_whitespace()` | a loop |
| C | a scan with `isspace` | a loop that writes into a buffer |
| C++ | `std::istringstream` and `>>` | a loop |
| Bash | `read -r -d '' -a words` | a loop |

- **Python.** `textwrap.wrap` "Wraps the single paragraph in text (a string) so every line is at most width characters long"[^3]. By default it cuts a long word. With `break_long_words=False`, "long words will not be broken, and some lines may be longer than width. (Long words will be put on a line by themselves, in order to minimize the amount by which width is exceeded.)"[^3] The option `break_on_hyphens=False` stops it from breaking a line after a hyphen in a word.
- **The loop.** The other solutions keep the current line. For each word, if the line is empty, the line becomes the word. If the length of the line, plus 1 for the space, plus the length of the word is at most `width`, the word goes on the line. Otherwise, the line goes into the result and the word starts a new line. At the end, a line that is not empty goes into the result.
- **C.** The solution writes the lines into a buffer from the caller, with a newline after each line, and returns the count of lines.
- **Bash.** `read -r -d '' -a words` splits the whole text, with its newlines, into an array. `read` returns 1 at the end of the input, so the solution adds `|| true`. The function prints one line for each line of the result.

The Python loop, and `textwrap` for the same long word:

```python run id=python-wrap
import textwrap


def wrap(text, width):
    lines = []
    line = ""
    for word in text.split():
        if not line:
            line = word
        elif len(line) + 1 + len(word) <= width:
            line += " " + word
        else:
            lines.append(line)
            line = word
    if line:
        lines.append(line)
    return lines


for line in wrap("The quick brown fox jumps over the lazy dog", 10):
    print(f"|{line:<10}|")
print(wrap("a supercalifragilistic word", 10))
print(textwrap.wrap("a supercalifragilistic word", 10))
print(textwrap.wrap("a supercalifragilistic word", 10, break_long_words=False))
```

```output for=python-wrap
|The quick |
|brown fox |
|jumps over|
|the lazy  |
|dog       |
['a', 'supercalifragilistic', 'word']
['a supercal', 'ifragilist', 'ic word']
['a', 'supercalifragilistic', 'word']
```

With the default options, `textwrap` cuts the long word to fill the lines.

## Edge cases

- **Empty text.** No words, so no lines. The result is an empty list, not a list with one empty line.
- **Only white space.** The same as empty text.
- **A text that fits.** One line: `wrap("short text", 20)` is `["short text"]`.
- **A line of exactly `width` characters.** It is allowed: `wrap("ab cd ef", 5)` is `["ab cd", "ef"]`. The test is "at most", not "less than".
- **A word longer than `width`.** It is a line of its own, and that line is longer than `width`. To cut it, the program must choose where, for example with a hyphen.
- **Runs of white space.** Tabs, newlines and many spaces count as one break: `wrap("one   two\tthree\nfour", 9)` is `["one two", "three", "four"]`.
- **Width 1.** Each word is a line.

## Pitfalls

- **An off by one in the test.** The space between two words also takes a column. A loop that adds only the length of the word makes lines that are 1 character too long:

```javascript wrong id=javascript-wrap-no-space
function wrap(text, width) {
  const lines = [];
  let line = "";
  for (const word of text.split(/\s+/).filter(Boolean)) {
    if (line && line.length + word.length > width) {
      lines.push(line);
      line = word;
    } else {
      line = line ? line + " " + word : word;
    }
  }
  if (line) lines.push(line);
  return lines;
}

const lines = wrap("abc de fg", 5);
console.log(JSON.stringify(lines), lines.map((l) => l.length));
```

```output for=javascript-wrap-no-space
["abc de","fg"] [ 6, 2 ]
```

The test must be `line.length + 1 + word.length > width`.

- **Characters are not columns.** The contract uses ASCII. With other text, the length of a string is not its width on the screen: a letter with an accent can be 2 code points, and a Chinese character takes 2 columns on a terminal.
- **The text keeps its spaces in `textwrap`.** `textwrap` keeps the spaces between two words on one line, so `textwrap.wrap("one   two", 9)` is `['one   two']`. The loop of this task puts one space between two words.
- **Paragraphs.** The function wraps one paragraph. To keep the paragraphs of a text, split the text at the empty lines first, and wrap each paragraph.
- **Words with a hyphen.** By default, `textwrap` can break a line after a hyphen. The loop of this task does not break a word.

## Related tasks

- [[pad-a-string]]: pad each line to the width, to align the text on the right or in a box.
- [[print-formatted-output]]: columns of a fixed width.

## References
