```python run id=encoded-sizes-and-null
import sys

# The same text takes a different number of bytes in each encoding.
for text in ["hello", "café", "日本", "😀"]:
    sizes = []
    for enc in ["latin-1", "utf-8", "utf-16-le", "utf-32-le"]:
        try:
            sizes.append(str(len(text.encode(enc))))
        except UnicodeEncodeError:
            sizes.append("-")
    print(f"{text!r}: latin-1={sizes[0]} utf-8={sizes[1]} utf-16={sizes[2]} utf-32={sizes[3]}")

# A Python string stores its length, so a null character is an ordinary character.
s = "a\0b"
print(len(s), s.encode("ascii"))

# A slice of a str is a new string with its own storage.
big = "x" * 10_000
part = big[:10]
print(sys.getsizeof(part) < sys.getsizeof(big))
```

```output for=encoded-sizes-and-null
'hello': latin-1=5 utf-8=5 utf-16=10 utf-32=20
'café': latin-1=4 utf-8=5 utf-16=8 utf-32=16
'日本': latin-1=- utf-8=6 utf-16=4 utf-32=8
'😀': latin-1=- utf-8=4 utf-16=4 utf-32=4
3 b'a\x00b'
True
```

- Verdict: native. The same text takes a different number of bytes in each encoding, and Latin-1 cannot encode `日本` or the emoji. A `str` stores its length, so `"a\0b"` has 3 characters and the null character is ordinary data. A slice is a new string with its own storage, so it is much smaller than the string it came from.
