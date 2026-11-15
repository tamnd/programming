```python run id=property-objects
class Rectangle:
    __slots__ = ("width", "height")  # the only fields an instance can have

    def __init__(self, width, height):
        self.width = width
        self.height = height

    @property
    def area(self):
        return self.width * self.height


r = Rectangle(3, 4)
print(r.width, r.area)
r.width = 5
print(r.area)
print(type(Rectangle.__dict__["area"]).__name__)
try:
    r.area = 1  # a property without a setter cannot be assigned
except AttributeError:
    print("AttributeError: area")
try:
    r.depth = 2  # __slots__ allows no other fields
except AttributeError:
    print("AttributeError: depth")
```

```output for=property-objects
3 12
20
property
AttributeError: area
AttributeError: depth
```

- Verdict: native. A field is an attribute that an instance stores, and `self.width = width` creates it. A property is an object of the type `property` that lives in the class and runs a function on each read, so `r.area` has no parentheses and always gives the current area.
- A property without a setter is read-only. `__slots__` replaces the attribute dictionary of each instance with fixed storage, which saves memory and stops typing mistakes such as `r.widht = 5` from creating a field.
- A field can become a property later without a change to the code that uses it, so Python code starts with plain fields.
