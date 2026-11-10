```python run id=hmac-pickle
import hashlib
import hmac
import pickle
import secrets

KEY = secrets.token_bytes(32)  # known only to the server


def save(value) -> bytes:
    data = pickle.dumps(value)
    tag = hmac.new(KEY, data, hashlib.sha256).digest()
    return tag + data


def load(blob: bytes):
    tag, data = blob[:32], blob[32:]
    expected = hmac.new(KEY, data, hashlib.sha256).digest()
    if not hmac.compare_digest(tag, expected):
        raise ValueError("bad signature")
    return pickle.loads(data)  # only bytes that this server wrote


print(load(save({"theme": "dark"})))

crafted = b"cbuiltins\nprint\n(S'code ran during loading'\ntR."
try:
    load(bytes(32) + crafted)
except ValueError as e:
    print("refused:", e)
```

```output for=hmac-pickle
{'theme': 'dark'}
refused: bad signature
```

The server signs each stream that it writes and checks the tag before `pickle.loads`. A stream that the server did not write has no valid tag, so it never reaches the unpickler. `hmac.compare_digest` compares in constant time. The control fails if the key leaks, and it does not help when another party is allowed to write the data.

```python run id=restricted-unpickler
import builtins
import io
import pickle

SAFE_BUILTINS = {"range", "complex", "set", "frozenset", "slice"}


class RestrictedUnpickler(pickle.Unpickler):
    def find_class(self, module, name):
        if module == "builtins" and name in SAFE_BUILTINS:
            return getattr(builtins, name)
        raise pickle.UnpicklingError(f"global '{module}.{name}' is forbidden")


def restricted_loads(data: bytes):
    return RestrictedUnpickler(io.BytesIO(data)).load()


print(restricted_loads(pickle.dumps([1, 2, range(3)])))

crafted = b"cbuiltins\nprint\n(S'code ran during loading'\ntR."
try:
    restricted_loads(crafted)
except pickle.UnpicklingError as e:
    print("refused:", e)
```

```output for=restricted-unpickler
[1, 2, range(0, 3)]
refused: global 'builtins.print' is forbidden
```

The `pickle` documentation gives this allowlist. `find_class` runs for every class and every function that the stream asks for, so it can refuse `print` before the call.

- Verdict: stdlib. `pickle` and `shelve` run code from the data. `json` builds only dictionaries, lists, strings, numbers, booleans and `None`.
- Use `json` and validate the fields for any data from outside the process.
- Use an HMAC when the program must read back its own pickles from a place that others can reach.
- `find_class` is an allowlist, not a sandbox. Keep the list to plain data types.
