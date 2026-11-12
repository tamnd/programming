---
tag: X93KWZ
kind: format
slug: yaml
name: YAML
aliases: [YAML Ain't Markup Language]
area: data-and-persistence/configuration-formats
level: core
summary: >-
  YAML is a text format for data that uses indentation to show structure. Its nodes are scalars, sequences and mappings. A schema decides if an unquoted scalar is a string, a number, a boolean or null.
history:
  year: 2001
  people: [Clark Evans, Oren Ben-Kiki, Ingy döt Net]
  note: The first YAML framework was written in Perl in 2001. Clark Evans, Oren Ben-Kiki and Ingy döt Net published YAML 1.0 in early 2004, after three years of design work on the yaml-core mailing list.
same_as:
  wikidata: Q281876
  wikipedia: YAML
sources:
  - url: https://yaml.org/spec/1.2.2/
    title: "O. Ben-Kiki, C. Evans, I. döt Net, YAML Ain't Markup Language (YAML) revision 1.2.2, 2021-10-01"
    locator: the abstract; 1.2 YAML History; 3.1 Processes; 3.2.1.1 Nodes; 3.2.1.2 Tags; 3.2.1.3 Node Comparison; 3.2.2.1 Mapping Key Order; 3.2.2.2 Anchors and Aliases; 3.3 Loading Failure Points; 3.3.2 Resolved Tags; 5.2 Character Encodings; 6.1 Indentation Spaces; 6.6 Comments; 6.8.1 "YAML" Directives; 7.3.3 Plain Style; 8.1.1.2 Block Chomping Indicator; 9.1.2 Document Markers; 10.1 Failsafe Schema; 10.2 JSON Schema; 10.3 Core Schema; 10.4 Other Schemas
  - url: https://yaml.org/spec/1.2.2/ext/changes/
    title: YAML Specification Changes, revision 1.2.2
    locator: Changes in version 1.2 (revision 1.2.0) (2009-07-21); Changes in version 1.1 (2005-01-18); First published as version 1.0 (2004-01-29)
  - url: https://yaml.org/spec/1.2.1/
    title: YAML Ain't Markup Language (YAML) Version 1.2, revision 1.2.1, 2009-10-01
    locator: 1.3 Relation to JSON
  - url: https://yaml.org/type/bool.html
    title: Boolean Language-Independent Type for YAML Version 1.1, Working Draft 2005-01-18
    locator: Regexp
  - url: https://yaml.org/type/int.html
    title: Integer Language-Independent Type for YAML Version 1.1, Working Draft 2005-02-11
    locator: Definition; Regexp
  - url: https://yaml.org/type/float.html
    title: Floating-Point Language-Independent Type for YAML Version 1.1, Working Draft 2005-01-18
    locator: Regexp
  - url: https://yaml.org/type/null.html
    title: Null Language-Independent Type for YAML Version 1.1, Working Draft 2005-01-18
    locator: Regexp
  - url: https://yaml.org/type/timestamp.html
    title: Timestamp Language-Independent Type for YAML Version 1.1, Working Draft 2005-01-18
    locator: Regexp
  - url: https://yaml.org/type/merge.html
    title: Merge Key Language-Independent Type for YAML Version 1.1, Working Draft 2005-01-18
    locator: Regexp; Definition
  - url: https://www.rfc-editor.org/rfc/rfc9512
    title: "R. Polli, E. Wilde, E. Aro, RFC 9512: YAML Media Type, February 2024"
    locator: the abstract; 3.2 YAML Streams; 3.3 Filename Extension; 3.4 YAML and JSON; 4.1 Arbitrary Code Execution; 4.2 Resource Exhaustion; 4.4 Expressing Booleans
  - url: https://pyyaml.org/wiki/PyYAML
    title: PyYAML, project page
    locator: the overview
  - url: https://github.com/yaml/pyyaml/wiki/PyYAML-yaml.load(input)-Deprecation
    title: PyYAML wiki, PyYAML yaml.load(input) Deprecation
    locator: the first paragraphs and the list of loaders
  - url: https://github.com/yaml/pyyaml/blob/main/CHANGES
    title: PyYAML CHANGES
    locator: 5.1 (2019-03-13), pull 257; 6.0 (2021-10-13), pull 561
  - url: https://codeberg.org/snakeyaml/snakeyaml
    title: SnakeYAML, README
    locator: Overview; SnakeYAML features
  - url: https://codeberg.org/snakeyaml/snakeyaml/wiki/Changes
    title: SnakeYAML wiki, Changes
    locator: 2.0 (2023-02-26), issue 565
  - url: https://codeberg.org/snakeyaml/snakeyaml-engine
    title: SnakeYAML Engine, README
    locator: Overview; API; SnakeYAML Engine features
  - url: https://crates.io/crates/saphyr/0.3.3
    title: saphyr 0.3.3, crates.io
    locator: the description
  - url: https://crates.io/crates/serde-saphyr/2.1.1
    title: serde-saphyr 2.1.1, crates.io
    locator: the description
  - url: https://crates.io/crates/serde_yaml/0.9.34+deprecated
    title: serde_yaml 0.9.34+deprecated, crates.io
    locator: the version name
  - url: https://pypi.org/project/ruamel.yaml/
    title: ruamel.yaml, PyPI
    locator: the summary
  - url: https://www.npmjs.com/package/js-yaml
    title: js-yaml, npm
    locator: the description
  - url: https://www.npmjs.com/package/yaml
    title: yaml, npm
    locator: the description
  - url: https://github.com/yaml/go-yaml
    title: go-yaml, the YAML organization fork of github.com/go-yaml/yaml
    locator: the repository description
  - url: https://github.com/aaubry/YamlDotNet
    title: YamlDotNet
    locator: the repository description
  - url: https://github.com/jbeder/yaml-cpp
    title: yaml-cpp
    locator: the repository description
  - url: https://github.com/yaml/libyaml
    title: LibYAML
    locator: the repository description
  - url: https://github.com/mikefarah/yq
    title: yq
    locator: the repository description
  - url: https://github.com/cloudbase/powershell-yaml
    title: powershell-yaml
    locator: the repository description
---

## What it is

YAML is a text format for data. The specification calls it a "human-friendly, cross language, Unicode based data serialization language designed around the common native data types of dynamic programming languages".[^1] The specification says that "many large-scale projects use it as their primary interface language".[^1]

A YAML document describes a graph of nodes. There are three kinds of node. A scalar is one value, written as a series of Unicode characters. A sequence is an ordered list of nodes. A mapping is an unordered set of key and value pairs, and each key must be unique.[^1] A key can be any node, not only a string. Each node also has a tag, which names its data type. A global tag is a URI, such as `tag:yaml.org,2002:int`. A local tag starts with `!` and is private to one application.[^1]

The specification defines a load in three stages. The parser turns the characters into a serialization tree. The composer turns the tree into a representation graph, and it resolves aliases at this stage. The constructor turns the graph into the native data of the program. A dump runs the same stages in reverse.[^1] Comments, indentation, quote styles, key order and anchor names are presentation or serialization details. A loader can discard them.[^1] For this reason, most libraries cannot read a file and write it back with its comments in place.

YAML has no fixed set of scalar types in its grammar. The text `10` is a plain scalar. A schema decides if it is the integer ten or the string "10". The specification defines three schemas, and they give different answers for the same text.[^1] Most problems with YAML come from this rule.

### History

The YAML 1.2.2 specification gives the history of the format. Clark Evans and Oren Ben-Kiki had worked on the SML-DEV mailing list, which tried to simplify XML. Ingy döt Net had written a plain text serialization module for Perl. The first YAML framework was written in Perl in 2001, and Ruby shipped a YAML framework in its core distribution in 2003. Evans, Ben-Kiki and döt Net published YAML 1.0 in early 2004, after three years of design work on the yaml-core mailing list.[^1] The changes page gives the date of 1.0 as 2004-01-29.[^2]

YAML 1.1 followed in 2005. Its main change was a different tag syntax, with the `%TAG` directive and verbatim tags such as `!<baz>`.[^2] A set of type drafts on yaml.org defined the scalar types for 1.1, such as booleans, integers, floats, null, timestamps and the merge key.[^4][^5][^9] At about the same time, the authors learned of JSON. The specification says that "by sheer coincidence, JSON was almost a complete subset of YAML". In 2006, Kyrylo Simonov wrote PyYAML and LibYAML. Many YAML libraries in other languages are built on LibYAML.[^1]

YAML 1.2 came out on 2009-07-21.[^2] The specification says that its "primary focus was making YAML a strict superset of JSON", and that it "removed many of the problematic implicit typing recommendations".[^1] Revision 1.2.1 says that every JSON file is also a valid YAML file. It also points to one difference: JSON says that the keys of an object SHOULD be unique, and YAML says that they MUST be.[^3] A design team began to meet in 2020. It published revision 1.2.2 in October 2021, with no normative changes to 1.2.[^1] In February 2024, RFC 9512 registered the media type `application/yaml` and the suffix `+yaml`.[^10]

## Syntax or structure

The document below shows the main parts of the syntax. Indentation shows the structure of block collections. A `- ` starts a sequence entry, and `key: value` is a mapping entry. A `#` starts a comment.

```text
%YAML 1.2
---
name: web               # a plain scalar
replicas: 3
ports: [80, 443]        # a flow sequence
labels: {tier: front}   # a flow mapping
owner: "Hà Nội Café"    # a double-quoted scalar
path: 'C:\temp'         # a single-quoted scalar, no escapes
defaults: &base
  timeout: 30
  retries: 2
job:
  settings: *base       # an alias to the node with anchor base
script: |
  make
  make test
summary: >-
  These two lines fold
  into one line.
...
```

The structure follows these rules from the specification:

- **Indentation.** Indentation is made of space characters only. The specification says that "tab characters must not be used in indentation".[^1] A tab in the indentation is an error, not a different level.
- **Documents.** A stream holds zero or more documents. The line `---` marks the end of the directives and the start of a document. The line `...` marks the end of a document.[^1] Documents in a stream are independent of each other.[^2]
- **Directives.** The line `%YAML 1.2` says which version the document follows. A 1.2 processor must accept a document with `%YAML 1.1`. It should process it as a 1.2 document and give a warning where the two versions differ.[^1] Under the specification, the directive does not bring back the type rules of 1.1.
- **Flow style.** Brackets and braces give a sequence or a mapping on one line, with commas between the entries. This style is close to JSON.
- **Scalar styles.** A plain scalar has no quotes and no escapes. It must not contain `: ` or ` #`, because these start a value and a comment.[^1] A single-quoted scalar has no escapes except `''` for one quote. A double-quoted scalar has escapes such as `\n`, `\t` and `\u00e9`.
- **Block scalars.** After `|`, the text keeps its line breaks. After `>`, single line breaks fold into spaces. A chomping indicator controls the final line break. With `-` (strip), the scalar has no final line break. With no indicator (clip), it keeps one line break. With `+` (keep), it keeps the final line break and the trailing empty lines.[^1]
- **Comments.** A comment starts with `#`, and white space must separate it from other tokens.[^1] So `a#b` is one plain scalar.
- **Anchors and aliases.** `&name` puts an anchor on a node, and `*name` refers back to that node. An alias refers to the last node before it with that anchor, so anchors need not be unique.[^1]
- **Tags.** `!!int 10` gives the node the tag `tag:yaml.org,2002:int`. A tag such as `!invoice` is local to an application.[^1]
- **Encoding.** On input, a processor must support UTF-8 and UTF-16, and also UTF-32 for JSON compatibility. A byte order mark at the start sets the encoding.[^1] RFC 9512 prefers the file extension `.yaml` and says that `.yml` is still used.[^10]

### Schemas and tag resolution

A plain scalar starts with the non-specific tag `?`. The processor resolves it to a real tag. The specification lets the result depend on the path to the node and on its content.[^1] Chapter 10 recommends three schemas:

- The **failsafe schema** has only the tags `!!map`, `!!seq` and `!!str`. Every scalar is a string.[^1]
- The **JSON schema** adds `!!null`, `!!bool`, `!!int` and `!!float`, with the exact spelling of JSON. A plain scalar that matches none of its patterns is an error.[^1]
- The **core schema** is "the recommended default schema". It accepts more spellings, and a plain scalar that matches nothing is a string.[^1]

The patterns of the core schema are below, copied from section 10.3.2.[^1]

```text
null | Null | NULL | ~ | (empty)              -> tag:yaml.org,2002:null
true | True | TRUE | false | False | FALSE    -> tag:yaml.org,2002:bool
[-+]? [0-9]+                                  -> tag:yaml.org,2002:int (base 10)
0o [0-7]+                                     -> tag:yaml.org,2002:int (base 8)
0x [0-9a-fA-F]+                               -> tag:yaml.org,2002:int (base 16)
[-+]? ( \. [0-9]+ | [0-9]+ ( \. [0-9]* )? ) ( [eE] [-+]? [0-9]+ )?
                                              -> tag:yaml.org,2002:float
[-+]? ( \.inf | \.Inf | \.INF )               -> tag:yaml.org,2002:float (infinity)
\.nan | \.NaN | \.NAN                         -> tag:yaml.org,2002:float (not a number)
anything else                                 -> tag:yaml.org,2002:str
```

The 1.1 type drafts used wider patterns. A boolean could be `y`, `yes`, `on`, `n`, `no` or `off`, in three cases.[^4] An integer could be binary with `0b`, octal with a leading `0`, or base 60 with colons, as in `12:30`. The drafts say that `_` characters in a number are ignored.[^5] A float could also be base 60.[^6] A string such as `2026-11-10` was a timestamp.[^8] The key `<<` merged other mappings into the current one.[^9] YAML 1.2 dropped all of these rules.[^2] Many libraries still follow 1.1. PyYAML calls itself "a complete YAML 1.1 parser",[^11] and SnakeYAML calls itself "a YAML 1.1 processor".[^14]

## Examples

The program below is not a YAML parser. It applies the resolution patterns of three schemas to a list of plain scalars. The patterns for YAML 1.1 come from the type drafts on yaml.org,[^4][^5][^6][^7][^8] and the patterns for 1.2 come from sections 10.2.2 and 10.3.2 of the specification.[^1] The program leaves out the base 60 form of the 1.1 float. Python has no YAML module in its standard library, so the program uses only `re`.

```python run id=resolve
import re
from datetime import date


def int_1_1(text):
    text = text.replace("_", "")
    sign = -1 if text.startswith("-") else 1
    text = text.lstrip("+-")
    if ":" in text:
        value = 0
        for part in text.split(":"):
            value = value * 60 + int(part)
    elif text.startswith("0b"):
        value = int(text[2:], 2)
    elif text.startswith("0x"):
        value = int(text[2:], 16)
    elif text.startswith("0") and len(text) > 1:
        value = int(text, 8)
    else:
        value = int(text)
    return sign * value


def float_text(text):
    special = text.lower().replace(".inf", "inf").replace(".nan", "nan")
    return float(special.replace("_", ""))


YAML_1_1 = [
    (r"~|null|Null|NULL|", lambda text: None),
    (r"y|Y|yes|Yes|YES|n|N|no|No|NO|true|True|TRUE|false|False|FALSE|on|On|ON|off|Off|OFF",
     lambda text: text.lower() in ("y", "yes", "true", "on")),
    (r"[-+]?0b[0-1_]+|[-+]?0[0-7_]+|[-+]?(0|[1-9][0-9_]*)|[-+]?0x[0-9a-fA-F_]+|[-+]?[1-9][0-9_]*(:[0-5]?[0-9])+",
     int_1_1),
    (r"[-+]?([0-9][0-9_]*)?\.[0-9.]*([eE][-+][0-9]+)?|[-+]?\.(inf|Inf|INF)|\.(nan|NaN|NAN)", float_text),
    (r"[0-9][0-9][0-9][0-9]-[0-9][0-9]-[0-9][0-9]", date.fromisoformat),
]

CORE = [
    (r"null|Null|NULL|~|", lambda text: None),
    (r"true|True|TRUE|false|False|FALSE", lambda text: text.lower() == "true"),
    (r"[-+]?[0-9]+", int),
    (r"0o[0-7]+", lambda text: int(text[2:], 8)),
    (r"0x[0-9a-fA-F]+", lambda text: int(text[2:], 16)),
    (r"[-+]?(\.[0-9]+|[0-9]+(\.[0-9]*)?)([eE][-+]?[0-9]+)?|[-+]?(\.inf|\.Inf|\.INF)|\.nan|\.NaN|\.NAN", float_text),
]

JSON = [
    (r"null", lambda text: None),
    (r"true|false", lambda text: text == "true"),
    (r"-?(0|[1-9][0-9]*)", int),
    (r"-?(0|[1-9][0-9]*)(\.[0-9]*)?([eE][-+]?[0-9]+)?", float),
]


def resolve(schema, text, otherwise):
    for pattern, convert in schema:
        if re.fullmatch(pattern, text):
            value = convert(text)
            return repr(value) if isinstance(value, str) else str(value)
    return otherwise(text)


print(f"{'plain scalar':14}{'YAML 1.1':14}{'1.2 core':14}1.2 JSON")
for text in ["NO", "on", "010", "0o10", "0x1F", "1_000", "12:30", "1.10", ".inf", "~", "True", "2026-11-10", ""]:
    row = [resolve(YAML_1_1, text, repr), resolve(CORE, text, repr), resolve(JSON, text, lambda text: "error")]
    print(f"{text!r:14}{row[0]:14}{row[1]:14}{row[2]}")
```

```output for=resolve
plain scalar  YAML 1.1      1.2 core      1.2 JSON
'NO'          False         'NO'          error
'on'          True          'on'          error
'010'         8             10            error
'0o10'        '0o10'        8             error
'0x1F'        31            31            error
'1_000'       1000          '1_000'       error
'12:30'       750           '12:30'       error
'1.10'        1.1           1.1           1.1
'.inf'        inf           inf           error
'~'           None          None          error
'True'        True          True          error
'2026-11-10'  2026-11-10    '2026-11-10'  error
''            None          None          error
```

The country code of Norway, `NO`, is the boolean false under 1.1 and a string under the core schema. The text `010` is 8 under 1.1 and 10 under the core schema. The time `12:30` is the integer 750 under 1.1. The version `1.10` is the float 1.1 under every schema, so the trailing zero is lost. Under the JSON schema, most of these plain scalars are errors, because JSON has no such spellings. A program that reads a file under one schema and writes it for a reader with another schema can change the data with no error.

An alias makes a second reference to a node. A loader that builds a tree, not a graph, copies the node once for each alias. The program below writes a document of nine levels. Each level is a sequence of nine aliases to the level above. RFC 9512 shows the same attack with two items on each level, and calls it a "Billion Laughs Attack".[^10]

```python run id=alias-expansion
levels, width = 9, 9
lines = ["a0: &a0 [" + ", ".join(["lol"] * width) + "]"]
for level in range(1, levels):
    aliases = ", ".join([f"*a{level - 1}"] * width)
    lines.append(f"a{level}: &a{level} [{aliases}]")
text = "\n".join(lines) + "\n"

print(lines[1])
print(len(text.encode("utf-8")), "bytes,", text.count("*"), "aliases")
scalars = sum(width ** (level + 1) for level in range(levels))
print(f"{scalars:,} scalars if a loader copies every alias")
```

```output for=alias-expansion
a1: &a1 [*a0, *a0, *a0, *a0, *a0, *a0, *a0, *a0, *a0]
486 bytes, 72 aliases
435,848,049 scalars if a loader copies every alias
```

The example files use real YAML libraries. The Rust file uses saphyr and serde-saphyr, which follow YAML 1.2. The Java file compares SnakeYAML, which follows 1.1, with SnakeYAML Engine, which follows 1.2. Both files show the scalar types, repeated keys and the limits on aliases.

## Versions

| Version | Date | Main changes |
|---|---|---|
| 1.0 | 2004-01-29 | the first published version[^2] |
| 1.1 | 2005-01-18 | the `%TAG` directive; `!!str` became a default tag and `!bar` a local tag; verbatim tags `!<baz>`[^2] |
| 1.2 (revision 1.2.0) | 2009-07-21 | a valid superset of JSON; the core schema replaced the 1.1 type library[^2] |
| 1.2.1 | 2009-10-01 | corrections to 1.2[^2] |
| 1.2.2 | 2021-10-01 | no normative changes; errata, clearer wording and examples[^1][^2] |

The changes from 1.1 to 1.2 that affect data are these:[^2]

- Only `true` and `false`, also as `True` and `TRUE`, are booleans. The words `y`, `yes`, `on` and their negatives are strings.
- `_` cannot be used in a number.
- An octal number needs the prefix `0o`, so `010` is 10 and not 8.
- The binary and base 60 integer forms are dropped.
- The types `!!pairs`, `!!omap`, `!!set`, `!!timestamp` and `!!binary` are dropped.
- The merge key `<<` and the value key `=` are removed.
- The characters U+0085, U+2028 and U+2029 are no longer line breaks.
- In a double-quoted scalar, `\/` is a valid escape.
- In flow style, a quoted key needs no space after the colon, as in `{"key":value}`.
- Documents in a stream no longer inherit the directives of the documents before them.

Many libraries did not adopt the 1.2 type rules. A library can follow the 1.2 grammar and still resolve plain scalars with the 1.1 patterns. Read the documentation of each library to learn its schema, and test it with the scalars in the example above.

## Pitfalls

- **Unquoted words that become booleans.** Under 1.1, `NO`, `n`, `off` and `y` are booleans. RFC 9512 gives the example `{insecure: n}`, which a 1.1 reader takes as false and a 1.2 reader as the string "n".[^10] Quote every string that a 1.1 reader can take as another type.
- **Numbers that change.** Under 1.1, `010` is 8, `1_000` is 1000 and `12:30` is 750.[^5] Under the core schema, they are 10, a string and a string.[^1] Version numbers such as `1.10` become the float 1.1 under both. Quote version numbers, times, postal codes and phone numbers.
- **Writers that do not quote for 1.1.** In the examples, saphyr and SnakeYAML Engine write the string "1_000" with no quotes. A 1.1 reader takes that text as the integer 1000. SnakeYAML, a 1.1 writer, puts quotes around it. Check which reader will consume the output.
- **Dates.** Under 1.1, `2026-11-10` is a timestamp.[^8] In the Java example, SnakeYAML builds a `java.util.Date` from it, and SnakeYAML Engine keeps a string. A program that expects a string gets another type.
- **Repeated keys.** The specification says that keys must be unique, and lists a repeated key as a failure point of a load.[^1] Some libraries keep the last value with no error. In the examples, SnakeYAML keeps the last value by default, and saphyr, serde-saphyr and SnakeYAML Engine reject the document. Two equal keys can have different text. The specification gives `0o13` and `0xB` as the same integer key.[^1]
- **Tags that run code.** A tag can ask the loader to build an object of any class. RFC 9512 says that code execution in a deserializer should be off by default.[^10] Before PyYAML 5.1, `yaml.load` could call any Python function, such as `os.system`, through the tag `!!python/object/new`.[^12] PyYAML 5.1 deprecated `yaml.load` without a loader, and PyYAML 6.0 made the `Loader` argument required.[^13] Use `yaml.safe_load` for input from outside. SnakeYAML 2.0 stopped global tags by default, to fix CVE-2022-1471.[^15] The SnakeYAML Engine README says that the Engine builds no custom objects, so parsing any document is safe unless the program enables custom objects.[^16]
- **Alias expansion.** A small document with nested aliases can expand to millions of nodes, as the example shows. A document can also contain a cycle, such as `x: &x {y: *x}`. A program that walks the graph as a tree can loop forever.[^10] Use a library with limits on aliases and nodes, and set the limits for input from outside.
- **Tabs and indentation.** A tab in the indentation is an error.[^1] One space too many or too few can move a key to another mapping with no error.
- **Comments and formatting are lost.** A load discards comments, key order and quote styles.[^1] A program that loads a file, changes one value and dumps it, rewrites the whole file. Libraries such as ruamel.yaml keep comments and formatting on a round trip.[^20]
- **Plain scalars with special characters.** A plain scalar cannot contain `: ` or ` #`.[^1] The text `title: Note: read this` is an error, and `url: a #b` loses `#b` as a comment. A plain scalar also cannot start with most indicator characters, such as `*`, `&`, `!`, `%`, `@` or a backquote.[^1] Quote such values.
- **The merge key.** The key `<<` is part of the 1.1 type drafts and not part of 1.2.[^9][^2] Under the core schema, `<<` matches no pattern, so it resolves to a string. A file that uses merge keys depends on a 1.1 feature of its library.
- **More than one document.** A stream can hold more than one document. RFC 9512 says that an application that expects one document should signal an error when it gets more.[^10]

## Tools and libraries

| Language | Verdict | Libraries |
|---|---|---|
| Python | library | PyYAML (YAML 1.1),[^11] ruamel.yaml[^20] |
| JavaScript, TypeScript | library | js-yaml,[^21] yaml[^22] |
| Go | library | go-yaml from the YAML organization[^23] |
| C# | library | YamlDotNet[^24] |
| Rust | library | saphyr,[^17] serde-saphyr;[^18] serde_yaml is deprecated[^19] |
| Java, Kotlin | library | SnakeYAML (YAML 1.1),[^14] SnakeYAML Engine (YAML 1.2)[^16] |
| C++ | library | yaml-cpp[^25] |
| C | library | LibYAML[^26] |
| Bash | library | the command-line tool yq[^27] |
| PowerShell | library | the module powershell-yaml[^28] |

None of the tier 1 languages has YAML in its standard library. The YAML organization keeps LibYAML, and PyYAML can use it for speed.[^26][^11] The tool yq reads YAML, JSON and other formats, and applies a query to them.[^27]

To pick a library, check four points. First, check the schema it uses for plain scalars, 1.1 or 1.2. Second, check what it does with repeated keys. Third, check that it builds no objects from tags unless the program asks it to. Fourth, check that it limits aliases, depth and size.

## References
