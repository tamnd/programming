---
tag: EVGWJP
kind: task
slug: implement-a-string-form-for-a-type
name: Implement a string form for a type
aliases: [implement toString, override toString, implement __str__, implement Display, implement String method, print an object, custom string form]
area: objects-and-classes/standard-object-methods
level: core
summary: Give a Color type a display form such as #FF8000 that the normal string conversion of the language calls, so that printing and joining the value need no extra code, and a debug form for programmers.
contract: contract.yaml
category: objects
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust]
see_also: [string-representation, universal-object-methods, implement-equality-for-a-type, record-type, concatenate-strings]
sources:
  - url: https://docs.oracle.com/en/java/javase/21/docs/api/java.base/java/lang/Object.html
    title: Object (Java SE 21 & JDK 21), API documentation
    locator: toString
  - url: https://docs.python.org/3/reference/datamodel.html
    title: Data model, The Python Language Reference
    locator: 3.3.1 Basic customization, object.__repr__ and object.__str__
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.object.tostring
    title: Object.ToString Method, .NET API reference
    locator: Remarks
  - url: https://pkg.go.dev/fmt
    title: fmt package, Go standard library
    locator: type Stringer; type GoStringer
  - url: https://doc.rust-lang.org/std/fmt/trait.Display.html
    title: Display trait, Rust standard library
    locator: Description
  - url: https://doc.rust-lang.org/std/fmt/trait.Debug.html
    title: Debug trait, Rust standard library
    locator: Description
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Object/toString
    title: Object.prototype.toString(), JavaScript reference, MDN
    locator: Description
  - url: https://en.cppreference.com/w/c/io/fprintf
    title: printf, fprintf, sprintf, snprintf, cppreference.com
    locator: Parameters; Return value
  - url: https://docs.oracle.com/en/java/javase/21/docs/api/java.base/java/lang/Record.html
    title: Record (Java SE 21 & JDK 21), API documentation
    locator: toString
---

## Task

Write a type `Color` with three channels, red, green and blue, each 0 to 255. Its display form is the hexadecimal notation of the web: a `#` and two upper-case hexadecimal digits for each channel, so `Color(255, 128, 0)` shows as `#FF8000`. Give the type this form through the normal string conversion of the language, so that `print(c)`, `"color " + c` or `format!("{c}")` shows `#FF8000` with no call to a special method. Where the language has a second form for programmers, also give a debug form that shows the type and the fields, such as `Color(r=255, g=128, b=0)`.

Then use it. The function `format_colors(reds, greens, blues)` makes the value `Color(reds[i], greens[i], blues[i])` for each index and returns the display form of each one. For example, `format_colors([255, 10], [128, 11], [0, 12])` is `["#FF8000", "#0A0B0C"]`. The lists have 0 to 10,000 items, and the tests also check the forms through the print functions of each language.

Java states the aim of the method: it "returns a string that "textually represents" this object", and the result "should be a concise but informative representation that is easy for a person to read".[^1]

## Solutions

| Language | Display form | Debug form | Called by |
|---|---|---|---|
| Python | `__str__` | `__repr__` | `print`, `str`, f-strings; `repr` and lists for the debug form |
| JavaScript | `toString()` | a `nodejs.util.inspect.custom` method in Node.js | `String(c)`, template literals, `+` |
| TypeScript | as JavaScript | as JavaScript | as JavaScript |
| Java | override `toString()` | none; one method for both | `+`, `String.valueOf`, `println`, `List.toString` |
| C# | override `ToString()` | none; one method for both | interpolation, `string.Join`, `Console.WriteLine` |
| Go | a `String() string` method | a `GoString() string` method | `%v` and `%s`, `Print`; `%#v` for the Go syntax form |
| Rust | `impl Display` | `#[derive(Debug)]` | `{}`, `to_string()`; `{:?}` for the debug form |
| C | a function that fills a buffer | a second function | the program calls them |
| C++ | `operator<<` for `std::ostream` | none in the standard | `std::cout << c`, `std::ostringstream` |

