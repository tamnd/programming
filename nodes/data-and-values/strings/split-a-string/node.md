---
tag: W33A9B
kind: task
slug: split-a-string
name: Split a string
aliases: [split a string on a delimiter, tokenize a string, split a string into fields, explode a string]
area: data-and-values/strings
level: core
summary: Split a string into fields on a literal separator, keep the empty fields, stop after a limit, and also split on white space and on a regular expression.
contract: contract.yaml
category: text-and-strings
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
  - url: https://docs.python.org/3/library/stdtypes.html#str.split
    title: The Python Standard Library, Built-in Types, str.split
    locator: str.split(sep=None, maxsplit=-1); str.isspace
  - url: https://docs.python.org/3/library/re.html#re.split
    title: The Python Standard Library, re, Regular expression operations
    locator: re.split
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/String/split
    title: MDN Web Docs, String.prototype.split()
    locator: Parameters (separator, limit); Description
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/lang/String.html#split(java.lang.String,int)
    title: Java SE 25 API, java.lang.String
    locator: split(String, int), split(String)
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/regex/Pattern.html
    title: Java SE 25 API, java.util.regex.Pattern
    locator: Predefined character classes (\s); UNICODE_CHARACTER_CLASS; quote(String); split(CharSequence, int)
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.string.split?view=net-10.0
    title: .NET API reference, String.Split
    locator: Split(Char[], Int32, StringSplitOptions), Remarks; Split(String, Int32, StringSplitOptions), Remarks
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.text.regularexpressions.regex.split?view=net-10.0
    title: .NET API reference, Regex.Split
    locator: Split(String, String), Remarks
  - url: https://pkg.go.dev/strings#Split
    title: Go standard library, package strings
    locator: Split, SplitN, Fields
  - url: https://pkg.go.dev/regexp#Regexp.Split
    title: Go standard library, package regexp
    locator: package overview; Regexp.Split
  - url: https://doc.rust-lang.org/std/primitive.str.html#method.split
    title: The Rust Standard Library, primitive type str
    locator: split, splitn, split_whitespace, split_ascii_whitespace
  - url: https://docs.rs/regex/latest/regex/
    title: The regex crate for Rust, crate documentation
  - url: https://www.rfc-editor.org/rfc/rfc3629.html
    title: F. Yergeau, RFC 3629, UTF-8, a transformation format of ISO 10646 (2003)
    locator: Section 3, UTF-8 definition
  - url: https://pkg.go.dev/unicode#IsSpace
    title: Go standard library, package unicode
    locator: IsSpace
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Lexical_grammar#white_space
    title: MDN Web Docs, Lexical grammar
    locator: White space
  - url: https://en.cppreference.com/w/c/string/byte/strtok
    title: cppreference.com, strtok, strtok_s
  - url: https://pubs.opengroup.org/onlinepubs/9799919799/functions/regcomp.html
    title: The Open Group, POSIX.1-2024, regcomp, regerror, regexec, regfree
    locator: Description (REG_EXTENDED, REG_NOTBOL, pmatch[0])
  - url: https://en.cppreference.com/w/cpp/ranges/split_view
    title: cppreference.com, std::ranges::split_view, std::views::split
    locator: Notes
  - url: https://www.gnu.org/software/bash/manual/bash.html#Word-Splitting
    title: GNU Bash Reference Manual, Word Splitting
  - url: https://www.gnu.org/software/bash/manual/bash.html#Shell-Parameter-Expansion
    title: GNU Bash Reference Manual, Shell Parameter Expansion
    locator: "${parameter#word}, ${parameter%%word}"
  - url: https://www.gnu.org/software/bash/manual/bash.html#Pattern-Matching
    title: GNU Bash Reference Manual, Pattern Matching
  - url: https://www.gnu.org/software/bash/manual/bash.html#index-read
    title: GNU Bash Reference Manual, Bash Builtin Commands
    locator: read, the -a and -d options
  - url: https://www.gnu.org/software/bash/manual/bash.html#Conditional-Constructs
    title: GNU Bash Reference Manual, Conditional Constructs
    locator: "[[ expression ]], the =~ operator and BASH_REMATCH"
---

## Task

Split a string into fields on a separator. The separator is a literal string of one or more characters. It is not a pattern, so a `.` or a `|` in it matches only itself.

Keep every empty field. Two separators side by side give an empty field between them. A separator at the start or at the end gives an empty field there. So a string with k separators gives k + 1 fields, and the empty string gives one empty field. In data such as CSV an empty field is a value, so the split must keep it.

