---
tag: NEW-json-schema
kind: standard
slug: json-schema
name: JSON Schema
aliases: [JSON Schema 2020-12, application/schema+json]
area: data-and-persistence/data-formats
level: core
summary: >-
  JSON Schema is a JSON vocabulary that describes the structure of JSON data. A validator checks a document against a schema and reports each error with its location.
see_also: [json, json-pointer, read-and-write-json, jsonpath]
sources:
- url: https://json-schema.org/draft/2020-12/json-schema-core
  title: "A. Wright, H. Andrews, B. Hutton, G. Dennis, JSON Schema: A Media Type for Describing JSON Documents, draft-bhutton-json-schema-01, June 2022"
  locator: Abstract; 4.2.2 Instance Equality; 4.3 JSON Schema Documents; 8.1.1 The "$schema" Keyword; 8.2.4 Schema Re-Use With "$defs"; 10.3.2.3 additionalProperties; 12.3 Minimum Information; Appendix G ChangeLog
- url: https://json-schema.org/draft/2020-12/json-schema-validation
  title: "A. Wright, H. Andrews, B. Hutton, JSON Schema Validation: A Vocabulary for Structural Validation of JSON, draft-bhutton-json-schema-validation-01, June 2022"
  locator: 6.1.1 type; 6.3.1 maxLength; 6.5.3 required; 7 Vocabularies for Semantic Content With "format"
- url: https://spec.openapis.org/oas/v3.1.0.html
  title: OpenAPI Specification v3.1.0
  locator: 4.4 Data Types
- url: https://json-schema.org/specification-links
  title: JSON Schema, Specification Links
  locator: Understanding draft names and numbers; Table of All Versions of Everything
- url: https://python-jsonschema.readthedocs.io/en/stable/validate/
  title: jsonschema documentation, Schema Validation
  locator: the Validator protocol, format_checker
- url: https://ajv.js.org/guide/getting-started.html
  title: Ajv documentation, Getting started
  locator: Basic data validation; Best performance
- url: https://learn.microsoft.com/en-us/dotnet/standard/serialization/system-text-json/extract-schema
  title: .NET documentation, JSON schema exporter
  locator: the introduction and the first example
---

## What it is

JSON Schema is a vocabulary, written in JSON, that describes what a JSON document must look like. The core specification says that it is "a JSON-based format for describing the structure of JSON data"[^1]. A schema is itself a JSON document. A program called a validator applies the schema to a JSON value, the instance, and tells whether the instance is valid.

A schema has keywords. The schema `{"type": "integer", "minimum": 1}` accepts the instance `7` and rejects the instances `0` and `"7"`. Each keyword is an assertion about the instance, an annotation that gives information about it, or an applicator that applies other schemas to parts of it. The core specification defines the keywords that identify and combine schemas, and a companion specification, JSON Schema Validation, defines keywords such as `type`, `minimum` and `required`[^1][^2].

Programs use JSON Schema to check input. Code generators and user interface renderers also use schemas[^1]. OpenAPI 3.1 describes the bodies of HTTP requests and responses with a Schema Object, which "is a superset of JSON Schema Specification Draft 2020-12"[^3].

## Syntax or structure

A schema is a JSON object or a boolean. The boolean schema `true` accepts every instance, and `false` accepts none[^1]. A schema object has keywords, and the keywords with a dollar sign identify and connect schemas:

| Keyword | Meaning |
|---|---|
| `$schema` | the dialect of the schema, as a URI, such as `https://json-schema.org/draft/2020-12/schema` |
| `$id` | the URI of the schema resource |
| `$ref` | a reference to another schema, for example `#/$defs/tag`, a JSON Pointer in a URI fragment |
| `$defs` | a place for subschemas that `$ref` uses; it does not change the result by itself |

The validation keywords check the value of an instance[^2]:

- **Any type.** `type` gives one type or a list of types. The types are `null`, `boolean`, `object`, `array`, `number`, `string`, and `integer`, "which matches any number with a zero fractional part"[^2]. `enum` and `const` compare the instance with fixed values.
- **Numbers.** `minimum`, `maximum`, `exclusiveMinimum`, `exclusiveMaximum` and `multipleOf`.
- **Strings.** `minLength`, `maxLength` and `pattern`. The length of a string is "the number of its characters as defined by RFC 8259"[^2], so a validator counts code points.
- **Objects.** `required` lists names, and "An object instance is valid against this keyword if every item in the array is the name of a property in the instance"[^2]. `properties` gives a schema for each named member, and `additionalProperties` gives a schema for the other members[^1].
- **Arrays.** `items`, `prefixItems`, `contains`, `minItems`, `maxItems` and `uniqueItems`.
- **Combinations.** `allOf`, `anyOf`, `oneOf`, `not`, and `if` with `then` and `else`.

A keyword that does not apply to the type of the instance has no effect. `maxLength` restricts only strings, so a number is valid against it[^1]. To restrict the type, add `type`.

