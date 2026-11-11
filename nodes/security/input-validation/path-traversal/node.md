---
tag: HZ0NPM
kind: weakness
slug: path-traversal
name: Path traversal
aliases: [directory traversal, dot-dot-slash, cwe-22]
area: security/input-validation
level: core
summary: >-
  Path traversal happens when a program builds a file path from outside input and does not check where the path points, so `..` or an absolute path reaches files outside the intended directory.
cwe: 22
abstraction: base
languages_affected: [any-that-opens-files]
owasp_top10_2025: A01
cwe_top25_2025: 6
history:
  year: 2000
  people: []
  note: CERT/CC vulnerability note VU#111677 described the "Web Server Folder Directory Traversal" vulnerability in Microsoft IIS 4 and 5, where a relative reference encoded with certain unicode characters escaped the web folder.
same_as:
  wikidata: Q442856
  wikipedia: Path_traversal_attack
sources:
  - url: https://cwe.mitre.org/data/definitions/22.html
    title: "CWE-22: Improper Limitation of a Pathname to a Restricted Directory ('Path Traversal')"
    locator: Description; Extended Description; Alternate Terms; Common Consequences; Potential Mitigations; Relationships; Observed Examples
  - url: https://top10.owasp.org/2025/A01_2025-Broken_Access_Control/
    title: "OWASP Top 10:2025, A01:2025 Broken Access Control"
    locator: List of Mapped CWEs
  - url: https://cwe.mitre.org/top25/archive/2025/2025_cwe_top25.html
    title: 2025 CWE Top 25 Most Dangerous Software Weaknesses
    locator: the ranked list, rank 6, CVEs in KEV
  - url: https://www.kb.cert.org/vuls/id/111677
    title: "CERT/CC Vulnerability Note VU#111677, Microsoft IIS 4.0 / 5.0 vulnerable to directory traversal via extended unicode in url"
    locator: Overview; Description; Date Public
  - url: https://www.cve.org/CVERecord?id=CVE-2000-0884
    title: CVE-2000-0884, Microsoft IIS 4.0 and 5.0
    locator: the description and the publication date
  - url: https://community.owasp.org/attacks/Path_Traversal
    title: OWASP Community, Path Traversal
    locator: Overview; How to protect yourself
  - url: https://docs.python.org/3.14/library/os.path.html
    title: The Python Standard Library, os.path, Python 3.14
    locator: os.path.join; os.path.commonpath; os.path.realpath
  - url: https://docs.python.org/3.14/library/pathlib.html
    title: The Python Standard Library, pathlib, Python 3.14
    locator: PurePath.is_relative_to; Path.resolve
  - url: https://docs.python.org/3.14/library/tarfile.html
    title: The Python Standard Library, tarfile, Python 3.14
    locator: Extraction filters; OutsideDestinationError
  - url: https://pkg.go.dev/os#Root
    title: Go standard library, package os, type Root
    locator: type Root; OpenRoot; OpenInRoot
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/nio/file/Path.html
    title: Java SE 25 API, java.nio.file.Path
    locator: normalize; startsWith; toRealPath
  - url: https://nodejs.org/api/path.html
    title: Node.js documentation, Path
    locator: path.resolve; path.relative
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.io.path.combine
    title: .NET API, Path.Combine Method
    locator: Remarks
  - url: https://doc.rust-lang.org/std/path/struct.PathBuf.html
    title: Rust standard library, std::path::PathBuf
    locator: PathBuf::push
  - url: https://codeql.github.com/codeql-query-help/python/py-path-injection/
    title: CodeQL query help, Uncontrolled data used in path expression (py/path-injection)
    locator: Recommendation; Example
  - url: https://codeql.github.com/codeql-query-help/python/py-tarslip/
    title: CodeQL query help, Arbitrary file write during tarfile extraction (py/tarslip)
    locator: the query description
  - url: https://docs.astral.sh/ruff/rules/tarfile-unsafe-members/
    title: Ruff rules, tarfile-unsafe-members (S202)
    locator: What it does; Why is this bad?
  - url: https://github.com/securego/gosec/blob/master/RULES.md
    title: gosec rules
    locator: G304, G305 and G703
  - url: https://www.cve.org/CVERecord?id=CVE-2007-4559
    title: CVE-2007-4559, Python tarfile
    locator: the description and the publication date
  - url: https://www.cve.org/CVERecord?id=CVE-2019-10743
    title: CVE-2019-10743, archiver
    locator: the description and the publication date
  - url: https://www.cve.org/CVERecord?id=CVE-2020-3452
    title: CVE-2020-3452, Cisco ASA and FTD web services interface
    locator: the description and the publication date
  - url: https://www.cve.org/CVERecord?id=CVE-2021-41773
    title: CVE-2021-41773, Apache HTTP Server 2.4.49
    locator: the description and the publication date
  - url: https://www.cve.org/CVERecord?id=CVE-2022-31503
    title: CVE-2022-31503, orchest
    locator: the description and the publication date
---

## Description