The search for the separator goes from left to right, and two matches do not overlap. So `"aaa"` split on `"aa"` gives `""` and `"a"`.

A limit n stops the split after n - 1 separators. The last field then holds the rest of the string, with its separators. A limit of 0 means no limit.

Each solution also has two related functions. `split_whitespace` splits on runs of white space and gives no empty fields, as when you read words. `split_regex` splits on each match of a regular expression, with the same rules as the main function and no limit.

## Solutions

- **Python.** `s.split(sep, maxsplit)` keeps empty fields and puts the rest in the last field. Its `maxsplit` counts splits, not fields, so the code passes `limit - 1`. `s.split()` with no separator splits on runs of white space. `re.split` splits on a regular expression.
- **JavaScript and TypeScript.** `s.split(sep)` keeps empty fields. Its `limit` argument drops the rest of the string, so the code splits fully and joins the rest again. `s.split(/\s+/)` with a filter splits on white space, and `split` with a `RegExp` splits on a pattern. The TypeScript file adds types only.
- **Java.** `String.split` takes a regular expression, so the code quotes the separator with `Pattern.quote`. A limit of 0 removes the trailing empty strings, so the code gives -1 for no limit. `Pattern.split(s, -1)` splits on a pattern.
- **C#.** `s.Split(sep)` and `s.Split(sep, count)` take a string separator, keep empty fields and put the rest in the last field. `Split((char[]?)null, StringSplitOptions.RemoveEmptyEntries)` splits on white space. `Regex.Split` splits on a pattern.
- **Go.** `strings.Split` and `strings.SplitN` match the contract. `SplitN` with n = 0 returns nil, so the code calls `Split` for no limit. `strings.Fields` splits on white space, and `Regexp.Split(s, -1)` splits on a pattern.
- **Rust.** `s.split(sep)` and `s.splitn(n, sep)` return iterators of slices that borrow from `s`. `split_ascii_whitespace` splits on ASCII white space. A pattern can also be a `char`, a slice of `char` values or a closure. The standard library has no regular expressions, so the Rust solution has no `split_regex`. The `regex` crate supplies them.
- **C.** The C function `strtok` skips empty fields (see Pitfalls). So the code finds each separator with `strstr`. It returns an array of fields that point into the input, and it copies no text. `split_regex` uses POSIX `regcomp` and `regexec` with `REG_EXTENDED`.
- **C++.** A loop with `std::string_view::find` returns views into the input. `std::views::split` also splits a `string_view`, but it has no limit. `split_regex` calls `std::regex_search` in a loop.
- **Bash.** A loop takes the text before the first separator with `${s%%"$sep"*}` and removes it with `${s#*"$sep"}`. The quotes make the separator literal, because a quoted pattern character matches itself. `read -a` with `IFS` set to space, tab and newline splits on runs of white space. `[[ $s =~ $re ]]` finds a regular expression match, and the code cuts the string at that match.

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`. The cases of `split_whitespace` and `split_regex` are in `cases/whitespace.jsonl` and `cases/regex.jsonl`.

- `empty`: the empty string gives one empty field, `[""]`, and not an empty list. `split_whitespace` gives `[]` for it.
- `leading-separator`, `trailing-separators` and `adjacent-separators`: each gives an empty field. Java `split` with no limit, C `strtok` and Bash `read` lose some of them.
- `only-separator`: `","` gives two empty fields.
- `overlapping-match`: the search goes on after the end of a match, so `"aaa"` on `"aa"` gives `["", "a"]`.
- `dot-separator` and `pipe-separator`: in a regular expression, `.` matches any character, and `|` alone matches the empty string. So a function that takes a pattern gives the wrong fields for these separators.
- `non-ascii`: the separator `→` is 3 bytes in UTF-8. The C, C++, Go and Rust solutions search bytes. In UTF-8 a first byte never has the form of a continuation byte, so a byte search for a whole character never matches inside another character.
- `limit-one`, `limit-above-count` and `limit-keeps-trailing-empty`: a limit of 1 gives the whole string, and a limit above the number of fields has no effect.
- An empty separator is not in the contract. Go `strings.Split` splits after each UTF-8 sequence, and JavaScript `split("")` splits into UTF-16 code units. Rust `split("")` also gives an empty string at the start and at the end.

## Pitfalls

- **Java `split` takes a regular expression and drops trailing empty strings.** The `.` matches every character, so every field is empty, and the limit of 0 removes all of them:

```java wrong id=java-split-regex
import java.util.Arrays;

