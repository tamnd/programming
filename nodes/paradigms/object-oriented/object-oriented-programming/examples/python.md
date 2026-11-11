```python run id=classes
class Shape:
    def __init__(self, name):
        self.name = name

    def area(self):
        raise NotImplementedError

    def describe(self):
        return f"{self.name} with area {self.area():.2f}"


class Circle(Shape):
    def __init__(self, r):
        super().__init__("circle")
        self.r = r

    def area(self):
        return 3.14159 * self.r * self.r


class Square(Shape):
    def __init__(self, side):
        super().__init__("square")
        self.side = side

    def area(self):
        return self.side * self.side


class Label:
    def describe(self):
        return "a label, not a shape"


for thing in [Circle(1), Square(2), Label()]:
    print(thing.describe())
print(Circle.__mro__)
```

```output for=classes
circle with area 3.14
square with area 4.00
a label, not a shape
(<class '__main__.Circle'>, <class '__main__.Shape'>, <class 'object'>)
```

```python wrong id=class-attribute
class Dog:
    tricks = []

    def __init__(self, name):
        self.name = name

    def add_trick(self, trick):
        self.tricks.append(trick)


a = Dog("Fido")
b = Dog("Buddy")
a.add_trick("roll over")
b.add_trick("play dead")
print(a.tricks)
```

```output for=class-attribute
['roll over', 'play dead']
```

- Verdict: native. Every value in Python is an object, and a class is an object too. A class can have more than one base class, and `super()` follows the method resolution order in `__mro__`.
- The loop calls `describe` on a `Label` that has no relation to `Shape`. Python looks up methods by name when the call runs, so any object with the right method works. This is duck typing.
- Privacy is a convention. A name that starts with an underscore is internal, but the language does not stop other code from using it.
- In the wrong block, `tricks` is an attribute of the class, so all dogs share one list. Create the list in `__init__` with `self.tricks = []`.
