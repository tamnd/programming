```python run id=identity-and-shared-bytes
a = [1, 2, 3]
b = a
c = [1, 2, 3]
print("a is b:", a is b, "same id:", id(a) == id(b))
print("a is c:", a is c, "a == c:", a == c)

data = bytearray(b"hello")
view = memoryview(data)[1:4]  # a window on the same bytes, no copy
view[0] = ord("E")
print(data, "view length", view.nbytes)

```

```output for=identity-and-shared-bytes
a is b: True same id: True
a is c: False a == c: True
bytearray(b'hEllo') view length 3
```

- Verdict: stdlib. Python code works with references and never sees an address. `is` compares identity, and `id` gives a number that is the same for one object while it lives; in CPython it is the address. Two equal lists are different objects. A `memoryview` is a window on the bytes of another object at an offset, so a write through the view changes the `bytearray`. The `ctypes` module in the idea section gives real addresses with `addressof`.
