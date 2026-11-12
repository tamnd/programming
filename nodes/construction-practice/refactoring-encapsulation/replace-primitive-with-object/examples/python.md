```python run id=give-it-a-type
import re
from dataclasses import dataclass

@dataclass(frozen=True)
class Email:
    value: str

    def __post_init__(self):
        v = self.value.strip().lower()
        if not re.fullmatch(r"[^@\s]+@[^@\s]+", v):
            raise ValueError(f"not an email: {self.value!r}")
        object.__setattr__(self, "value", v)  # normalize once, in a frozen class

    @property
    def domain(self):
        return self.value.split("@", 1)[1]

# The dataclass generates __eq__ and __hash__ from the fields.
assert Email(" Ana@Example.org") == Email("ana@example.org")
assert Email("Cy@EXAMPLE.org").domain == "example.org"
assert len({Email("a@b.c"), Email("A@B.C")}) == 1  # usable as a set member
try:
    Email("not-an-email")
    raise AssertionError("expected a ValueError")
except ValueError as e:
    print(e)
print("replace primitive with object: tests pass")
```

```output
not an email: 'not-an-email'
replace primitive with object: tests pass
```
