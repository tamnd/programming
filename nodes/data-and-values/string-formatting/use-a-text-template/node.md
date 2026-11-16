---
tag: ZT1PKS
kind: task
slug: use-a-text-template
name: Use a text template
aliases: [text template, template substitution, placeholder substitution, fill a template, string.Template, mail merge, "$name placeholders", "${name}", safe_substitute, template engine, envsubst, mustache]
area: data-and-values/string-formatting
level: core
summary: >-
  Fill a template that is data, not code: change $name and ${name} to values from a table and $$ to $, in one pass, with string.Template, a regular expression with a callback, or a small scanner.
contract: contract.yaml
category: text-and-strings
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
- url: https://docs.python.org/3/library/string.html
  title: string, Common string operations, Python documentation
  locator: Template strings ($-strings)
- url: https://pkg.go.dev/text/template
  title: Package text/template, The Go Standard Library
  locator: Overview; Template.Option
- url: https://www.gnu.org/software/bash/manual/html_node/Shell-Parameter-Expansion.html
  title: Shell Parameter Expansion, Bash Reference Manual
  locator: 3.5.3
- url: https://handlebarsjs.com/guide/
  title: Introduction, Handlebars
  locator: What is Handlebars; HTML escaping
---

## Task

Fill a text template: a text with named places that a program changes to values. A template is data. It can come from a file, a database or a translator, so the program does not know it when it is compiled. This is the difference from [[string-interpolation]], where the placeholders are part of the source code.

The template syntax is the one of Python `string.Template`[^1]:

- "$$ is an escape; it is replaced with a single $."[^1]
- "$identifier names a substitution placeholder"[^1], and the name is the key of the value. The name starts with a letter or an underscore, and "The first non-identifier character after the $ character terminates this placeholder specification"[^1].
- "${identifier} is equivalent to $identifier. It is required when valid identifier characters follow the placeholder but are not part of the placeholder"[^1], as in `${noun}ification`.

The function `substitute(template, names, values)` returns the filled text. `values[i]` is the value of `names[i]`.

| Template | Values | Result |
|---|---|---|
| `Hello, $name!` | name = `Ada` | `Hello, Ada!` |
| `${item}s cost $$${price}` | item = `Apple`, price = `3` | `Apples cost $3` |
| `$$name` | name = `Ada` | `$name` |

A placeholder with a name that is not in the table is an error. The function reads the template once, from left to right: a value is copied into the result and is not read again as a template.

## Solutions

| Language | Solution |
|---|---|
| Python | `string.Template(template).substitute(mapping)` |
| JavaScript, TypeScript | `template.replace(regex, callback)` |
| Java | `Matcher.replaceAll(callback)` with `Matcher.quoteReplacement` |
| C# | `Regex.Replace(template, pattern, evaluator)` |
| Go | `regexp.Regexp.ReplaceAllStringFunc` |
| Rust | a scanner over the text, with `find('$')` |
| C | a scanner that writes into a buffer |
| C++ | a scanner over a `std::string_view` |
| Bash | a loop with `[[ =~ ]]` and an associative array |

The regular expression of the solutions is `\$(?:(\$)|\{(NAME)\}|(NAME))?`, where `NAME` is `[A-Za-z_][A-Za-z0-9_]*`. Group 1 is `$$`, group 2 is `${name}` and group 3 is `$name`. If no group matches, the `$` is not valid. The callback looks up the name and returns the value. Each match is replaced once, so a value with a `$` stays as it is.

