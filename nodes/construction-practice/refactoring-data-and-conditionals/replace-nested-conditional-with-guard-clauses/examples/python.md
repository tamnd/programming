```python run id=special-cases-first
def refund_before(t: dict, now: int) -> int:
    if t["cancelled_by_organizer"]:
        result = t["price_cents"]
    else:
        if t["used"]:
            result = 0
        else:
            if now > t["event_start"]:
                result = 0
            else:
                result = max(0, t["price_cents"] - 250)
    return result


def refund(t: dict, now: int) -> int:
    if t["cancelled_by_organizer"]:
        return t["price_cents"]
    if t["used"]:
        return 0
    if now > t["event_start"]:
        return 0
    return max(0, t["price_cents"] - 250)


# A reversed condition: all the work was inside `if seats > 0`.
def seat_label_before(seats: int) -> str:
    if seats > 0:
        word = "seat" if seats == 1 else "seats"
        return f"{seats} {word} left"
    else:
        return "sold out"


def seat_label(seats: int) -> str:
    if seats <= 0:
        return "sold out"
    word = "seat" if seats == 1 else "seats"
    return f"{seats} {word} left"


for c in (False, True):
    for u in (False, True):
        for now in (50, 150):
            t = {"cancelled_by_organizer": c, "used": u, "event_start": 100, "price_cents": 4000}
            assert refund(t, now) == refund_before(t, now)
for s in (-1, 0, 1, 7):
    assert seat_label(s) == seat_label_before(s)
print("guard clauses: tests pass")
```

```output
guard clauses: tests pass
```