public class Main {
  public static void main(String[] args) {
    System.out.println(Arrays.toString("192.168.0.1".split(".")));
    System.out.println(Arrays.toString("a,b,,".split(",")));
  }
}
```

```output
[]
[a, b]
```

Quote the separator, and give a negative limit:

```java run id=java-split-quoted
import java.util.Arrays;
import java.util.regex.Pattern;

public class Main {
  public static void main(String[] args) {
    System.out.println(Arrays.toString("192.168.0.1".split(Pattern.quote("."), -1)));
    System.out.println(Arrays.toString("a,b,,".split(Pattern.quote(","), -1)));
  }
}
```

```output
[192, 168, 0, 1]
[a, b, , ]
```

- **The JavaScript limit drops the rest.** Python, Go, Rust, Java and C# put the rest of the string in the last field. JavaScript stops when it has `limit` fields and does not return the rest:

```javascript wrong id=js-split-limit
console.log("key=value=more".split("=", 2));
```

```output
[ 'key', 'value' ]
```

- **Python `split()` and `split(" ")` differ.** With no argument, Python splits on runs of white space and drops the empty strings at the ends. With `" "`, each space is a separator:

```python run id=python-split-space
text = "  two  words "
print(text.split())
print(text.split(" "))
```

```output
['two', 'words']
['', '', 'two', '', 'words', '']
```

- **`IFS` splitting in Bash drops a trailing empty field.** A non-white-space `IFS` character ends a field, so a separator at the end does not start another field. The input has four fields, but `read` gives three:

```bash wrong id=bash-ifs-trailing
IFS=, read -r -a fields <<<'a,b,,'
printf '%d fields:' "${#fields[@]}"
printf ' [%s]' "${fields[@]}"
echo
```

```output
3 fields: [a] [b] []
```

- **C `strtok` skips empty fields and changes the input.** It treats its second argument as a set of characters and skips a run of them. It also writes null characters into the string, and it keeps its position in a static variable, so it is not thread-safe:

```c wrong id=c-strtok
#include <stdio.h>
#include <string.h>

int main(void) {
    char line[] = "a,,b,";
    for (char *field = strtok(line, ","); field != NULL; field = strtok(NULL, ",")) {
        printf("[%s]\n", field);
    }
}
```

```output
[a]
[b]
```

- **A string literal as the C++ `std::views::split` delimiter.** A `const char` array includes its terminating null character, so the delimiter becomes `", \0"`. That never occurs, and the result is one field:

```cpp wrong id=cpp-split-literal
#include <print>
#include <ranges>
#include <string_view>

int main() {
    std::string_view text = "a, b, c";
    for (auto field : std::views::split(text, ", ")) {
        std::println("[{}]", std::string_view(field));
    }
}
```

```output
[a, b, c]
```

Use a `std::string_view` delimiter:

```cpp run id=cpp-split-view
#include <print>
#include <ranges>
#include <string_view>

int main() {
    using namespace std::string_view_literals;
    std::string_view text = "a, b, c";
    for (auto field : std::views::split(text, ", "sv)) {
        std::println("[{}]", std::string_view(field));
    }
}
```

```output
[a]
[b]
[c]
```

- **The languages do not agree on white space.** Python `split()`, Go `strings.Fields`, Rust `split_whitespace`, JavaScript `\s` and C# `Char.IsWhiteSpace` use a Unicode definition of white space. Each of them splits on U+00A0 NO-BREAK SPACE. Java `\s` matches only `[ \t\n\x0B\f\r]`, unless the pattern sets `UNICODE_CHARACTER_CLASS` or `(?U)`. Rust `split_ascii_whitespace` and the C and Bash solutions also split only on ASCII characters. So the contract allows only space, tab and LF:

```java run id=java-whitespace-nbsp
public class Main {
  public static void main(String[] args) {
    String text = "no\u00a0break";
    System.out.println("fields with \\s: " + text.split("\\s+").length);
    System.out.println("fields with (?U)\\s: " + text.split("(?U)\\s+").length);
  }
}
```

```output
fields with \s: 1
fields with (?U)\s: 2
```

- **Capturing groups in a split pattern.** Python `re.split`, JavaScript `split` and .NET `Regex.Split` put the text of each capturing group in the result, between the fields. Use a non-capturing group `(?:...)` in a split pattern.

## Related tasks

- [[reverse-a-string]]: another job where bytes, code units and characters are not the same.
- [[sort-a-list-of-records]]: sort the records after you split each line into fields.
- [[read-an-environment-variable]]: a value such as a list of paths needs a split.
- [[parse-command-line-arguments]]: read options and file names from the argument list.

## References
