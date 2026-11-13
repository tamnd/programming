---
tag: 9JQ7BS
kind: standard
slug: jsonpath
name: JSONPath
aliases: [RFC 9535, JSON Path]
area: data-and-persistence/data-formats
level: advanced
summary: >-
  JSONPath (RFC 9535) is a query language for JSON. A query selects zero or more values from a document with names, indexes, slices, wildcards, descendant segments and filters.
see_also: [json, json-pointer, json-patch]
sources:
- url: https://www.rfc-editor.org/rfc/rfc9535
  title: "S. Gössner, G. Normington, C. Bormann (Ed.), RFC 9535: JSONPath: Query Expressions for JSON, February 2024"
  locator: 1.2 History; 1.4 Overview of JSONPath Expressions; 2.1 Overview; 2.3 Selectors; 2.4 Function Extensions; 2.5.2 Descendant Segment; 2.6 Semantics of null; 2.7 Normalized Paths; 3.1 Registration of Media Type application/jsonpath; 4 Security Considerations; Appendix C JSON Pointer
- url: https://goessner.net/articles/JsonPath/
  title: "S. Gössner, JSONPath - XPath for JSON, 2007-02-21"
  locator: the syntax table and the examples
- url: https://www.rfc-editor.org/rfc/rfc6901
  title: "P. Bryan (Ed.), K. Zyp, M. Nottingham (Ed.), RFC 6901: JavaScript Object Notation (JSON) Pointer, April 2013"
  locator: 3 Syntax; 4 Evaluation
- url: https://cburgmer.github.io/json-path-comparison/
  title: C. Burgmer, JSONPath Comparison
  locator: Explanation; the results table
- url: https://jmespath.org/specification.html
  title: JMESPath Specification
  locator: Grammar; Projections; Filter Expressions
- url: https://jqlang.org/manual/
  title: jq manual
  locator: path(path_expression); getpath(PATHS)
- url: https://kubernetes.io/docs/reference/kubectl/jsonpath/
  title: Kubernetes documentation, JSONPath Support
  locator: the functions table; Regular expressions in JSONPath
- url: https://www.postgresql.org/docs/17/functions-json.html
  title: PostgreSQL 17 documentation, 9.16 JSON Functions and Operators
  locator: 9.16.2 The SQL/JSON Path Language; jsonb_path_query
- url: https://docs.rs/serde_json_path/latest/serde_json_path/
  title: serde_json_path documentation, crate serde_json_path
  locator: the crate description; Features
- url: https://learn.microsoft.com/en-us/dotnet/api/system.text.json.nodes.jsonnode.getpath
  title: .NET API documentation, JsonNode.GetPath Method
  locator: Returns
---

## What it is

JSONPath is a query language for JSON. RFC 9535 says that it "defines a string syntax for selecting and extracting JSON (RFC 8259) values from within a given JSON value"[^1]. The query `$.store.book[*].author` means: from the root, take the member `store`, then the member `book`, then every element of that array, then the member `author` of each element.

The result of a query is a nodelist, a list of nodes. "Nodes are the selectable parts of the query argument"[^1]. A query can select zero, one or many nodes. An empty nodelist is a valid result, so a query that matches nothing is not an error.

Stefan Gössner proposed JSONPath in an article dated 2007-02-21, as an XPath for JSON[^2]. Many libraries implemented the proposal, but they did not agree on the details[^1][^4]. RFC 9535 is "based on Stefan Gössner's popular JSONPath proposal", and it makes a normative specification from the experience with these implementations[^1].

JSONPath and JSON Pointer do different things. The RFC says that JSONPath "is not intended as a replacement but as a more powerful companion" of JSON Pointer[^1]. A pointer such as `/store/book/0` identifies one value[^3]. A JSONPath query can also search for many values.

## Syntax or structure

"A JSONPath expression consists of an identifier followed by a series of zero or more segments, each of which contains one or more selectors"[^1]. The identifier `$` is the root node. The identifier `@` is the current node, and it is valid only inside a filter. The main parts are these[^1]:

| Syntax | Meaning |
|---|---|
| `$` | the root node |
| `[<selectors>]` | a child segment: it applies the selectors to each input node |
| `..[<selectors>]` | a descendant segment: it applies the selectors to each input node and to all its descendants |
| `.name`, `.*`, `..name`, `..*` | the short forms of `['name']`, `[*]`, `..['name']` and `..[*]` |
| `'name'` | a name selector: the member with that name |
| `*` | a wildcard selector: all children |
| `3`, `-1` | an index selector: an array element; a negative index counts from the end |
| `0:100:5` | an array slice selector: start, end and step |
| `?<logical-expr>` | a filter selector: the children for which the expression is true |
| `length(@.foo)` | a function expression in a filter |

