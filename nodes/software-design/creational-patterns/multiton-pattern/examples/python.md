```python run id=multiton
import threading


class Currency:
    _DECIMALS = {"USD": 2, "EUR": 2, "JPY": 0}  # the valid keys
    _instances: dict[str, "Currency"] = {}
    _lock = threading.Lock()

    def __init__(self, code: str, decimals: int):
        self.code, self.decimals = code, decimals

    @classmethod
    def of(cls, code: str) -> "Currency":
        with cls._lock:
            instance = cls._instances.get(code)
            if instance is None:
                if code not in cls._DECIMALS:
                    raise ValueError(code)
                instance = cls(code, cls._DECIMALS[code])
                cls._instances[code] = instance  # one instance per key
            return instance

    @classmethod
    def count(cls) -> int:
        return len(cls._instances)


print("USD has", Currency.of("USD").decimals, "decimals")
print("JPY has", Currency.of("JPY").decimals, "decimals")
print("same USD object:", Currency.of("USD") is Currency.of("USD"))
print("instances:", Currency.count())
try:
    Currency.of("XYZ")
except ValueError as e:
    print(f"{e}: unknown currency")

seen, seen_lock = set(), threading.Lock()
start = threading.Barrier(8)


def worker():
    start.wait()  # all eight threads ask at the same time
    c = Currency.of("EUR")
    with seen_lock:
        seen.add(id(c))


threads = [threading.Thread(target=worker) for _ in range(8)]
for t in threads:
    t.start()
for t in threads:
    t.join()
print("8 threads ask for EUR:", len(seen), "object")
print("instances:", Currency.count())
```

```output
USD has 2 decimals
JPY has 0 decimals
same USD object: True
instances: 2
XYZ: unknown currency
8 threads ask for EUR: 1 object
instances: 3
```

- Verdict: idiom. A class-level `dict` and a `classmethod` give one instance per key. The lock makes the check and the insert one step.
- `logging.getLogger(name)` is the standard multiton: calls with the same name always return the same `Logger` object.
- A `weakref.WeakValueDictionary` in place of the `dict` lets an instance go away when no code uses it, so the registry does not keep every key for ever.
