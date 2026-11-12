```python run id=load-file
import os
import tempfile
import tomllib

text = '[tool.example]\nname = "billing"\npaths = ["src", "tests"]\n'
folder = tempfile.mkdtemp()
path = os.path.join(folder, "pyproject.toml")
with open(path, "w", encoding="utf-8") as handle:
    handle.write(text)

with open(path, "rb") as handle:
    print(tomllib.load(handle))

try:
    with open(path, encoding="utf-8") as handle:
        tomllib.load(handle)
except TypeError as error:
    print("TypeError:", error)

os.remove(path)
os.rmdir(folder)
```

```output
{'tool': {'example': {'name': 'billing', 'paths': ['src', 'tests']}}}
TypeError: File must be opened in binary mode, e.g. use `open('foo.toml', 'rb')`
```

```python run id=keys-and-strings
import tomllib

text = r'''
3.14159 = "pi"
"quoted key" = 1
site."google.com" = true
path = 'C:\Users\tam\config'
regex = '<\i\c*\s*>'
tab = "a\tb"
lines = """
first \
  second"""
'''
for key, value in tomllib.loads(text).items():
    print(repr(key), repr(value))
```

```output
'3' {'14159': 'pi'}
'quoted key' 1
'site' {'google.com': True}
'path' 'C:\\Users\\tam\\config'
'regex' '<\\i\\c*\\s*>'
'tab' 'a\tb'
'lines' 'first second'
```

```python run id=json-escapes
import json
import tomllib

name = "smile \U0001F600"
escaped = json.dumps(name)
print(escaped)
print(tomllib.loads(f"name = {json.dumps(name, ensure_ascii=False)}\n"))
try:
    tomllib.loads(f"name = {escaped}\n")
except tomllib.TOMLDecodeError as error:
    print("TOMLDecodeError:", error)
print(tomllib.loads('name = "smile \\U0001F600"\n'))
```

```output
"smile \ud83d\ude00"
{'name': 'smile 😀'}
TOMLDecodeError: Escaped character is not a Unicode scalar value (at line 1, column 21)
{'name': 'smile 😀'}
```

```python run id=version-1-1
import sys
import tomllib

documents = {
    "multi-line inline table": "point = {\n  x = 1,\n  y = 2,\n}\n",
    "time without seconds": "alarm = 07:30\n",
    "escape \\e": 'bell = "\\e[1m"\n',
}
print(sys.version_info[:2])
for name, text in documents.items():
    try:
        print(f"{name}: {tomllib.loads(text)}")
    except tomllib.TOMLDecodeError as error:
        print(f"{name}: TOMLDecodeError: {error}")
```

```output
(3, 14)
multi-line inline table: TOMLDecodeError: Invalid initial character for a key part (at line 1, column 10)
time without seconds: TOMLDecodeError: Expected newline or end of document after a statement (at line 1, column 10)
escape \e: TOMLDecodeError: Unescaped '\' in a string (at line 1, column 11)
```

- Verdict: stdlib, read only. Python 3.11 added `tomllib`. It parses with `tomllib.loads` (a string) and `tomllib.load` (a binary file). It has no function to write TOML.
- `load` needs a file opened in binary mode. A file opened in text mode raises `TypeError`.
- `3.14159` as a key makes a nested table, and a quoted key can hold a dot. A literal string keeps backslashes as they are. A backslash at the end of a line in a multi-line basic string removes the newline and the indentation that follow it.
- `json.dumps` writes a character outside the Basic Multilingual Plane as two surrogate escapes. TOML rejects them, so a JSON string is not always a TOML string. `ensure_ascii=False`, or the escape `\U0001F600`, gives a valid value.
- The `tomllib` of Python 3.14 implements TOML 1.0.0, so it rejects the syntax that TOML 1.1.0 added: inline tables over several lines, times without seconds and the escape `\e`.
- To write TOML, use a package from PyPI such as Tomli-W. To change a file and keep its comments, use TOML Kit.
