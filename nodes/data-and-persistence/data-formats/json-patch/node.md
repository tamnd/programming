---
tag: 3J41ZW
kind: standard
slug: json-patch
name: JSON Patch and JSON Merge Patch
aliases: [RFC 6902, RFC 7396, RFC 7386, merge patch, application/json-patch+json, application/merge-patch+json]
area: data-and-persistence/data-formats
level: advanced
summary: >-
  JSON Patch (RFC 6902) is a list of operations that change a JSON document. JSON Merge Patch (RFC 7396) is a partial document that is merged into the target. Both are bodies for HTTP PATCH.
see_also: [json, json-pointer, jsonpath, json-schema]
sources:
- url: https://www.rfc-editor.org/rfc/rfc5789
  title: "L. Dusseault, J. Snell, RFC 5789: PATCH Method for HTTP, March 2010"
  locator: 2 The PATCH Method; 2.2 Error Handling
- url: https://www.rfc-editor.org/rfc/rfc6902
  title: "P. Bryan (Ed.), M. Nottingham (Ed.), RFC 6902: JavaScript Object Notation (JSON) Patch, April 2013"
  locator: 1 Introduction; 3 Document Structure; 4 Operations; 4.1 to 4.6; 5 Error Handling; 6 IANA Considerations
- url: https://www.rfc-editor.org/rfc/rfc7396
  title: "P. Hoffman, J. Snell, RFC 7396: JSON Merge Patch, October 2014"
  locator: 1 Introduction; 2 Processing Merge Patch Documents; 3 Example; Appendix A Example Test Cases
- url: https://learn.microsoft.com/en-us/dotnet/api/system.text.json.nodes.jsonnode.deepequals
  title: .NET API reference, JsonNode.DeepEquals
  locator: the method description
- url: https://www.rfc-editor.org/rfc/rfc6901
  title: "P. Bryan (Ed.), K. Zyp, M. Nottingham (Ed.), RFC 6901: JavaScript Object Notation (JSON) Pointer, April 2013"
  locator: 4 Evaluation
- url: https://www.rfc-editor.org/rfc/rfc7386
  title: "P. Hoffman, J. Snell, RFC 7386: JSON Merge Patch, October 2014 (obsoleted by RFC 7396)"
  locator: 2 Processing Merge Patch Documents
- url: https://python-json-patch.readthedocs.io/en/latest/tutorial.html
  title: python-json-patch documentation, Tutorial
  locator: Creating a Patch; Applying a Patch
- url: https://jakarta.ee/specifications/jsonp/2.1/apidocs/jakarta.json/jakarta/json/JsonPatch.html
  title: Jakarta JSON Processing 2.1 API, interface JsonPatch
  locator: the interface description
- url: https://learn.microsoft.com/en-us/aspnet/core/web-api/jsonpatch
  title: Microsoft Learn, JSON Patch support in ASP.NET Core web API
  locator: the introduction
---

## What it is

JSON Patch and JSON Merge Patch are two formats that describe a change to a JSON document. A client sends the change instead of the full new document. Both formats were made for the HTTP method PATCH, which RFC 5789 defines to "apply partial modifications to a resource"[^1].

JSON Patch, defined in RFC 6902, is "a format (identified by the media type "application/json-patch+json") for expressing a sequence of operations to apply to a target JSON document"[^2]. Each operation has a name, such as `add` or `remove`, and a target given as a JSON Pointer. The operations run in order, and the patch can test a value before it changes it.

JSON Merge Patch, defined in RFC 7396, is a partial copy of the document. It "describes changes to be made to a target JSON document using a syntax that closely mimics the document being modified"[^3]. A member in the patch replaces or adds the member in the target, and a `null` removes it. The format is simple to read and write, but it cannot express every change.

| Property | JSON Patch[^2] | JSON Merge Patch[^3] |
|---|---|---|
| Media type | `application/json-patch+json` | `application/merge-patch+json` |
| Form | an array of operation objects | a JSON value with the shape of the target |
| Arrays | insert, remove or move one element | replace the whole array |
| Set a member to `null` | yes, with `add` or `replace` | no, `null` means remove |
| Conditions | the `test` operation | none |
| Order of changes | the order of the operations | not significant |

