---
tag: H38B5D
kind: concept
slug: string-representation
name: String representation of an object
aliases: [string form, toString, __str__, __repr__, repr, Display trait, Debug trait, Stringer, ToString, printable representation, debug representation]
area: objects-and-classes/standard-object-methods
level: core
summary: >-
  The string representation of an object is the text that a program gets when it prints the object or joins it to a string, often in two forms: a display form for users and a debug form for programmers.
applies_to: core-languages
viz: optional
history:
  year: 1996
  note: JDK 1.0 put toString on java.lang.Object, so every Java object has a string form; Python separates repr and str, and Rust and Go later split the display form and the debug form into two traits or two format verbs.
see_also: [universal-object-methods, record-type, object-serialization, sensitive-information-exposure, equality-and-hashing-contract]
sources:
  - url: https://docs.python.org/3/reference/datamodel.html
    title: Data model, The Python Language Reference
    locator: 3.3.1 Basic customization, object.__repr__ and object.__str__
  - url: https://pkg.go.dev/fmt
    title: fmt package, Go standard library
    locator: Printing; type Stringer; type GoStringer
  - url: https://docs.oracle.com/en/java/javase/21/docs/api/java.base/java/lang/Object.html
    title: Object (Java SE 21 & JDK 21), API documentation
    locator: toString
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.object.tostring
    title: Object.ToString Method, .NET API reference
    locator: Remarks
  - url: https://kotlinlang.org/docs/data-classes.html
    title: Data classes, Kotlin documentation
    locator: Introduction
  - url: https://doc.rust-lang.org/std/fmt/trait.Debug.html
    title: Debug trait, Rust standard library
    locator: Description
  - url: https://doc.rust-lang.org/std/fmt/trait.Display.html
    title: Display trait, Rust standard library
    locator: Description
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Object/toString
    title: Object.prototype.toString(), JavaScript reference, MDN
    locator: Description
  - url: https://en.wikipedia.org/wiki/Java_version_history
    title: Java version history, Wikipedia
    locator: Release table
---

## Idea

A program often needs an object as text: to print it, to write it to a log, to show it in a debugger or a test failure, or to put it in a message. The string representation is the text that the object gives for these uses. Most languages call one method for it, such as `toString`, `__str__` or `String()`, and give a default when the type does not define one.

There are two different readers of this text. A user wants a short, friendly form, such as `Dune (1965)`. A programmer wants a form that shows the type and every field without doubt, such as `Book(title='Dune', year=1965)`. Python, Rust and Go keep the two forms apart. Python's `__repr__` computes "the “official” string representation of an object", while `__str__` computes "the “informal” or nicely printable string representation".[^1] Java and C# have one method, so each type must choose.

## How it works

The language calls the method in a fixed set of places. In Python, `print`, `str()` and f-strings call `__str__`, and `repr()`, the interactive prompt and the items of a list call `__repr__`:

```python run id=str-and-repr
class Book:
    def __init__(self, title, year):
        self.title, self.year = title, year

    def __repr__(self):
        return f"Book(title={self.title!r}, year={self.year!r})"

    def __str__(self):
        return f"{self.title} ({self.year})"


b = Book("Dune", 1965)
print(b)
print(repr(b))
print([b])
```

```output for=str-and-repr
Dune (1965)
Book(title='Dune', year=1965)
[Book(title='Dune', year=1965)]
```

The debug form uses the debug form of each field: `!r` puts the title in quotes, so `'Dune'` is clearly a string. Python's advice for `__repr__` is: "If at all possible, this should look like a valid Python expression that could be used to recreate an object with the same value".[^1]

The two forms have different rules:

| | Display form | Debug form |
|---|---|---|
| Reader | a user | a programmer |
| Content | the parts that matter to the user | the type name and every field |
| Strings | without quotes | with quotes and escapes |
| Python | `__str__` | `__repr__` |
| Rust | `Display`, `{}` | `Debug`, `{:?}` |
| Go | `String()`, `%v` | `%+v` and `%#v`, or `GoString()` |
| Default | often none, or the debug form | the type name and an address, or the fields |

