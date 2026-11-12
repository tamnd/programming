```python run id=types-of-objects
# Every object has a type, and the type is an object of type `type`.
for v in [7, 7.0, 7j, True, "7", b"7", (7,), [7], {7}, {7: 7}, None]:
    print(f"{v!r:>8} {type(v).__name__}")

# bool is a subtype of int, so True is also an int.
print(isinstance(True, int), True + True, bool.__mro__)

# Annotations are not checked when the program runs.
def double(n: int) -> int:
    return n * 2


print(double(21), double("ab"))

# type() with three arguments makes a new class at run time.
Point = type("Point", (), {"x": 0, "y": 0, "__repr__": lambda s: f"Point({s.x}, {s.y})"})
p = Point()
p.x = 3
print(p, type(p).__name__, type(Point).__name__)

# The same operation is valid for some types and not for others.
for a, b in [(1, 2), ("a", "b"), ([1], [2]), (1, "b")]:
    try:
        print(repr(a + b))
    except TypeError as e:
        print("TypeError:", e)
```

```output for=types-of-objects
       7 int
     7.0 float
      7j complex
    True bool
     '7' str
    b'7' bytes
    (7,) tuple
     [7] list
     {7} set
  {7: 7} dict
    None NoneType
True 2 (<class 'bool'>, <class 'int'>, <class 'object'>)
42 abab
Point(3, 0) Point type
3
'ab'
[1, 2]
TypeError: unsupported operand type(s) for +: 'int' and 'str'
```

- Verdict: native. Each object has a type, and `type()` returns it. `bool` is a subclass of `int`, so `True + True` is `2`. The annotation `n: int` is not checked, so `double("ab")` returns `"abab"`. `type()` with three arguments makes a new class, whose type is `type`. `+` works for two numbers, two strings or two lists, and raises `TypeError` for an `int` and a `str`.
