---
tag: VX3ENF
kind: task
slug: interpolate-values-into-a-string
name: Interpolate values into a string
aliases: [string interpolation, interpolate variables, f-string, template literal, interpolated string, "$\"...\"", format!, expressions in strings, escape braces, embed values in text, Mary had a little lamb]
area: data-and-values/string-formatting
level: intro
summary: >-
  Build a sentence from values with string interpolation: a name, a count with the right plural and a computed total. Escape the literal braces, and never treat a value as a format.
contract: contract.yaml
category: text-and-strings
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
- url: https://github.com/acmeism/RosettaCodeData/blob/main/Task/String-interpolation-included-/00-TASK.txt
  title: Rosetta Code, String interpolation (included) (task text, from acmeism/RosettaCodeData)
  locator: task text
- url: https://peps.python.org/pep-0498/
  title: "PEP 498, Literal String Interpolation"
  locator: Abstract; Specification
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Template_literals
  title: Template literals, MDN Web Docs
  locator: Description
- url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/tokens/interpolated
  title: String interpolation using $, C# reference
  locator: Special characters
- url: https://pkg.go.dev/fmt
  title: Package fmt, The Go Standard Library
  locator: Overview
- url: https://doc.rust-lang.org/std/fmt/index.html
  title: Module std::fmt, The Rust Standard Library
  locator: Named parameters; Escaping
- url: https://www.gnu.org/software/bash/manual/html_node/Double-Quotes.html
  title: Double Quotes, Bash Reference Manual
  locator: 3.1.2.3
---

## Task

Build a string from values. The Rosetta Code task says: "Given a string and defined variables or values, string interpolation is the replacement of defined character sequences in the string by values or variable values"[^1]. Its example puts the word little into the sentence Mary had a little lamb. This task adds three more parts: a number, an expression and literal braces.

The function `describe_order(name, count, price)` returns the sentence:

```text
Order {NAME}: COUNT items, total TOTAL
```

- `NAME` is the name, inside literal braces.
- `COUNT` is the count, and the word is `item` if the count is 1 and `items` otherwise.
- `TOTAL` is `count * price`, rounded to 2 decimals.

For example, `describe_order("Ada", 3, 1.5)` is `"Order {Ada}: 3 items, total 4.50"`, and `describe_order("Bob", 1, 0.5)` is `"Order {Bob}: 1 item, total 0.50"`.

The name is a value, not a part of the template. If the name is `{x}`, `50% off` or `${HOME}`, the result has those characters as they are.

## Solutions

Six tier 1 languages have interpolation in the language. Java, Go and C have none, so their solutions use a format function.

| Language | Mechanism | Literal brace |
|---|---|---|
| Python | f-string `f"...{name}..."` | `{{` and `}}` |
| JavaScript, TypeScript | template literal `` `...${name}...` `` | `{` and `}` as they are |
| Java | `String.format` | `{` and `}` as they are |
| C# | interpolated string `$"...{name}..."` | `{{` and `}}` |
| Go | `fmt.Sprintf` | `{` and `}` as they are |
| Rust | `format!("...{name}...")` | `{{` and `}}` |
| C | `snprintf` | `{` and `}` as they are |
| C++ | `std::format` | `{{` and `}}` |
| Bash | double quotes `"...${name}..."` | `{` and `}` as they are |

- **Python.** An f-string. "F-strings provide a way to embed expressions inside string literals, using a minimal syntax"[^2]. A placeholder can hold an expression, such as `count * price`, with a format after a colon. For a brace, "any doubled braces '{{' or '}}' inside literal portions of an f-string are replaced by the corresponding single brace"[^2], so `{{{name}}}` is a brace, the name and a brace.
- **JavaScript.** A template literal, in backticks. Template literals are used "for string interpolation (to create strings by doing substitution of placeholders)"[^3]. A single brace needs no escape, because a placeholder starts with `${`.
- **TypeScript.** The same code as JavaScript, with types.
- **Java.** Java has no interpolation in the language, so the solution uses `String.format` with `Locale.ROOT`.
- **C#.** An interpolated string. "To include a brace"[^4] in the result, use two braces. "To use a conditional operator in an interpolation expression, enclose that expression in parentheses"[^4], because a colon starts the format. `string.Create(CultureInfo.InvariantCulture, $"...")` formats the number with a dot.
- **Go.** `fmt.Sprintf`. Go has no interpolation, and the package fmt "implements formatted I/O with functions analogous to C's printf and scanf"[^5].
- **Rust.** `format!` takes variables by name: "If a named parameter does not appear in the argument list, format! will reference a variable with that name in the current scope"[^6]. A name in braces must be a variable, not an expression, so the solution computes the total and the plural ending first. "the { character is escaped with {{"[^6].
- **C.** `snprintf` into a buffer. The solution returns the length of the full text.
- **C++.** `std::format`, with the brace syntax of Rust and Python. It has no names, so the values come after the format string.
- **Bash.** Double quotes expand `$name` and `${name}`. "Enclosing characters in double quotes"[^7] keeps each character, except `$`, a backquote, a backslash and sometimes `!`. Bash has no float arithmetic, so the solution uses `awk` for the total.