- **Python** defines both methods. `__repr__` should "look like a valid Python expression that could be used to recreate an object with the same value", and `__str__` is the informal form.[^2] The format spec `02X` in the f-string pads each channel to two upper-case digits. A list shows its items with `repr`, so `str([c])` is `[Color(r=255, g=128, b=0)]`.
- **Java** overrides `toString` in a record. The record would generate a form from its components, and the override replaces it. The documentation recommends the override for all classes: "It is recommended that all subclasses override this method."[^1] `String.format` with the conversion `%02X` for each channel gives the digits.
- **C#** overrides `ToString` in a sealed record, which also replaces the generated form. Without an override, the default "returns the fully qualified name of the type of the Object".[^3] The format string `{R:X2}` gives two upper-case hexadecimal digits.
- **Go** gives the type a `String` method, so the type implements `fmt.Stringer`. "The String method is used to print values passed as an operand to any format that accepts a string or to an unformatted printer such as Print."[^4] A `GoString` method gives the form for `%#v`; here it is `color.New(255, 128, 0)`, which is valid Go.
- **Rust** implements `Display` and derives `Debug`. "Implementing this trait for a type will automatically implement the ToString trait for the type, allowing the usage of the .to_string() method."[^5] `Display` "is for user-facing output, and so cannot be derived", but the documentation of `Debug` says: "Generally speaking, you should just derive a Debug implementation."[^5][^6] The derived form is `Color { r: 255, g: 128, b: 0 }`.
- **JavaScript and TypeScript** override `toString`, which "is meant to be overridden by derived objects for custom type coercion logic".[^7] The method pads each channel with `padStart(2, "0")`. Node.js calls the method whose key is the registered symbol `nodejs.util.inspect.custom` for `console.log` and `util.inspect`, so the class gives the debug form there.
- **C** has no methods, so `color_format` writes the form into a buffer of `COLOR_STR_SIZE` bytes, which holds 7 characters and the null terminator. For `snprintf`, "At most bufsz - 1 characters are written", and its return value is "number of bytes that would be written not including the null terminator", so `color_debug` returns the length of the full text and the caller can see a cut.[^8]
- **C++** defines `operator<<` for `std::ostream`, which works with `std::cout` and `std::ostringstream`. The class stores the channels as `std::uint8_t`, which is a character type, so `to_string` converts each one to `unsigned` before it formats it.

### Alternatives

- **Generated forms.** A Java record, a C# record, a Kotlin data class, a Python `@dataclass` and Rust's `#[derive(Debug)]` generate a form from the fields. Keep it when the debug form is enough, and override it only for the display form.
- **A format with options.** In C#, `IFormattable` lets the caller choose a form with a format string. The `ToString` documentation advises it when the string "can be formatted in multiple ways".[^3] In Go, a type with a `Format` method implements `fmt.Formatter` and reads the verb and the flags itself.[^4]
- **A separate method.** A method such as `toHex()` is clear when the type has many forms and none of them is the natural one. The normal conversion then gives the debug form.

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`.

- `empty`: no colors give an empty list.
- `single`: one color, `#010203`.
- `zero`: all channels 0 give `#000000`, not `#000`.
- `max`: all channels 255 give `#FFFFFF`.
- `padding`: the values 10, 11 and 12 need a leading zero each, `#0A0B0C`. Without the width of two digits, the form is `#ABC`, which a reader takes as a different color.

## Pitfalls

- **A form without padding.** `hex(10)` in Python, `(10).toString(16)` in JavaScript and `%X` in C give one digit. Use a width and a fill character, such as `{:02X}` or `%02X`, and test a channel below 16.
- **A string form that calls itself.** In Go, a `String` method that formats the receiver with `%v` or `%s` calls itself. The `fmt` documentation warns: "convert the value before recurring".[^4] The same loop happens in Java when `toString` uses `"" + this`.
- **A string form that throws.** The print functions call the method at times that the type does not choose, such as in a log line or a debugger. C# says: "Your ToString() override should not throw an exception."[^3]
- **Secrets in the form.** A generated form shows every field, including a password or a token. Override it, or leave the field out, before a value goes to a log.
- **`uint8_t` in a C++ stream.** `std::cout << c.r` with a `std::uint8_t` field prints a character, not a number. Convert the field to `unsigned` first.
- **Code that parses the string form.** A generated form can change between versions. Of the form that a Java record generates, the documentation says: "The precise format produced by this implicitly provided implementation is subject to change, so the present syntax should not be parsed by applications to recover record component values."[^9] Use a separate, documented format for data that a program reads.

## Related tasks

- [[implement-equality-for-a-type]] gives the same kind of value type equality and a hash.
- [[implement-ordering-for-a-type]] adds an order to a value type.
- [[define-a-record-type]] gets a generated string form from a record or a data class.

## References
