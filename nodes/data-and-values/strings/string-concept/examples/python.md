```python run id=str-bytes-and-bytearray
# str holds code points, bytes holds bytes, and bytearray is a mutable bytes.
s = "naïve 😀"
print(type(s).__name__, len(s), len(s.encode("utf-8")))

# += makes a new object. The old object does not change.
a = "ab"
b = a
a += "c"
print(a, b, a is b)

# bytearray can change in place.
ba = bytearray(b"cat")
ba[0] = ord("b")
print(ba, bytes(ba).decode("ascii"))

# str and bytes do not mix.
try:
    "id: " + b"42"
except TypeError as e:
    print(type(e).__name__)

# A string is also a sequence: it has in, iteration and slices.
print("ïv" in s, [c for c in "abc"], s[::-1])
```

```output for=str-bytes-and-bytearray
str 7 11
abc ab False
bytearray(b'bat') bat
TypeError
True ['a', 'b', 'c'] 😀 evïan
```

- Verdict: native. `str` holds code points, so `"naïve 😀"` has a length of 7, and its UTF-8 bytes have a length of 11. `a += "c"` makes a new object, and `b` still refers to the old string. A `bytearray` changes in place. `"id: " + b"42"` raises `TypeError`, because text and bytes do not mix. A string is also a sequence, with `in`, iteration and slices.
