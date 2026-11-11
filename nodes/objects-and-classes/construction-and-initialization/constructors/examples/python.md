```python run id=dataclass-init
from dataclasses import dataclass, field


@dataclass(frozen=True)
class Money:
    cents: int
    currency: str = "EUR"  # a default for an optional field

    def __post_init__(self):  # runs after the generated __init__
        if len(self.currency) != 3:
            raise ValueError(f"bad currency {self.currency!r}")

    @classmethod
    def of(cls, text):  # a named constructor
        amount, currency = text.split()
        return cls(round(float(amount) * 100), currency)


@dataclass
class Order:
    lines: list = field(default_factory=list)  # a new list for each object


print(Money(250), Money.of("3.10 USD"))
try:
    Money(1, "euro")
except ValueError as e:
    print(e)
a, b = Order(), Order()
a.lines.append("tea")
print(a, b)
```

```output for=dataclass-init
Money(cents=250, currency='EUR') Money(cents=310, currency='USD')
bad currency 'euro'
Order(lines=['tea']) Order(lines=[])
```

```python wrong id=mutable-default
class Order:
    def __init__(self, lines=[]):  # the default list is made once, with the function
        self.lines = lines


a = Order()
b = Order()
a.lines.append("tea")
print(b.lines, a.lines is b.lines)
```

```output for=mutable-default
['tea'] True
```

- Verdict: native. Calling a class runs two methods: `__new__` makes the object and `__init__` sets its attributes. Most classes define only `__init__`, and a `classmethod` gives a named constructor that parses or converts its input and then calls the class. A `dataclass` writes `__init__` from the field list, and `__post_init__` runs after it to check the values. `field(default_factory=list)` makes a new list for each object.
- The wrong block uses a list as the default value of a parameter of `__init__`. Python evaluates a default once, when it defines the function, so every `Order` made without an argument shares one list. Use `None` as the default and create the list in the body, or use `default_factory` in a dataclass.
