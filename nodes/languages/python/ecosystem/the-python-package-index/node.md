---
tag: KR2M2T
kind: feature
owner: python
slug: the-python-package-index
name: The Python Package Index
aliases: [PyPI, Python Package Index, PyPA, Python Packaging Authority, package name normalization, simple repository API, wheel file name]
area: languages/python/ecosystem
level: intro
summary: >-
  PyPI is the public repository of Python software, run by the PSF. pip installs from it. Project names are compared after normalization, and malware on PyPI is reported through the project page.
grammar: null
spec: {standard: null, url: "https://packaging.python.org/en/latest/specifications/simple-repository-api/", locator: "Simple repository API"}
see_also: [python/the-pep-process, python/governance-of-python]
sources:
  - url: https://pypi.org/
    title: PyPI, the Python Package Index
    locator: home page statistics
    accessed: 2026-11-16
  - url: https://www.pypa.io/en/latest/
    title: Python Packaging Authority, PyPA documentation
    locator: Python Packaging Authority
    accessed: 2026-11-16
  - url: https://packaging.python.org/en/latest/specifications/name-normalization/
    title: Names and normalization, Python Packaging User Guide
    locator: Name format; Name normalization
    accessed: 2026-11-16
  - url: https://packaging.python.org/en/latest/specifications/simple-repository-api/
    title: Simple repository API, Python Packaging User Guide
    locator: Base HTML API
    accessed: 2026-11-16
  - url: https://packaging.python.org/en/latest/specifications/binary-distribution-format/
    title: Binary distribution format, Python Packaging User Guide
    locator: File name convention
    accessed: 2026-11-16
  - url: https://packaging.python.org/en/latest/glossary/
    title: Glossary, Python Packaging User Guide
    locator: Distribution Package; Import Package
    accessed: 2026-11-16
  - url: https://peps.python.org/pep-0541/
    title: "PEP 541: Package Index Name Retention"
    locator: Abstract
    accessed: 2026-11-16
  - url: https://pypi.org/security/
    title: Security, PyPI
    locator: Reporting a security issue
    accessed: 2026-11-16
  - url: https://go.dev/ref/mod#module-proxy
    title: Go Modules Reference
    locator: Module paths; Module proxies
    accessed: 2026-11-16
---

## Synopsis

```python run id=normalize
import re

def normalize(name):
    return re.sub(r"[-_.]+", "-", name).lower()

for name in ['Friendly-Bard', 'friendly.bard', 'friendly_bard', 'FrIeNdLy-._.-bArD']:
    print(name, '->', normalize(name))
```

```output for=normalize
Friendly-Bard -> friendly-bard
friendly.bard -> friendly-bard
friendly_bard -> friendly-bard
FrIeNdLy-._.-bArD -> friendly-bard
```

The Python Package Index (PyPI) at https://pypi.org/ is the public repository of Python software. pip, the installer of the PyPA, downloads projects from it (rule r3). PyPI compares project names after normalization, so the four names above are one project.

## Semantics

**r1.** "The Python Package Index (PyPI) is a repository of software for the Python programming language."[^1] It is "Developed and maintained by the Python Software Foundation and Python community, for the Python community."[^1]

**r2.** On 2026-11-16, the PyPI home page showed 943,245 projects, 10,213,023 releases, 22,948,786 files and 1,143,500 users.[^1]

**r3.** "The Python Packaging Authority (PyPA) is a working group that maintains a core set of software projects used in Python packaging."[^2] pip is "the most prominent software used to interact with PyPI."[^2] The PyPA publishes the Python Packaging User Guide, which is "the authoritative resource on how to package, publish, and install Python projects using current tools."[^2]

