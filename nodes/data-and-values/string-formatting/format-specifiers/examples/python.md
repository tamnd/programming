```python run id=format-mini-language
x = 3.14159
n = 1234567

print(f"[{x:8.2f}] [{x:<8.2f}] [{x:08.2f}] [{x:+.1f}]")
print(f"[{'abc':>6}] [{'abc':*^9}] [{n:,}] [{n:_}]")
print(f"[{255:x}] [{255:#06x}] [{255:b}] [{0.25:.0%}] [{n:.3e}]")
print("{0} {1} {0}".format("a", "b"), format(x, ".3f"))
print("%5d|%-5s|%.2f" % (42, "ab", x))
```

```output for=format-mini-language
[    3.14] [3.14    ] [00003.14] [+3.1]
[   abc] [***abc***] [1,234,567] [1_234_567]
[ff] [0x00ff] [11111111] [25%] [1.235e+06]
a b a 3.142
   42|ab   |3.14
```

- Verdict: native. The format mini-language works in f-strings, in `str.format` and in the function `format`. It has fill and alignment with `<`, `>` and `^`, a sign, `#` for the base prefix, zero fill, a width, `,` or `_` for digit groups, a precision and a type. The older operator `%` uses the C specifiers.