A validator reports each error with two JSON Pointers. The instance location is "The location of the JSON value within the instance being validated"[^1]. The keyword location is the path of the keyword in the schema, and it includes each `$ref` on the way, as in `/properties/width/$ref/minimum`[^1]. The specification defines four output formats: Flag, Basic, Detailed and Verbose[^1].

## Examples

The program below is a small validator for a subset of JSON Schema 2020-12. It yields each error with its instance location and its keyword location, as the Basic output format does[^1]. The schema describes a user record: an integer `id` of at least 1, a short `name`, an optional `role` from a list, a list of `tags` through a `$ref` to `$defs`, and no other members.

```python run id=validate
import json

# A subset of JSON Schema 2020-12: type, enum, required, properties, additionalProperties,
# items, minimum, maxLength and $ref to a JSON Pointer in the same document.
TYPES = {
    "null": lambda x: x is None,
    "boolean": lambda x: isinstance(x, bool),
    "object": lambda x: isinstance(x, dict),
    "array": lambda x: isinstance(x, list),
    "string": lambda x: isinstance(x, str),
    "number": lambda x: isinstance(x, (int, float)) and not isinstance(x, bool),
    "integer": lambda x: isinstance(x, (int, float)) and not isinstance(x, bool) and float(x).is_integer(),
}


def token(key):
    return str(key).replace("~", "~0").replace("/", "~1")


def validate(schema, instance, root, at="", kw=""):
    """Yields (instanceLocation, keywordLocation, message) for each failed assertion."""
    if schema is True:
        return
    if schema is False:
        yield at, kw, "false schema"
        return
    if "$ref" in schema:
        target = root
        for t in schema["$ref"].removeprefix("#/").split("/"):
            target = target[t.replace("~1", "/").replace("~0", "~")]
        yield from validate(target, instance, root, at, kw + "/$ref")
    if "type" in schema:
        names = schema["type"] if isinstance(schema["type"], list) else [schema["type"]]
        if not any(TYPES[n](instance) for n in names):
            yield at, kw + "/type", f"not of type {schema['type']}"
    if "enum" in schema and instance not in schema["enum"]:
        yield at, kw + "/enum", f"{json.dumps(instance)} is not one of {json.dumps(schema['enum'])}"
    if TYPES["number"](instance) and "minimum" in schema and instance < schema["minimum"]:
        yield at, kw + "/minimum", f"{instance} is less than {schema['minimum']}"
    if isinstance(instance, str) and "maxLength" in schema and len(instance) > schema["maxLength"]:
        yield at, kw + "/maxLength", f"{len(instance)} characters, more than {schema['maxLength']}"
    if isinstance(instance, dict):
        for name in schema.get("required", []):
            if name not in instance:
                yield at, kw + "/required", f"no property {json.dumps(name)}"
        props = schema.get("properties", {})
        for name, value in instance.items():
            if name in props:
                yield from validate(props[name], value, root, f"{at}/{token(name)}", f"{kw}/properties/{token(name)}")
            elif "additionalProperties" in schema:
                yield from validate(schema["additionalProperties"], value, root, f"{at}/{token(name)}", kw + "/additionalProperties")
    if isinstance(instance, list) and "items" in schema:
        for i, value in enumerate(instance):
            yield from validate(schema["items"], value, root, f"{at}/{i}", kw + "/items")


schema = json.loads("""{
  "$schema": "https://json-schema.org/draft/2020-12/schema",
  "type": "object",
  "required": ["id", "name"],
  "properties": {
    "id": {"type": "integer", "minimum": 1},
    "name": {"type": "string", "maxLength": 10},
    "role": {"enum": ["admin", "user"]},
    "tags": {"type": "array", "items": {"$ref": "#/$defs/tag"}}
  },
  "additionalProperties": false,
  "$defs": {"tag": {"type": "string", "maxLength": 3}}
}""")

for text in ['{"id": 7, "name": "Ada", "tags": ["new"]}',
             '{"id": 7.0, "name": "Ada", "role": "user"}',
             '{"id": 0, "name": "Ada Lovelace", "role": "root", "tags": ["new", "beta", 3], "mail": "a@b"}',
             '{"name": null}']:
    errors = list(validate(schema, json.loads(text), schema))
    print("valid" if not errors else "invalid", text)
    for at, kw, message in errors:
        print(f"  {at or '(root)':8} {kw:38} {message}")
```

```output for=validate
valid {"id": 7, "name": "Ada", "tags": ["new"]}
valid {"id": 7.0, "name": "Ada", "role": "user"}
invalid {"id": 0, "name": "Ada Lovelace", "role": "root", "tags": ["new", "beta", 3], "mail": "a@b"}
  /id      /properties/id/minimum                 0 is less than 1
  /name    /properties/name/maxLength             12 characters, more than 10
  /role    /properties/role/enum                  "root" is not one of ["admin", "user"]
  /tags/1  /properties/tags/items/$ref/maxLength  4 characters, more than 3
  /tags/2  /properties/tags/items/$ref/type       not of type string
  /mail    /additionalProperties                  false schema
invalid {"name": null}
  (root)   /required                              no property "id"
  /name    /properties/name/type                  not of type string
```

