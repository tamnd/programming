```python run id=bool-is-int-and-truth
# bool is a subclass of int: True is 1 and False is 0.
print(True + True, sum([True, False, True]), isinstance(True, int), True == 1)

# So 1 and True are the same dictionary key.
print({1: "one", True: "true"})


# A class decides its truth value with __bool__ or __len__.
class Account:
    def __init__(self, balance):
        self.balance = balance

    def __bool__(self):
        return self.balance > 0


class Queue:
    def __init__(self, items):
        self.items = items

    def __len__(self):
        return len(self.items)


print(bool(Account(0)), bool(Account(5)), bool(Queue([])), bool(Queue([1])))

# and and or return one of their operands, not always a bool.
print(repr("" or "default"), repr("x" and 0), repr(None or []))

# bool() of any non-empty string is True, including "False".
print(bool("False"), "False" == str(False))

# all and any reduce many Booleans to one.
print(all([]), any([]), all(n > 0 for n in [1, 2, 3]), any(n > 2 for n in [1, 2]))
```

```output for=bool-is-int-and-truth
2 2 True True
{1: 'true'}
False True False True
'default' 0 []
True True
True False True False
```

- Verdict: native. `bool` is a subclass of `int`, so `True + True` is `2` and `1` and `True` are the same dictionary key. `__bool__` and `__len__` give an object a truth value. `and` and `or` return one of their operands, so `"" or "default"` is `"default"`. `bool("False")` is `True`, because the string is not empty. `all` of an empty list is `True` and `any` of it is `False`.