## Syntax or structure

A JSON Patch document "is a JSON [RFC4627] document that represents an array of objects"[^2]. Each object has an `op` member and a `path` member. The value of `op` "MUST be one of "add", "remove", "replace", "move", "copy", or "test"; other values are errors"[^2], and `path` is a JSON Pointer[^2]. Other members depend on the operation:

| Operation | Members | Effect[^2] |
|---|---|---|
| `add` | `path`, `value` | inserts into an array, adds a new member, or replaces an existing member; `-` as the last token appends to an array |
| `remove` | `path` | removes the value; the target must exist |
| `replace` | `path`, `value` | the same as `remove` and then `add` at the same location; the target must exist |
| `move` | `from`, `path` | the same as `remove` at `from` and then `add` at `path`; `from` must not be a prefix of `path` |
| `copy` | `from`, `path` | the same as `add` at `path` with the value at `from` |
| `test` | `path`, `value` | fails if the value at `path` is not equal to `value` |

The example patch of RFC 6902 shows all six operations:

```json
[
  { "op": "test", "path": "/a/b/c", "value": "foo" },
  { "op": "remove", "path": "/a/b/c" },
  { "op": "add", "path": "/a/b/c", "value": [ "foo", "bar" ] },
  { "op": "replace", "path": "/a/b/c", "value": 42 },
  { "op": "move", "from": "/a/b/c", "path": "/a/b/d" },
  { "op": "copy", "from": "/a/b/d", "path": "/a/b/e" }
]
```

The operations apply "sequentially in the order they appear in the array", and each one sees the result of the one before it[^2]. If an operation fails, evaluation "SHOULD terminate and application of the entire patch document SHALL NOT be deemed successful"[^2]. Because PATCH in HTTP is atomic, a failed patch changes nothing[^2][^1].

For `test`, two values are equal if they "are of the same JSON type" and equal under the rules of that type. Numbers are equal "if their values are numerically equal", arrays compare element by element in order, and the order of object members is not significant[^2].

A JSON Merge Patch is any JSON value. RFC 7396 defines its meaning with the function `MergePatch`[^3]:

```text
define MergePatch(Target, Patch):
  if Patch is an Object:
    if Target is not an Object:
      Target = {} # Ignore the contents and set it to an empty Object
    for each Name/Value pair in Patch:
      if Value is null:
        if Name exists in Target:
          remove the Name/Value pair from Target
      else:
        Target[Name] = MergePatch(Target[Name], Value)
    return Target
  else:
    return Patch
```

A patch that is not an object replaces the whole target. RFC 7396 says that it "is not possible to patch part of a target that is not an object, such as to replace just some of the values in an array"[^3].

## Examples

The function `apply_patch` below implements the six operations of RFC 6902 in Python. It works on a deep copy, so a failed patch leaves the input as it was. The second patch is the example of section 5 of the RFC: the `replace` succeeds, the `test` fails, and the document does not change[^2].

