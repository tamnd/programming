```python run id=let-the-code-say-it
import math


# Before: comments explain what each block does.
def shipping_cents_before(parcels, zone):
    # weight must be positive for every parcel
    # compute the total weight in grams
    grams = 0
    for p in parcels:
        grams += p["grams"]
    # round up to whole kilograms
    kg = math.ceil(grams / 1000)
    # look up the rate for the zone (1 = domestic, 2 = europe, other = world)
    rate = 450 if zone == 1 else 900 if zone == 2 else 1600
    # the carrier charges at least 2 kg per shipment, see contract section 4
    return max(kg, 2) * rate


# After: names and an assertion say what; one comment says why.
RATE_CENTS_PER_KG = {"domestic": 450, "europe": 900, "world": 1600}


def zone_name(zone):
    return {1: "domestic", 2: "europe"}.get(zone, "world")


def billable_kilograms(parcels):
    """Return the weight to charge for, in whole kilograms, with the carrier's minimum applied."""
    assert all(p["grams"] > 0 for p in parcels), "every parcel must have a positive weight"
    kg = math.ceil(sum(p["grams"] for p in parcels) / 1000)
    # The carrier charges at least 2 kg per shipment (contract, section 4).
    return max(kg, 2)


def shipping_cents(parcels, zone):
    return billable_kilograms(parcels) * RATE_CENTS_PER_KG[zone_name(zone)]


parcels = [{"grams": 700}, {"grams": 1450}]
for zone in [1, 2, 3]:
    assert shipping_cents(parcels, zone) == shipping_cents_before(parcels, zone)
assert shipping_cents([{"grams": 300}], 1) == 900
assert shipping_cents(parcels, 2) == 2700
if __debug__:
    try:
        shipping_cents([{"grams": 0}], 1)
        raise RuntimeError("the assertion must fail")
    except AssertionError as e:
        assert "positive weight" in str(e)
print("shipping: tests pass")
```

```output
shipping: tests pass
```

- The docstring of `billable_kilograms` is a comment that tools read: `help(billable_kilograms)` prints it. It says what the function promises, which is not a smell.
- `assert` is skipped when Python runs with `-O`, so the test of the failing case is inside `if __debug__:`. A check that must run in production should raise `ValueError` in place of the assertion.
