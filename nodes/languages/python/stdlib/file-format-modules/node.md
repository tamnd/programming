---
tag: 0C5Q0P
kind: feature
owner: python
slug: file-format-modules
name: File format modules
aliases: [csv, json, tomllib, configparser, plistlib, CSV files, JSON, TOML, INI files, property lists]
area: languages/python/stdlib
level: core
summary: >-
  The standard library reads and writes common data files: csv for comma-separated values, json for JSON, tomllib for TOML, configparser for INI-style files and plistlib for Apple property lists.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/library/fileformats.html", locator: "File Formats"}
proposals: []
see_also: [python/pickle-and-object-serialization, python/xml-and-html-modules, python/the-sqlite3-module, python/text-and-binary-io, python/data-compression]
sources:
  - url: https://docs.python.org/3.15/library/fileformats.html
    title: File Formats, Python 3.15 documentation
    locator: File Formats
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/csv.html
    title: csv, CSV File Reading and Writing, Python 3.15 documentation
    locator: Introduction; csv.reader(); csv.writer(); Footnotes
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/json.html
    title: json, JSON encoder and decoder, Python 3.15 documentation
    locator: Introduction; Basic Usage; Encoders and Decoders
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/tomllib.html
    title: tomllib, Parse TOML files, Python 3.15 documentation
    locator: Introduction; load(); Conversion Table
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/configparser.html
    title: configparser, Configuration file parser, Python 3.15 documentation
    locator: Introduction; Quick Start; Supported Datatypes
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/plistlib.html
    title: plistlib, Generate and parse Apple .plist files, Python 3.15 documentation
    locator: Introduction
    accessed: 2026-11-16
---

## Synopsis

```python run id=three-formats
import csv
import io
import json
import tomllib

text = 'name,langs\nann,"python,go"\nbob,c\n'
rows = list(csv.DictReader(io.StringIO(text, newline="")))
print(rows[0])
print(json.dumps(rows[1]))
config = tomllib.loads('[server]\nport = 8080\nhosts = ["a", "b"]\n')
print(config)
```

```output for=three-formats
{'name': 'ann', 'langs': 'python,go'}
{"name": "bob", "langs": "c"}
{'server': {'port': 8080, 'hosts': ['a', 'b']}}
```

## Semantics

**r1.** "The modules described in this chapter parse various miscellaneous file formats that aren’t markup languages and are not related to e-mail."[^1] The chapter has `csv`, `configparser`, `tomllib` and `plistlib`. The `json` module is in another chapter, but it does the same kind of job.

