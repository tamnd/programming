---
tag: NZAZBX
kind: standard
slug: json-pointer
name: JSON Pointer
aliases: [RFC 6901, JSON pointer path]
area: data-and-persistence/data-formats
level: advanced
summary: >-
  JSON Pointer (RFC 6901) is a string such as /users/0/name that identifies one value inside a JSON document. JSON Patch, JSON Schema and many APIs use it to name a location.
see_also: [json, json-patch, jsonpath, json-schema]
sources:
- url: https://www.rfc-editor.org/rfc/rfc6901
  title: "P. Bryan (Ed.), K. Zyp, M. Nottingham (Ed.), RFC 6901: JavaScript Object Notation (JSON) Pointer, April 2013"
  locator: 3 Syntax; 4 Evaluation; 5 JSON String Representation; 6 URI Fragment Identifier Representation; 7 Error Handling
- url: https://www.rfc-editor.org/rfc/rfc6902
  title: "P. Bryan (Ed.), M. Nottingham (Ed.), RFC 6902: JavaScript Object Notation (JSON) Patch, April 2013"
  locator: 4 Operations; 4.1 add
- url: https://json-schema.org/draft/2020-12/json-schema-core
  title: "A. Wright, H. Andrews, B. Hutton, G. Dennis, JSON Schema: A Media Type for Describing JSON Documents, draft 2020-12"
  locator: 5 Fragment Identifiers; 8.2.4 Schema Re-Use With "$defs"; 12.3.3 Instance Location
- url: https://www.rfc-editor.org/rfc/rfc9535
  title: "S. Gössner, G. Normington, C. Bormann (Ed.), RFC 9535: JSONPath: Query Expressions for JSON, February 2024"
  locator: Appendix C JSON Pointer
- url: https://python-json-pointer.readthedocs.io/en/latest/tutorial.html
  title: python-json-pointer documentation, Tutorial
  locator: resolve_pointer
- url: https://docs.rs/serde_json/latest/serde_json/enum.Value.html
  title: serde_json documentation, enum Value
  locator: Value::pointer; Value::pointer_mut
- url: https://jakarta.ee/specifications/jsonp/2.1/apidocs/jakarta.json/jakarta/json/JsonPointer.html
  title: Jakarta JSON Processing 2.1 API, interface JsonPointer
  locator: the interface description; Method Summary
---

## What it is

JSON Pointer is a string syntax that identifies one value inside a JSON document. RFC 6901 defines it as "a string syntax for identifying a specific value within a JavaScript Object Notation (JSON) document"[^1]. The pointer `/users/0/name` means: in the root object, take the member `users`, then the element at index 0 of that array, then the member `name` of that object.

A pointer is a path of keys and indexes, written as text. The text form lets one JSON document refer to a location in another one. JSON Patch uses a pointer in each operation: an operation object "MUST have exactly one "path" member", and the value of that member is "a string containing a JSON-Pointer value"[^2]. JSON Schema uses pointers in URI fragments, so `"$ref": "#/$defs/positiveInteger"` refers to a subschema in the same document[^3]. The output format of JSON Schema also reports the location of a bad value as a pointer: the instance location "MUST be expressed as a JSON Pointer"[^3].

JSON Pointer is not a query language. RFC 9535, which defines JSONPath, says that "JSON Pointer is for identifying a single value within a JSON value whose structure is known"[^4]. A pointer has no wildcards and no filters. It identifies one value, or it fails.

## Syntax or structure

A pointer is "a sequence of zero or more reference tokens, each prefixed by a '/' (%x2F) character"[^1]. The grammar in ABNF is:

```text
json-pointer    = *( "/" reference-token )
reference-token = *( unescaped / escaped )
unescaped       = %x00-2E / %x30-7D / %x7F-10FFFF
escaped         = "~" ( "0" / "1" )
array-index     = %x30 / ( %x31-39 *(%x30-39) )
```

The rules of the grammar and of the evaluation are these[^1]:

- The empty string identifies the whole document. The pointer `/` is not the root. It identifies the member whose name is the empty string.
- A `~` in a member name is written as `~0`, and a `/` is written as `~1`. To decode a token, an evaluator replaces `~1` with `/` first, and then `~0` with `~`.
- On an object, a token selects the member with that name. The names are compared code point by code point, with no Unicode normalization.
- On an array, a token must be `0` or a decimal number with no leading zero. The token `-` refers to "the (nonexistent) member after the last array element".
- A pointer that does not match the grammar, or that refers to a value that does not exist, is an error. RFC 6901 does not say how an application handles the error[^1].

A pointer has two text forms. In a JSON string, the usual JSON escapes apply, so the pointer `/i\j` is written as `"/i\\j"`. In a URI fragment, the pointer is encoded as UTF-8 and percent-encoded, so `/c%d` becomes `#/c%25d`[^1].

## Examples

The function `resolve` below follows the evaluation steps of RFC 6901. The test document and the pointers are the examples of section 5 of the RFC[^1]. The last lines show the errors: a pointer with no leading `/`, an index with a leading zero, the token `-`, an index past the end and an escape that is not `~0` or `~1`.

