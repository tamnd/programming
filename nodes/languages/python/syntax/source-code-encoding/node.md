---
tag: 3DA80Q
kind: feature
owner: python
slug: source-code-encoding
name: Source Code Encoding
aliases: [coding declaration, encoding cookie, "# -*- coding: utf-8 -*-", source file encoding]
area: languages/python/syntax
level: intro
summary: >-
  Python reads a source file as UTF-8 unless a comment on the first or second line declares another encoding. A UTF-8 byte order mark at the start is ignored.
grammar: "a comment on line 1 or 2 that matches coding[=:]\\s*([-\\w.]+) (Language Reference, 2.1.4 Encoding declarations)"
spec: {standard: null, url: "https://docs.python.org/3.15/reference/lexical_analysis.html#encoding-declarations", locator: "2.1.4 Encoding declarations"}
see_also: [python/comments-in-python, python/identifiers-in-python, python/string-literals]
sources:
  - url: https://docs.python.org/3.15/reference/lexical_analysis.html#encoding-declarations
    title: The Python Language Reference, Python 3.15
    locator: 2.1.4 Encoding declarations
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/lexical_analysis.html#lexical-analysis
    title: The Python Language Reference, Python 3.15
    locator: 2 Lexical analysis
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/tokenize.html#tokenize.detect_encoding
    title: tokenize, Python 3.15 documentation
    locator: tokenize.detect_encoding()
    accessed: 2026-11-16
  - url: https://peps.python.org/pep-0263/
    title: "PEP 263: Defining Python Source Code Encodings"
    locator: Defining the Encoding; Concepts
    accessed: 2026-11-16
  - url: https://peps.python.org/pep-3120/
    title: "PEP 3120: Using UTF-8 as the default source encoding"
    locator: Specification
    accessed: 2026-11-16
  - url: https://peps.python.org/pep-0008/#source-file-encoding
    title: "PEP 8: Style Guide for Python Code"
    locator: Source File Encoding
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec#Source_code_representation
    title: The Go Programming Language Specification
    locator: Source code representation
    accessed: 2026-11-16
---

## Synopsis

```text
# -*- coding: latin-1 -*-           line 1: the form that GNU Emacs also reads
#!/usr/bin/env python3              line 1: a shebang, then
# vim:fileencoding=cp1252           line 2: the form that Vim also reads
```

A file with no declaration is UTF-8. Most files need no declaration.[^1]

## Semantics

**r1.** The lexical analyzer finds the encoding of the program text first, "UTF-8 by default", and decodes the text into characters. If the text cannot be decoded, the result is a `SyntaxError`.[^2] Example: [`utf8-default`](#utf8-default), [`no-declaration`](#no-declaration).

**r2.** A comment on the first or second line that matches the regular expression `coding[=:]\s*([-\w.]+)` is an encoding declaration, and the group names the encoding. The declaration must be on a line of its own. When it is on the second line, the first line must be a comment only, such as a shebang line.[^1] Example: [`declared-latin1`](#declared-latin1).

**r3.** The encoding name must be one that Python knows. The encoding applies to all lexical analysis: string literals, comments and identifiers.[^1]

**r4.** If the encoding is UTF-8, by default or by declaration, a UTF-8 byte order mark (`b'\xef\xbb\xbf'`) at the start of the file is ignored "rather than being a syntax error".[^1] When a file has a byte order mark and a declaration that disagree, the result is a `SyntaxError`.[^3] Example: [`detect`](#detect), [`bom-conflict`](#bom-conflict).

**r5.** Any Unicode code point except NUL can appear in the source.[^1]

**r6.** `tokenize.detect_encoding()` applies the same rules and returns the name of the encoding. It returns `'utf-8-sig'` when it finds a byte order mark, and `'utf-8'` when there is no declaration.[^3] Example: [`detect`](#detect).

**r7.** An encoding must let the tokenizer read the first two lines as ASCII. So encodings such as Shift_JIS are allowed, but UTF-16, which uses two or more bytes for each character, is not.[^4]

## Examples

`compile()` takes the source as bytes, as the interpreter reads a file, so the examples can show each case in one program.

UTF-8 needs no declaration (rule r1).

```python run id=utf8-default
source = "name = 'Zoë'\nprint(name, len(name))\n".encode("utf-8")
exec(compile(source, "demo.py", "exec"))
```

```output for=utf8-default
Zoë 3
```

The byte `\xe9` is `é` in Latin-1. With the declaration, the file decodes (rule r2).

```python run id=declared-latin1
source = b"# -*- coding: latin-1 -*-\nprint('caf\xe9')\n"
exec(compile(source, "demo.py", "exec"))
```

```output for=declared-latin1
café
```

The encoding that the rules give for five files (rules r2, r4 and r6). `detect_encoding` gives the normal name of the codec, so `latin-1` shows as `iso-8859-1`.

```python run id=detect
import io
import tokenize

samples = {
    "no declaration": b"x = 1\n",
    "Emacs form": b"# -*- coding: latin-1 -*-\nx = 1\n",
    "Vim form": b"# vim:fileencoding=cp1252\nx = 1\n",
    "shebang first": b"#!/usr/bin/env python3\n# coding: utf-8\nx = 1\n",
    "byte order mark": b"\xef\xbb\xbfx = 1\n",
}
for label, data in samples.items():
    encoding, _ = tokenize.detect_encoding(io.BytesIO(data).readline)
    print(f"{label:16} {encoding}")
```

```output for=detect
no declaration   utf-8
Emacs form       iso-8859-1
Vim form         cp1252
shebang first    utf-8
byte order mark  utf-8-sig
```

## Versions

> [!since v="3.0"]
> UTF-8 is the default source encoding (PEP 3120). An explicit declaration still "takes precedence over the default".[^5]

PEP 263 made the declaration a part of the language for Python 2, where the default was ASCII.[^4]

## Pitfalls

### Latin-1 bytes with no declaration

A file saved by an editor in Latin-1 or Windows-1252, with no declaration, is read as UTF-8. The first byte that is not valid UTF-8 stops the program before it starts (rule r1).

```python fails id=no-declaration expect=exception:SyntaxError
source = b"print('caf\xe9')\n"
exec(compile(source, "demo.py", "exec"))
```

### A declaration on the third line does nothing

Only the first two lines can hold the declaration (rule r2). On the third line it is a normal comment, so the file is read as UTF-8.

```python wrong id=third-line
source = b"# A tool.\n# Author: Zoe\n# -*- coding: latin-1 -*-\nprint('caf\xe9')\n"
try:
    exec(compile(source, "demo.py", "exec"))
except SyntaxError as error:
    print(type(error).__name__)
```

```output for=third-line
SyntaxError
```

### A byte order mark with another declared encoding

A byte order mark says UTF-8. A declaration of another encoding in the same file is an error (rule r4).

```python fails id=bom-conflict expect=exception:SyntaxError
source = b"\xef\xbb\xbf# -*- coding: latin-1 -*-\nx = 1\n"
exec(compile(source, "demo.py", "exec"))
```

### Declarations in new code

PEP 8 says that the code of the core distribution "should always use UTF-8, and should not have an encoding declaration".[^6] A declaration of UTF-8 is harmless, but it adds nothing.

## Comparison

Go fixes the encoding in its specification: "Source code is Unicode text encoded in UTF-8." A compiler may ignore a byte order mark at the start.[^7] Python has the same default, but a file can declare another encoding (rule r2).