**r2.** "The so-called CSV (Comma Separated Values) format is the most common import and export format for spreadsheets and databases." "The lack of a well-defined standard means that subtle differences often exist in the data produced and consumed by different applications."[^2] A dialect describes one variant. Example: [`csv-quoting`](#csv-quoting).

**r3.** For `csv.reader`, "Each row read from the csv file is returned as a list of strings. No automatic data type conversion is performed".[^2] Example: [`csv-strings`](#csv-strings).

**r4.** A file for `csv` "should be opened with newline=''".[^2] Otherwise "newlines embedded inside quoted fields will not be interpreted correctly".[^2] Example: [`csv-quoting`](#csv-quoting).

**r5.** `json.dumps` turns Python values into JSON text, and `json.loads` reads it back. A `list` or a `tuple` becomes a JSON array, and `None` becomes `null`.[^3] Example: [`json-round-trip`](#json-round-trip).

**r6.** "Keys in key/value pairs of JSON are always of the type str. When a dictionary is converted into JSON, all the keys of the dictionary are converted to strings."[^3] Example: [`json-int-keys`](#json-int-keys).

**r7.** `tomllib` reads TOML. "This module does not support writing TOML."[^4] `tomllib.load` needs "a readable and binary file object" and returns a dict.[^4] A TOML offset date-time becomes a `datetime.datetime`. Example: [`toml-types`](#toml-types).

**r8.** `configparser` reads "a basic configuration language which provides a structure similar to what’s found in Microsoft Windows INI files."[^5] "Config parsers do not guess datatypes of values in configuration files, always storing them internally as strings."[^5] Example: [`ini-file`](#ini-file).

**r9.** `plistlib` reads and writes "the “property list” files used by Apple, primarily on macOS and iOS. This module supports both binary and XML plist files."[^6] Example: [`plist`](#plist).

## Examples

```python run id=csv-quoting
import csv
import io

out = io.StringIO(newline="")
writer = csv.writer(out)
writer.writerow(["id", "note"])
writer.writerow([1, 'says "hi", then\nleaves'])
writer.writerow([2, "plain"])
print(repr(out.getvalue()))
back = list(csv.reader(io.StringIO(out.getvalue(), newline="")))
print(back)
```

```output for=csv-quoting
'id,note\r\n1,"says ""hi"", then\nleaves"\r\n2,plain\r\n'
[['id', 'note'], ['1', 'says "hi", then\nleaves'], ['2', 'plain']]
```

The writer puts quotes around a field with a comma, a quote or a newline, and it doubles each quote in the field. The default `excel` dialect ends each row with `\r\n`. The reader undoes all of this, and the number `1` comes back as the string `'1'` (rule r3).

```python run id=json-round-trip
import json

data = {"name": "ann", "tags": ("a", "b"), "score": 9.5, "admin": False, "boss": None}
text = json.dumps(data, indent=2, sort_keys=True)
print(text)
back = json.loads(text)
print(back["tags"], back == data)
```

```output for=json-round-trip
{
  "admin": false,
  "boss": null,
  "name": "ann",
  "score": 9.5,
  "tags": [
    "a",
    "b"
  ]
}
['a', 'b'] False
```

The tuple comes back as a list, so the result is not equal to the input (rule r5). `indent` and `sort_keys` make the text easy to read and to compare.

```python run id=toml-types
import tomllib

doc = """
title = "demo"
created = 2026-11-16T09:30:00Z

[owner]
name = "ann"

[[plugins]]
name = "lint"
enabled = true
"""
config = tomllib.loads(doc)
print(config["created"], type(config["created"]).__name__)
print(config["owner"], config["plugins"])
```

```output for=toml-types
2026-11-16 09:30:00+00:00 datetime
{'name': 'ann'} [{'name': 'lint', 'enabled': True}]
```

A table becomes a dict, and an array of tables (`[[plugins]]`) becomes a list of dicts (rule r7).

```python run id=ini-file
import configparser

parser = configparser.ConfigParser()
parser.read_string("""
[DEFAULT]
Timeout = 30

[server]
port = 8080
debug = no
""")
section = parser["server"]
print(section["port"], type(section["port"]).__name__)
print(section.getint("port"), section.getboolean("debug"), section["timeout"])
print(bool(section["debug"]), list(section))
```

```output for=ini-file
8080 str
8080 False 30
True ['port', 'debug', 'timeout']
```

Each value is a string (rule r8). `getint` and `getboolean` convert it. `bool('no')` is `True`, because any non-empty string is true: "simply passing the value to bool() would do no good since bool('False') is still True."[^5] The `DEFAULT` section gives `timeout` to every section, and "keys in sections are case-insensitive and stored in lowercase".[^5]

```python run id=plist
import plistlib

data = {"name": "demo", "version": 3, "items": [1, 2]}
xml = plistlib.dumps(data)
print(xml.splitlines()[0])
binary = plistlib.dumps(data, fmt=plistlib.FMT_BINARY)
print(binary[:8], plistlib.loads(binary) == data, plistlib.loads(xml) == data)
```

```output for=plist
b'<?xml version="1.0" encoding="UTF-8"?>'
b'bplist00' True True
```

`dumps` writes XML by default. `loads` finds the format from the data (rule r9).

## Versions

> [!since v="3.11"]
> `tomllib` is new, "with support for TOML 1.0.0."[^4]

> [!since v="3.15"]
> `tomllib` supports TOML 1.1.0.[^4]

## Pitfalls

```python wrong id=csv-strings
import csv
import io

rows = list(csv.reader(io.StringIO("9\n10\n100\n", newline="")))
values = [row[0] for row in rows]
print(sorted(values), max(values))
print(sorted(int(v) for v in values))
```

```output for=csv-strings
['10', '100', '9'] 9
[9, 10, 100]
```

The values are strings, so they sort as text and `'9'` is the largest (rule r3). Convert each field to the type you need.

```python wrong id=json-int-keys
import json

counts = {1: "one", 2: "two"}
back = json.loads(json.dumps(counts))
print(back, back == counts)
```

```output for=json-int-keys
{'1': 'one', '2': 'two'} False
```

The integer keys come back as strings (rule r6). Convert them after loading, or store the data as a list of pairs.

```python fails id=json-set expect=exception:TypeError
import json

print(json.dumps({"tags": {"a", "b"}}))
```

JSON has no set type, so `dumps` raises `TypeError`. Convert the set to a sorted list, or pass a `default` function that does it.

## Comparison

These formats store plain data: strings, numbers, lists and maps. For arbitrary Python objects, see [[python/pickle-and-object-serialization]], which is not safe for untrusted input. For XML, see [[python/xml-and-html-modules]]. For JSON input from outside, the docs warn: "A malicious JSON string may cause the decoder to consume considerable CPU and memory resources. Limiting the size of data to be parsed is recommended."[^3]
