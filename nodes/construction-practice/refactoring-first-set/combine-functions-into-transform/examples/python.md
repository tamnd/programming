```python run id=compute-once
import copy
import math

DIVISOR = 5000  # cm^3 per kg of volumetric weight

def enrich_parcel(source):
    p = copy.deepcopy(source)
    p["volume"] = p["length"] * p["width"] * p["height"]
    p["chargeable_kg"] = math.ceil(max(p["weight_kg"], p["volume"] / DIVISOR))
    return p

raw = {"id": "P1", "length": 40, "width": 30, "height": 20, "weight_kg": 2, "tags": ["fragile"]}
parcel = enrich_parcel(raw)
assert parcel["chargeable_kg"] == 5 and "chargeable_kg" not in raw

parcel["tags"].append("express")
assert raw["tags"] == ["fragile"]  # deep copy: the list is not shared

# With a shallow copy, the nested list is shared with the source.
shallow = copy.copy(raw)
shallow["tags"].append("express")
print("source tags after a change to the shallow copy:", raw["tags"])
assert raw["tags"] == ["fragile", "express"]
print("combine functions into transform: tests pass")
```

```output
source tags after a change to the shallow copy: ['fragile', 'express']
combine functions into transform: tests pass
```