Bracket notation and dot notation give the same query, so `$['store']['book'][0]['title']` and `$.store.book[0].title` are equal[^1]. One bracket can hold more than one selector, separated by commas, as in `$..book[0,1]`.

A filter can use the comparison operators `==`, `!=`, `<`, `<=`, `>` and `>=`, the logical operators `&&`, `||` and `!`, parentheses, and function expressions[^1]. A query by itself in a filter is an existence test: `$..book[?@.isbn]` selects the books that have a member `isbn`. RFC 9535 defines five functions: `length`, `count`, `match`, `search` and `value`. The functions `match` and `search` use the regular expression format I-Regexp of RFC 9485[^1].

A query that does not follow the grammar is an error before it runs. "A JSONPath implementation MUST raise an error for any query that is not well-formed and valid"[^1]. A valid query does not cause errors when it runs. An index past the end of an array selects nothing.

Each node in a value has exactly one Normalized Path. A Normalized Path is a query that uses only bracket notation, names in single quotes and non-negative indexes, such as `$['book'][3]`[^1]. An API can give the result of a query as the values, as the Normalized Paths of the nodes, or as both[^1].

## Examples

The program below is a small evaluator for a subset of RFC 9535. It handles names, wildcards, indexes, slices, descendant segments and simple filters, and it gives each result with its Normalized Path. The document and most queries are the examples of the RFC[^1]. The last line shows the escapes of a member name in a Normalized Path.

```python run id=evaluate
import json
import operator
import re

# A subset of RFC 9535: $, .name, ..name, .*, [*], ['name'], [index], [start:end:step],
# and filters of the form [?@.name] and [?@.name<op><number or 'string'>].
TOKEN = re.compile(r"""\.\.|\.|\[\?@\.(\w+)\s*(==|!=|<=|>=|<|>)?\s*(-?\d+(?:\.\d+)?|'[^']*')?\]|\['([^']*)'\]|\[(-?\d*):(-?\d*)(?::(-?\d+))?\]|\[(-?\d+)\]|\[\*\]|\*|\w+""")
ESCAPES = {"\b": "\\b", "\f": "\\f", "\n": "\\n", "\r": "\\r", "\t": "\\t", "'": "\\'", "\\": "\\\\"}
OPS = {"==": operator.eq, "!=": operator.ne, "<": operator.lt, "<=": operator.le, ">": operator.gt, ">=": operator.ge}


def parse(query):
    """Returns a list of (descendant, selector) pairs."""
    if not query.startswith("$"):
        raise ValueError("a query starts with $")
    steps, pos, desc = [], 1, False
    while pos < len(query):
        m = TOKEN.match(query, pos)
        if not m:
            raise ValueError(f"bad query at {pos}: {query[pos:]!r}")
        text, pos = m.group(0), m.end()
        if text in (".", ".."):
            desc = text == ".."
            continue
        steps.append((desc, m))
        desc = False
    return steps


def normal(name):
    """Writes a member name as in a Normalized Path (RFC 9535, section 2.7)."""
    text = "".join(ESCAPES.get(c, f"\\u{ord(c):04x}" if ord(c) < 0x20 else c) for c in name)
    return f"'{text}'"


def kind(x):
    if isinstance(x, str):
        return "string"
    if isinstance(x, (int, float)) and not isinstance(x, bool):
        return "number"
    return "other"


def test(item, name, op, lit):
    """The filter test for one child: an existence test, or a comparison with a literal."""
    if op is None:
        return isinstance(item, dict) and name in item
    want = lit[1:-1] if lit.startswith("'") else float(lit)
    if not isinstance(item, dict) or name not in item:
        return op == "!="  # An empty result is equal only to another empty result.
    got = item[name]
    if kind(got) == kind(want):
        return OPS[op](got, want)
    return op == "!="  # Values of different types are never equal or ordered.


def children(value, path, m):
    """Applies one selector to one node. Yields (path, value) pairs."""
    if isinstance(value, dict):
        items = [(f"{path}[{normal(k)}]", v) for k, v in value.items()]
    elif isinstance(value, list):
        items = [(f"{path}[{i}]", v) for i, v in enumerate(value)]
    else:
        return
    text = m.group(0)
    if text in ("*", "[*]"):
        yield from items
    elif m.group(1):
        yield from ((p, v) for p, v in items if test(v, *m.group(1, 2, 3)))
    elif isinstance(value, list):
        if m.group(8) is not None:
            i = int(m.group(8))
            i = i + len(value) if i < 0 else i
            if 0 <= i < len(value):
                yield items[i]
        elif m.group(5) is not None:
            start, end, step = (int(g) if g else None for g in m.group(5, 6, 7))
            if step != 0:
                yield from (items[i] for i in range(len(value))[slice(start, end, step)])
    else:
        name = m.group(4) if m.group(4) is not None else text
        if name in value:
            yield f"{path}[{normal(name)}]", value[name]


def descendants(path, value):
    """The node, then its descendants, arrays in order."""
    yield path, value
    if isinstance(value, dict):
        for k, v in value.items():
            yield from descendants(f"{path}[{normal(k)}]", v)
    elif isinstance(value, list):
        for i, v in enumerate(value):
            yield from descendants(f"{path}[{i}]", v)


def query(doc, q):
    nodes = [("$", doc)]
    for desc, m in parse(q):
        inputs = [d for p, v in nodes for d in descendants(p, v)] if desc else nodes
        nodes = [c for p, v in inputs for c in children(v, p, m)]
    return nodes


store = json.loads("""{"store": {"book": [
  {"category": "reference", "author": "Nigel Rees", "title": "Sayings of the Century", "price": 8.95},
  {"category": "fiction", "author": "Evelyn Waugh", "title": "Sword of Honour", "price": 12.99},
  {"category": "fiction", "author": "Herman Melville", "title": "Moby Dick", "isbn": "0-553-21311-3", "price": 8.99},
  {"category": "fiction", "author": "J. R. R. Tolkien", "title": "The Lord of the Rings", "isbn": "0-395-19395-8", "price": 22.99}],
  "bicycle": {"color": "red", "price": 399}}}""")

for q in ["$.store.book[*].author", "$..price", "$..book[-1].title", "$..book[:2].title",
          "$..book[?@.isbn].title", "$..book[?@.price<10].title", "$..book[?@.isbn!='0-553-21311-3'].title",
          "$..book[2].publisher"]:
    print(q)
    for path, value in query(store, q):
        print("   ", path, json.dumps(value))
print(normal("it's"), normal("a\\b"), normal("tab\there"), normal("\x0b"))
```

