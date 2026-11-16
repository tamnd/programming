```python run id=class-methods-per-subclass
class Order:
    created = 0

    def __init__(self):
        type(self).register()

    @classmethod
    def register(cls):
        cls.created += 1  # stores the sum on cls, which may be a subclass


class RushOrder(Order):
    pass


Order()
Order()
RushOrder()
print(Order.created, RushOrder.created)
print("created" in vars(RushOrder))
```

```output for=class-methods-per-subclass
2 3
True
```

- Verdict: native. A class attribute is a class variable, `@classmethod` makes a method that gets the class as `cls`, and `@staticmethod` makes a function that gets neither the class nor an instance.
- `cls.created += 1` reads the value through the class and its bases, but it stores the sum on `cls`. The first `RushOrder()` reads 2 from `Order` and stores 3 on `RushOrder`, so each class now has its own counter. Write `Order.created += 1` to keep one counter for the whole family of classes.
