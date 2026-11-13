---
tag: NEW-validate-data-against-a-schema
kind: task
slug: validate-data-against-a-schema
name: Validate data against a schema
aliases: [validate JSON, JSON Schema validation, check data against a schema, schema validation, validate XML against a schema]
area: data-and-persistence/data-formats
level: core
summary: Check a JSON value against the core keywords of JSON Schema 2020-12 and list each failed assertion with its instance location and keyword location as JSON Pointers.
contract: contract.yaml
category: data-formats
languages: [python, javascript, typescript, csharp, go]
see_also: [json-schema, read-and-write-json, json-pointer, deserialization-of-untrusted-data]
sources:
- url: https://json-schema.org/draft/2020-12/json-schema-core
  title: "A. Wright, H. Andrews, B. Hutton, G. Dennis, JSON Schema: A Media Type for Describing JSON Documents, draft-bhutton-json-schema-01, June 2022"
  locator: 4.3.2 Boolean JSON Schemas; 6.5 Extending JSON Schema; 10.3.1.2 items; 10.3.2.1 properties; 10.3.2.3 additionalProperties; 12.3 Minimum Information
- url: https://json-schema.org/draft/2020-12/json-schema-validation
  title: "A. Wright, H. Andrews, B. Hutton, JSON Schema Validation: A Vocabulary for Structural Validation of JSON, draft-bhutton-json-schema-validation-01, June 2022"
  locator: 6.1.1 type; 6.2.2 maximum; 6.2.4 minimum; 6.3.1 maxLength; 6.3.2 minLength; 6.5.3 required; 7.2 Implementation Requirements
- url: https://www.rfc-editor.org/rfc/rfc6901
  title: "P. Bryan (Ed.), K. Zyp, M. Nottingham (Ed.), RFC 6901: JavaScript Object Notation (JSON) Pointer, April 2013"
  locator: 3 Syntax
- url: https://docs.python.org/3.14/library/sys.html#sys.float_info
  title: The Python Standard Library, sys, Python 3.14
  locator: sys.float_info, dig
- url: https://python-jsonschema.readthedocs.io/en/stable/validate/
  title: jsonschema documentation, Schema Validation
  locator: The Basics, jsonschema.validate; the Validator protocol, format_checker
- url: https://ajv.js.org/guide/getting-started.html
  title: Ajv documentation, Getting started
  locator: Basic data validation; Best performance
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/String/length
  title: MDN Web Docs, String length
  locator: the introduction
- url: https://learn.microsoft.com/en-us/dotnet/api/system.string.enumeraterunes
  title: .NET API reference, String.EnumerateRunes Method
  locator: Definition
- url: https://pkg.go.dev/encoding/json
  title: Go standard library, package encoding/json
  locator: func Unmarshal
- url: https://pkg.go.dev/unicode/utf8
  title: Go standard library, package unicode/utf8
  locator: func RuneCount; func RuneCountInString
- url: https://go.dev/ref/spec
  title: The Go Programming Language Specification
  locator: For statements with range clause
- url: https://learn.microsoft.com/en-us/dotnet/standard/data/xml/xml-schema-xsd-validation-with-xmlschemaset
  title: .NET documentation, XML Schema (XSD) Validation with XmlSchemaSet
  locator: the example
- url: https://cheatsheetseries.owasp.org/cheatsheets/Input_Validation_Cheat_Sheet.html
  title: OWASP Cheat Sheet Series, Input Validation Cheat Sheet
  locator: Parse Safely, Then Validate; Validate Structured Data
---

## Task

Check a JSON value against a JSON Schema, and report each part of the value that breaks a rule.

Each solution is a function `validate(schema, instance)`. Both arguments are JSON texts. The result is a list of pairs, one for each assertion that fails. The first element of a pair is the instance location, a JSON Pointer to the value that failed. The second element is the keyword location, a JSON Pointer to the keyword in the schema. The output units of JSON Schema 2020-12 have these two fields, with the names `instanceLocation` and `keywordLocation`[^1]. An empty list means that the instance is valid.

The solutions apply nine keywords of JSON Schema 2020-12[^1][^2]:

- `type` accepts a value of one of the given types: `null`, `boolean`, `object`, `array`, `number`, `string` or `integer`. The type `integer` "matches any number with a zero fractional part"[^2], so `1.0` is an integer.
- `minimum` and `maximum` are inclusive limits for a number.
- `minLength` and `maxLength` are inclusive limits for the length of a string. The length is "the number of its characters as defined by RFC 8259"[^2], that is, the number of code points.
- `required` gives the names that an object must have.
- `properties` gives a schema for the member of each name, if the member exists.
- `additionalProperties` gives a schema for each member that `properties` does not name.
- `items` gives a schema for each element of an array.

