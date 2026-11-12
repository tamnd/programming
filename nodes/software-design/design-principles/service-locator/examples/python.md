```python run id=report-locator
class Locator:
    _services = {}

    @classmethod
    def register(cls, name, service):
        cls._services[name] = service

    @classmethod
    def get(cls, name):
        return cls._services[name]  # KeyError if nobody registered it


class Report:
    def print(self, total):
        fmt = Locator.get("format")
        return fmt(total)


class TaxReport:
    def print(self, total):
        tax = Locator.get("tax")  # nobody registered this service
        return Locator.get("format")(total + tax(total))


# Configuration: the program puts the services into the locator.
Locator.register("format", lambda n: f"total: {n}")

print(Report().print(12))
tax_report = TaxReport()  # building it does not fail
try:
    print(tax_report.print(12))
except KeyError as e:
    print(f"error: no service: {e.args[0]}")
```

```output
total: 12
error: no service: tax
```
