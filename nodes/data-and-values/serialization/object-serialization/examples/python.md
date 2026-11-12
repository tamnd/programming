```python run id=pickle-and-json
import datetime
import json
import pickle
from dataclasses import asdict, dataclass, field


@dataclass
class Person:
    name: str
    born: int
    tags: list = field(default_factory=list)


ada = Person("Ada", 1815, ["math"])
back = pickle.loads(pickle.dumps(ada))  # binary, Python only
print(type(back).__name__, "equal:", back == ada, "same object:", back is ada)
loop = []
loop.append(loop)
again = pickle.loads(pickle.dumps(loop))
print("pickle keeps a cycle:", again[0] is again)

text = json.dumps(asdict(ada))  # text, any language
print(text)
print("from JSON:", Person(**json.loads(text)) == ada)
print("a tuple comes back as:", json.loads(json.dumps((1, 2))))
try:
    json.dumps(loop)
except ValueError as err:
    print("ValueError:", err)
try:
    json.dumps({"when": datetime.date(2026, 1, 1)})
except TypeError as err:
    print("TypeError:", err)
print(json.dumps({"when": datetime.date(2026, 1, 1)}, default=str))
```

```output for=pickle-and-json
Person equal: True same object: False
pickle keeps a cycle: True
{"name": "Ada", "born": 1815, "tags": ["math"]}
from JSON: True
a tuple comes back as: [1, 2]
ValueError: Circular reference detected
TypeError: Object of type date is not JSON serializable
{"when": "2026-01-01"}
```

- Verdict: stdlib. `pickle` writes any graph of Python objects to a binary format that only Python reads, and it keeps shared objects and cycles. `json` writes the common types to text that any language reads, and it loses what JSON cannot say: a tuple comes back as a list, a cycle is an error, and a date needs a `default` function. A dataclass goes through `asdict` on the way out and through its constructor on the way in. Never unpickle data from a source that is not trusted.
