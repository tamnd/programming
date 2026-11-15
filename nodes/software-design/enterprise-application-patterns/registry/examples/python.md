```python run id=person-registry
class PersonFinder:
    def __init__(self, rows):
        self.rows = rows

    def find(self, person_id):
        return self.rows[person_id]


class Registry:
    """A well-known object with one explicit method for each service."""

    _sole = None

    def __init__(self, person_finder=None):
        self._person_finder = person_finder

    @classmethod
    def install(cls, registry):  # the start of the program, or a test, sets the registry
        cls._sole = registry

    @classmethod
    def person_finder(cls):
        if cls._sole is None or cls._sole._person_finder is None:
            raise LookupError("no person finder is registered")
        return cls._sole._person_finder


def greet(person_id):  # code deep in the program, with no reference to a finder
    print(f"hello, {Registry.person_finder().find(person_id)}")


class StubFinder:
    def find(self, person_id):
        return f"test person {person_id}"


Registry.install(Registry(person_finder=PersonFinder({1: "ann", 2: "bob"})))
greet(1)
greet(2)

Registry.install(Registry(person_finder=StubFinder()))  # a test installs a stub
print("in a test: ", end="")
greet(1)

Registry.install(Registry())
try:
    greet(1)
except LookupError as e:
    print("with an empty registry:", e)
```

```output
hello, ann
hello, bob
in a test: hello, test person 1
with an empty registry: no person finder is registered
```

- Verdict: idiom. The registry is a class with a class attribute that holds the sole instance, and class methods that the program calls. The methods hide the instance, so a test can install another one.
- Each service has its own explicit method, such as `person_finder()`. This is easier to read and check than a lookup by a string key.
