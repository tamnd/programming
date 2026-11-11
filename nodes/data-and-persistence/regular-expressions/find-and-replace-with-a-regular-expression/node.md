---
tag: AKFJ8P
kind: task
slug: find-and-replace-with-a-regular-expression
name: Find and replace with a regular expression
aliases: [regex replace, replace with a regex, find all matches of a regex, use capture groups, escape a string for a regex, regex substitution with a back reference]
area: data-and-persistence/regular-expressions
level: core
summary: Find each date YYYY-MM-DD in a text with a regular expression, return its capture groups, and write it as DD/MM/YYYY through references to the groups, with literal separators.
contract: contract.yaml
category: regular-expressions
languages: [python, javascript, typescript, java, c, cpp, csharp, go, bash]
sources:
  - url: https://docs.python.org/3/library/re.html
    title: The Python Standard Library, re
    locator: "Regular Expression Syntax: \\d, (?P<name>...), (?<!...), (?!...), possessive quantifiers; Flags: re.ASCII; Functions: re.match, re.fullmatch, re.sub (\\g<name>), re.escape; search() vs. match()"
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/String/replace
    title: MDN Web Docs, String.prototype.replace()
    locator: Specifying a string as the replacement
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/String/replaceAll
    title: MDN Web Docs, String.prototype.replaceAll()
    locator: Exceptions
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/RegExp/escape
    title: MDN Web Docs, RegExp.escape()
    locator: Description
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/RegExp/test
    title: MDN Web Docs, RegExp.prototype.test()
    locator: Description, lastIndex with the global flag
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Regular_expressions/Character_class_escape
    title: MDN Web Docs, Character class escape
    locator: Description, \d
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/regex/Pattern.html
    title: Java SE 25 API, java.util.regex.Pattern
    locator: Predefined character classes (\d); Groups and capturing, named groups; quote(String); UNICODE_CHARACTER_CLASS
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/regex/Matcher.html
    title: Java SE 25 API, java.util.regex.Matcher
    locator: appendReplacement(StringBuilder, String); replaceAll(Function); results(); quoteReplacement(String)
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.text.regularexpressions.regex.escape?view=net-10.0
    title: .NET API reference, Regex.Escape
    locator: Remarks
  - url: https://learn.microsoft.com/en-us/dotnet/standard/base-types/substitutions-in-regular-expressions
    title: .NET, Substitutions in regular expressions
    locator: the table of substitutions; Substituting a "$" Character
  - url: https://learn.microsoft.com/en-us/dotnet/standard/base-types/grouping-constructs-in-regular-expressions
    title: .NET, Grouping constructs in regular expressions
    locator: Named matched subexpressions; Zero-width negative lookbehind assertions
  - url: https://learn.microsoft.com/en-us/dotnet/standard/base-types/character-classes-in-regular-expressions
    title: .NET, Character classes in regular expressions
    locator: "Decimal digit character: \\d"
  - url: https://learn.microsoft.com/en-us/dotnet/standard/base-types/regular-expression-options
    title: .NET, Regular expression options
    locator: the table of options, NonBacktracking
  - url: https://learn.microsoft.com/en-us/dotnet/standard/base-types/backtracking-in-regular-expressions
    title: .NET, Backtracking in regular expressions
    locator: Backtracking with nested optional quantifiers; Specify a time-out interval
  - url: https://pkg.go.dev/regexp
    title: Go standard library, regexp
    locator: Overview (linear time); func QuoteMeta; func (*Regexp) Expand; func (*Regexp) FindStringSubmatchIndex
  - url: https://pkg.go.dev/regexp/syntax
    title: Go standard library, regexp/syntax
    locator: "Overview: Grouping ((?P<name>re), (?<name>re)); Perl character classes (\\d)"
  - url: https://docs.rs/regex/1.13.1/regex/
    title: The regex crate 1.13.1 for Rust
    locator: "Overview (no look-around, worst case O(m * n)); Perl character classes (\\d is \\p{Nd})"
  - url: https://docs.rs/regex/1.13.1/regex/struct.Regex.html
    title: The regex crate 1.13.1, struct Regex
    locator: captures_at; replace (the replacement string, $$, NoExpand)
  - url: https://docs.rs/regex/1.13.1/regex/fn.escape.html
    title: The regex crate 1.13.1, fn escape
    locator: Description
  - url: https://en.cppreference.com/w/cpp/regex/ecmascript.html
    title: cppreference.com, Modified ECMAScript regular expression grammar
    locator: Assertions
  - url: https://en.cppreference.com/w/cpp/regex.html
    title: cppreference.com, Regular expressions library
    locator: Regular expression grammars (the default); Algorithms (regex_match, regex_search, regex_replace)
  - url: https://pubs.opengroup.org/onlinepubs/9799919799/functions/regcomp.html
    title: The Open Group Base Specifications Issue 8, regcomp, regexec, regerror, regfree
    locator: DESCRIPTION (REG_EXTENDED, regmatch_t, rm_so and rm_eo)
  - url: https://pubs.opengroup.org/onlinepubs/9799919799/basedefs/V1_chap09.html
    title: The Open Group Base Specifications Issue 8, Base Definitions, Regular Expressions
    locator: 9.4.2 ERE Ordinary Characters; 9.4.3 ERE Special Characters
  - url: https://www.gnu.org/software/bash/manual/bash.html#Conditional-Constructs
    title: GNU Bash Reference Manual
    locator: "3.2.5.2 Conditional Constructs, [[ ... ]] and the =~ operator; 5.2 Bash Variables, BASH_REMATCH"
