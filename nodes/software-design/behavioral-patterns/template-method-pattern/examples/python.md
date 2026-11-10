```python run id=loader
from abc import ABC, abstractmethod
from typing import final


class Loader(ABC):
    @final
    def load(self, lines: list[str]) -> list[tuple[str, int]]:
        """The template method: the fixed steps, in a fixed order."""
        records = []
        for line in lines:
            line = line.strip()
            if not line or line.startswith("#"):
                continue
            record = self.parse(line)
            if self.accept(record):
                records.append(record)
        return records

    @abstractmethod
    def parse(self, line: str) -> tuple[str, int]:
        """A primitive operation: each subclass must give it."""

    def accept(self, record: tuple[str, int]) -> bool:
        """A hook: subclasses may override it. The default keeps every record."""
        return True


class CsvLoader(Loader):
    def parse(self, line):
        name, count = line.split(",")
        return name, int(count)


class KeyValueLoader(Loader):
    def parse(self, line):
        name, _, count = line.partition("=")
        return name, int(count)

    def accept(self, record):
        return record[1] > 0


def show(label, records):
    print(f"{label}: " + " ".join(f"{name}:{count}" for name, count in records))


show("csv", CsvLoader().load(["# stock", "pen,3", "", "book,1"]))
show("kv", KeyValueLoader().load(["pen=3", "# sold out", "cup=0"]))
```

```output for=loader
csv: pen:3 book:1
kv: pen:3
```

- Verdict: idiom. An abstract base class holds the template method `load`. `parse` is abstract, so a subclass without it cannot be created. `accept` is a hook with a default.
- `typing.final` marks `load` as a method that subclasses must not override. Python does not enforce it when the program runs. A type checker reports an override.
- The leading underscore convention, as in `_parse`, is the usual way to say that a step is for subclasses and not for clients. Python has no `protected`.
