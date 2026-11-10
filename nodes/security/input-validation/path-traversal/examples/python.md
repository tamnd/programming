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

The second call climbs out of `public` with `..`. The third call passes an absolute path, and `os.path.join` drops every segment before an absolute segment.

```python wrong id=startswith
import os

public = "/srv/app/public"
for name in ["report.txt", "../secret.txt"]:
    path = os.path.join(public, name)
    print(path, path.startswith(public))

print("/srv/app/public-old/notes.txt".startswith(public))
```

```output for=startswith
/srv/app/public/report.txt True
/srv/app/public/../secret.txt True
True
```

A string check on a path that is not normalized fails in two ways. `public/../secret.txt` starts with `public`, so the check passes. And `public-old` starts with the text `public`, so a sibling directory passes too.

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

```python run id=tar-filter since=3.12
import io
import tarfile
import tempfile

# Make an archive in memory with one member whose name climbs out.
buffer = io.BytesIO()
with tarfile.open(fileobj=buffer, mode="w") as tar:
    data = b"text"
    member = tarfile.TarInfo("../outside.txt")
    member.size = len(data)
    tar.addfile(member, io.BytesIO(data))

buffer.seek(0)
with tempfile.TemporaryDirectory() as dest, tarfile.open(fileobj=buffer) as tar:
    try:
        tar.extractall(dest, filter="data")
    except tarfile.OutsideDestinationError as error:
        print(type(error).__name__)
```

```output for=tar-filter
OutsideDestinationError
```

- Verdict: stdlib. `Path.resolve` (or `os.path.realpath`) makes the path absolute, removes `.` and `..`, and follows symbolic links. `Path.is_relative_to` then compares whole path components, not text. The check must come after the resolve.
- `os.path.join` and the `/` operator of `pathlib` drop the earlier segments when a later segment is absolute. Never pass input as a later segment without the check.
- `os.path.commonpath([root, path]) == root` is the same check for code that uses strings.
- A check and a later open are two steps. If another process can change the directory between them, it can swap a file for a symbolic link. Where other users can write to the directory, the check is not enough.
- The fourth example shows the extraction filters of `tarfile`. The `data` filter refuses members that would land outside the destination, and links to absolute paths. It is the default filter, and the documentation still says never to extract archives from untrusted sources without prior inspection. `zipfile` removes `..` and leading slashes from member names when it extracts.
- Ruff rule S202 reports calls to `tarfile.extractall`, and its documentation recommends `filter='data'`. CodeQL queries `py/path-injection` and `py/tarslip` follow input to file operations and archive members to the file system.