---

## Task

Find each date in a text and write it in another order. A date is `YYYY-MM-DD` with ASCII digits and no ASCII digit just before it or just after it. The result has `DD/MM/YYYY` in its place. The two separators are parameters, and each one is a literal string.

The contract has two entries:

- `reformat_dates(text, from_sep, to_sep)` returns the text with each date `YYYY<from_sep>MM<from_sep>DD` changed to `DD<to_sep>MM<to_sep>YYYY`. The rest of the text does not change.
- `find_dates(text, sep)` returns the list `[year, month, day]` of the capture groups of each date, in order. Its cases are in `cases/find.jsonl`.

The search goes from left to right, and two dates do not overlap. A match that fails the digit check does not hide a date that starts later inside it. The task checks the shape of a date only, so `2024-13-45` is a date here.

## Solutions

Each solution makes the pattern from three parts. The year, month and day are capture groups of `[0-9]` digits. The separator is escaped, so a `.` in it matches only a dot. The digit check uses the lookbehind `(?<![0-9])` and the lookahead `(?![0-9])` where the engine has them. The replacement refers to the groups by name, so a digit after a reference is not part of the reference. The separator in the replacement is escaped for the replacement syntax.

- **Python.** `re.escape` escapes the separator. `re.sub` replaces each match, and `\g<day>` refers to the group `(?P<day>...)`. In a replacement string, a backslash starts an escape, so the code doubles each backslash of `to_sep`. `finditer` and `Match.groups()` give the groups for `find_dates`.
- **JavaScript and TypeScript (Node.js).** `RegExp.escape` escapes the separator. `replaceAll` needs the flag `g`, and it throws a `TypeError` without it. In the replacement, `$<day>` refers to a named group and `$$` gives one `$`, so the code changes each `$` of `toSep` to `$$`. `matchAll` gives the groups.
- **Java.** `Pattern.quote` escapes the separator. `Matcher.replaceAll` takes `${day}` for a named group. `Matcher.quoteReplacement` escapes `$` and `\` in `toSep`. `Matcher.results()` gives one `MatchResult` for each match.
- **C#.** `Regex.Escape` escapes the separator. `Regex.Replace` takes `${day}`, and `$$` gives one `$`. `Matches` gives the groups.
- **Go.** `regexp.QuoteMeta` escapes the separator. Go does not accept lookbehind or lookahead (see the pitfalls), so the pattern has no digit check. A loop calls `FindStringSubmatchIndex` and checks the bytes next to each match. If the check fails, the next search starts one byte after the start of the failed match, so a date inside it is still found. `Expand` writes the template `${day}` and the other groups.
- **C.** `regex.h` has `regcomp`, `regexec`, `regerror` and `regfree`. None of them escapes a string or replaces a match, so `reformat_dates.c` does both. It puts a backslash before each ERE special character of the separator. `regcomp` with `REG_EXTENDED` compiles the pattern. `regexec` gives a `regmatch_t` for each group, with the byte offsets `rm_so` and `rm_eo`. The digit check and the restart are the same as in Go.
- **C++.** `std::regex` uses the modified ECMAScript grammar by default. Its assertions are `^`, `$`, `\b`, `\B`, `(?=...)` and `(?!...)`, so it has no lookbehind. The library has `regex_match`, `regex_search` and `regex_replace`, and no function that escapes a string, so the code has its own. A loop calls `regex_search` and does the digit check. The result is built from `m.str(3)`, `m.str(2)` and `m.str(1)`, so the separator is never read as a format.
- **Bash.** `[[ string =~ regex ]]` matches a POSIX extended regular expression, and a quoted part of the pattern matches as a literal string. So `"$3"` in the pattern is the separator with no escape. `BASH_REMATCH` holds the groups. The function tries a match at each position with `^`, checks the digits next to it, and builds the result in a string. `local LC_ALL=C` makes each byte one character, so the offsets count bytes.

Rust has no regular expressions in its standard library. This page uses the crate `regex` at version 1.13.1, so the Rust program is a page example and not a tested solution. The file `matrix.yaml` records this. The crate has no look-around, so the code does the digit check as in Go. `captures_at` starts a search at an offset and still sees the text before it:

```rust run id=rust-regex deps=deps/rust/Cargo.lock
use regex::{Captures, Regex};

