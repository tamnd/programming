```python run id=codes-python
OK, EMPTY, NOT_A_NUMBER, OUT_OF_RANGE = 0, 1, 2, 3
NAMES = ["ok", "empty", "not a number", "out of range"]


def parse_port(text):
    """Return (code, port). The port is valid only when the code is OK."""
    if text == "":
        return EMPTY, 0
    if not (text.isascii() and text.isdigit()):
        return NOT_A_NUMBER, 0
    port = int(text)
    if port > 65535:
        return OUT_OF_RANGE, 0
    return OK, port


for text in ["8080", "", "x", "99999"]:
    code, port = parse_port(text)
    if code != OK:
        print(f"'{text}' -> error code {code} ({NAMES[code]})")
    else:
        print(f"'{text}' -> port {port}")
```

```output for=codes-python
'8080' -> port 8080
'' -> error code 1 (empty)
'x' -> error code 2 (not a number)
'99999' -> error code 3 (out of range)
```

- Verdict: idiom. Python reports errors with exceptions, so a function that returns a code is a choice of the programmer. The operating system codes are still visible: an `OSError` has the attribute `errno`, and the module `errno` maps each code to a name such as `ENOENT`.
