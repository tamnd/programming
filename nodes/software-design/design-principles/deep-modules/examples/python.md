```python run id=config-reader
import json


# Shallow: each step is its own function; the caller must know them all.
def split_lines(text):
    return text.split("\n")


def drop_comments(lines):
    return [l for l in lines if not l.startswith("#")]


def drop_blank(lines):
    return [l for l in lines if l.strip() != ""]


def to_pair(line):
    key, value = line.split("=")
    return key.strip(), value.strip()


# Deep: one simple interface hides the same steps.
def parse_config(text):
    result = {}
    for line in text.split("\n"):
        s = line.strip()
        if s == "" or s.startswith("#"):
            continue
        key, value = s.split("=")
        result[key.strip()] = value.strip()
    return result


text = "# server\nhost = example.org\n\nport = 8080\n"
shallow = dict(to_pair(l) for l in drop_blank(drop_comments(split_lines(text))))
print(f"shallow: {json.dumps(shallow, separators=(',', ':'))}")
print(f"deep: {json.dumps(parse_config(text), separators=(',', ':'))}")
```

```output
shallow: {"host":"example.org","port":"8080"}
deep: {"host":"example.org","port":"8080"}
```