The second instance is valid, because `7.0` is an integer for JSON Schema. The third instance shows one error for each failed assertion, and the `$ref` in the keyword locations. The member `mail` fails against the schema `false` of `additionalProperties`. The validator uses the Python operator `in` for `enum`, which treats `true` and `1` as equal. A full validator uses the equality rules of the specification, which the Go example shows.

The example files of this page show other parts of the standard:

- **JavaScript.** The length of a string for `maxLength` is a count of code points, not of UTF-16 code units.
- **Go.** The instance equality of `enum` and `const`, with exact numbers.
- **C#.** A schema generated from a .NET type with `JsonSchemaExporter`.

## Versions

The versions of JSON Schema are Internet-Drafts, not RFCs. Each has a name and a meta-schema URI[^4]:

| Version | Published | Notes |
|---|---|---|
| Draft 0 to Draft 3 | 2009 to 2010 | the first drafts |
| Draft 4 | 31 January 2013 | |
| Draft 6 | 21 April 2017 | |
| Draft 7 | 19 March 2018 | |
| 2019-09 | 17 September 2019 | "formerly known as Draft 8" |
| 2020-12 | 16 June 2022 | the current release, `draft-bhutton-json-schema-01` |

Draft 5 did not change the meta-schemas, so "draft-05" is really draft-04[^4]. From 2019-09, the meta-schemas are "identified by the year and month of publication"[^4]. The table lists two sets of IETF documents for 2020-12, the -00 and the -01 drafts, with the same meta-schema identifier[^4].

The keywords changed between the versions. The 2019-09 release moved `definitions` into the core specification as `$defs`[^1]. The 2020-12 release changed arrays: the array form of `items` "is now "prefixItems"", and `items` "subsumes the old function of "additionalItems""[^1]. The keywords of the versions differ, so put `$schema` in each root schema. It tells the validator which dialect the schema uses[^1].

## Pitfalls

- **An integer test that uses the host type.** In JSON Schema, `3.0` is an integer and `true` is not a number[^2]. In Python, `True` is an `int` and `3.0` is a `float`, so a check with `isinstance` gives both answers wrong:

```python wrong id=integer-check
import json


def is_integer(x):
    # The check that many hand-written validators use.
    return isinstance(x, int)


for value in json.loads("[3, 3.0, true, 3.5]"):
    print(json.dumps(value), is_integer(value))
```

```output for=integer-check
3 True
3.0 False
true True
3.5 False
```

- **OpenAPI integers.** OpenAPI 3.1 defines `integer` as "a JSON number without a fraction or exponent part"[^3]. For OpenAPI, `3.0` is not an integer, but for JSON Schema it is[^2].
- **`format` does not validate by default.** In 2020-12, `format` is an annotation, and an assertion only if the schema uses the Format-Assertion vocabulary, which is "OPTIONAL" for implementations[^2]. The Python library `jsonschema` says that without a format checker, "no format validation is done"[^5]. To reject a bad email address, turn on the format checks of the validator, or add a `pattern`.
- **Unknown keywords are ignored.** "Unknown keywords SHOULD be treated as annotations"[^1]. A schema with the typo `requried` has no required members, and it accepts more than its author wants. Test each schema with invalid instances, and validate the schema against its meta-schema.
- **`additionalProperties` sees only its own schema object.** It applies to the members that `properties` and `patternProperties` in the same schema object do not match[^1]. If `properties` is inside an `allOf` branch, `additionalProperties: false` at the top rejects those members. Use `unevaluatedProperties` for that case, because it also sees the members that the subschemas of `allOf` evaluate[^1].
- **A missing `type`.** `{"minimum": 1}` accepts the string `"0"`, because `minimum` applies only to numbers.
- **Big numbers.** A validator that parses JSON into binary floating point cannot tell 9007199254740992 from 9007199254740993, so `enum` and `const` can match the wrong number. The Go example keeps the numbers exact.

## Tools and libraries

- **Python.** The library `jsonschema` has a validator class for each draft, such as `Draft202012Validator`, and the parameter `format_checker` turns on format checks[^5].
- **JavaScript.** Ajv "compiles schemas to functions and caches them"[^6]. Compile a schema once and use the function many times.
- **.NET.** `JsonSchemaExporter`, "introduced in .NET 9", generates a schema from a .NET type[^7]. The base library has no validator.
- **OpenAPI.** OpenAPI 3.1 uses JSON Schema 2020-12 for its data types[^3].

The page [[json-pointer]] covers the syntax of `$ref` fragments and error locations, and [[read-and-write-json]] covers how to parse the documents that a schema describes.

## References
