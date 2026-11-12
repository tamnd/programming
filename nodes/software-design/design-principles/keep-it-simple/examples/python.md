```python run id=simple-config
TEXT = "host=example.org\nport=8080\n# a comment\n"


def parse_config(src: str) -> dict[str, str]:
    out = {}
    for line in src.split("\n"):
        if line == "" or line.startswith("#"):
            continue
        key, value = line.split("=", 1)
        out[key] = value
    return out


print(parse_config(TEXT))
```

```output
{'host': 'example.org', 'port': '8080'}
```
