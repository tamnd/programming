```python wrong id=no-limits
import io
import zlib

# Each function stands for a request handler. The inputs are small on purpose.


# 1. Read the whole body into memory, however large it is.
def upload(stream):
    body = stream.read()
    return 200, f"stored {len(body)} bytes"


# 2. Parse nested lists with one call for each level.
def parse(text, i=0):
    items = []
    i += 1  # skip "["
    while text[i] != "]":
        child, i = parse(text, i)
        items.append(child)
    return items, i + 1


def nested(text):
    value, _ = parse(text)
    return 200, f"parsed {len(text)} characters"


# 3. Decompress the whole body in one call.
def inflate(data):
    out = zlib.decompress(data)
    return 200, f"{len(data) // 1024} KiB in, {len(out) // 1024} KiB out"


def serve(handler, arg):
    try:
        status, text = handler(arg)
    except Exception as e:
        status, text = 500, type(e).__name__
    print(handler.__name__, status, text)


serve(upload, io.BytesIO(b"x" * (3 * 1024 * 1024)))
serve(nested, "[" * 50_000 + "]" * 50_000)
serve(inflate, zlib.compress(bytes(2 * 1024 * 1024)))
```

```output for=no-limits
upload 200 stored 3145728 bytes
nested 500 RecursionError
inflate 200 2 KiB in, 2048 KiB out
```

The body is read in full, the deep nesting ends in `RecursionError`, and about 2 KiB of input becomes 2 MiB of output.

```python run id=limits
import io
import zlib

# The same handlers with a fixed limit on each resource.
MAX_BODY = 1024 * 1024  # bytes
MAX_DEPTH = 32  # levels of nesting
MAX_OUTPUT = 1024 * 1024  # bytes after decompression


class TooLarge(Exception):
    pass


class TooDeep(Exception):
    pass


# 1. Read at most one byte more than the limit, then decide.
def upload(stream):
    body = stream.read(MAX_BODY + 1)
    if len(body) > MAX_BODY:
        raise TooLarge("body")
    return 200, f"stored {len(body)} bytes"


# 2. Count the depth, and stop at the limit.
def parse(text, i=0, depth=1):
    if depth > MAX_DEPTH:
        raise TooDeep(f"more than {MAX_DEPTH} levels")
    items = []
    i += 1  # skip "["
    while text[i] != "]":
        child, i = parse(text, i, depth + 1)
        items.append(child)
    return items, i + 1


def nested(text):
    value, _ = parse(text)
    return 200, f"parsed {len(text)} characters"


# 3. Ask for at most one byte more than the limit of output.
def inflate(data):
    d = zlib.decompressobj()
    out = d.decompress(data, MAX_OUTPUT + 1)
    if len(out) > MAX_OUTPUT:
        raise TooLarge("decompressed body")
    return 200, f"{len(data)} bytes in, {len(out)} bytes out"


def serve(handler, arg):
    try:
        status, text = handler(arg)
    except TooLarge as e:
        status, text = 413, f"{e} too large"
    except TooDeep as e:
        status, text = 400, f"nesting too deep: {e}"
    print(handler.__name__, status, text)


serve(upload, io.BytesIO(b"x" * (3 * 1024 * 1024)))
serve(upload, io.BytesIO(b"x" * 1000))
serve(nested, "[" * 50_000 + "]" * 50_000)
serve(nested, "[[][[]]]")
serve(inflate, zlib.compress(bytes(2 * 1024 * 1024)))
serve(inflate, zlib.compress(b"hello"))
```

```output for=limits
upload 413 body too large
upload 200 stored 1000 bytes
nested 400 nesting too deep: more than 32 levels
nested 200 parsed 8 characters
inflate 413 decompressed body too large
inflate 200 13 bytes in, 5 bytes out
```

Each handler stops at its limit and answers with an error. Normal input still works.

```python fails expect=timeout timeout=2 id=evil-regex
import re

# A nested quantifier: the classic example from the OWASP ReDoS page.
EVIL = re.compile(r"(a+)+$")

# 30 letters and one character that cannot match. Each extra "a" doubles the work.
print(EVIL.match("a" * 30 + "!"))
```

The nested quantifier backtracks on the classic input. The block is stopped by its time limit of two seconds.

```python run id=one-quantifier
import re

MAX_LENGTH = 100

# One quantifier, no nesting. The engine has only one way to match each letter.
PLAIN = re.compile(r"a+")


def check(text):
    if len(text) > MAX_LENGTH:
        return "rejected: too long"
    return "match" if PLAIN.fullmatch(text) else "no match"


print(check("a" * 30 + "!"))
print(check("a" * 30))
print(check("a" * 10_000 + "!"))
```

```output for=one-quantifier
no match
match
rejected: too long
```

The pattern has one quantifier, and the input has a length limit before the match.

- Verdict: stdlib. `io`, `zlib` and `re` cover the pattern. `read(n)` and `decompress(data, max_length)` take a limit.
- `json.loads` and the other parsers in the standard library recurse in C or in Python. Check the size of the text before you parse it, and check the depth of the result if the format allows deep nesting.
- Do not raise `sys.setrecursionlimit` to make a deep input work. The documentation says that a limit that is too high can lead to a crash. Use an explicit depth limit or an iterative parser.
- The `re` module backtracks. Possessive quantifiers such as `a++` and atomic groups `(?>...)` stop it from giving back what a part of the pattern matched.
- The `int` to `str` limit of 4300 digits is on by default. Do not turn it off for input that you do not control.
