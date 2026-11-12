```python run id=rename-the-name
# Before and after a rename of a field.
class OrderBefore:
    def __init__(self, q):
        self.q = q  # q: the number of items

class Order:
    def __init__(self, quantity):
        self.quantity = quantity

# Code that reaches the field by a string. A rename tool that works on
# the syntax tree does not see "q" here as a reference to the field.
def read_field(obj, name):
    return getattr(obj, name)

assert read_field(OrderBefore(3), "q") == 3
try:
    read_field(Order(3), "q")  # the string was not renamed
    raise AssertionError("expected the old name to fail")
except AttributeError as e:
    print("missed by the rename:", e)

# The fix: change the strings by hand, and search for the old name as text.
assert read_field(Order(3), "quantity") == 3
print("rename variable: tests pass")
```

```output
missed by the rename: 'Order' object has no attribute 'q'
rename variable: tests pass
```
