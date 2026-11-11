```python run id=diamond
class Base:
    def setup(self):
        return ["Base"]


class Logging(Base):
    def setup(self):
        return ["Logging"] + super().setup()


class Caching(Base):
    def setup(self):
        return ["Caching"] + super().setup()


class Service(Logging, Caching):  # a diamond: two paths lead to Base
    def setup(self):
        return ["Service"] + super().setup()


print(Service().setup())  # each class runs once, in the order of the MRO
print([c.__name__ for c in Service.__mro__])

try:
    class Broken(Base, Logging):  # Base before its own subclass: no consistent order
        pass
except TypeError as e:
    print("TypeError:", e)
```

```output for=diamond
['Service', 'Logging', 'Caching', 'Base']
['Service', 'Logging', 'Caching', 'Base', 'object']
TypeError: Cannot create a consistent method resolution order (MRO) for bases Base, Logging
```

```python wrong id=shared-class-attribute
class Plugin:
    hooks = []  # one list, stored in the class Plugin

    def add_hook(self, name):
        self.hooks.append(name)


class Audit(Plugin):
    pass


class Cache(Plugin):
    pass


Audit().add_hook("log")
Cache().add_hook("evict")
print(Audit.hooks, Cache.hooks)
```

```output for=shared-class-attribute
['log', 'evict'] ['log', 'evict']
```

- Verdict: native. A class lists its base classes in parentheses, and Python supports multiple inheritance. The method resolution order, computed by the C3 algorithm, puts every class before its bases and keeps the order of the bases, so in the diamond each `setup` runs once and `super()` calls the next class in that order, not always the base class. A class list that has no such order, as in `Broken`, is a `TypeError` when the class is defined.
- The wrong block puts a list in a class attribute. Subclasses inherit the attribute itself, not a copy, so `Audit` and `Cache` add to the one list of `Plugin`. Create mutable state in `__init__`, or give each subclass its own list in `__init_subclass__`.