```output for=evaluate
$.store.book[*].author
    $['store']['book'][0]['author'] "Nigel Rees"
    $['store']['book'][1]['author'] "Evelyn Waugh"
    $['store']['book'][2]['author'] "Herman Melville"
    $['store']['book'][3]['author'] "J. R. R. Tolkien"
$..price
    $['store']['book'][0]['price'] 8.95
    $['store']['book'][1]['price'] 12.99
    $['store']['book'][2]['price'] 8.99
    $['store']['book'][3]['price'] 22.99
    $['store']['bicycle']['price'] 399
$..book[-1].title
    $['store']['book'][3]['title'] "The Lord of the Rings"
$..book[:2].title
    $['store']['book'][0]['title'] "Sayings of the Century"
    $['store']['book'][1]['title'] "Sword of Honour"
$..book[?@.isbn].title
    $['store']['book'][2]['title'] "Moby Dick"
    $['store']['book'][3]['title'] "The Lord of the Rings"
$..book[?@.price<10].title
    $['store']['book'][0]['title'] "Sayings of the Century"
    $['store']['book'][2]['title'] "Moby Dick"
$..book[?@.isbn!='0-553-21311-3'].title
    $['store']['book'][0]['title'] "Sayings of the Century"
    $['store']['book'][1]['title'] "Sword of Honour"
    $['store']['book'][3]['title'] "The Lord of the Rings"
$..book[2].publisher
'it\'s' 'a\\b' 'tab\there' '\u000b'
```

The output shows the rules of the RFC:

- `$..price` finds the prices of the books and of the bicycle. A descendant segment visits each node before its descendants, and the elements of an array in order[^1].
- In the filter `?@.price<10`, the operator `<` compares two numbers or two strings. A string and a number are never less than each other[^1].
- In `!=` with `@.isbn`, a book with no `isbn` is a match. A comparison with an empty nodelist is `==` only to another empty nodelist, and `!=` is the negation of `==`[^1].
- `$..book[2].publisher` gives an empty nodelist, not an error.

The example files of this page show other parts of the standard:

- **JavaScript.** The array slice selector, with the normative steps of the RFC and the examples of section 2.3.4.3.
- **Go.** A conversion from a Normalized Path to a JSON Pointer.
- **C#.** A walk over all the descendants of a `JsonNode` tree, with the path strings of `JsonNode.GetPath`. These strings are not Normalized Paths.

## Versions

