```python run id=code-point-index-and-slices
s = "naïve 😀"

# An index gives a string of one code point. A negative index counts from the end.
print(s[0], s[-1], type(s[0]).__name__)

# An index past the end raises IndexError, but a slice is cut to fit.
try:
    s[20]
except IndexError as e:
    print(type(e).__name__)
print(repr(s[5:20]))

# A code point is not always a whole character. Here "é" is "e" and U+0301.
t = "cafe\u0301"
print(t, len(t), t[3], f"U+{ord(t[4]):04X}")

# A byte offset from a UTF-8 buffer must be converted to a code point index.
data = s.encode("utf-8")
offset = data.index("😀".encode("utf-8"))
print(offset, len(data[:offset].decode("utf-8")))
```

```output for=code-point-index-and-slices
n 😀 str
IndexError
' 😀'
café 5 e U+0301
7 6
```

- Verdict: native. `s[0]` and `s[-1]` are strings of one code point, and a negative index counts from the end. `s[20]` raises `IndexError`, but the slice `s[5:20]` is cut to fit. In `"cafe\u0301"` the accent is a separate code point, so the text has 5 code points. The emoji starts at byte 7 of the UTF-8 data, and the 7 bytes before it decode to 6 code points.
