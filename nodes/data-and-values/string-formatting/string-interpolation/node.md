---
tag: AMEA3V
kind: concept
slug: string-interpolation
name: String interpolation
aliases: [interpolation, variable interpolation, variable substitution, variable expansion, f-string, formatted string literal, template literal, template string, interpolated string, string template, "${}", "$\"\""]
area: data-and-values/string-formatting
level: intro
summary: >-
  String interpolation puts the values of variables or expressions into a string literal at the places of placeholders. f-strings, template literals and $"..." strings are examples.
applies_to: core-languages
viz: optional
formal: none
see_also: [string-concept, format-specifiers, interpolate-values-into-a-string, print-formatted-output, concatenate-strings]
same_as:
  wikidata: Q7623982
  wikipedia: String_interpolation
sources:
- url: https://en.wikipedia.org/wiki/String_interpolation
  title: String interpolation, Wikipedia
  locator: Introduction; Security issues
- url: https://peps.python.org/pep-0498/
  title: PEP 498, Literal String Interpolation
  locator: Abstract; Escape sequences
- url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/tokens/interpolated
  title: String interpolation using $, C# language reference
  locator: Introduction; Special characters
- url: https://doc.rust-lang.org/std/fmt/index.html
  title: Module std::fmt, The Rust Standard Library
  locator: Named parameters; Escaping
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Template_literals
  title: Template literals, MDN Web Docs
  locator: Introduction; Tagged templates
- url: https://kotlinlang.org/docs/strings.html
  title: Strings, Kotlin documentation
  locator: String templates
- url: https://www.gnu.org/software/bash/manual/html_node/Double-Quotes.html
  title: Double Quotes, Bash Reference Manual
  locator: 3.1.2.3
- url: https://pkg.go.dev/fmt
  title: Package fmt, The Go Standard Library
  locator: Overview
---

## Idea

A program often builds text from values: a message with a name, a line with a count, a path with a number. String interpolation puts the values into the text where they belong. "In computer programming, string interpolation (or variable interpolation, variable substitution, or variable expansion) is the process of evaluating a string literal containing one or more placeholders, yielding a result in which the placeholders are replaced with their corresponding values"[^1].

In Python, an f-string has the prefix `f`, and each placeholder is an expression in braces:

```python run id=idea
name = "Ada"
count = 3
print(f"{name} has {count} new messages.")
print(f"Next week {name} has {count * 2}.")
print("{name} has {count}.")
```

```output for=idea
Ada has 3 new messages.
Next week Ada has 6.
{name} has {count}.
```

The first two strings get the values of `name`, `count` and `count * 2`. The third string has no prefix `f`, so the braces are only text. "String interpolation is an alternative to building a string via concatenation, which requires repeat quoting and unquoting"[^1]: `name + ' has ' + str(count) + ' new messages.'` gives the same text with more quotes and an explicit conversion.

## How it works