The Python solution, with all three parts in one f-string:

```python run id=python-f-string
def describe_order(name, count, price):
    return f"Order {{{name}}}: {count} item{'' if count == 1 else 's'}, total {count * price:.2f}"


print(describe_order("Ada", 3, 1.5))
print(describe_order("Bob", 1, 0.5))
print(describe_order("{x}", 2, 1.25))
```

```output for=python-f-string
Order {Ada}: 3 items, total 4.50
Order {Bob}: 1 item, total 0.50
Order {{x}}: 2 items, total 2.50
```

The Rust solution computes the expressions first, because a name in braces must be a variable:

```rust run id=rust-format-captures
fn describe_order(name: &str, count: i64, price: f64) -> String {
    let ending = if count == 1 { "" } else { "s" };
    let total = count as f64 * price;
    format!("Order {{{name}}}: {count} item{ending}, total {total:.2}")
}

fn main() {
    println!("{}", describe_order("Ada", 3, 1.5));
    println!("{}", describe_order("50% off", 4, 2.5));
}
```

```output for=rust-format-captures
Order {Ada}: 3 items, total 4.50
Order {50% off}: 4 items, total 10.00
```

## Edge cases

- **Zero items.** The word is `items`: `Order {Ada}: 0 items, total 0.00`.
- **One item.** The word is `item`. The test for the plural is `count == 1`, not `count > 1`, because 0 also takes the plural in English.
- **Empty name.** The result has two braces with nothing between them: `Order {}: 2 items, total 2.00`.
- **Braces, percent signs and dollar signs in the name.** `{x}` gives `Order {{x}}: ...`, `50% off` gives `Order {50% off}: ...`, and `${HOME}` gives `Order {${HOME}}: ...`. The value is never read as a template.
- **Rounding of the total.** `3 * 19.99` is a little less than 59.97 as a binary float, and it rounds to `59.97`.
- **Large totals.** `1000 * 1234.56` gives `1234560.00`, with no exponent and no group separators.
- **Negative price.** A refund of `-4.5` for 2 items gives `total -9.00`.

## Pitfalls

- **A value used as a template.** Interpolation reads the template at compile time, so a value with braces is safe. But code that builds a format string from a value is not safe. In Python, `("Order " + name).format()` fails for the name `{x}`, because `format` reads the braces of the value as a placeholder:

```python wrong id=python-value-as-template
name = "{x}"
try:
    print(("Order " + name + ": done").format())
except KeyError as e:
    print("KeyError", e)
print(f"Order {name}: done")
```

```output for=python-value-as-template
KeyError 'x'
Order {x}: done
```

The same mistake in C, `printf(name)`, is a security weakness. Pass the value as an argument.

- **An f-string without the f.** `"Order {name}"` without the `f` is a normal string, so the braces stay in the result. In JavaScript, `"${name}"` in double quotes is also a normal string. Only backticks make a template literal.
- **A conditional in C#.** `$"{count == 1 ? "" : "s"}"` does not compile, because the colon starts a format. Put the expression in parentheses.
- **Single quotes in Bash.** `'$name'` in single quotes stays as the text `$name`. Use double quotes, and quote each expansion so that the shell does not split the value into words.
- **The locale of the number.** In C#, an interpolated string formats a number with the culture of the thread, so a German computer gives `4,50`. Use `string.Create(CultureInfo.InvariantCulture, ...)` or `FormattableString.Invariant` for text that a program reads. See [[locale-aware-formatting]].
- **Interpolation into code.** Do not build SQL, HTML or shell commands with interpolation. A value with a quote changes the command. Use parameters or a library that escapes the values.

## Related tasks

- [[string-interpolation]]: how interpolation works in each language.
- [[print-formatted-output]]: values in fixed columns.
- [[format-specifiers]]: the formats after the colon, such as `.2f`.
- [[locale-aware-formatting]]: numbers with the symbols of a language and region.

## References