**r4.** "A valid name consists only of ASCII letters and numbers, period, underscore and hyphen. It must start and end with a letter or number."[^3] Example: [`valid-names`](#valid-names).

**r5.** "The name should be lowercased with all runs of the characters ., -, or _ replaced with a single - character."[^3] The normalization "should be done before lookups and comparisons."[^3] Example: [`normalize`](#normalize).

**r6.** A package index serves the simple repository API. "A repository that implements the simple API is defined by its base URL."[^4] The base URL of PyPI is https://pypi.org/simple/.[^4] Each project has the URL `/<project>/`, "where the <project> is replaced by the normalized name for that project."[^4] Example: [`simple-url`](#simple-url).

**r7.** A built distribution is a wheel file. "The wheel filename is {distribution}-{version}(-{build tag})?-{python tag}-{abi tag}-{platform tag}.whl."[^5] Example: [`wheel-name`](#wheel-name).

**r8.** A distribution package is "A versioned archive file that contains Python packages, modules, and other resource files that are used to distribute a Release."[^6] An import package is "A Python module which can contain other modules or recursively, other packages."[^6] The name on PyPI is the name of the distribution package.

**r9.** PEP 541 sets the rules for conflicts about names. It clarifies the "expectations of package owners regarding ownership of a package name on the Package Index, specifically with regards to conflict resolution."[^7]

**r10.** To report a malicious project, log in to PyPI and open the project page. "At the top of the Security tab, click Report project as malware."[^8] "Valid malware reports may include examples of typo-squatting, dependency confusion, data exfiltration, obfuscation, command/control, etc."[^8] A security issue in PyPI itself goes to security@pypi.org, not to a public forum.[^8]

## Examples

Check a name against the regular expression of the specification (rule r4):

```python run id=valid-names
import re

VALID = re.compile(r'^([A-Z0-9]|[A-Z0-9][A-Z0-9._-]*[A-Z0-9])\Z', re.IGNORECASE)

for name in ['requests', 'zope.interface', 'my_tool', '-dash-first', 'café', 'x']:
    print(f'{name!r:16} {bool(VALID.match(name))}')
```

```output for=valid-names
'requests'       True
'zope.interface' True
'my_tool'        True
'-dash-first'    False
'café'           False
'x'              True
```

Build the simple API URL of a project (rule r6):

```python run id=simple-url
import re

def normalize(name):
    return re.sub(r"[-_.]+", "-", name).lower()

BASE = 'https://pypi.org/simple/'
for name in ['Django', 'zope.interface', 'typing_extensions']:
    print(BASE + normalize(name) + '/')
```

```output for=simple-url
https://pypi.org/simple/django/
https://pypi.org/simple/zope-interface/
https://pypi.org/simple/typing-extensions/
```

Split a wheel file name into its parts (rule r7). This file name has no build tag:

```python run id=wheel-name
filename = 'example_pkg-2.1.0-cp313-cp313-manylinux_2_17_x86_64.whl'
parts = filename.removesuffix('.whl').split('-')
fields = ['distribution', 'version', 'python tag', 'abi tag', 'platform tag']
for field, value in zip(fields, parts):
    print(f'{field:12} {value}')
```

```output for=wheel-name
distribution example_pkg
version      2.1.0
python tag   cp313
abi tag      cp313
platform tag manylinux_2_17_x86_64
```

## Pitfalls

A name on PyPI is not always the name that you import (rule r8). The distribution package and the import package are different things, and they can have different names. Read the project page to find the import name.

Names that differ only in case or in the characters `.`, `_` and `-` are the same project (rule r5). `pip install Friendly_Bard` and `pip install friendly-bard` install the same project.

A name that differs by one letter from a popular name is a different project, because normalization changes only case and the characters `.`, `_` and `-`. PyPI lists typo-squatting as one kind of malware report (rule r10). Copy the name from the project page or from the documentation of the project.

The number of projects on PyPI does not tell you which projects are maintained or safe. Check the release history, the source repository and the maintainers before you add a dependency.

## Comparison

"A module path is the canonical name for a module, declared with the module directive in the module’s go.mod file."[^9] "A module path should describe both what the module does and where to find it."[^9] The go command can download modules through a module proxy: "A module proxy is an HTTP server that can respond to GET requests for paths specified below."[^9] A Python project name is a short name on PyPI, such as `requests`. A Go module path is a path such as `golang.org/x/net`.

[^1]: [PyPI, the Python Package Index](https://pypi.org/)
[^2]: [Python Packaging Authority](https://www.pypa.io/en/latest/)
[^3]: [Names and normalization](https://packaging.python.org/en/latest/specifications/name-normalization/)
[^4]: [Simple repository API](https://packaging.python.org/en/latest/specifications/simple-repository-api/)
[^5]: [Binary distribution format](https://packaging.python.org/en/latest/specifications/binary-distribution-format/)
[^6]: [Glossary, Python Packaging User Guide](https://packaging.python.org/en/latest/glossary/)
[^7]: [PEP 541: Package Index Name Retention](https://peps.python.org/pep-0541/)
[^8]: [Security, PyPI](https://pypi.org/security/)
[^9]: [Go Modules Reference](https://go.dev/ref/mod#module-proxy)
