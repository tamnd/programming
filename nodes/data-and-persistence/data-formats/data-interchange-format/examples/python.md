```python run id=plist-pickle-json
import datetime, json, pickle, plistlib

value = {"when": datetime.datetime(2024, 1, 15, 9, 30), "seen": {3, 1, 2}, "point": (1, 2)}

# pickle keeps the Python types, because it writes Python objects.
back = pickle.loads(pickle.dumps(value))
print(back == value, type(back["seen"]).__name__, type(back["point"]).__name__)

# json has no date, set or tuple type.
try:
    json.dumps(value)
except TypeError as e:
    print("json:", type(e).__name__)

# A default function picks a JSON form for each extra type, and the reader gets plain lists and strings.
def to_json(v):
    if isinstance(v, datetime.datetime):
        return v.isoformat()
    if isinstance(v, set):
        return sorted(v)
    raise TypeError(type(v).__name__)

text = json.dumps(value, default=to_json, sort_keys=True)
print(text)
print(json.loads(text))

# plistlib writes the property lists of Apple systems, as XML or binary, and has a date type.
xml_plist = plistlib.dumps({"when": value["when"], "n": 1})
bin_plist = plistlib.dumps({"when": value["when"], "n": 1}, fmt=plistlib.FMT_BINARY)
print(len(xml_plist), len(bin_plist), bin_plist[:8])
print(plistlib.loads(bin_plist))
```

```output for=plist-pickle-json
True set tuple
json: TypeError
{"point": [1, 2], "seen": [1, 2, 3], "when": "2024-01-15T09:30:00"}
{'point': [1, 2], 'seen': [1, 2, 3], 'when': '2024-01-15T09:30:00'}
276 68 b'bplist00'
{'n': 1, 'when': datetime.datetime(2024, 1, 15, 9, 30)}
```

- Verdict: stdlib. The standard library has `json`, `csv`, `xml.etree.ElementTree`, `plistlib` and `tomllib`, and the Python-only `pickle`. `pickle` gives back a `datetime`, a `set` and a `tuple`, because it writes Python objects. `json.dumps` raises `TypeError` for the same value, so the program must choose a JSON form for each extra type with `default`, and the reader then gets lists and strings. `plistlib` writes the same dictionary as an XML property list or as a much smaller binary one that starts with `bplist00`.