When a type gives only one form, the other falls back. Python: "If a class defines __repr__() but not __str__(), then __repr__() is also used when an “informal” string representation of instances of that class is required."[^1] Go's `fmt` calls `String()` for `%v`, but not for `%#v`.[^2]

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | native | `__str__` and `__repr__` on `object`; `print`, `str`, `repr` and f-strings call them |
| JavaScript | native | `Object.prototype.toString`; the default gives `"[object Object]"` |
| TypeScript | native | as JavaScript |
| Java | native | `Object.toString`; records generate it |
| Kotlin | native | `Any.toString`; data classes generate it |
| C# | native | `Object.ToString`; records generate it |
| Go | stdlib | `fmt.Stringer` and `fmt.GoStringer`; the verbs `%v`, `%+v` and `%#v` |
| Rust | stdlib | traits `Display` and `Debug`; `Debug` is derivable |
| C | absent | no methods; a program writes a function that fills a buffer with `snprintf` |
| C++ | idiom | `operator<<` for `std::ostream`, or a `std::formatter` specialization for `std::format` |
| Bash | n/a | all values are strings |
| PowerShell | native | .NET objects with `ToString`; the format system shows properties as a table or a list |
| SQL | n/a | `CAST(x AS VARCHAR)` converts values; no user types with methods in the standard |
| Regex | n/a | no objects |
| HTML | n/a | no objects |
| CSS | n/a | no objects |

- **Java** gives every object `toString`, and string concatenation calls it. The method should return "a concise but informative representation that is easy for a person to read".[^3] The default of `Object` is the class name, `@` and the hash code in hexadecimal, which is useful only to tell objects apart.
- **C#** works the same way. The default `ToString` returns "the fully qualified name of the object's type",[^4] and records generate a form with the type name and their properties.
- **Kotlin** data classes generate the debug form: "toString() of the form "User(name=John, age=42)"".[^5]
- **Rust** has no default. `Debug` "should format the output in a programmer-facing, debugging context. Generally speaking, you should just derive a Debug implementation."[^6] `Display` is written by hand: "Display is similar to Debug, but Display is for user-facing output, and so cannot be derived."[^7]
- **Go** prints any value. `%v` shows the fields, `%+v` adds their names and `%#v` shows Go syntax. A `String() string` method changes `%v` and `%s`: "If an operand implements method String() string, that method will be invoked to convert the object to a string".[^2]
- **JavaScript** calls `toString` when an object becomes a string, for example in a template literal. MDN says that the method "is meant to be overridden by derived objects for custom type coercion logic."[^8] Without an override, all objects print as `[object Object]`.

## Variations and related ideas

- **Generated forms.** Records in Java and C#, Kotlin data classes, Python dataclasses and Rust's `#[derive(Debug)]` write the debug form from the fields. A type then needs code only for the display form.
- **Format strings.** C#'s `IFormattable`, Python's `__format__` and Rust's `Formatter` options let the caller choose the form, such as a number of decimal places or a date pattern. The string representation is the form with no options.
- **Serialization.** A string form for people is not a data format. To write an object and read it back, use [object serialization](/object-serialization/), such as JSON, which has a defined grammar. Python's `repr` of simple values comes close to a round trip, but it is not a contract.
- **Pretty-printing.** Rust's `{:#?}`, Python's `pprint` and Node.js's `util.inspect` split a large debug form over several lines with indentation.

## Pitfalls

- **The default form in user text.** A message such as `"Order: " + order` prints the class name, `@` and a hexadecimal hash code in Java, and `[object Object]` in JavaScript when the type has no override:

```javascript run id=object-object
const order = { id: 7, total: 12.5 };
console.log(`Order: ${order}`);
console.log(`Order: ${JSON.stringify(order)}`);
```

```output for=object-object
Order: [object Object]
Order: {"id":7,"total":12.5}
```

- **Secrets in the string form.** A generated `toString` prints every field, so a record with a password or a token puts it in every log line that prints the record. Override the method to hide such fields. See [exposure of sensitive information](/sensitive-information-exposure/).
- **Logic that parses the string form.** Java says of `toString`: "The string output is not necessarily stable over time or across JVM invocations."[^3] A program that parses the form breaks when a field is added. Read the fields, or use a defined format.
- **An exception or a side effect in the method.** Debuggers, loggers and error handlers call the method, often while something else is going wrong. A `toString` that reads a database, takes a lock or throws makes those tools fail.
- **Cycles.** A generated form that prints every field never ends when two objects refer to each other, for example a parent with a list of children that each point back to the parent. Leave the back reference out of the form.

## Cost

The string form builds a new string each time, so its cost grows with the size of the object and of its fields. A log statement such as `log.debug("state: " + obj)` builds the string even when debug logging is off; a logger that takes the object, or a lambda, builds it only when the message is written. Generated forms of large collections can be long; Python's `reprlib` gives a `repr` with a limit on the size.

## History

JDK 1.0, released on January 23, 1996,[^9] put `toString` on `java.lang.Object`, next to `equals` and `hashCode`, so every Java object has a string form. Python's data model has two methods, `__repr__` and `__str__`, for the two readers. Rust and Go later made the split part of the standard library: Rust with the traits `Display` and `Debug`, and Go with `String()` and the verbs of `fmt`.

## References
