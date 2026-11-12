```python run id=say-what-it-is
import warnings
from dataclasses import dataclass


@dataclass
class Organization:
    title: str  # renamed from name
    country: str

    @property
    def name(self) -> str:
        """The old name, kept while callers move to title."""
        warnings.warn("use title", DeprecationWarning, stacklevel=2)
        return self.title


org = Organization(title="Harbour Light Trust", country="NO")
assert org.title == "Harbour Light Trust"
with warnings.catch_warnings(record=True) as caught:
    warnings.simplefilter("always")
    assert org.name == org.title
assert caught and issubclass(caught[0].category, DeprecationWarning)
print("rename field: tests pass")
```

```output
rename field: tests pass
```