```python run id=rfc-examples
import json
import re

INDEX = re.compile(r"0|[1-9][0-9]*")


def resolve(doc, pointer):
    """Returns the value that an RFC 6901 pointer identifies in doc."""
    if pointer == "":
        return doc
    if not pointer.startswith("/") or re.search(r"~(?![01])", pointer):
        raise ValueError(f"bad pointer {pointer!r}")
    value = doc
    for token in pointer[1:].split("/"):
        token = token.replace("~1", "/").replace("~0", "~")
        if isinstance(value, dict) and token in value:
            value = value[token]
        elif isinstance(value, list) and INDEX.fullmatch(token) and int(token) < len(value):
            value = value[int(token)]
        else:
            raise LookupError(f"no value at {pointer!r}")
    return value


def escape(name):
    """Turns a member name into a reference token."""
    return name.replace("~", "~0").replace("/", "~1")


doc = json.loads(r'''{"foo": ["bar", "baz"], "": 0, "a/b": 1, "c%d": 2, "e^f": 3,
  "g|h": 4, "i\\j": 5, "k\"l": 6, " ": 7, "m~n": 8}''')
for p in ["/foo", "/foo/0", "/", "/a~1b", "/c%d", "/e^f", "/g|h", "/i\\j", '/k"l', "/ ", "/m~0n"]:
    print(f"{p!r:10} {json.dumps(resolve(doc, p))}")
print("/" + escape("m~n"), "/" + escape("a/b"))

for p in ["foo", "/foo/01", "/foo/-", "/foo/2", "/x~2y"]:
    try:
        resolve(doc, p)
    except (ValueError, LookupError) as error:
        print(type(error).__name__, error)
```

```output for=rfc-examples
'/foo'     ["bar", "baz"]
'/foo/0'   "bar"
'/'        0
'/a~1b'    1
'/c%d'     2
'/e^f'     3
'/g|h'     4
'/i\\j'    5
'/k"l'     6
'/ '       7
'/m~0n'    8
/m~0n /a~1b
ValueError bad pointer 'foo'
LookupError no value at '/foo/01'
LookupError no value at '/foo/-'
LookupError no value at '/foo/2'
ValueError bad pointer '/x~2y'
```

The function `escape` makes a token from a member name. It replaces `~` before `/`, the reverse of the decode order. The example files of this page show the same resolver in other languages:

- **JavaScript.** `JSON.parse` gives plain objects. The resolver checks `Object.hasOwn`, so the pointer `/constructor` does not find a property of `Object.prototype`.
- **Go.** `json.Unmarshal` into `any` gives maps and slices, and a type switch walks them.
- **C#.** `System.Text.Json.Nodes` gives a tree of nodes. `TryGetPropertyValue` tells a member with the value `null` from a member that does not exist.

## Versions

RFC 6901 was published in April 2013, together with RFC 6902 (JSON Patch)[^1][^2]. It has no later version. Its references to JSON point to RFC 4627, an older JSON specification, but the pointer rules do not depend on the version of JSON.

Some applications add rules of their own. RFC 6902 uses the token `-` to append to an array: "If the "-" character is used to index the end of the array (see [RFC6901]), this has the effect of appending the value to the array"[^2]. RFC 9535 says that a Normalized JSONPath "can be converted into a JSON Pointer by converting the syntax, without knowledge of any JSON value", but the reverse is not always possible, because a numeric token can name an array element or an object member[^4].

## Pitfalls

- **The decode order matters.** If an evaluator replaces `~0` first, the token `~01` becomes `~1` and then `/`. The correct result is `~1`, the name of a member that has a tilde and a one in it[^1]:

```python wrong id=decode-order
def wrong(token):
    return token.replace("~0", "~").replace("~1", "/")


def right(token):
    return token.replace("~1", "/").replace("~0", "~")


# The member name "~1" is written as the token "~01".
print(wrong("~01"), right("~01"))
```

```output for=decode-order
/ ~1
```

- **A numeric token is not always an index.** On the object `{"0": "x"}`, the pointer `/0` selects the member named `0`. The meaning of a token depends on the value that the evaluator is at, not on the text of the token[^4].
- **`/` is not the root.** The root is the empty pointer. `/` refers to the member with the empty name.
- **`-` is never a value.** A plain lookup of `/items/-` is always an error[^1]. Only an application such as JSON Patch gives it a meaning[^2].
- **Leading zeros.** `/items/01` is an error, but a careless parser that calls a general integer function accepts it as index 1. Check the token against the `array-index` rule before you convert it.
- **Inherited properties in JavaScript.** A lookup with `value[token]` on a parsed object also finds properties such as `constructor` and `__proto__`. Check `Object.hasOwn` before each step, as in the JavaScript example.
- **Two encodings in a URI.** In a URI fragment, `~1` stays as it is, but `%` must become `%25`. Do not mix the percent encoding of the URI with the tilde escapes of the pointer[^1].
- **Duplicate member names.** If a referenced name is not unique in an object, the referenced member "is undefined, and evaluation fails"[^1]. Check for duplicate names when the input is not trusted.

## Tools and libraries

- Python: the library `jsonpointer`, with `resolve_pointer(obj, pointer)`[^5].
- Rust: `serde_json::Value::pointer`, which "Looks up a value by a JSON Pointer" and returns `None` when there is no such value[^6].
- Java: the interface `jakarta.json.JsonPointer` of Jakarta JSON Processing. Its methods `add()`, `replace()` and `remove()` "execute operations specified in RFC 6902"[^7].
- JavaScript, Go and C#: the standard libraries have no pointer function. The resolvers in the examples are short, and libraries for each language exist.

The page [[json-patch]] uses pointers to name the targets of its operations, and [[jsonpath]] covers the query language that selects many values.

## References