**A literal with placeholders.** An interpolated string is a literal in the source code, with a mark that tells the compiler to look for placeholders. The mark is a prefix (`f` in Python, `$` in C#), a different quote (the backtick in JavaScript), or the kind of quote (double quotes in a shell). "Placeholders are usually represented by a bare or a named sigil (typically $ or %), e.g. $apples or %apples, or with braces, e.g. {apples}, sometimes both, e.g. ${apples}"[^1].

**The compiler splits the literal.** The compiler or the interpreter splits the literal into its text parts and its expressions. At run time, the program computes each expression, converts the value to text, and joins the parts. For Python, "an f-string is really an expression evaluated at run time, not a constant value"[^2]. In C#, "the compiler replaces items with interpolation expressions by the string representations of the expression results"[^3]. So interpolation is a short form for a concatenation or a call to a format function, and the result is an ordinary string.

**Variables or expressions.** In some languages a placeholder holds only a name, and in others it holds any expression. Python's f-strings "provide a way to embed expressions inside string literals, using a minimal syntax"[^2]. JavaScript's `${...}` and C#'s `{...}` also accept any expression. A Rust format string with a name such as `{count}` reads a variable: "If a named parameter does not appear in the argument list, format! will reference a variable with that name in the current scope"[^4]. But a Rust placeholder cannot hold an expression such as `count * 2`. A shell expands `$name` and `${name}`, and an arithmetic expansion `$((...))` for numbers.

**Format specifiers.** Many languages accept a format after the expression, for example a width or the number of decimals: `{price:.2f}` in Python, `{price:F2}` in C# and `{price:.2}` in Rust. The page [[format-specifiers]] describes these mini-languages.

**Literal braces.** If braces mark a placeholder, a brace in the text needs an escape. In Python, "the correct way to have a literal brace appear in the resulting string value is to double the brace"[^2]. C# is the same: to include a brace in the text, "use two braces"[^3], `{{` or `}}`. In Rust, "the { character is escaped with {{ and the } character is escaped with }}"[^4]. JavaScript needs no escape for braces, because a placeholder starts with `${`. A `$` before a brace needs a backslash: `` `\${x}` ``.

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | native | f-strings: `f"{name} has {count}"`, with any expression and an optional format |
| JavaScript | native | template literals: `` `${name} has ${count}` ``, and tagged templates |
| TypeScript | native | template literals, as in JavaScript, and template literal types |
| Java | idiom | no interpolation; `String.format`, `"...".formatted(...)` or `+` |
| Kotlin | native | string templates: `"$name has ${count * 2}"` |
| C# | native | interpolated strings: `$"{name} has {count}"`, with a format after `:` |
| Go | idiom | no interpolation; `fmt.Sprintf("%s has %d", name, count)` |
| Rust | native | names in format strings: `format!("{name} has {count}")`; no expressions |
| C | absent | no interpolation; `snprintf` with a format string |
| C++ | idiom | no interpolation; `std::format("{} has {}", name, count)` |
| Bash | native | parameter expansion in double quotes: `"$name has $count"` |
| PowerShell | native | expandable strings: `"$name has $($count * 2)"` |
| SQL | idiom | no interpolation; the operator `\|\|` or a function such as `format` in some databases |
| Regex | n/a | a pattern is not a string with values; replacement strings refer to groups such as `$1` |
| HTML | n/a | HTML has no expressions; template engines add them |
| CSS | n/a | no interpolation; `var()` and `calc()` compute values, but not strings |

The examples show the languages with a native form, and the idioms of the others.

**JavaScript.** "Template literals are literals delimited with backtick (`) characters, allowing for multi-line strings, string interpolation with embedded expressions, and special constructs called tagged templates"[^5]. A tag is a function that gets the text parts and the values separately, so "a tagged template literal may not result in a string"[^5].

**Kotlin.** "String templates let you embed variables and expressions directly inside a String literal. This process is called interpolation"[^6]. `$name` takes a name, and `${...}` takes an expression.

**Bash.** "Enclosing characters in double quotes" keeps most characters as they are, but `$`, the backquote and the backslash keep their special meaning[^7]. So `"$name"` expands the variable, and `'$name'` in single quotes is the text `$name`.

**Go and C.** Go has no interpolation. Its package fmt "implements formatted I/O with functions analogous to C's printf and scanf"[^8], so `fmt.Sprintf` takes a format string and a list of values. C has only `printf` and `snprintf`.

## Variations and related ideas

- **Format functions.** `printf`, `String.format`, `str.format` and `std::format` take a format string and the values separately. The format string can come from a file or a translation, which interpolation cannot do. See [[format-specifiers]].
- **Templates.** A template engine fills a text that the program reads at run time, with names and loops. See [[use-a-text-template]].
- **Tagged templates and custom handlers.** A JavaScript tag function and a C# interpolated string handler receive the parts and the values before the join. A library uses them to escape values for SQL or HTML, or to skip the work when a log level is off.
- **Raw strings.** "Two types of literal expression are usually offered: one with interpolation enabled, the other without"[^1]. For example, "in Unix shells, single-quoted strings are raw, while double-quoted strings are interpolated"[^1].

## Pitfalls

- **The prefix is missing.** Without the prefix `f` or `$`, the braces are text. No error occurs, and the output contains `{name}`, as the third line of the idea example shows. JavaScript has the same trap with normal quotes:

```javascript wrong id=normal-quotes
const name = "Ada";
console.log("Hello, ${name}!");
console.log(`Hello, ${name}!`);
```

```output for=normal-quotes
Hello, ${name}!
Hello, Ada!
```

- **Injection.** "String interpolation, like string concatenation, may lead to security problems"[^1]. A value from a user can contain SQL, HTML or shell syntax. Never interpolate such a value into a query or a command. Use parameters in a query, and an escaping template for HTML.
- **A shell splits an unquoted expansion.** In Bash, `$name` outside quotes goes through word splitting and filename expansion. A value with spaces becomes several words. Write `"$name"`.
- **Default formats.** Each value goes through its default conversion to text. A float can show many digits, and an object can show a type name. Give a format, such as `{price:.2f}`, when the text is for people.
- **Interpolation is not a template.** An interpolated string is source code, so its text cannot come from a configuration file or a translation catalog. Use a format function or a template for text that changes at run time.

## Cost

Interpolation costs the same as the concatenation or the format call that it replaces: one conversion for each value and one new string. The compiler parses the literal once, at compile time, so there is no parse at run time. Some compilers do better: C# interpolated string handlers write the parts into one buffer, and a Rust format string is checked at compile time.

## History

Shells and scripting languages had interpolation first, and the idea spread to many languages: "String interpolation is common in many programming languages which make heavy use of string representations of data, such as Apache Groovy, Julia, Kotlin, Perl, PHP, Python, Ruby, Scala, Swift, Tcl and most Unix shells"[^1]. Python added f-strings with PEP 498, created in August 2015, for Python 3.6[^2]. JavaScript template literals have been "available across browsers since September 2015"[^5].

## References
