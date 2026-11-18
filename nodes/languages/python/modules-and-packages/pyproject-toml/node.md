---
tag: NEW-python-pyproject-toml
kind: feature
owner: python
slug: pyproject-toml
name: pyproject.toml
aliases: [pyproject, "[build-system]", "[project]", "[tool]", build-system table, project table, tool table, optional-dependencies, extras, requires-python, project.scripts, dynamic metadata, tomllib]
area: languages/python/modules-and-packages
level: core
summary: >-
  pyproject.toml is the TOML file of a Python project. [build-system] names the build backend, [project] holds the core metadata and [tool] holds the settings of other tools.
grammar: null
spec: {standard: null, url: "https://packaging.python.org/en/latest/specifications/pyproject-toml/", locator: "pyproject.toml specification"}
see_also: [python/managing-packages-with-pip, python/build-backends-and-wheels, python/virtual-environments-and-packages]
sources:
- url: https://packaging.python.org/en/latest/specifications/pyproject-toml/
  title: pyproject.toml specification, Python Packaging User Guide
  locator: "[build-system] table; [project] table; dynamic; [tool] table; History"
  accessed: 2026-11-18
- url: https://packaging.python.org/en/latest/guides/writing-pyproject-toml/
  title: Writing your pyproject.toml, Python Packaging User Guide
  locator: Declaring the build backend; Basic information; Dependencies and requirements; Creating executable scripts
  accessed: 2026-11-18
- url: https://peps.python.org/pep-0518/
  title: "PEP 518: Specifying Minimum Build System Requirements for Python Projects"
  locator: Specification; File Format
  accessed: 2026-11-18
- url: https://peps.python.org/pep-0621/
  title: "PEP 621: Storing project metadata in pyproject.toml"
  locator: Abstract; Motivation
  accessed: 2026-11-18
- url: https://docs.python.org/3.15/library/tomllib.html
  title: tomllib, Parse TOML files
  locator: tomllib.load; tomllib.loads; TOMLDecodeError; Conversion Table; Limits and interoperability considerations
  accessed: 2026-11-18
- url: https://packaging.python.org/en/latest/specifications/core-metadata/
  title: Core metadata specifications, Python Packaging User Guide
  locator: Provides-Extra
  accessed: 2026-11-18
- url: https://doc.rust-lang.org/cargo/reference/manifest.html
  title: The Manifest Format, The Cargo Book
  locator: The Manifest Format
  accessed: 2026-11-18
---

## Synopsis

```toml
[build-system]
requires = ["hatchling >= 1.26"]       # what the build needs
build-backend = "hatchling.build"      # the object that builds

[project]
name = "spam-eggs"                     # required, never dynamic
version = "2.0.1"                      # a string, or listed in dynamic
description = "Lovely Spam! Wonderful Spam!"
requires-python = ">=3.9"
dependencies = ["httpx", "rich>=13"]
dynamic = []                           # keys that the backend fills in

[project.optional-dependencies]       # extras: pip install spam-eggs[cli]
cli = ["click"]

[project.scripts]                      # commands
spam-cli = "spam:main_cli"

[tool.spam]                            # settings of the tool "spam"
color = true
```

## Semantics

