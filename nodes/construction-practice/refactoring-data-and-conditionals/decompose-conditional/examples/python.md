```python run id=name-the-parts
from dataclasses import dataclass

DAY_SECONDS = 86400
GRACE_DAYS = 3


@dataclass(frozen=True)
class Member:
    age: int
    tier: str


@dataclass(frozen=True)
class Loan:
    due: int  # seconds since the epoch
    price_cents: int
    member: Member


def late_fee_before(loan: Loan, now: int) -> int:
    days_late = (now - loan.due) // DAY_SECONDS
    if days_late > 3 and not (loan.member.age < 18 or loan.member.tier == "staff"):
        fee = min(days_late * 25, loan.price_cents)
    else:
        fee = min(days_late * 10, 500) if days_late > 3 else 0
    return fee


def late_fee(loan: Loan, now: int) -> int:
    days_late = (now - loan.due) // DAY_SECONDS
    if days_late <= GRACE_DAYS:
        return 0
    if pays_full_fee(loan.member):
        return full_fee(days_late, loan.price_cents)
    return reduced_fee(days_late)


def pays_full_fee(member: Member) -> bool:
    return not (member.age < 18 or member.tier == "staff")


def full_fee(days_late: int, price_cents: int) -> int:
    return min(days_late * 25, price_cents)


def reduced_fee(days_late: int) -> int:
    return min(days_late * 10, 500)


for age in (12, 40):
    for tier in ("staff", "public"):
        loan = Loan(due=0, price_cents=1999, member=Member(age, tier))
        for d in (0, 3, 4, 30, 200):
            assert late_fee(loan, d * DAY_SECONDS) == late_fee_before(loan, d * DAY_SECONDS)
print("decompose conditional: tests pass")
```

```output
decompose conditional: tests pass
```
