```python run id=conversions
print(1 + 2.5)
print(int(3.9), int(-3.9))
print(int(" 42 ") + 1, float("2.5") * 2)
print(str(7) + " days")
print(bool(""), bool("0"), bool([]), bool([0]))
print(float(2**53 + 1) == 2**53 + 1)

for text in ["42", "3.5", "4 2"]:
    try:
        print(text, "->", int(text))
    except ValueError as error:
        print(text, "->", error)

try:
    print("1" + 1)
except TypeError as error:
    print("TypeError:", error)
```

```output for=conversions
3.5
3 -3
43 5.0
7 days
False True False True
False
42 -> 42
3.5 -> invalid literal for int() with base 10: '3.5'
4 2 -> invalid literal for int() with base 10: '4 2'
TypeError: can only concatenate str (not "int") to str
```

- Verdict: native. In mixed arithmetic Python widens `int` to `float` and `float` to `complex`. It never converts between `str` and a number on its own.
- `int(x)` truncates a float toward zero. On a string it parses, accepts white space around the digits, and raises `ValueError` for any other text.
- `bool(x)` is the conversion that `if` uses. Empty strings and empty containers are false, so `"0"` and `[0]` are true.
- A float has 53 bits of significand, so `float(2**53 + 1)` is not equal to the integer.