Many programs take a file name from outside and join it to a fixed directory: a server that sends files from `public`, a tool that writes uploads to `uploads`, a program that extracts an archive. The file system then resolves the joined path one component at a time. A `..` component means the parent directory, and a path that starts with `/`, or with a drive letter on Windows, starts at the root. Path traversal happens when the input holds such components and the program does not check where the result points.[^1] The program meant a file in one directory. The file system opens a file somewhere else.

The root cause is that the program treats the name as data, but the file system reads it as a path, which has its own syntax. A check on the text does not match the way the file system resolves the path. `public/../secret.txt` starts with the text `public`, and it still names a file outside it. The fix is to compute the path that the file system will use and check that path, or to take no path from the input at all.

What the attacker gets depends on the operation. A read can expose configuration files, keys and source code. A write can create or change files, and if the file is a program, a library or a configuration that a program loads, it can lead to code execution.[^1]

CWE-22 names this weakness. It is a base weakness under CWE-706, use of an incorrectly resolved name or reference. Its children are relative path traversal (CWE-23), with `..`, and absolute path traversal (CWE-36), with a full path.[^1] CWE-22 prefers the term "path traversal" to "directory traversal".[^1] OWASP also lists the names "dot-dot-slash", "directory climbing" and "backtracking".[^6] CWE-22 is rank 6 of the 2025 CWE Top 25, and 10 of its CVEs are in the CISA catalogue of known exploited vulnerabilities.[^3] In the OWASP Top 10:2025 it is part of A01:2025 Broken Access Control.[^2]

### Three ways out

- **`..` components.** Each `..` climbs one directory. The check must see the path after the file system has resolved them.[^1]
- **An absolute name.** Many join functions drop the earlier parts when a later part is absolute. Python's `os.path.join`, .NET's `Path.Combine` and Rust's `PathBuf::push` all do this, and Node's `path.resolve` works from right to left until it has an absolute path.[^7][^13][^14][^12] CWE-22 lists CVE-2022-31503 as an example of this case in Python.[^1]
- **A symbolic link.** A link inside the directory can point outside it. A check on the text cannot see it, because the link is in the file system and not in the name. CWE-59 names this weakness.[^1]

### Archives

An archive member has a name, and the name is a path. A program that extracts an archive joins each member name to the destination, so a member named `../outside.txt` lands outside it. This form is often called Zip Slip.[^20] CVE-2007-4559 describes it in the `tarfile` module of Python.[^19] The Python documentation describes extraction filters for `tarfile`, and the `data` filter refuses members that would land outside the destination. The documentation still says never to extract archives from untrusted sources without prior inspection.[^9]

### History

CERT/CC made the "Web Server Folder Directory Traversal" vulnerability in Microsoft IIS 4 and 5 public on 10 October 2000, as vulnerability note VU#111677.[^4] By design, IIS refused a relative reference such as `/data/../../../winnt/file.dat`. When a reference was encoded with certain unicode characters, IIS failed to prevent the access. In the executable `scripts` directory, this let a visitor run programs with the rights of the `IUSR_machinename` account.[^4] The note said that the vulnerability was like others that had been widely exploited, and that systems with it were likely to be compromised.[^4] CVE-2000-0884 is the record for it.[^5] The lesson is that the check must see the path in the same form as the code that opens the file. CWE-22 says to decode and canonicalize input before validation.[^1]

## Vulnerable code

The function below joins a name to the `public` directory and opens the result. The second call uses the classic demonstration input `../secret.txt`. The third passes an absolute path.

```python wrong id=join
import os
import tempfile

with tempfile.TemporaryDirectory() as base:
    public = os.path.join(base, "public")
    os.mkdir(public)
    with open(os.path.join(public, "report.txt"), "w") as f:
        f.write("quarterly report")
    with open(os.path.join(base, "secret.txt"), "w") as f:
        f.write("the secret key")

    def read_file(name):
        with open(os.path.join(public, name)) as f:
            return f.read()

    print(read_file("report.txt"))
    print(read_file("../secret.txt"))
    print(read_file(os.path.join(base, "secret.txt")))
```

```output for=join
quarterly report
the secret key
the secret key
```

Both calls read the secret file. `..` climbs out of `public`, and `os.path.join` drops every segment before an absolute segment.[^7] The same weakness appears with `filepath.Join` in Go, `Path.resolve` in Java, `path.join` in Node.js, `Path.Combine` in .NET and `Path::join` in Rust. The example files show each one.

## Fixed code

The function resolves the joined path and then checks that the result is inside the resolved root. `Path.resolve` makes the path absolute, follows symbolic links and removes `..` components. `is_relative_to` compares whole components, not text.[^8]