- **2007.** Gössner published the proposal. It used `?(...)` for filters and `(...)` for script expressions that "the underlying script engine" evaluates, for example `$..book[?(@.price<10)]`[^2].
- **2007 to 2024.** Libraries in many languages implemented the proposal, each with its own details. JSONPath "delegated parts of a query to the underlying runtime", so "JSONPath expressions became decreasingly portable"[^1]. The JSONPath Comparison project runs the same queries on many implementations and marks each result that "does not match the consensus"[^4].
- **February 2024.** RFC 9535 was published as an IETF Standards Track document[^1]. It registers the media type `application/jsonpath` for a query[^1]. A filter is now written `?@.price<10` without parentheses, and there are no script expressions.

## Pitfalls

- **Many dialects have the same name.** An old library, a JSONPath in a tool and RFC 9535 can give different results for the same query[^4]. Before you move a query from one tool to another, check which dialect each tool uses.
- **A filter that the host language evaluates is code.** The 2007 proposal handed filter text to the script engine[^2]. The RFC says that this approach "is well known to lead to injection attacks"[^1]. A query that comes from a user must never go to `eval`:

```python wrong id=eval-filter
from types import SimpleNamespace

books = [{"title": "Sayings of the Century", "price": 8.95}, {"title": "Sword of Honour", "price": 12.99}]


def old_filter(items, expr):
    """The style of the 2007 proposal: the host language evaluates the filter text."""
    return [b["title"] for b in items if eval(expr.replace("@.", "x."), {}, {"x": SimpleNamespace(**b)})]


print(old_filter(books, "@.price < 10"))
# The filter text comes from a request parameter. It is code, and the server runs it.
print(old_filter(books, "@.price < 10 or print('this code ran on the server')"))
```

```output for=eval-filter
['Sayings of the Century']
this code ran on the server
['Sayings of the Century']
```

- **Queries made from strings.** A query built from a variable, such as a member name from a request, can change its meaning. The RFC says that these variables "need to be validated" and "translated (e.g., by escaping string delimiters)"[^1].
- **Expensive queries.** An attacker can send a query that causes "possibly exponential, CPU usage" or, with a recursive descendant segment, a stack overflow. The RFC says that implementations "need to have appropriate resource management"[^1].
- **Object order is not defined.** For a wildcard or a descendant segment, "The order in which the children of an object appear in the resultant nodelist is not stipulated"[^1]. Do not depend on the order of the results from an object. Arrays keep their order.
- **Duplicates stay.** "A node may be selected more than once and appears that number of times in the nodelist. Duplicate nodes are not removed"[^1]. Use the Normalized Paths to remove duplicates.
- **Step 0 is not an error.** "When step is 0, no elements are selected", which "differs from the behavior of Python, which raises an error in this case"[^1].
- **null is a value.** "JSON null is treated the same as any other JSON value, i.e., it is not taken to mean "undefined" or "missing""[^1]. The existence test `?@.foo` is true for a member `foo` with the value `null`. To test for null, write `?@.foo == null`.
- **Members, not names.** A query selects member values. "Member names and members (name/value pairs) cannot be selected"[^1]. To get the names, use the Normalized Paths of the results.

## Tools and libraries

- **Other JSON query languages.** JMESPath has its own specification. Its expressions start with a name, not with `$`, and an expression such as `foo[*].bar` makes what the specification calls "a projection"[^5]. jq has its own filter language. Its function `path(path_expression)` gives each path as an array of "object keys" and "array indices", and `getpath` reads the values at such paths[^6].
- **kubectl.** The Kubernetes command-line tool takes JSONPath templates in curly braces, such as `{.items[*].metadata.name}`. It adds the `range` and `end` operators, and it says that "JSONPath regular expressions are not supported"[^7].
- **SQL.** PostgreSQL implements the SQL/JSON path language of the SQL standard, a different language with a similar look. Its filter is written after the path with `?` and parentheses, as in the path `'$.a[*] ? (@ >= $min && @ <= $max)'` in an example of `jsonb_path_query`[^8].
- **Rust.** The crate `serde_json_path` runs queries on a `serde_json::Value`, and it "intends to adhere to the IETF JSONPath standard ( RFC 9535 )"[^9].
- **.NET.** `System.Text.Json` has no query function. `JsonNode.GetPath` returns "The JSON Path value" of a node, as a string for messages[^10].
- **Python, JavaScript, Java and Go.** The standard libraries have no JSONPath. Libraries exist for each language. The JSONPath Comparison lists many of them and their results for each query[^4].

The page [[json-pointer]] covers the syntax that identifies one value, and [[json-patch]] uses pointers to change a document.

## References