**r1.** "The pyproject.toml file acts as a configuration file for packaging-related tools (as well as other tools). The pyproject.toml file is written in TOML."[^1] The packaging guide names the tables: "The [build-system] table is strongly recommended. It allows you to declare which build backend you use and which other dependencies are needed to build your project. The [project] table is the format that most build backends use to specify your project’s basic metadata, such as the dependencies, your name, etc."[^2] The specification adds: "Other tables are reserved for future use (tool-specific configuration should use the [tool] table)."[^1] Example: [`read`](#read).

**r2.** "The [build-system] table declares any Python level dependencies that must be installed in order to run the project’s build system successfully."[^1] "The [build-system] table contains a build-backend key, which specifies the build backend to be used. It also contains a requires key, which is a list of dependencies needed to build the project"[^2]. The page [[python/build-backends-and-wheels]] shows how a frontend such as pip uses the two keys.

**r3.** "The [project] table specifies the project’s core metadata. There are two kinds of metadata: static and dynamic."[^1] "Dynamic metadata is listed via the dynamic key (defined later in this specification) and represents metadata that a tool will later provide."[^1] The key `name` is "required and is the only field that cannot be marked as dynamic."[^2] The key `version` is required, and the file can give it or list it in `dynamic`.[^1] Example: [`read`](#read).

**r4.** Each key of `[project]` has a "Corresponding core metadata field"[^1]. For example, `description` goes into `Summary` and `requires-python` goes into `Requires-Python`.[^1] For `dependencies`: "Each string maps directly to a Requires-Dist entry."[^1] For `optional-dependencies`: "Each value in the array thus becomes a corresponding Requires-Dist entry for the matching Provides-Extra metadata. The optionality of these dependencies is recorded by modifying the environment marker clause on the related Requires-Dist entries to check the extra name."[^1] Example: [`metadata`](#metadata).

**r5.** Each key has a TOML type. For `version` the TOML type is string, and the value is "The version of the project, as defined in the Version specifier specification."[^1] `tomllib.load` reads a file: "The first argument should be a readable and binary file object. Return a dict."[^5] A TOML string becomes a `str`, and a TOML float becomes a `float`.[^5] Examples: [`read`](#read), [`float-version`](#float-version).

**r6.** "The [tool] table is where any tool related to your Python project, not just build tools, can have users specify configuration data as long as they use a sub-table within [tool], e.g. the flit tool would store its configuration in [tool.flit]."[^1] Example: [`read`](#read).

**r7.** For the name, "Tools SHOULD normalize this name, as soon as it is read for internal consistency."[^1] The guide gives the rule: "Comparison of project names is case insensitive and treats arbitrarily long runs of underscores, hyphens, and/or periods as equal."[^2] Example: [`names`](#names).

**r8.** "A TOMLDecodeError will be raised on an invalid TOML document."[^5] `TOMLDecodeError` is a "Subclass of ValueError"[^5]. Example: [`bare-string`](#bare-string).

## Examples

The file `pyproject.toml` has the three tables. `tomllib.load` gives nested dicts and lists (rules r1, r3, r5 and r6).

```python norun reason=fragment file=pyproject.toml group=read
[build-system]
requires = ["hatchling >= 1.26"]
build-backend = "hatchling.build"

[project]
name = "spam-eggs"
version = "2.0.1"
dependencies = ["httpx", "rich>=13"]

[project.optional-dependencies]
cli = ["click"]

[tool.spam]
color = true
```

```python run file=main.py group=read id=read
import tomllib

with open("pyproject.toml", "rb") as f:
    data = tomllib.load(f)

print(list(data))
print(data["build-system"]["build-backend"])
project = data["project"]
print(project["name"], project["version"])
print(project["dependencies"])
print(project["optional-dependencies"])
print(data["tool"]["spam"])
```

```output for=read
['build-system', 'project', 'tool']
hatchling.build
spam-eggs 2.0.1
['httpx', 'rich>=13']
{'cli': ['click']}
{'color': True}
```

A build backend writes the keys of `[project]` as core metadata fields (rule r4). This program does the same mapping for five keys. An extra adds an environment marker to each of its dependencies. The core metadata specification gives this example: "Provides-Extra: pdf Requires-Dist: reportlab; extra == 'pdf'"[^6].

```python run file=main.py group=metadata id=metadata
import tomllib

TEXT = """
[project]
name = "spam-eggs"
version = "2.0.1"
description = "Lovely Spam! Wonderful Spam!"
requires-python = ">=3.9"
dependencies = ["httpx"]

[project.optional-dependencies]
cli = ["click", "rich>=13"]
"""

project = tomllib.loads(TEXT)["project"]
fields = {"name": "Name", "version": "Version",
          "description": "Summary", "requires-python": "Requires-Python"}
for key, field in fields.items():
    print(f"{field}: {project[key]}")
for dependency in project["dependencies"]:
    print(f"Requires-Dist: {dependency}")
for extra, dependencies in project["optional-dependencies"].items():
    print(f"Provides-Extra: {extra}")
    for dependency in dependencies:
        print(f"Requires-Dist: {dependency}; extra == '{extra}'")
```

```output for=metadata
Name: spam-eggs
Version: 2.0.1
Summary: Lovely Spam! Wonderful Spam!
Requires-Python: >=3.9
Requires-Dist: httpx
Provides-Extra: cli
Requires-Dist: click; extra == 'cli'
Requires-Dist: rich>=13; extra == 'cli'
```

The function `normalize` follows the rule of r7. The four spellings come from the packaging guide, and they all name one project.[^2]

```python run file=main.py group=names id=names
import re


def normalize(name):
    return re.sub(r"[-_.]+", "-", name).lower()


for name in ["Cool-Stuff", "cool.stuff", "COOL_STUFF", "CoOl__-.-__sTuFF"]:
    print(f"{name:18} {normalize(name)}")
```

```output for=names
Cool-Stuff         cool-stuff
cool.stuff         cool-stuff
COOL_STUFF         cool-stuff
CoOl__-.-__sTuFF   cool-stuff
```

## Versions

`tomllib` is in the standard library. "The module was added with support for TOML 1.0.0."[^5] Then "Changed in version 3.15: Added TOML 1.1.0 support."[^5]

The specification has a history.[^1] PEP 518 added `[build-system]` and `[tool]` in May 2016. PEP 621 added `[project]` in November 2020. PEP 639 changed `license` and added `license-files` in December 2024. PEP 794 added `import-names` and `import-namespaces` in October 2025.

## Pitfalls

### A version without quotes

The value of `version` must be a TOML string (rule r5). Without quotes, `1.10` is a TOML float, and the float is `1.1`.

```python wrong file=main.py group=float-version id=float-version
import tomllib

project = tomllib.loads('[project]\nname = "spam"\nversion = 1.10\n')["project"]
print(repr(project["version"]))
```

```output for=float-version
1.1
```

Write `version = "1.10"`.

### A string without quotes

A TOML string needs quotes. `tomllib` raises `TOMLDecodeError` for a bare word (rule r8).

```python fails file=main.py group=bare-string id=bare-string expect=exception:TOMLDecodeError
import tomllib

tomllib.loads("[project]\nname = spam-eggs\n")
```

### No [build-system] table

A frontend does not stop when the table is missing. The specification says: "If the file exists but is lacking the [build-system] table then the default values as specified above should be used."[^1] The default is the setuptools example: "Build tools are expected to use the example configuration file above as their default semantics when a pyproject.toml file is not present."[^1] Write the table, so that the frontend uses the backend that you chose.

### A dynamic name

"A build back-end MUST raise an error if the metadata specifies name in dynamic."[^1] The name must be in the file (rule r3).

### Keys at the top of [tool]

A tool must use its own sub-table, such as `[tool.flit]` (rule r6). Settings directly in `[tool]` do not belong to a tool.

## Comparison

**Rust** uses TOML for the same job: "The Cargo.toml file for each package is called its manifest. It is written in the TOML format. It contains metadata that is needed to compile the package."[^7] PEP 518 says: "The TOML format is already in use by the Rust community as part of their Cargo package manager"[^3]. In the list of manifest sections, `[package]` has the keys `name` and `version`, `[dependencies]` holds the dependencies, and the key `metadata` holds "Extra settings for external tools."[^7] In `pyproject.toml`, these are `[project]` and `[tool]` (rules r3 and r6). The list of Cargo sections has no key that names a build backend, and `pyproject.toml` has `[build-system]` (rule r2).

## Implementation notes

PEP 518 explains the choice of TOML: "This format was chosen as it is human-usable (unlike JSON [6]), it is flexible enough (unlike configparser [8]), stems from a standard (also unlike configparser [8]), and it is not overly complex (unlike YAML [7])."[^3] PEP 621 prefers static metadata. One of its aims is to "Encourage users to specify core metadata statically for speed, ease of specification, unambiguity, and deterministic consumption by build back-ends"[^4].

`tomllib` only reads: "This module does not support writing TOML."[^5] The order of keys stays: "CPython implementation detail: tomllib loads dictionary entries in the order they appear in the source."[^5] The example [`read`](#read) uses this order.

## References
