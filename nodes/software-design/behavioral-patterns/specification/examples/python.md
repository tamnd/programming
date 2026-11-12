```python run id=containers
class Spec:
    def __and__(self, other):
        return AndSpec(self, other)

    def __or__(self, other):
        return OrSpec(self, other)

    def __invert__(self):
        return NotSpec(self)


class Refrigerated(Spec):
    def is_satisfied_by(self, c):
        return c.refrigerated


class Clean(Spec):
    def is_satisfied_by(self, c):
        return c.clean


class AndSpec(Spec):
    def __init__(self, a, b):
        self.a, self.b = a, b

    def is_satisfied_by(self, c):
        return self.a.is_satisfied_by(c) and self.b.is_satisfied_by(c)


class OrSpec(Spec):
    def __init__(self, a, b):
        self.a, self.b = a, b

    def is_satisfied_by(self, c):
        return self.a.is_satisfied_by(c) or self.b.is_satisfied_by(c)


class NotSpec(Spec):
    def __init__(self, a):
        self.a = a

    def is_satisfied_by(self, c):
        return not self.a.is_satisfied_by(c)


class Container:
    def __init__(self, name, refrigerated, clean):
        self.name, self.refrigerated, self.clean = name, refrigerated, clean


containers = [Container("c1", True, True), Container("c2", True, False), Container("c3", False, True)]


def select(spec):
    return ", ".join(c.name for c in containers if spec.is_satisfied_by(c))


print("meat:", select(Refrigerated() & Clean()))
print("not refrigerated:", select(~Refrigerated()))
print("refrigerated or clean:", select(Refrigerated() | Clean()))
```

```output
meat: c1
not refrigerated: c3
refrigerated or clean: c1, c2, c3
```

- Verdict: idiom. The base class overloads `&`, `|` and `~`, so specifications combine with operators.
