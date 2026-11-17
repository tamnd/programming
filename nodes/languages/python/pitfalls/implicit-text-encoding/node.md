---
tag: HWYW0D
kind: feature
owner: python
slug: implicit-text-encoding
name: Implicit text encoding
aliases: [open without encoding, default encoding, locale encoding, EncodingWarning, UnicodeDecodeError on Windows, mojibake, PLW1514, W1514, unspecified-encoding, PEP 597, PEP 686]
area: languages/python/pitfalls
level: core
summary: >-
  open() in text mode with no encoding argument used the locale encoding before 3.15, so the same code read UTF-8 files wrong on some systems. Pass encoding="utf-8", or encoding="locale" on purpose.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/library/io.html#text-encoding", locator: "io, Text Encoding"}
proposals: [pep-597, pep-686]
see_also: [python/reading-and-writing-files, python/text-and-binary-io, python/encoding-and-decoding, python/unicode-in-python, python/development-mode-and-x-options, python/warnings, python/subprocess, python/saving-structured-data-with-json]
sources:
  - url: https://docs.python.org/3.15/whatsnew/3.10.html#optional-encodingwarning-and-encoding-locale-option
    title: What’s New In Python 3.10
    locator: Optional EncodingWarning and encoding="locale" option
    accessed: 2026-11-18
  - url: https://peps.python.org/pep-0597/
    title: PEP 597, Add optional EncodingWarning
    locator: Motivation
    accessed: 2026-11-18
  - url: https://docs.python.org/3.15/library/functions.html#open
    title: Built-in Functions, Python 3.15 documentation
    locator: open()
    accessed: 2026-11-18
  - url: https://docs.python.org/3.15/library/os.html#python-utf-8-mode
    title: os, Miscellaneous operating system interfaces, Python 3.15 documentation
    locator: Python UTF-8 Mode
    accessed: 2026-11-18
  - url: https://docs.python.org/3.15/library/io.html#text-encoding
    title: io, Core tools for working with streams, Python 3.15 documentation
    locator: Text Encoding; Opt-in EncodingWarning; io.text_encoding()
    accessed: 2026-11-18
  - url: https://docs.astral.sh/ruff/rules/unspecified-encoding/
    title: unspecified-encoding (PLW1514), Ruff documentation
    locator: What it does; Why is this bad?
    accessed: 2026-11-18
  - url: https://pylint.readthedocs.io/en/stable/user_guide/messages/warning/unspecified-encoding.html
    title: unspecified-encoding / W1514, Pylint documentation
    locator: Message emitted; Description
    accessed: 2026-11-18
  - url: https://docs.python.org/3.15/whatsnew/3.15.html
    title: What’s New In Python 3.15
    locator: "PEP 686: Python now uses UTF-8 as the default encoding"
    accessed: 2026-11-18
---

## Synopsis

```python wrong id=wrong-codec
import pathlib
import tempfile

with tempfile.TemporaryDirectory() as folder:
    path = pathlib.Path(folder, "menu.txt")
    path.write_text("café, phở\n", encoding="utf-8")

    with open(path, encoding="cp1252", errors="replace") as f:
        print(f.read().strip())

    with open(path, encoding="utf-8") as f:
        print(f.read().strip())
```

```output for=wrong-codec
cafÃ©, phá»Ÿ
café, phở
```

The first read decodes UTF-8 bytes with a Windows code page, which is what an `open` with no encoding did on many Windows systems before Python 3.15. The second read names the encoding of the file.

## Semantics

