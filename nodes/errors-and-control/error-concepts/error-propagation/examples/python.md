```python run id=propagate-python
class ConfigError(Exception):
    pass


def parse_port(text):
    if not (text.isascii() and text.isdigit()):
        raise ValueError(f"not a number: {text}")
    return int(text)


def load_config(line):
    value = line.partition("=")[2]
    try:
        return parse_port(value)
    except ValueError as e:
        # Add context, and keep the original error as the cause.
        raise ConfigError(f"config line '{line}'") from e


for line in ["port=8080", "port=x"]:
    try:
        print(f"port {load_config(line)}")
    except ConfigError as e:
        print(f"error: {e}: {e.__cause__}")
```

```output for=propagate-python
port 8080
error: config line 'port=x': not a number: x
```

- Verdict: native. An exception goes up the call stack until a matching `except` clause catches it. `raise ... from e` adds context and keeps the first exception as `__cause__`, and the traceback shows both.
