---
tag: RR1JRW
kind: format
slug: toml
name: TOML
aliases: [Tom's Obvious, Minimal Language]
area: data-and-persistence/data-formats
level: core
summary: >-
  TOML is a text format for configuration files. A TOML document is one table of key and value pairs, with strings, integers, floats, booleans, four date and time types, arrays and nested tables.
history:
  year: 2013
  people: [Tom Preston-Werner, Pradyun Gedam]
  note: Version 0.1.0 of the specification came out on 17 March 2013. Version 1.0.0 came out on 11 January 2021, and version 1.1.0 on 18 December 2025.
same_as:
  wikidata: Q28449455
  wikipedia: TOML
sources:
  - url: https://toml.io/en/v1.0.0
    title: "T. Preston-Werner, P. Gedam, et al., TOML v1.0.0, 11 January 2021"
    locator: Preliminaries; Keys; String; Integer; Float; Offset Date-Time; Local Date-Time; Inline Table
  - url: https://toml.io/en/v1.1.0
    title: "T. Preston-Werner, P. Gedam, et al., TOML v1.1.0, 18 December 2025"
    locator: Objectives; Preliminaries; Comment; Keys; String; Integer; Float; Offset Date-Time; Local Date-Time; Array; Table; Inline Table; Array of Tables; Filename Extension; MIME Type
  - url: https://github.com/toml-lang/toml/blob/main/CHANGELOG.md
    title: TOML Changelog
    locator: the entries 0.1.0, 0.2.0, 0.3.0, 0.4.0, 0.5.0, 1.0.0-rc.1, 1.0.0 and 1.1.0
  - url: https://github.com/toml-lang/toml/blob/1.1.0/toml.abnf
    title: TOML ABNF grammar, at the tag 1.1.0
    locator: the rules toml, expression, keyval, key, date-time, table, std-table, inline-table and array-table
  - url: https://github.com/toml-lang/toml/blob/main/README.md
    title: TOML README
    locator: Objectives; Comparison with Other Formats
  - url: https://www.iana.org/assignments/media-types/media-types.xhtml
    title: IANA, Media Types registry
    locator: the application registry, the entry toml
  - url: https://docs.python.org/3.14/library/tomllib.html
    title: The Python Standard Library, tomllib, Python 3.14
    locator: the introduction; tomllib.load; tomllib.loads; TOMLDecodeError; Conversion Table
  - url: https://peps.python.org/pep-0680/
    title: "T. Hukkinen, S. Jain, PEP 680: tomllib: Support for Parsing TOML in the Standard Library, 2022"
    locator: Motivation; Rationale; Security Implications; Rejected Ideas, Including an API for writing TOML
  - url: https://docs.python.org/3.15/whatsnew/3.15.html#tomllib
    title: What's New In Python 3.15
    locator: Improved modules, tomllib
  - url: https://docs.rs/toml/1.2.1+spec-1.1.0/toml/
    title: toml 1.2.1+spec-1.1.0 documentation
    locator: the crate documentation, TOML values, from_str and to_string
  - url: https://docs.rs/toml_edit/0.25.19+spec-1.1.0/toml_edit/
    title: toml_edit 0.25.19+spec-1.1.0 documentation
    locator: the crate documentation, Example and Limitations
  - url: https://github.com/tomlj/tomlj
    title: TomlJ
    locator: the README, Error reporting and recovery; Other highlights
  - url: https://doc.rust-lang.org/cargo/reference/manifest.html
    title: The Cargo Book, The Manifest Format
    locator: the first paragraph
  - url: https://peps.python.org/pep-0518/
    title: "B. Cannon, N. Smith, D. Stufft, PEP 518: Specifying Minimum Build System Requirements for Python Projects, 2016"
    locator: Abstract; Overview of file formats considered
  - url: https://github.com/toml-lang/toml-test
    title: toml-test
    locator: the README, the first paragraphs
  - url: https://github.com/BurntSushi/toml
    title: BurntSushi/toml, TOML parser for Go
    locator: the README, the first paragraph
  - url: https://github.com/pelletier/go-toml
    title: go-toml v2
    locator: the README, the first paragraph and Stdlib behavior
  - url: https://github.com/squirrelchat/smol-toml
    title: smol-toml
    locator: the README, the first paragraph
  - url: https://github.com/xoofx/Tomlyn
    title: Tomlyn
    locator: the README, Features
  - url: https://github.com/marzer/tomlplusplus
    title: toml++ (tomlplusplus)
    locator: the README, Library features
  - url: https://github.com/mikefarah/yq
    title: yq
    locator: the README, the input and output formats
  - url: https://github.com/orchestr7/ktoml
    title: ktoml
    locator: the README
---

## What it is

TOML is a text format for configuration files. The name stands for "Tom's Obvious, Minimal Language". The specification gives three aims. TOML is a minimal configuration format that people can read because its semantics are obvious. It maps unambiguously to a hash table. Programs in many languages can parse it into data structures.[^2] A TOML document is one table. The table holds key and value pairs, and a value can be another table, so a document is a tree of tables.

A value has one of these types: string, integer, float, boolean, offset date-time, local date-time, local date, local time, array and inline table.[^2] There is no null. A key is always a string, even when it is written as digits.[^2] The top level is always a table. TOML cannot hold an array or a float at the top level, and it has no marker for the start or the end of a document. So the README of the project says that TOML is not for the serialization of arbitrary data.[^5]

The README of the project compares TOML with JSON and YAML. TOML and JSON both use common data types that a machine can parse. TOML and YAML both have features for human readers, such as comments. TOML has comments, which JSON does not have, and it keeps a smaller grammar than YAML.[^5] In TOML, a string always has quotes, and the only booleans are `true` and `false`.[^2] PEP 518 chose TOML for the file `pyproject.toml` of Python projects. It compares TOML with YAML, JSON and other formats. It rejects YAML for two main reasons: the specification is large, and a tool that calls an unsafe load function can run arbitrary code.[^14] Cargo, the package manager of Rust, writes its manifest `Cargo.toml` in TOML.[^13]

The file extension is `.toml`, and the media type is `application/toml`.[^2] IANA has the media type in its registry.[^6]

### History

Tom Preston-Werner wrote TOML. The byline of the specification names him, Pradyun Gedam and other authors.[^2] Version 0.1.0 came out on 17 March 2013. It was the first release, and it said that TOML follows SemVer for its version numbers.[^3] Version 0.2.0 (September 2013) renamed "key groups" to "tables" and added arrays of tables. Version 0.3.0 (November 2014) added multi-line and literal strings and moved datetimes to RFC 3339. Version 0.4.0 (February 2015) added inline tables and underscores in numbers. Version 0.5.0 (July 2018) added dotted keys, hexadecimal, octal and binary integers, `inf` and `nan`, the three local date and time types, and the ABNF grammar.[^3]

Version 1.0.0 came out on 11 January 2021.[^3] PEP 680 took this release as the sign that the format is stable, and it added the read-only module `tomllib` to the Python standard library, based on the library tomli.[^8] Version 1.1.0 came out on 18 December 2025. It relaxed some rules and added some escapes (see Versions).[^3]

## Syntax or structure

The ABNF file of the project is the formal grammar, and the text of the specification adds the rules that the grammar cannot state, such as "a table is defined once".[^4][^3] The excerpt below is from the file at the tag 1.1.0.[^4]

```abnf
toml = expression *( newline expression )

expression =  ws [ comment ]
expression =/ ws keyval ws [ comment ]
expression =/ ws table ws [ comment ]

keyval = key keyval-sep val
key = simple-key / dotted-key
val = string / boolean / array / inline-table / date-time / float / integer

date-time      = offset-date-time / local-date-time / local-date / local-time
partial-time   = time-hour ":" time-minute [ ":" time-second [ time-secfrac ] ]
full-time      = partial-time time-offset
offset-date-time = full-date time-delim full-time
local-date-time = full-date time-delim partial-time

table = std-table / array-table
std-table = std-table-open key std-table-close
array-table = array-table-open key array-table-close
```

A document is a sequence of lines. Each line is empty, a comment, a key and value pair, or a table header. A key, the equals sign and the value must be on the same line, though some values can continue over more lines.[^2] A document must be valid UTF-8. Version 1.1.0 says that a parser must reject a file that is not valid UTF-8, or replace the bad bytes with U+FFFD, and that rejection is preferred.[^2] A newline is LF or CRLF. Whitespace is tab or space, and TOML is case-sensitive.[^2]

**Comments.** A `#` outside a string starts a comment to the end of the line. Version 1.1.0 says that a parser must not change keys or values because of a comment.[^2]

**Keys.** A key is bare, quoted or dotted. A bare key has only the ASCII letters, the ASCII digits, `_` and `-`. A quoted key follows the rules of a basic or a literal string. A dotted key is a sequence of bare or quoted keys joined by `.`, and it makes nested tables: `physical.color = "orange"` puts the key `color` in the table `physical`.[^2] Because a bare key can be all digits, `3.14159 = "pi"` is a dotted key with the two parts `3` and `14159`, not a float.[^2] A key defined twice is an error, and a bare key and a quoted key with the same text are the same key.[^2]

**Strings.** There are four forms. A basic string is in double quotes and has escapes such as `\n`, `\t`, `\"`, `\\`, `\uXXXX` and `\UXXXXXXXX`. A literal string is in single quotes and has no escapes, which suits Windows paths and regular expressions. Each form has a multi-line variant with three quotes. A newline directly after the opening delimiter is trimmed. In a multi-line basic string, a backslash at the end of a line removes the newline and the whitespace that follows it.[^2] An escape must be a Unicode scalar value, so a string cannot hold a lone surrogate. For binary data, the specification recommends an encoding such as Base64.[^2]

**Integers.** An integer is decimal, with an optional sign and no leading zeros. A non-negative integer can also be hexadecimal (`0x`), octal (`0o`) or binary (`0b`). An underscore can go between two digits, as in `1_000`.[^2] Version 1.0.0 says that a parser should accept every 64-bit signed integer, from -2^63 to 2^63-1, and must raise an error for an integer that it cannot hold without loss.[^1] Version 1.1.0 lets an implementation support any size, but keeps the recommendation and the error rule.[^2]

**Floats.** A float has a fraction, an exponent or both, and the decimal point must have a digit on each side, so `.7` and `7.` are not valid. The special values are `inf`, `+inf`, `-inf`, `nan`, `+nan` and `-nan`, always in lowercase.[^2] Version 1.0.0 says floats should be IEEE 754 binary64 values.[^1]

**Booleans.** The only booleans are `true` and `false`, in lowercase.[^2]

**Date and time.** TOML has four types, based on RFC 3339. An offset date-time, such as `1979-05-27T07:32:00-07:00`, is one instant. A local date-time, such as `1979-05-27T07:32:00`, has no offset, and it cannot become an instant without more information. A local date is a whole day, and a local time is a time of day with no date. A space can replace the `T`. Implementations must support at least millisecond precision, and they truncate extra digits.[^2]

**Arrays.** An array is a list of values in square brackets. The values can be of different types, and the array can span lines, with comments and a trailing comma.[^2]

**Tables.** A header `[name]` on its own line starts a table. The keys under it, up to the next header or the end of the file, belong to it. The keys before the first header belong to the root table. A header can use a dotted name such as `[servers.alpha]`, and TOML makes the parent tables that the name needs.[^2] A table can be defined only once. The key and value pairs of a table are not in a guaranteed order.[^2]

**Inline tables.** The form `point = { x = 1, y = 2 }` defines a table inside a value. An inline table is complete: no key can be added to it later, from outside the braces.[^2]

**Arrays of tables.** A header in double brackets, `[[product]]`, adds one more table to the array `product` each time it appears. A sub-table header such as `[product.size]` refers to the last table of the array.[^2]

### The rules for defining tables

Most errors in TOML files come from the rule that each table is defined once. The rule has several forms:[^2]

- Two headers with the same name are an error.
- A header cannot name a table that dotted keys already defined. In `[fruit]` with `apple.color = "red"`, a later `[fruit.apple]` is an error. A header for a deeper table, such as `[fruit.apple.texture]`, is allowed.
- Dotted keys cannot add to a table that a header defined.
- Keys cannot be added to an inline table after it is closed, and an inline table cannot add keys to a table that exists.
- `[[fruits]]` cannot append to an array that a key and value pair defined, such as `fruits = []`.
- A header can define a parent table after its child, as `[x]` after `[x.y.z.w]`, because the child header only made `x` implicitly.

## Examples

The document below has each type of value. The Python module `tomllib` parses it into a `dict`. It maps an offset date-time to a `datetime` with a `tzinfo`, a local date-time to a `datetime` with no `tzinfo`, a local date to a `date` and a local time to a `time`.[^7]

```python run id=parse-config
import tomllib

text = """
# A service configuration
name = "billing"
port = 8080
ratio = 0.25
debug = false
started = 2026-11-10T09:30:00+07:00
cutoff = 2026-11-10T17:00:00
holiday = 2026-12-25
backup = 02:30:00
hosts = ["alpha", "beta"]

[database]
url = 'postgres://db.local/billing'
pool.size = 10

[[worker]]
queue = "invoices"

[[worker]]
queue = "refunds"
"""

config = tomllib.loads(text)
for key, value in config.items():
    print(f"{key:9} {type(value).__name__:9} {value!r}")
print(config["started"].utcoffset(), config["cutoff"].tzinfo)
```

```output
name      str       'billing'
port      int       8080
ratio     float     0.25
debug     bool      False
started   datetime  datetime.datetime(2026, 11, 10, 9, 30, tzinfo=datetime.timezone(datetime.timedelta(seconds=25200)))
cutoff    datetime  datetime.datetime(2026, 11, 10, 17, 0)
holiday   date      datetime.date(2026, 12, 25)
backup    time      datetime.time(2, 30)
hosts     list      ['alpha', 'beta']
database  dict      {'url': 'postgres://db.local/billing', 'pool': {'size': 10}}
worker    list      [{'queue': 'invoices'}, {'queue': 'refunds'}]
7:00:00 None
```

The rules for defining tables are the part of the format that a person breaks most often. Each document below breaks one rule, except the last, which adds a sub-table to a table that a header defined. The parser raises `TOMLDecodeError` with the line and the column.[^7]

```python run id=redefinition
import tomllib

documents = {
    "key twice": 'name = "a"\nname = "b"\n',
    "table twice": "[fruit]\napple = 1\n[fruit]\npear = 2\n",
    "dotted, then header": "[fruit]\napple.color = 'red'\n[fruit.apple]\nsize = 3\n",
    "inline, then extend": "[product]\ntype = { name = 'Nail' }\ntype.edible = false\n",
    "append to static array": "fruits = []\n[[fruits]]\n",
    "header, then subtable": "[fruit]\napple = 1\n[fruit.pear]\nsize = 2\n",
}
for name, text in documents.items():
    try:
        tomllib.loads(text)
        print(f"{name}: valid")
    except tomllib.TOMLDecodeError as error:
        print(f"{name}: {error}")
```

```output
key twice: Cannot overwrite a value (at line 2, column 11)
table twice: Cannot declare ('fruit',) twice (at line 3, column 7)
dotted, then header: Cannot declare ('fruit', 'apple') twice (at line 3, column 13)
inline, then extend: Cannot mutate immutable namespace ('product', 'type') (at line 3, column 20)
append to static array: Cannot mutate immutable namespace ('fruits',) (at line 2, column 9)
header, then subtable: valid
```

A TOML float becomes a binary floating-point number by default, so `0.10` loses its decimal form. The argument `parse_float` gets the text of each float, and `decimal.Decimal` keeps it exact.[^7] Python integers have no fixed size, so `tomllib` keeps an integer above 2^63-1 without loss. A parser with 64-bit integers must reject the same document (see the Rust example).[^2]

```python run id=numbers
import tomllib
from decimal import Decimal

text = "price = 0.10\nlimit = 9223372036854775808\nmask = 0xFF_FF\nmissing = nan\n"
data = tomllib.loads(text)
print(data)
print(repr(tomllib.loads(text, parse_float=Decimal)["price"]))
```

```output
{'price': 0.1, 'limit': 9223372036854775808, 'mask': 65535, 'missing': nan}
Decimal('0.10')
```

The example files show how Python, Rust and Java read and write TOML, how each one reports errors, and which writers keep the comments of a file.

## Versions

| Version | Date | Main changes |
|---|---|---|
| 0.1.0 | 17 March 2013 | the first release[^3] |
| 0.2.0 | 24 September 2013 | "tables" replace "key groups"; arrays of tables[^3] |
| 0.3.0 | 10 November 2014 | exponents in floats; multi-line and literal strings; RFC 3339 datetimes[^3] |
| 0.4.0 | 12 February 2015 | inline tables; underscores in numbers; Unicode escapes must be scalar values[^3] |
| 0.5.0 | 11 July 2018 | dotted keys; hexadecimal, octal and binary integers; `inf` and `nan`; local date-time, local date and local time; the ABNF grammar; the extension `.toml` and the media type `application/toml`[^3] |
| 1.0.0-rc.1 | 1 April 2020 | arrays with values of different types; raw tabs in basic strings; inline tables are immutable[^3] |
| 1.0.0 | 11 January 2021 | clarifications about how tables are created and defined, and about the root table[^3] |
| 1.1.0 | 18 December 2025 | newlines and trailing commas in inline tables; the escapes `\xHH` and `\e`; seconds are optional in times[^3] |

Before 1.0.0, all values of an array had to have the same type. The release candidate 1.0.0-rc.1 removed this rule, so `[1, "two"]` is valid in 1.0.0.[^3]

Version 1.1.0 is the first release after 1.0.0 that changes the syntax. In 1.0.0, an inline table must be on one line and must not have a trailing comma.[^1] In 1.1.0, it can span lines and can end with a comma. A basic string can use `\xHH` for a code point up to 255 and `\e` for the escape character U+001B. A time can leave out the seconds, as in `07:32`, and `:00` is assumed.[^2] A document that uses these forms is not valid TOML 1.0.0, so a parser of 1.0.0 rejects it. Check which version a parser implements before a file uses the forms of 1.1.0. The documentation of `tomllib` in Python 3.14 names TOML 1.0.0.[^7] The release notes of Python 3.15 say that its `tomllib` supports TOML 1.1.0.[^9] The Rust crate `toml` puts the version of the specification in its own version string, as in `1.2.1+spec-1.1.0`.[^10]

## Pitfalls

- **A table defined twice.** Two `[name]` headers, a header after dotted keys for the same table, or dotted keys that add to a header table are all errors. Keep all the keys of a table under one header.
- **A dotted key that looks like a number.** `3.14159 = "pi"` is the table `3` with the key `14159`.[^2]
- **Inline tables are closed.** An inline table cannot be extended later. Use a `[table]` header when other parts of the file must add keys.[^2]
- **Integers outside 64 bits.** A parser with 64-bit integers raises an error for 2^63, and Python returns the exact value. A file that needs such numbers does not move well between languages. Write them as strings.
- **Floats in money.** `0.10` becomes the nearest binary64 value in most parsers. Use `parse_float=Decimal` in Python, or store the amount as an integer of the smallest unit, or as a string.
- **Local date and time values have no zone.** `2026-11-10T17:00:00` is not an instant. The program must choose a zone, and two programs can choose differently.[^2] Write an offset when the value is an instant.
- **No null.** TOML has no null value. Leave the key out, and let the program apply a default.[^2]
- **The order of keys.** The specification does not guarantee an order for the pairs of a table.[^2] A program that needs an order must use an array.
- **Writers lose comments.** A program that parses a file into maps and writes it again loses the comments and the layout. PEP 680 gives this as a reason why `tomllib` has no writer.[^8] Use a library that keeps the format, such as `toml_edit` in Rust or TomlJ in Java, to change a file that people edit.[^11][^12] Test the edit: in the Rust example, `toml_edit` drops the comment at the end of a line when the program replaces the value on that line.
- **`tomllib` is read-only.** The module has no `dump` function. Its documentation names the packages Tomli-W, to write TOML, and TOML Kit, to edit TOML and keep its style.[^7]
- **Strings written by a JSON encoder.** A JSON string is not always a valid TOML basic string. JSON writes a character outside the Basic Multilingual Plane as two `\u` escapes of surrogates, and TOML rejects an escape that is not a scalar value.[^2] The Python example file shows this.
- **The TOML version of the parser.** A file with multi-line inline tables or times without seconds is valid in 1.1.0 and invalid in 1.0.0.
- **Untrusted input.** The documentation of `tomllib` warns that a malicious document can make the parser use much CPU time and memory. Limit the size of the input.[^7]

## Tools and libraries

| Language | Verdict | Parse and write |
|---|---|---|
| Python | stdlib (read only) | `tomllib.load`, `tomllib.loads`; Tomli-W and TOML Kit to write[^7] |
| Rust | library | `toml::from_str`, `toml::to_string`; `toml_edit::DocumentMut` to edit a file and keep its format[^10][^11] |
| Java | library | TomlJ `Toml.parse`, `toToml`[^12] |
| Go | library | BurntSushi/toml, go-toml v2[^16][^17] |
| JavaScript, TypeScript | library | smol-toml[^18] |
| C# | library | Tomlyn[^19] |
| C++ | library | toml++[^20] |
| Kotlin | library | ktoml, or a Java library on the JVM[^22] |
| Bash | library | the command-line tool `yq` reads and writes TOML[^21] |

The project toml-test is a test suite for TOML parsers and writers, independent of any language. It has valid documents, which a decoder must turn into the expected JSON, and invalid documents, which it must reject.[^15] The READMEs of BurntSushi/toml, go-toml v2, smol-toml, Tomlyn and toml++ each say that the library supports TOML 1.1.0.[^16][^17][^18][^19][^20]

## References