- **Python.** `substitute` "Performs the template substitution, returning a new string"[^1]. A missing name raises `KeyError`. `safe_substitute` is different: "if placeholders are missing from mapping and kwds, instead of raising a KeyError exception, the original placeholder will appear in the resulting string intact"[^1].
- **JavaScript.** `String.prototype.replace` with a function calls it for each match, with the groups as arguments. The text that the function returns is used as it is.
- **TypeScript.** The same as JavaScript, with types.
- **Java.** `Matcher.replaceAll` with a function reads `$` and `\` in the text that the function returns, as group references. The solution must give the value to `Matcher.quoteReplacement` (see Pitfalls).
- **C#.** `Regex.Replace` with a `MatchEvaluator` uses the returned text as it is.
- **Go.** `ReplaceAllStringFunc` calls the function for each match, and uses the text as it is. The function cannot return an error, so the solution keeps the first error in a variable. The package `text/template` is a full template language, with the syntax `{{.Name}}`: "Templates are executed by applying them to a data structure"[^2]. By default, a missing key prints `<no value>`. With the option `"missingkey=error"`, "Execution stops immediately with an error"[^2].
- **Rust.** The standard library has no regular expressions. The scanner finds the next `$` with `find`, copies the text before it, and reads the placeholder.
- **C.** The scanner looks up each name in the two arrays, and returns -1 for an error.
- **C++.** The same scanner, with `std::string_view` and a `std::unordered_map`.
- **Bash.** The shell itself expands `$name`: "The basic form of parameter expansion is ${parameter}, which substitutes the value of parameter"[^3]. But `eval` on a template runs any command in it, so the solution does not use it. A loop takes the text before the first `$` with a regular expression, and adds the value from an associative array.

The Python solution, with the errors:

```python run id=python-template
from string import Template

t = Template("${item}s cost $$${price}")
print(t.substitute(item="Apple", price="3"))
print(t.substitute({"item": "Pear", "price": "$x"}))
print(Template("Hello, $name!").safe_substitute())
try:
    Template("Hello, $name!").substitute()
except KeyError as e:
    print("KeyError", e)
try:
    Template("Cost: $5").substitute()
except ValueError as e:
    print("ValueError", e)
```

```output for=python-template
Apples cost $3
Pears cost $$x
Hello, $name!
KeyError 'name'
ValueError Invalid placeholder in string: line 1, col 7
```

The second line shows that a value is not a template: the `$x` in the value stays.

## Edge cases

- **No placeholders.** The template is the result.
- **Empty template.** The result is empty.
- **The escape.** `$$name` is `$name`: the first two characters make one `$`, and `name` is plain text.
- **A name followed by letters.** `${n}th` with n = `4` is `4th`. Without braces, `$nth` is the name `nth`.
- **A name followed by punctuation.** `$a.$b` is `1.2`, because a dot ends a name.
- **A value with a `$` or braces.** `$x and ${y}` in a value is copied as it is.
- **An empty value.** `[$v]` with v = an empty string is `[]`.
- **A value that is not used.** It is not an error.
- **An unknown name.** An error, not an empty string: an empty string in a letter or a command is a silent bug.

## Pitfalls

- **Two passes.** A loop that replaces each name in turn, with `replace` on the whole text, reads the values again. If the first value has `$b` in it, the second pass changes it. One pass over the template does not have this problem.
- **The replacement string in Java.** In `replaceAll`, `$` in the replacement is a group reference:

```java wrong id=java-replace-all-dollar
import java.util.regex.Matcher;

public class Main {
    public static void main(String[] args) {
        String template = "Total: $price";
        String value = "$5";
        System.out.println(template.replace("$price", value));
        try {
            System.out.println(template.replaceAll("\\$price", value));
        } catch (RuntimeException e) {
            System.out.println(e.getClass().getSimpleName() + ": " + e.getMessage());
        }
        System.out.println(template.replaceAll("\\$price", Matcher.quoteReplacement(value)));
    }
}
```

```output for=java-replace-all-dollar
Total: $5
IndexOutOfBoundsException: No group 5
Total: $5
```

`String.replace` takes the text as it is. `replaceAll` and `Matcher.appendReplacement` need `Matcher.quoteReplacement`. In JavaScript, C# and Go, the text that a replacement function returns is used as it is.

- **Templates from users.** A template that is code, such as a Python format string with attribute access or a shell string for `eval`, can read or run more than the values. Use a template language that only substitutes names, and do not give users templates in a language that can run code.
- **Escaping for the output.** The values are copied as they are. For HTML, a value with `<` must be escaped. Handlebars does it by default: "Because it was originally designed to generate HTML, Handlebars escapes values returned by a {{expression}}"[^4].
- **A missing name that is not an error.** Python `safe_substitute`, Go `text/template` without an option and many template engines keep the placeholder or print an empty value. Choose the error for data that must be complete.

## Related tasks

- [[interpolate-values-into-a-string]]: placeholders in the source code.
- [[string-interpolation]]: the concept of placeholders in string literals.

## References