/// Returns the pattern of a date YYYY<sep>MM<sep>DD. The crate has no
/// look-around, so the pattern does not check the digits next to the date.
fn date_pattern(sep: &str) -> Regex {
    // regex::escape makes each special character of sep match only itself.
    let s = regex::escape(sep);
    Regex::new(&format!(r"(?<year>[0-9]{{4}}){s}(?<month>[0-9]{{2}}){s}(?<day>[0-9]{{2}})")).unwrap()
}

fn is_digit_at(text: &str, i: usize) -> bool {
    text.as_bytes().get(i).is_some_and(u8::is_ascii_digit)
}

/// Returns the captures of each date in text that has no digit next to it.
fn dates<'t>(re: &Regex, text: &'t str) -> Vec<Captures<'t>> {
    let mut found = Vec::new();
    let mut pos = 0;
    while let Some(c) = re.captures_at(text, pos) {
        let m = c.get(0).unwrap();
        if (m.start() > 0 && is_digit_at(text, m.start() - 1)) || is_digit_at(text, m.end()) {
            // A date can start inside this match. m.start() is an ASCII digit,
            // so m.start() + 1 is the start of a character.
            pos = m.start() + 1;
            continue;
        }
        pos = m.end();
        found.push(c);
    }
    found
}

/// Changes each date YYYY<from_sep>MM<from_sep>DD in text to DD<to_sep>MM<to_sep>YYYY.
fn reformat_dates(text: &str, from_sep: &str, to_sep: &str) -> String {
    let mut out = String::new();
    let mut last = 0;
    for c in dates(&date_pattern(from_sep), text) {
        let m = c.get(0).unwrap();
        out.push_str(&text[last..m.start()]);
        // to_sep is pushed as it is, so a $ in it has no special meaning.
        for part in [&c["day"], to_sep, &c["month"], to_sep, &c["year"]] {
            out.push_str(part);
        }
        last = m.end();
    }
    out.push_str(&text[last..]);
    out
}

