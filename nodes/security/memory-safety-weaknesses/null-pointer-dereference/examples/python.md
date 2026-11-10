```python fails expect=exception:AttributeError id=none-attribute
from dataclasses import dataclass


@dataclass
class User:
    quota: int


users = {"alice": User(10), "bob": User(20)}

for name in ["alice", "nobody"]:
    user = users.get(name)  # None when the key is missing
    print(f"{name}: quota {user.quota}")
```

`dict.get` returns `None` for a missing key. `None` has no attribute `quota`, so the attribute reference raises `AttributeError: 'NoneType' object has no attribute 'quota'`.

```python run id=check-none
from dataclasses import dataclass


@dataclass
class User:
    quota: int


users = {"alice": User(10), "bob": User(20)}


def find_user(name: str) -> User | None:
    return users.get(name)


for name in ["alice", "nobody"]:
    user = find_user(name)
    if user is None:
        print(f"{name}: no such user")
        continue
    print(f"{name}: quota {user.quota}")
```

```output for=check-none
alice: quota 10
nobody: no such user
```

- Verdict: absent. Any name can hold `None`, and the interpreter finds the error only when the line runs. The error is an exception, not a memory access.
- Write `User | None` in the type hints of functions that can return `None`. A type checker then reports a use without a check. The interpreter does not check hints.
- Test with `is None`, not with a truth test, because an empty value is also false.
