```python run id=one-job-each
import re
from typing import Final


def handle_before(text: str) -> str:
    text = text.strip()
    text = text.lower()
    result = re.sub(r"\s+", "-", text)
    result = result[:12]
    return result


def handle(raw_text: str) -> str:
    trimmed: Final = raw_text.strip()
    lower: Final = trimmed.lower()
    slug: Final = re.sub(r"\s+", "-", lower)
    clipped: Final = slug[:12]  # a type checker reports a second assignment to a Final name
    return clipped


for s in ["  Ana Pires ", "Very Long Display Name", "x"]:
    assert handle(s) == handle_before(s)
assert handle("  Ana Pires ") == "ana-pires"
print("split variable: tests pass")
```

```output
split variable: tests pass
```
