---
tag: 129FHG
kind: feature
owner: python
slug: building-strings
name: Building strings
aliases: [str.join, string builder, io.StringIO, string concatenation in a loop, += on strings, quadratic concatenation, bytes.join]
area: languages/python/strings-and-text
level: core
summary: >-
  A str cannot change, so each + makes a new string, and a loop of += can take quadratic time. Collect the parts in a list and call str.join() once, or write them to an io.StringIO.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/builtins/stdtypes.html#common-sequence-operations", locator: "Common Sequence Operations, note on concatenation"}
see_also: [python/strings, python/string-methods, python/bytes-and-bytearray, python/formatted-string-literals]
sources:
  - url: https://docs.python.org/3.15/builtins/stdtypes.html#common-sequence-operations
    title: Built-in Types, Python 3.15 documentation
    locator: Common Sequence Operations, notes; str.join()
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/io.html#io.StringIO
    title: io, Core tools for working with streams
    locator: class io.StringIO; getvalue()
    accessed: 2026-11-16
  - url: https://peps.python.org/pep-0008/#programming-recommendations
    title: "PEP 8: Style Guide for Python Code"
    locator: Programming Recommendations
    accessed: 2026-11-16
  - url: https://pkg.go.dev/strings#Builder
    title: Package strings, Go standard library
    locator: type Builder
    accessed: 2026-11-16
---

## Synopsis

```python run id=basics
words = ["spam", "eggs", "ham"]
print(", ".join(words))
print("-".join("Python"))
lines = []
for i in range(3):
    lines.append(f"line {i}")
print("\n".join(lines))
```

```output for=basics
spam, eggs, ham
P-y-t-h-o-n
line 0
line 1
line 2
```

The separator string calls join(), and the argument gives the parts. A loop adds parts to a list, and one join() at the end makes the string.

## Semantics

**r1.** "Concatenating immutable sequences always results in a new object. This means that building up a sequence by repeated concatenation will have a quadratic runtime cost in the total sequence length."[^1]

**r2.** "To get a linear runtime cost, you must switch to one of the alternatives below: if concatenating str objects, you can build a list and use str.join() at the end or else write to an io.StringIO instance and retrieve its value when complete".[^1]

**r3.** str.join() returns "a string which is the concatenation of the strings in iterable."[^1] "A TypeError will be raised if there are any non-string values in iterable, including bytes objects. The separator between elements is the string providing this method."[^1] Example: [`join-int`](#join-int).

**r4.** io.StringIO is "A text stream using an in-memory text buffer."[^2] getvalue() returns "a str containing the entire contents of the buffer."[^2] Example: [`stringio`](#stringio).

**r5.** For bytes, "you can similarly use bytes.join() or io.BytesIO, or you can do in-place concatenation with a bytearray object."[^1] Example: [`binary`](#binary).

**r6.** CPython can sometimes make `s += t` fast, but PEP 8 says: "do not rely on CPython’s efficient implementation of in-place string concatenation for statements in the form a += b or a = a + b. This optimization is fragile even in CPython (it only works for some types) and isn’t present at all in implementations that don’t use refcounting."[^3]

## Examples

```python run id=stringio
import io

out = io.StringIO()
out.write("total: ")
print(3, "items", file=out)
print("done", file=out)
text = out.getvalue()
print(repr(text))
```

```output for=stringio
'total: 3 items\ndone\n'
```

print() can write to the buffer with `file=`, so code that prints to the screen can also build a string (rule r4).

```python run id=generator
row = (1, 2.5, None)
print(" | ".join(str(x) for x in row))
print("".join(c.upper() if i % 2 else c for i, c in enumerate("python")))
```

```output for=generator
1 | 2.5 | None
pYtHoN
```

join() accepts any iterable of strings, so a generator expression can convert the parts on the way.

```python run id=binary
import io

chunks = [b"GET ", b"/ ", b"HTTP/1.1"]
print(b"".join(chunks))
buf = bytearray()
for chunk in chunks:
    buf += chunk
print(bytes(buf) == b"".join(chunks))
stream = io.BytesIO()
stream.write(b"abc")
stream.write(b"def")
print(stream.getvalue())
```

```output for=binary
b'GET / HTTP/1.1'
True
b'abcdef'
```

A bytearray grows in place, so `+=` on it does not copy the whole buffer each time (rule r5).

## Pitfalls

A loop of `+=` on a str gives the right result, but each step can copy the whole string (rules r1 and r6):

```python wrong id=plus-loop
s = ""
for i in range(5):
    s += str(i) + ","
print(s)
print("".join(str(i) + "," for i in range(5)))
```

```output for=plus-loop
0,1,2,3,4,
0,1,2,3,4,
```

The two lines are equal. Use the join() form when the loop is long.

join() does not convert numbers:

```python fails id=join-int expect=exception:TypeError
", ".join([1, 2, 3])
```

Convert each part first, as in `", ".join(map(str, [1, 2, 3]))` (rule r3).

## Comparison

Go has a type for this job: "A Builder is used to efficiently build a string using Builder.Write methods. It minimizes memory copying. The zero value is ready to use."[^4] Go `strings.Builder` is close to Python io.StringIO (rule r4). Go also has `strings.Join(parts, sep)`, which takes the separator as the second argument, where Python calls `sep.join(parts)`.