fn main() {
    println!("{}", reformat_dates("Due 2024-01-02.", "-", "/"));
    println!("{}", reformat_dates("2024-01-0234-05-06", "-", "/"));
    println!("{}", reformat_dates("2024.01.02 2024x01x02", ".", "$1"));
    for c in dates(&date_pattern("-"), "12024-01-02 2024-01-02 2025-12-31") {
        println!("{} {} {}", &c["year"], &c["month"], &c["day"]);
    }
}
```

```output for=rust-regex
Due 02/01/2024.
2024-01-06/05/0234
02$101$12024 2024x01x02
2024 01 02
2025 12 31
```

### Syntax between engines

| Engine | Named group | Reference in a replacement | Literal `$` | `\d` | Lookbehind |
|---|---|---|---|---|---|
| Python `re` | `(?P<n>...)` | `\g<n>`, `\g<1>` | `$` | Unicode digits | yes |
| JavaScript | `(?<n>...)` | `$<n>`, `$1` | `$$` | `[0-9]` | yes |
| Java | `(?<n>...)` | `${n}`, `$1` | `\$` | `[0-9]` | yes |
| .NET | `(?<n>...)` | `${n}`, `$1` | `$$` | Unicode digits | yes |
| Go `regexp` | `(?P<n>...)`, `(?<n>...)` | `${n}`, `$1` | `$$` | `[0-9]` | no |
| Rust `regex` | `(?P<n>...)`, `(?<n>...)` | `${n}`, `$1` | `$$` | Unicode digits | no |
| POSIX ERE (C, Bash) | none | none | not special | none | no |

In Python, a backslash in the replacement starts an escape such as `\n`. In Java, a backslash escapes the next character. In Go and Rust, `$1x` is the group named `1x`, so write `${1}x`. In Java, `$12` is group 12 only if the pattern has 12 groups. In Python, `\20` is group 20, and `\g<2>0` is group 2 and a `0`. Java sets `\d` to Unicode digits with the flag `UNICODE_CHARACTER_CLASS`. .NET sets it to `[0-9]` with `RegexOptions.ECMAScript`.

### Backtracking and linear time

The default engines of Python and .NET backtrack. For some patterns, such as `^(a+)+$`, the time of a failed match grows exponentially with the length of the text. Go `regexp` runs in time linear in the size of the input. The Rust crate `regex` has a worst case of O(m × n) for a pattern of size m and a text of size n. Both engines have no lookaround and no back references. .NET has the option `RegexOptions.NonBacktracking`, which is linear in the length of the input, and a time-out for each match. Python has possessive quantifiers and atomic groups, which stop the backtracking in one part of a pattern. A pattern from a user, or a pattern with nested quantifiers on text from a user, is a risk of [[uncontrolled-resource-consumption]]. The pattern of this task has a fixed length and no nested quantifier, so the work at each start position has a fixed limit.

## Edge cases

The cases are in `cases/basic.jsonl`, `cases/edge.jsonl` and `cases/find.jsonl`.

- `empty-text` and `empty-to-sep`: an empty text gives an empty result. An empty `to_sep` joins the parts.
- `whole-text`, `adjacent` and `multiline`: a date at the start or the end of the text, two dates with a comma between them, and dates on separate lines. The pattern has no `^` or `$`, so no multiline flag is needed.
- `digit-before` and `digit-after`: `12024-01-02` and `2024-01-023` hold no date. Without the digit check, a search finds `2024-01-02` inside them.
- `date-after-rejected`: in `2024-01-0234-05-06`, the first match `2024-01-02` fails the digit check, and the date `0234-05-06` starts inside it. A loop that goes on after the end of a failed match misses this date. A lookbehind, or a restart one character after the start of the failed match, finds it.
- `chained`: in `2024-01-02-03`, the first date is replaced and `-03` stays.
- `other-separator`, `dot-separator`, `backslash-separator` and `plus-separator`: the separator matches only itself. An unescaped `.` matches the `x` of `2024x01x02`, an unescaped `+` makes `0+` mean one or more zeros, and an unescaped `\` is an error or an escape.
- `dollar-in-replacement`, `backslash-in-replacement`, `digit-in-replacement` and `whole-match-in-replacement`: `$1`, `\1\`, `0` and `$&` as `to_sep` are copied as they are. A `0` after a numbered reference such as `$2` can make the reference `$20`, which the named groups avoid.
- `unicode-digits`: Arabic-Indic digits and fullwidth digits are not dates, so `\d` is wrong in Python, .NET and Rust.
- `non-ascii-text` and `non-ascii-separator`: text with an emoji, and the separator `–` (U+2013). Go, C, C++, Rust and Bash work on UTF-8 bytes. The restart after a failed match moves one byte, and that byte is an ASCII digit, so the next search starts at the start of a character.
- `not-a-calendar-date`: `2024-13-45` becomes `45/13/2024`. To check the calendar, parse each match as a date.

## Pitfalls

- **`\d` matches more than ASCII digits.** In Python, .NET and the Rust crate, `\d` matches each Unicode decimal digit. A later `int()` or a date parser then gets text that it did not expect. Use `[0-9]`, or the flag `re.ASCII` in Python:

```python wrong id=python-unicode-digit
import re

text = "٢٠٢٤-٠١-٠٢ 2024-01-02"
print(re.findall(r"\d{4}-\d{2}-\d{2}", text))
print(re.findall(r"\d{4}-\d{2}-\d{2}", text, flags=re.ASCII))
```

```output for=python-unicode-digit
['٢٠٢٤-٠١-٠٢', '2024-01-02']
['2024-01-02']
```

- **`re.match` checks only the start.** `re.match` anchors the pattern at the start of the text, but not at the end. So a check of the whole text with `re.match` accepts text after the date. Use `re.fullmatch`:

```python wrong id=python-match
import re

date = r"[0-9]{4}-[0-9]{2}-[0-9]{2}"
for text in ["2024-01-02", "2024-01-02; rm -rf /"]:
    print(repr(text), bool(re.match(date, text)), bool(re.fullmatch(date, text)))