```python run id=apply
import copy
import json
import re

INDEX = re.compile(r"0|[1-9][0-9]*")


class PatchError(Exception):
    pass


def tokens(pointer):
    if pointer == "":
        return []
    if not pointer.startswith("/"):
        raise PatchError(f"bad pointer {pointer!r}")
    return [t.replace("~1", "/").replace("~0", "~") for t in pointer[1:].split("/")]


def step(value, token):
    if isinstance(value, dict) and token in value:
        return value[token]
    if isinstance(value, list) and INDEX.fullmatch(token) and int(token) < len(value):
        return value[int(token)]
    raise PatchError(f"no value at {token!r}")


def get(doc, pointer):
    for token in tokens(pointer):
        doc = step(doc, token)
    return doc


def add(doc, pointer, value):
    if pointer == "":
        return value
    *path, last = tokens(pointer)
    container = doc
    for token in path:
        container = step(container, token)
    if isinstance(container, dict):
        container[last] = value
    elif isinstance(container, list):
        i = len(container) if last == "-" else int(last) if INDEX.fullmatch(last) else -1
        if not 0 <= i <= len(container):
            raise PatchError(f"bad index {last!r}")
        container.insert(i, value)
    else:
        raise PatchError(f"cannot add to a {type(container).__name__}")
    return doc


def remove(doc, pointer):
    *path, last = tokens(pointer) or [None]
    if last is None:
        raise PatchError("cannot remove the root")
    container = get(doc, "".join("/" + t.replace("~", "~0").replace("/", "~1") for t in path))
    step(container, last)  # The target must exist.
    return container.pop(int(last) if isinstance(container, list) else last)


def equal(a, b):
    """The test rules of RFC 6902: the same JSON type, and equal values."""
    if isinstance(a, bool) or isinstance(b, bool):
        return type(a) is type(b) and a == b
    if isinstance(a, (int, float)) and isinstance(b, (int, float)):
        return a == b
    if type(a) is not type(b):
        return False
    if isinstance(a, list):
        return len(a) == len(b) and all(map(equal, a, b))
    if isinstance(a, dict):
        return a.keys() == b.keys() and all(equal(a[k], b[k]) for k in a)
    return a == b


def apply_patch(doc, patch):
    """Applies the operations in order to a copy, so a failed patch changes nothing."""
    doc = copy.deepcopy(doc)
    for op in patch:
        kind, path = op["op"], op["path"]
        if kind == "add":
            doc = add(doc, path, copy.deepcopy(op["value"]))
        elif kind == "remove":
            remove(doc, path)
        elif kind == "replace":
            get(doc, path)
            if path == "":
                doc = copy.deepcopy(op["value"])
            else:
                remove(doc, path)
                doc = add(doc, path, copy.deepcopy(op["value"]))
        elif kind == "move":
            if path.startswith(op["from"] + "/"):
                raise PatchError("cannot move a value into its own child")
            value = get(doc, op["from"])
            if op["from"] != "":
                remove(doc, op["from"])
            doc = add(doc, path, value)
        elif kind == "copy":
            doc = add(doc, path, copy.deepcopy(get(doc, op["from"])))
        elif kind == "test":
            if not equal(get(doc, path), op["value"]):
                raise PatchError(f"test failed at {path!r}")
        else:
            raise PatchError(f"unknown op {kind!r}")
    return doc


doc = {"name": "lamp", "tags": ["home"], "stock": {"count": 3}}
patch = json.loads("""[
  {"op": "test", "path": "/stock/count", "value": 3},
  {"op": "replace", "path": "/stock/count", "value": 2},
  {"op": "add", "path": "/tags/-", "value": "sale"},
  {"op": "add", "path": "/tags/0", "value": "new"},
  {"op": "copy", "from": "/name", "path": "/title"},
  {"op": "move", "from": "/stock", "path": "/inventory"},
  {"op": "remove", "path": "/name"}
]""")
print(json.dumps(apply_patch(doc, patch)))

bad = [{"op": "replace", "path": "/stock/count", "value": 42}, {"op": "test", "path": "/stock/count", "value": "C"}]
try:
    apply_patch(doc, bad)
except PatchError as error:
    print("PatchError:", error)
print(json.dumps(doc))
```

```output for=apply
{"tags": ["new", "home", "sale"], "title": "lamp", "inventory": {"count": 2}}
PatchError: test failed at '/stock/count'
{"name": "lamp", "tags": ["home"], "stock": {"count": 3}}
```

`merge_patch` implements the `MergePatch` function. The input is the example of section 3 of RFC 7396: the patch changes `title`, adds `phoneNumber`, removes `familyName` and replaces the array `tags`[^3]. The last line shows that a merge patch cannot store a `null`.

