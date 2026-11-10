```python run id=export
from abc import ABC, abstractmethod


class Writer(ABC):
    @abstractmethod
    def header(self) -> str: ...

    @abstractmethod
    def row(self, name: str, count: int) -> str: ...


class CsvWriter(Writer):
    def header(self) -> str:
        return "name,count"

    def row(self, name: str, count: int) -> str:
        return f"{name},{count}"


class TextWriter(Writer):
    def header(self) -> str:
        return "Inventory"

    def row(self, name: str, count: int) -> str:
        return f"{name}: {count}"


class Exporter(ABC):
    def export(self, rows: list[tuple[str, int]]) -> str:
        writer = self.create_writer()  # the factory method
        lines = [writer.header()]
        lines += [writer.row(name, count) for name, count in rows]
        return "\n".join(lines)

    @abstractmethod
    def create_writer(self) -> Writer: ...


class CsvExporter(Exporter):
    def create_writer(self) -> Writer:
        return CsvWriter()


class TextExporter(Exporter):
    def create_writer(self) -> Writer:
        return TextWriter()


rows = [("pen", 3), ("book", 1)]
for exporter in [CsvExporter(), TextExporter()]:
    print(exporter.export(rows))
```

```output
name,count
pen,3
book,1
Inventory
pen: 3
book: 1
```

- Verdict: idiom. Python has no construct for the pattern. A method that subclasses override is the classic form, and `abc.abstractmethod` stops code from creating an `Exporter` that has no factory method.
- A class is a callable that returns an instance, so the usual Python form passes the class itself: `Exporter(make_writer=CsvWriter)`. The node page shows this variant.
- `collections.defaultdict(list)` uses the same idea in the standard library: `list` is the factory that makes each missing value.
