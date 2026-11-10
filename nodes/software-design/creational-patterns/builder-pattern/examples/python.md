```python run id=request
from dataclasses import dataclass, field, replace


@dataclass(frozen=True, kw_only=True)
class Request:
    url: str
    method: str = "GET"
    headers: tuple[str, ...] = ()
    timeout: int = 30
    retries: int = 0

    def __post_init__(self) -> None:
        if self.timeout <= 0:
            raise ValueError("timeout must be positive")


def show(request: Request) -> None:
    headers = ", ".join(request.headers)
    print(f"{request.method} {request.url} timeout={request.timeout} retries={request.retries} headers=[{headers}]")


orders = Request(url="https://example.com/orders")
show(orders)
show(replace(orders, method="POST", headers=("Accept: application/json",), timeout=5, retries=2))
try:
    Request(url="https://example.com/orders", timeout=0)
except ValueError as error:
    print(f"error: {error}")
```

```output for=request
GET https://example.com/orders timeout=30 retries=0 headers=[]
POST https://example.com/orders timeout=5 retries=2 headers=[Accept: application/json]
error: timeout must be positive
```

- Verdict: native. Keyword arguments with default values do the work of a builder for construction. The caller names each part, leaves out the parts that have defaults, and gives them in any order.
- `kw_only=True` makes every field keyword-only, so a caller cannot pass `timeout` and `retries` by position and swap them.
- `frozen=True` makes the product immutable after `__init__`, and `__post_init__` checks the parts as a whole, as a `build` method does.
- `dataclasses.replace` makes a copy with some fields changed. It takes the place of a builder that starts from an existing product. It calls `__init__`, so the check runs again.
- The headers are a tuple, not a list. A mutable default would be one object shared by every instance, and `dataclass` rejects a `list` default for this reason (see Consequences).
