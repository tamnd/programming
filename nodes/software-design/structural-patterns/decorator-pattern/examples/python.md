```python run id=repository
from typing import Protocol


class Repository(Protocol):
    def get(self, key: str) -> str: ...
    def name(self) -> str: ...


class Store:
    def get(self, key: str) -> str:
        print(f"store: load {key}")
        return f"value-{key}"

    def name(self) -> str:
        return "store"


class Logged:
    def __init__(self, inner: Repository) -> None:
        self.inner = inner

    def get(self, key: str) -> str:
        print(f"log: get {key}")
        return self.inner.get(key)

    def name(self) -> str:
        return self.inner.name()


class Cached:
    def __init__(self, inner: Repository) -> None:
        self.inner = inner
        self.cache: dict[str, str] = {}

    def get(self, key: str) -> str:
        if key not in self.cache:
            self.cache[key] = self.inner.get(key)
        return self.cache[key]

    def name(self) -> str:
        return self.inner.name()


repo: Repository = Logged(Cached(Store()))
for key in ["a", "a", "b"]:
    print(repo.get(key))
print(f"name: {repo.name()}")
```

```output
log: get a
store: load a
value-a
log: get a
value-a
log: get b
store: load b
value-b
name: store
```

- Verdict: idiom. Python has no decorator construct of the Design Patterns kind, so the pattern is two classes that hold the inner object and forward to it.
- `typing.Protocol` states the interface for a type checker. The classes do not inherit from it. At run time any object with `get` and `name` works.
- The `@` syntax of Python is a different construct with the same name. It applies a function to a function or a class when the code defines it. The node page shows `functools.wraps` and `functools.cache` used this way.
- A `__getattr__` method that returns `getattr(self.inner, name)` forwards every attribute that the class does not define, so `name` would not need to be written. The cost is that a type checker cannot see the forwarded methods.