```python run id=merge
import json


def merge_patch(target, patch):
    """The MergePatch function of RFC 7396. It returns a new value."""
    if not isinstance(patch, dict):
        return patch
    result = dict(target) if isinstance(target, dict) else {}
    for name, value in patch.items():
        if value is None:
            result.pop(name, None)
        else:
            result[name] = merge_patch(result.get(name), value)
    return result


target = json.loads("""{"title": "Goodbye!", "author": {"givenName": "John", "familyName": "Doe"},
  "tags": ["example", "sample"], "content": "This will be unchanged"}""")
patch = json.loads("""{"title": "Hello!", "phoneNumber": "+01-123-456-7890",
  "author": {"familyName": null}, "tags": ["example"]}""")
print(json.dumps(merge_patch(target, patch)))

# A merge patch cannot set a member to null: null means "remove".
print(json.dumps(merge_patch({"a": 1}, {"a": None})))
```

```output for=merge
{"title": "Hello!", "author": {"givenName": "John"}, "tags": ["example"], "content": "This will be unchanged", "phoneNumber": "+01-123-456-7890"}
{}
```

The example files of this page show more:

- **JavaScript.** A merge patch made from two versions of a document, and applied again.
- **Go.** A merge patch on decoded maps, and the reason that a struct cannot hold a merge patch: a `nil` pointer field does not tell `{"note": null}` from `{}`.
- **C#.** The `test` operation with `JsonNode.DeepEquals`, which "Compares the values of two nodes, including the values of all descendant nodes"[^4].

## Versions

RFC 6902 (JSON Patch) and RFC 6901 (JSON Pointer) were published together in April 2013[^2][^5]. Neither has a later version.

JSON Merge Patch was first published as RFC 7386 in October 2014[^6]. RFC 7396 replaced it in the same month, and its header says "Obsoletes: 7386"[^3]. The two texts have the same `MergePatch` steps, but in the text of RFC 7386 some lines of the pseudocode have the wrong indentation[^6][^3]. Use RFC 7396.

## Pitfalls

- **A naive `test` compares the wrong types.** In Python, `True == 1` is true, so a `test` that uses `==` accepts `1` for `true`. RFC 6902 requires the same JSON type[^2]:

```python wrong id=bool-equal
# A test operation that compares with == accepts values of the wrong JSON type.
print(True == 1, [1, 0] == [True, False], {"on": 1} == {"on": True})
```

```output for=bool-equal
True True True
```

The `equal` function of the example checks for `bool` before it compares numbers.

- **A partial apply.** An implementation that changes the document in place and stops at the first error leaves a document that is neither the old one nor the new one. Apply the operations to a copy, or undo them, so the patch is atomic[^2].
- **Array indexes move.** After `add` at `/tags/0`, every later index in the same array is one larger, because the elements "are shifted one position to the right"[^2]. A patch that a program writes must use the indexes of the document at that step, not of the original. A `test` before a change finds a wrong index early.
- **`add` on a missing parent.** `add` creates the last member only. RFC 6902 says that the `add` to `/a/b` is an error on `{ "q": { "bar": 2 } }`, "because "a" does not exist"[^2].
- **`null` in a merge patch.** A merge patch cannot give a member the value `null`, and RFC 7396 says that the format suits documents that "do not make use of explicit null values"[^3]. Use JSON Patch when `null` is a real value.
- **Arrays in a merge patch.** A merge patch replaces a whole array. Two clients that each add one element to the same array lose one change. Use JSON Patch with `-` to append.
- **The wrong media type.** A server must know which format the body uses. Send `application/json-patch+json` or `application/merge-patch+json`, not `application/json`[^2][^3].
- **Lost updates.** A patch applies to the current state of a resource. RFC 6902 shows the request with an `If-Match` header[^2], so the server applies the patch only to the version that the client saw.

## Tools and libraries

- Python: the library `jsonpatch`. `jsonpatch.make_patch(src, dst)` makes a patch from two documents, and `jsonpatch.apply_patch(obj, patch)` applies one[^7].
- Java: the interface `jakarta.json.JsonPatch` of Jakarta JSON Processing. `Json.createPatch(JsonArray)` makes a patch, and `apply` applies it[^8].
- C#: ASP.NET Core web APIs read JSON Patch requests with the NuGet package `Microsoft.AspNetCore.JsonPatch.SystemTextJson`[^9].
- JavaScript and Go: the merge patch function is short, as in the examples. JSON Patch needs a library or a resolver such as the one on [[json-pointer]].

## References