The schema `true` accepts every value, and the schema `false` accepts no value[^1]. A keyword that tests one type applies only to a value of that type, so `{"minimum": 5}` accepts the string `"abc"`. The solutions ignore all other keywords. The core specification says: "Implementations SHOULD treat keywords they do not support as annotations"[^1].

For the schema and the instance

```json
{"type": "object", "required": ["name", "age"],
 "properties": {"name": {"type": "string", "minLength": 1}, "age": {"type": "integer", "minimum": 0}}}
```

```json
{"name": "", "age": 3.5}
```

the result is `[["/age", "/properties/age/type"], ["/name", "/properties/name/minLength"]]`.

The pairs are sorted by the instance location and then by the keyword location, in code point order. Thus each language gives the same list. In a JSON Pointer, "'~' needs to be encoded as '~0' and '/' needs to be encoded as '~1'"[^3]. The solution reports `invalid-json` when the schema or the instance is not a JSON text.

The contract limits the input in three ways. Each number has at most 15 significant digits. A decimal number with at most 15 significant digits goes to a double and back with no change[^4], so the comparisons give the same result in each language. Each name holds only characters below U+D800, so UTF-16 order and code point order agree. The schema has no keyword that starts with a dollar sign, such as `$ref`, because a reference needs the URI rules of the core specification.

## Solutions

The standard libraries of the tier-1 languages have no JSON Schema validator. Each solution parses the two texts with the JSON parser of its standard library. Then a recursive function walks the schema and the instance together and carries the two locations. Each failed assertion adds a pair to a list, and the list is sorted at the end. In real code, use a library that implements the full specification, such as jsonschema for Python[^5] or Ajv for JavaScript[^6]. The page [[json-schema]] describes the format and its versions. The file `matrix.yaml` records the verdicts for the other languages.

- **Python.** `json.loads` makes `true` a `bool`, and `bool` is a subclass of `int`. Thus the solution tests for `bool` before `int`. A `float` is an integer when `is_integer()` returns `True`. `len` counts the code points of a string.
- **JavaScript and TypeScript (Node.js).** `typeof null` and `typeof []` both give `"object"`, so the solution tests for `null` and `Array.isArray` first. `Number.isInteger` tests for a zero fractional part. The `length` of a string is its length "in UTF-16 code units"[^7], so the solution counts the code points with the spread syntax `[...value]`. It uses `Object.hasOwn` for each name, because the operator `in` also finds inherited names such as `toString`.
- **C#.** `JsonDocument.Parse` gives a tree of `JsonElement` values, and `ValueKind` gives the type of each value. `EnumerateRunes` returns "an enumeration of Rune from this string"[^8], one for each code point, and the solution counts them. `string.CompareOrdinal` sorts by UTF-16 code units, which agrees with code point order under the contract.
- **Go.** `json.Unmarshal` into an `any` stores a `bool`, a `float64`, a `string`, a `[]any`, a `map[string]any` or `nil`[^9], and a type switch selects the keywords for each. `len` counts the bytes of a string, and `utf8.RuneCountInString` counts its runes[^10]. "The iteration order over maps is not specified"[^11], so the members come in a different order on each run, and the sort makes the result stable.

An XML document has its own schema languages, such as XML Schema (XSD). In .NET, `XmlReaderSettings` with `ValidationType.Schema` makes an `XmlReader` validate the document while it reads it, and a `ValidationEventHandler` gets each error[^12]. Here the age is negative, and the element `role` is not in the schema:

```csharp run id=xsd-validate
using System.Xml;
using System.Xml.Schema;

string xsd = """
    <xs:schema xmlns:xs="http://www.w3.org/2001/XMLSchema">
      <xs:element name="user">
        <xs:complexType>
          <xs:sequence>
            <xs:element name="name" type="xs:string"/>
            <xs:element name="age" type="xs:nonNegativeInteger"/>
          </xs:sequence>
        </xs:complexType>
      </xs:element>
    </xs:schema>
    """;
string xml = """
    <user>
      <name>Ada</name>
      <age>-1</age>
      <role>admin</role>
    </user>
    """;
var settings = new XmlReaderSettings { ValidationType = ValidationType.Schema };
settings.Schemas.Add(null, XmlReader.Create(new StringReader(xsd)));
settings.ValidationEventHandler += (sender, e) =>
    Console.WriteLine($"{e.Severity} at line {e.Exception.LineNumber}: {e.Message}");
using (XmlReader reader = XmlReader.Create(new StringReader(xml), settings))
{
    while (reader.Read()) { }
}
```

