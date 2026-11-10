```python run id=unlimited
def order_fits(price, quantity, budget):
    return price * quantity <= budget


print("3 items:", order_fits(100, 3, 1000))
print("42949673 items:", order_fits(100, 42949673, 1000))
print("product:", 100 * 42949673)
```

```output for=unlimited
3 items: True
42949673 items: False
product: 4294967300
```

Python integers have unlimited precision, so the product is exact and the check is correct.

```python wrong id=ctypes
import ctypes

total = 100 * 42949673
print("exact:", total)
print("as c_int32:", ctypes.c_int32(total).value)
```

```output for=ctypes
exact: 4294967300
as c_int32: 4
```

The overflow comes back at the edge, when a value goes into a fixed-width type for C code, a file format or a network protocol. The `ctypes` documentation says that the integer types do no overflow checking.

```python run id=struct
import struct

total = 100 * 42949673
try:
    struct.pack("<i", total)
except struct.error as error:
    print("refused:", error)
```

```output for=struct
refused: 'i' format requires -2147483648 <= number <= 2147483647
```

- Verdict: native. `int` has unlimited precision, so arithmetic on integers does not overflow.
- `ctypes`, `array` and libraries for numeric arrays store values in fixed-width types. Check the range before a value goes in.
- `struct.pack` raises `struct.error` for a value that does not fit its format.