**r1.** Before Python 3.15, "The default encoding of TextIOWrapper and open() is platform and locale dependent. Since UTF-8 is used on most Unix platforms, omitting encoding option when opening UTF-8 files (e.g. JSON, YAML, TOML, Markdown) is a very common bug."[^1] The code works on one computer and gives wrong text or an error on another. Example: [`wrong-codec`](#wrong-codec).

**r2.** PEP 597 gives a case: "using long_description = open("README.md").read() in setup.py is a common mistake. Many Windows users cannot install such packages if there is at least one non-ASCII character (e.g. emoji, author names, copyright symbols, and the like) in their UTF-8-encoded README.md file."[^2]

**r3.** In Python 3.15, the default is UTF-8: "The default encoding is UTF-8; if Python UTF-8 Mode is disabled, the default is platform-dependent (whatever locale.getencoding() returns)."[^3] The mode can still be turned off: "It may be disabled by setting PYTHONUTF8=0 as an environment variable or by using the -X utf8=0 command line option."[^4] So code with no encoding still depends on how Python starts. Example: [`text-encoding`](#text-encoding).

**r4.** The fix is to name the encoding. Use `encoding='utf-8'` for files that the program owns, and for formats such as JSON. When the locale encoding is the intent, the documentation says that `encoding='locale'` "specifies the current locale encoding explicitly."[^3] For bytes with no fixed text encoding, open the file in binary mode, with `"rb"`.

**r5.** The warning finds the calls. "To find where the default encoding is used, you can enable the -X warn_default_encoding command line option or set the PYTHONWARNDEFAULTENCODING environment variable, which will emit an EncodingWarning when the default encoding is used."[^5] Example: [`find-with-warning`](#find-with-warning).

**r6.** A library function that takes `encoding=None` and passes it to `open` can call `io.text_encoding`, so that the warning points at the caller. The function "returns encoding if it is not None. Otherwise, it returns "locale" or "utf-8" depending on UTF-8 Mode."[^5] The documentation adds: "please consider using UTF-8 by default (i.e. encoding="utf-8") for new APIs."[^5]

**r7.** The linters:

- Ruff PLW1514 "Checks for uses of open and related calls without an explicit encoding argument."[^6] Its reason: "Using open in text mode without an explicit encoding can lead to non-portable code, with differing behavior across platforms."[^6]
- Pylint W1514 `unspecified-encoding` has the message "Using open without explicitly specifying an encoding".[^7]

## Examples

```python run id=find-with-warning
import pathlib
import subprocess
import sys
import tempfile

with tempfile.TemporaryDirectory() as folder:
    path = pathlib.Path(folder, "notes.txt")
    path.write_text("hello\n", encoding="utf-8")
    code = "import sys\nopen(sys.argv[1]).close()\nopen(sys.argv[1], encoding='utf-8').close()\n"
    result = subprocess.run(
        [sys.executable, "-X", "utf8=0", "-X", "warn_default_encoding", "-c", code, str(path)],
        capture_output=True, text=True, encoding="utf-8",
    )
    print(result.stderr.count("EncodingWarning"))
```

```output for=find-with-warning
1
```

The option makes Python warn about the call with no encoding, and not about the call that names one (rule r5).

```python run id=text-encoding
import subprocess
import sys

code = "import io; print(io.text_encoding(None))"
for mode in ["utf8=0", "utf8=1"]:
    result = subprocess.run(
        [sys.executable, "-X", mode, "-c", code],
        capture_output=True, text=True, encoding="utf-8",
    )
    print(mode, result.stdout.strip())
```

```output for=text-encoding
utf8=0 locale
utf8=1 utf-8
```

The default of `open` depends on the UTF-8 Mode, and the mode depends on how Python starts (rule r3). A named encoding does not.

## Versions

> [!since v="3.10"]
> `encoding='locale'` and `EncodingWarning` are new. The warning "is emitted when sys.flags.warn_default_encoding is true and locale-specific default encoding is used."[^1]

> [!since v="3.15"]
> "Python now uses UTF-8 as the default encoding, independent of the system’s environment. This means that I/O operations without an explicit encoding, for example, open('flying-circus.txt'), will use UTF-8."[^8]

## Pitfalls

```python fails id=old-file expect=exception:UnicodeDecodeError
import pathlib
import tempfile

with tempfile.TemporaryDirectory() as folder:
    path = pathlib.Path(folder, "old.txt")
    path.write_bytes("café\n".encode("cp1252"))

    with open(path, encoding="utf-8") as f:
        print(f.read())
```

A file that an older program wrote in a locale encoding is not valid UTF-8. With the UTF-8 default of Python 3.15, or with `encoding='utf-8'`, the read raises `UnicodeDecodeError`. Name the encoding that the file really has, here `encoding='cp1252'` (rule r4).

```python run id=bytes-for-unknown
import pathlib
import tempfile

with tempfile.TemporaryDirectory() as folder:
    path = pathlib.Path(folder, "blob.bin")
    path.write_bytes(b"caf\xe9\n")

    with open(path, "rb") as f:
        data = f.read()
    print(data)
    print(data.decode("utf-8", errors="replace").strip())
```

```output for=bytes-for-unknown
b'caf\xe9\n'
caf�
```

Binary mode reads the bytes with no decoding. The program can then choose the codec and the error rule (rule r4).

## Comparison

In Python, a text file is a stream of `str`, so `open` must choose a codec. Before 3.15, that codec came from the system (rule r1). From 3.15, it is UTF-8 by default, unless the UTF-8 Mode is off (rule r3). In all versions, a named encoding gives the same result everywhere. For files, see [[python/reading-and-writing-files]]. For codecs, see [[python/encoding-and-decoding]].