```output for=xsd-validate
Error at line 3: The 'age' element is invalid - The value '-1' is invalid according to its datatype 'http://www.w3.org/2001/XMLSchema:nonNegativeInteger' - Value '-1' was either too large or too small for NonNegativeInteger.
Error at line 4: The element 'user' has invalid child element 'role'.
```

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`. Each case gives the schema, the instance and the sorted pairs.

- `integer-from-fraction-zero`, `integer-from-exponent` and `integer-with-fraction`: `1.0` and `1e2` are integers, and `3.5` is not.
- `boolean-not-integer` and `boolean-not-number`: `true` and `false` are booleans, not numbers.
- `null-not-object` and `array-not-object`: `null` and `[]` are not objects.
- `type-list-match` and `type-list-miss`: `{"type": ["string", "null"]}` accepts `null` and rejects `5`.
- `max-length-astral`, `min-length-astral` and `combining-mark`: the emoji U+1F600 has the length 1, also when the text writes it as two escapes. The letter `e` and the combining acute accent U+0301 have the length 2.
- `other-type-keywords` and `other-type-keywords-number`: the keywords for numbers, objects and arrays do not apply to a string, and the keywords for strings and objects do not apply to a number.
- `unknown-keyword` and `format-is-annotation`: `{"minimun": 5}` accepts `1`, and `{"format": "email"}` accepts any string.
- `true-schema`, `false-schema`, `empty-schema` and `false-member-schema`: the schema `false` fails at its own location, which is the empty pointer at the root.
- `pointer-escape` and `pointer-escape-order`: the names `a/b`, `m~n` and `~1` become `a~1b`, `m~0n` and `~01` in the pointers.
- `absent-property`, `required-all-missing` and `required-null-member`: a missing member is not an error for `properties`. Three missing names give one error for `required`. A member with the value `null` exists.
- `inclusive-bounds`, `negative-zero` and `fraction-minimum`: `0` and `10` pass the limits 0 and 10, and `-0` passes the minimum 0.
- `index-order` and `keyword-order`: the pointer `/10` sorts before `/2`, and `/minimum` sorts before `/type`.
- `inherited-name` and `proto-member`: the names `toString`, `constructor` and `__proto__` are normal names.
- `non-ascii-name`, `nested-items` and `deep-path`: names outside ASCII, an array of arrays, and pointers of five steps.

The cases in `cases/errors.jsonl` are failures. The toolchain does not load this file yet, so the test files hold these cases by name. A trailing comma, `NaN`, an empty text, a text with no end and single quotes are not JSON, so each gives `invalid-json`.

## Pitfalls

- **A property in `properties` is optional.** The OWASP Input Validation Cheat Sheet says that "listing a property alone neither requires it nor rejects unknown fields"[^13]. Name each member that must exist in `required`. To reject other members, set `additionalProperties` to `false`. The cases `absent-property` and `no-additional` show both rules.

- **A misspelled keyword is not an error.** A validator ignores a keyword that it does not know[^1], so `{"minimun": 5}` accepts every value. Test each schema with an instance that must fail.

- **`format` does not validate by default.** In JSON Schema 2020-12, the value of `format` is an annotation unless the schema asks for the Format-Assertion vocabulary[^2]. The jsonschema library for Python says that, with no format checker, "no format validation is done"[^5]. Give a format checker to the validator, or check the format in the program.

- **A Python bool is an int.** A check for `integer` with `isinstance` accepts `true` and rejects `1.0`:

```python wrong id=python-bool-int
import json

for text in ["true", "1", "1.0", "1.5"]:
    value = json.loads(text)
    print(text, type(value).__name__, isinstance(value, int))
```

```output
true bool True
1 int True
1.0 float False
1.5 float False
```

Test for `bool` first, and accept a `float` when `is_integer()` returns `True`.

- **JavaScript types and lengths differ from JSON Schema.** `length` counts UTF-16 code units, `typeof` gives `"object"` for `null` and for an array, and `in` finds inherited names:

```javascript run id=js-types
const s = "\u{1F600}";
console.log(s.length, [...s].length);
console.log(typeof null, typeof [], Array.isArray([]));
const value = JSON.parse("{}");
console.log("toString" in value, Object.hasOwn(value, "toString"));
```

```output
2 1
object object true
true false
```

- **Validate the data where it enters the program.** OWASP says: "Reject invalid requests with a clear error; do not continue with partially validated data"[^13]. Validation after parsing does not protect the parser: "A schema check after parsing cannot protect a parser that has already exhausted resources"[^13]. Limit the size of the input before you parse it.

## Related tasks

- [[json-schema]]: the format, its versions and its libraries.
- [[read-and-write-json]]: parse a JSON text and get a value at a path.
- [[json-pointer]]: the syntax of the two locations.
- [[deserialization-of-untrusted-data]]: parse untrusted data into values, not objects.
- [[utf-8]]: the bytes of the strings whose code points the length counts.
- [[sort-a-list-of-records]]: sort records by two keys.

## References