```python run id=resolve
import os
import tempfile
from pathlib import Path

with tempfile.TemporaryDirectory() as base:
    public = Path(base, "public")
    public.mkdir()
    (public / "report.txt").write_text("quarterly report")
    Path(base, "secret.txt").write_text("the secret key")
    os.symlink(Path(base, "secret.txt"), public / "link.txt")

    root = public.resolve()

    def read_file(name):
        path = (root / name).resolve()
        if not path.is_relative_to(root):
            raise PermissionError("the path is outside the public directory")
        return path.read_text()

    tests = [
        ("report.txt", "report.txt"),
        ("../secret.txt", "../secret.txt"),
        ("an absolute path", str(Path(base, "secret.txt"))),
        ("a symlink out", "link.txt"),
    ]
    for label, name in tests:
        try:
            print(f"{label}: {read_file(name)}")
        except PermissionError as error:
            print(f"{label}: refused, {error}")
```

```output for=resolve
report.txt: quarterly report
../secret.txt: refused, the path is outside the public directory
an absolute path: refused, the path is outside the public directory
a symlink out: refused, the path is outside the public directory
```

The order matters. The check comes after the resolve, so it sees the path that `open` will use. A check before the resolve, or a check on strings, fails. The Python example file shows a `startswith` check that a sibling directory such as `public-old` passes. Java's `Path.startsWith`, Rust's `Path::starts_with` and Python's `os.path.commonpath` compare components and do not have this problem.[^11][^14][^7]

### The check and the open

A check and a later open are two steps. If another process can change the directory between them, it can put a symbolic link where a file was. Go's `os.Root` closes this gap. It opens the directory once, and each method on the root refuses a name with a component that leads outside, and refuses a symbolic link that points outside.[^10] Its documentation also lists what it does not stop: it does not stop traversal of file system boundaries, Linux bind mounts, `/proc` special files or access to Unix device files.[^10] The Go example file shows it.

### Avoid the name

OWASP puts first a design without user input in file system calls. It suggests an index: the user sends `5`, and the program maps `5` to a file name that it chose.[^6] A key in a database row works in the same way. When the program must take a name, it should not let the user supply all parts of the path, and it should accept only known good input and not try to clean bad input.[^6]

## How to detect it

- **Static analysis with taint tracking.** CodeQL follows input from a request to a file system call. Its queries are `py/path-injection`, `js/path-injection`, `java/path-injection`, `go/path-injection` and `cs/path-injection`, and `py/tarslip` follows archive member names to the file system.[^15][^16]
- **Pattern linters.** gosec rule G304 reports a file path from input, G305 reports path traversal in zip extraction, and G703 follows input to a file path.[^18] Ruff rule S202 reports calls to `tarfile.extractall`.[^17]
- **Code review.** Search for `open`, `send_file`, `ReadFile`, `File`, `Path.Combine`, `os.path.join`, `extractall` and the unzip code. For each call, ask if any part of the path comes from input, and where the check is.
- **Tests.** Put `../` and an absolute path through each input that names a file, and put an archive with a member named `../outside.txt` through each extract step. The program must refuse them.

## How to prevent it

- **Take no path from input.** Map an index or an id to a file name that the program chose.[^6]
- **Open through a root.** Where the platform has it, use an API that opens files beneath one directory and refuses names and links that lead out, such as Go's `os.Root`.[^10]
- **Resolve, then check.** Resolve the joined path, following links, and check by components that it is inside the resolved root.[^1][^8] Decode the input fully before the check.[^1]
- **Allow-list file names.** Limit the characters, allow at most one `.`, exclude directory separators, and allow only listed extensions.[^1] Do not remove `../` from the input. CWE-22 warns that a filter can leave a new `../` behind.[^1]
- **Extract archives with a filter.** Check each member name before it is written, or use a filter such as the `data` filter of `tarfile`.[^9]
- **Use least privilege.** Run the program with an account that cannot read or write files outside its data. A jail or a sandbox limits the files that the process can reach.[^1][^6]

## Real cases

| Case | Date | What happened |
|---|---|---|
| VU#111677 | 2000-10-10 | Microsoft IIS 4 and 5 failed to stop a relative reference encoded with certain unicode characters, so a visitor could read files outside the web folder and run programs in it.[^4][^5] |
| CVE-2007-4559 | published 2007-08-28 | The `extract` and `extractall` functions of Python's `tarfile` module let a `..` sequence in a member name overwrite files.[^19] |
| CVE-2019-10743 | published 2019-10-28 | Zip Slip in the archiver library for Go: a member name was joined to the destination directory, so the file landed outside it.[^20][^1] |
| CVE-2020-3452 | published 2020-07-22 | Directory traversal in the web services interface of Cisco ASA and FTD software let an unauthenticated attacker read sensitive files. CWE-22 says that it was exploited in the wild.[^21][^1] |
| CVE-2021-41773 | published 2021-10-05 | A change to path normalization in Apache HTTP Server 2.4.49 let requests map to files outside the directories of Alias-like directives, and with CGI enabled, to code execution. The record says that it was exploited in the wild.[^22] |
| CVE-2022-31503 | published 2022-07-11 | orchest passed input to Flask's `send_file`, and an absolute path read files outside the intended directory. CWE-22 says that `os.path.join` reset the path to the absolute input.[^23][^1] |

## References
