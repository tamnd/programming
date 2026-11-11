```python run id=port
from dataclasses import dataclass


@dataclass(frozen=True)
class Ok[T]:
    value: T


@dataclass(frozen=True)
class Err:
    message: str
    missing: bool = False


type Result[T] = Ok[T] | Err


def lookup(config: dict[str, str], key: str) -> Result[str]:
    if key not in config:
        return Err(f"missing key {key!r}", missing=True)
    return Ok(config[key])


def parse_port(text: str) -> Result[int]:
    if not text.isdecimal() or not 1 <= int(text) <= 65535:
        return Err(f"invalid port {text!r}")
    return Ok(int(text))


def port_setting(config: dict[str, str]) -> Result[int]:
    match lookup(config, "port"):
        case Err() as error:
            return error
        case Ok(text):
            return parse_port(text)


for config in [{"port": "8080"}, {"port": "x"}, {}]:
    match port_setting(config):
        case Ok(port):
            print("port", port)
        case Err(missing=True):
            print("use the default 80")
        case Err(message):
            print("error:", message)

print("find:", "port=8080".find("host"))
```

```output
port 8080
error: invalid port 'x'
use the default 80
find: -1
```

- Verdict: idiom. Python reports errors with exceptions. A function can return a value that holds either the result or the error, and the caller tests it with `match`.
- The two dataclasses form a tagged union. `match` takes the value apart with class patterns such as `Ok(port)`, because a dataclass sets `__match_args__`.
- Nothing forces the caller to test the result. A type checker such as mypy can check the annotations, but the interpreter ignores them.
- A few built-in functions return a special value in place of an error: `str.find` returns `-1`, and `dict.get` returns `None`.
