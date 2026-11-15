```python run id=tax-service-stub
from typing import Protocol


class TaxService(Protocol):
    def rate_bp(self, region: str) -> int: ...  # the rate in basis points: 650 is 6.5%


class RemoteTaxService:
    """The real service. Here it stands for a remote call that is not reachable."""

    def rate_bp(self, region):
        raise ConnectionError("the tax service is not reachable")


class TaxServiceStub:
    """Runs locally, fast and in memory, with canned rates."""

    RATES = {"north": 650, "south": 0}

    def __init__(self):
        self.calls = 0

    def rate_bp(self, region):
        self.calls += 1
        return self.RATES.get(region, 500)  # a default rate for the other regions


class BillingService:
    def __init__(self, taxes: TaxService):
        self.taxes = taxes

    def total_cents(self, amount_cents, region):
        return amount_cents + amount_cents * self.taxes.rate_bp(region) // 10000


def money(cents):
    return f"{cents // 100}.{cents % 100:02d}"


try:
    BillingService(RemoteTaxService()).total_cents(10000, "north")
except ConnectionError as e:
    print("real service: error:", e)

stub = TaxServiceStub()
billing = BillingService(stub)
for region in ["north", "south", "east"]:
    print(f"{region}: 100.00 with tax is {money(billing.total_cents(10000, region))}")
print("the stub got", stub.calls, "calls")
```

```output
real service: error: the tax service is not reachable
north: 100.00 with tax is 106.50
south: 100.00 with tax is 100.00
east: 100.00 with tax is 105.00
the stub got 3 calls
```

- Verdict: idiom. `TaxService` is a `typing.Protocol`, so the stub and the real service need no common base class. `BillingService` gets the service in its constructor, and the program or the test chooses which one.
- The stub counts its calls, so a test can also check how the code uses the service.
