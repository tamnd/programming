```python run id=missing-setting
config = {"maxConections": "50"}  # a typo in the file


def max_connections_slow():
    return int(config.get("maxConnections", 10))


def max_connections_fast():
    try:
        return int(config["maxConnections"])
    except KeyError:
        raise LookupError("maxConnections not found in config") from None


print(f"slow: {max_connections_slow()}")
try:
    print(f"fast: {max_connections_fast()}")
except LookupError as e:
    print(f"fast: {e}")
```

```output
slow: 10
fast: maxConnections not found in config
```
