```python run id=one-rule
def ships_today_before(order: dict, hour: int) -> bool:
    if hour >= 15:
        return False
    if any(i["backordered"] for i in order["items"]):
        return False
    if order["country"] != "NO":
        return False
    return True


def misses_todays_dispatch(order: dict, hour: int) -> bool:
    return hour >= 15 or any(i["backordered"] for i in order["items"]) or order["country"] != "NO"


def ships_today(order: dict, hour: int) -> bool:
    return not misses_todays_dispatch(order, hour)


# A nested check with no else joins with `and`.
def gift_wrap_fee_before(order: dict) -> int:
    if order["gift"]:
        if order["country"] == "NO":
            return 30
    return 0


def gift_wrap_fee(order: dict) -> int:
    return 30 if order["gift"] and order["country"] == "NO" else 0


for hour in (9, 15):
    for items in ([], [{"backordered": True}]):
        for country in ("NO", "SE"):
            for gift in (False, True):
                o = {"items": items, "country": country, "gift": gift}
                assert ships_today(o, hour) == ships_today_before(o, hour)
                assert gift_wrap_fee(o) == gift_wrap_fee_before(o)
print("consolidate conditional expression: tests pass")
```

```output
consolidate conditional expression: tests pass
```
