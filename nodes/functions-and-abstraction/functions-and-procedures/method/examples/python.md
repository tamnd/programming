```python run id=method-python
class Rectangle:
    def __init__(self, width, height):
        self.width = width
        self.height = height

    def area(self):
        return self.width * self.height

    @staticmethod
    def square(side):
        return Rectangle(side, side)


r = Rectangle(3, 4)
print(r.area())
print(Rectangle.square(5).area())
```

```output for=method-python
12
25
```

- Verdict: native. `area` is an instance method with the receiver `self`. `square` is a static method, which has no receiver.
