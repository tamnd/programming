```python run id=port-setting
import re

text = "8080x"


def lenient(s):
    # reads digits until it cannot, like parseInt
    m = re.match(r"\s*\d+", s)
    return int(m.group()) if m else None


def strict(s):
    try:
        return int(s)  # the whole text must be a number
    except ValueError:
        return "rejected"


print(f"lenient: {lenient(text)}")
print(f"strict: {strict(text)}")
```

```output
lenient: 8080
strict: rejected
```