```

```output for=python-match
'2024-01-02' True True
'2024-01-02; rm -rf /' True False
```

- **A `$` in the replacement is a group reference.** In Java, a value such as `$1.50` refers to group 1, and the call throws. A function that returns the replacement does not help, because Java parses its result in the same way. `Matcher.quoteReplacement` makes the value literal:

```java wrong id=java-dollar
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class Main {
    public static void main(String[] args) {
        Pattern price = Pattern.compile("\\{price\\}");
        String value = "$1.50";
        try {
            System.out.println(price.matcher("Cost: {price}").replaceAll(value));
        } catch (IndexOutOfBoundsException e) {
            System.out.println(e.getMessage());
        }
        try {
            System.out.println(price.matcher("Cost: {price}").replaceAll(m -> value));
        } catch (IndexOutOfBoundsException e) {
            System.out.println(e.getMessage());
        }
        System.out.println(price.matcher("Cost: {price}").replaceAll(Matcher.quoteReplacement(value)));
    }
}
```

```output for=java-dollar
No group 1
No group 1
Cost: $1.50
```

- **`$3_` in Go is the group named `3_`.** Go takes the longest name after `$`, and a missing group gives an empty string. Write `${3}_`:

```go wrong id=go-dollar-name
package main

import (
	"fmt"
	"regexp"
)

func main() {
	re := regexp.MustCompile(`([0-9]{4})-([0-9]{2})-([0-9]{2})`)
	fmt.Printf("%q\n", re.ReplaceAllString("2024-01-02", "$3_$2_$1"))
	fmt.Printf("%q\n", re.ReplaceAllString("2024-01-02", "${3}_${2}_${1}"))
}
```

```output for=go-dollar-name
"2024"
"02_01_2024"
```

- **Go rejects lookbehind, lookahead and back references.** A pattern from another engine can fail to compile in Go. The error message does not always name the feature:

```go run id=go-unsupported
package main

import (
	"fmt"
	"regexp"
)

func main() {
	for _, expr := range []string{`(?<![0-9])[0-9]{4}`, `[0-9]{4}(?![0-9])`, `([0-9])\1`} {
		_, err := regexp.Compile(expr)
		fmt.Println(err)
	}
}
```

```output for=go-unsupported
error parsing regexp: invalid named capture: `(?<![0-9])[0-9]{4}`
error parsing regexp: invalid or unsupported Perl syntax: `(?!`
error parsing regexp: invalid escape sequence: `\1`
```

- **A regular expression with the flag `g` keeps state.** In JavaScript, `test` and `exec` on a regular expression with `g` start at `lastIndex`, and a match sets `lastIndex` to its end. A shared pattern then fails on every second call. Use a pattern without `g` for a test:

```javascript wrong id=js-lastindex
const date = /[0-9]{4}-[0-9]{2}-[0-9]{2}/g;
for (const text of ["2024-01-02", "2024-01-03", "2024-01-04"]) {
  console.log(text, date.test(text), date.lastIndex);
}
```

```output for=js-lastindex
2024-01-02 true 10
2024-01-03 false 0
2024-01-04 true 10
```

- **A backtracking engine can take exponential time.** The .NET documentation gives `^(a+)+$` as an example. A time-out stops the match, and `RegexOptions.NonBacktracking` gives the answer in linear time:

```csharp run id=csharp-backtracking
using System.Text.RegularExpressions;

string text = new string('a', 40) + "!";
var timeout = TimeSpan.FromMilliseconds(200);
try
{
    Console.WriteLine(new Regex("^(a+)+$", RegexOptions.None, timeout).IsMatch(text));
}
catch (RegexMatchTimeoutException)
{
    Console.WriteLine("backtracking: timed out");
}
Console.WriteLine("non-backtracking: " + new Regex("^(a+)+$", RegexOptions.NonBacktracking).IsMatch(text));
```

```output for=csharp-backtracking
backtracking: timed out
non-backtracking: False
```

## Related tasks

- [[uncontrolled-resource-consumption]]: a pattern that backtracks on text from a user can use all the CPU time.
- [[parse-and-format-a-date]]: check that a match is a real date, and format it.
- [[split-a-string]]: split a text on a separator, which is often a literal string and not a pattern.
- [[parse-an-integer]]: change a captured group from text to a number.
- [[read-a-file-line-by-line]]: read a large text one line at a time and replace in each line.

## References
