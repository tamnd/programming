---
tag: 0E3AHP
kind: feature
owner: python
slug: saving-structured-data-with-json
name: Saving structured data with json
aliases: [json module, json.dump, json.load, json.dumps, json.loads, serialize to JSON, JSONEncoder]
area: languages/python/io-and-os
level: intro
summary: >-
  The json module turns lists, dicts, strings, numbers, booleans and None into JSON text and back. dump() and load() work on files, dumps() and loads() on strings.
grammar: "json.dump(obj, fp, *, indent=None, sort_keys=False, default=None, ...); json.load(fp, *, object_hook=None, ...)"
spec: {standard: null, url: "https://docs.python.org/3.15/library/json.html", locator: "Basic usage; Encoders and decoders"}
see_also: [python/reading-and-writing-files, python/output-formatting, json, read-and-write-json, pretty-print-json]
sources:
  - url: https://docs.python.org/3.15/tutorial/inputoutput.html#saving-structured-data-with-json
    title: The Python Tutorial, 7. Input and output
    locator: 7.2.2. Saving structured data with json
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/json.html
    title: json, Python 3.15 documentation
    locator: json.dump(); json.load(); json.JSONDecoder; json.JSONEncoder
    accessed: 2026-11-16
  - url: https://pkg.go.dev/encoding/json
    title: Package encoding/json, Go standard library
    locator: func Marshal
    accessed: 2026-11-16
---

## Synopsis

```python run id=synopsis
import json

config = {'name': 'demo', 'ports': [8080, 8081], 'debug': False, 'owner': None}

with open('config.json', 'w', encoding='utf-8') as f:
    json.dump(config, f)

with open('config.json', encoding='utf-8') as f:
    loaded = json.load(f)

print(open('config.json', encoding='utf-8').read())
print(loaded == config)
```

```output for=synopsis
{"name": "demo", "ports": [8080, 8081], "debug": false, "owner": null}
True
```

`json.dump()` writes the dictionary to the file as JSON text, and `json.load()` reads it back as an equal dictionary.

## Semantics

**r1.** "The standard module called json can take Python data hierarchies, and convert them to string representations; this process is called serializing. Reconstructing the data from the string representation is called deserializing."[^1]

**r2.** `json.dumps()` serializes an object "to a JSON formatted str".[^2] `json.dump()` serializes it "as a JSON formatted stream to fp (a .write()-supporting file-like object)".[^2] `json.loads()` and `json.load()` do the reverse. "The json module always produces str objects, not bytes objects, therefore fp.write() must support str input."[^2]

**r3.** The encoder maps Python types to JSON types.[^2]

| Python | JSON |
|---|---|
| `dict` | object |
| `list`, `tuple` | array |
| `str` | string |
| `int`, `float`, int- and float-derived enums | number |
| `True`, `False` | `true`, `false` |
| `None` | `null` |

The decoder maps the other way, and a JSON array becomes a `list`.[^2] So a tuple comes back as a list. Example: [`types`](#types).

**r4.** "Keys in key/value pairs of JSON are always of the type str. When a dictionary is converted into JSON, all the keys of the dictionary are converted to strings."[^2] So "loads(dumps(x)) != x if x has non-string keys."[^2]

**r5.** `indent` pretty-prints the output: "A positive integer indents that many spaces per level".[^2] `sort_keys=True` sorts the keys of each dictionary.[^2] With `ensure_ascii=True`, the default, "the output is guaranteed to have all incoming non-ASCII and non-printable characters escaped."[^2] Example: [`pretty`](#pretty).

**r6.** For other types, pass `default`, "A function that is called for objects that can’t otherwise be serialized. It should return a JSON encodable version of the object or raise a TypeError."[^2] On the way back, `object_hook` is "a function that is called with the result of any JSON object literal decoded (a dict). The return value of this function will be used instead of the dict."[^2] Example: [`custom`](#custom).

**r7.** "JSON files must be encoded in UTF-8, the default encoding for text files."[^1] `json.load()` also accepts a binary file.[^1]

## Examples

```python run id=types
import json

data = {'point': (1, 2), 1: 'one', 'pi': 3.14}
text = json.dumps(data)
print(text)
back = json.loads(text)
print(back)
print(back == data)
```

```output for=types
{"point": [1, 2], "1": "one", "pi": 3.14}
{'point': [1, 2], '1': 'one', 'pi': 3.14}
False
```

The tuple becomes a JSON array and comes back as a list (rule r3). The key `1` becomes the string `"1"` (rule r4). So the result is not equal to the original.

```python run id=pretty
import json

print(json.dumps({'b': 1, 'a': ['x', 'é']}, indent=2, sort_keys=True))
print(json.dumps({'a': 'é'}, ensure_ascii=False))
print(json.dumps([1, 2], separators=(',', ':')))
```

```output for=pretty
{
  "a": [
    "x",
    "\u00e9"
  ],
  "b": 1
}
{"a": "é"}
[1,2]
```

`indent=2` puts each item on its own line, and `sort_keys=True` puts `a` before `b`. The default escapes `é`, and `ensure_ascii=False` keeps it. The separators `(',', ':')` give "the most compact JSON" (rule r5).[^2]

```python run id=custom
import json
from datetime import date

def encode(obj):
    if isinstance(obj, date):
        return {'__date__': obj.isoformat()}
    raise TypeError(f'{type(obj).__name__} is not JSON serializable')

def decode(d):
    if '__date__' in d:
        return date.fromisoformat(d['__date__'])
    return d

text = json.dumps({'due': date(2026, 11, 16)}, default=encode)
print(text)
print(json.loads(text, object_hook=decode))
```

```output for=custom
{"due": {"__date__": "2026-11-16"}}
{'due': datetime.date(2026, 11, 16)}
```

The `default` function turns a date into a dictionary that JSON can hold, and `object_hook` turns that dictionary back into a date (rule r6).

## Pitfalls

A set is not JSON:

```python fails id=set expect=exception:TypeError
import json

json.dumps({'tags': {'a', 'b'}})
```

"If None (the default), TypeError is raised."[^2] Convert the set to a sorted list first, or pass `default`.

One file holds one JSON value. "Unlike pickle and marshal, JSON is not a framed protocol, so trying to serialize multiple objects with repeated calls to dump() using the same fp will result in an invalid JSON file."[^2] Put the objects in one list, or write one `dumps()` result per line and read the file line by line.

Python writes values that are not valid JSON. With the default `allow_nan=True`, NaN and the infinities "will be encoded as such. This behavior is not JSON specification compliant".[^2] Pass `allow_nan=False` to get a ValueError instead.

Do not use `pickle` for data from others. The tutorial warns that pickle "is also insecure by default: deserializing pickle data coming from an untrusted source can execute arbitrary code, if the data was crafted by a skilled attacker."[^1]

## Comparison

Go maps JSON onto typed values. "Marshal returns the JSON encoding of v."[^3] "Struct values encode as JSON objects."[^3] Python has no struct type in the default mapping. A Python class needs a `default` function or a conversion to a dict, such as `dataclasses.asdict()`.
