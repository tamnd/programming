---
tag: 0VP887
kind: task
slug: list-files-in-a-directory
name: List files in a directory
aliases: [list a directory, walk a directory tree, read the entries of a folder, recursive directory listing]
area: data-and-persistence/paths-and-directories
level: core
summary: List the entries of one directory or of a whole tree in a fixed sorted order, list symbolic links without following them, and report an error for a missing directory.
contract: contract.yaml
category: file-input
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
  - url: https://pubs.opengroup.org/onlinepubs/9799919799/functions/readdir.html
    title: The Open Group, POSIX.1-2024, readdir
    locator: Description, the entries for dot and dot-dot
  - url: https://pubs.opengroup.org/onlinepubs/9799919799/functions/fstatat.html
    title: The Open Group, POSIX.1-2024, fstat, fstatat, lstat and stat
    locator: Description, AT_SYMLINK_NOFOLLOW
  - url: https://docs.python.org/3/library/os.html#os.scandir
    title: The Python Standard Library, os, scandir
    locator: os.scandir, os.DirEntry.is_dir, os.walk
  - url: https://nodejs.org/api/fs.html#fspromisesreaddirpath-options
    title: Node.js documentation, File system, fsPromises.readdir
    locator: the withFileTypes option; Class fs.Dirent, dirent.isDirectory and dirent.isSymbolicLink
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/nio/file/Files.html#walk(java.nio.file.Path,int,java.nio.file.FileVisitOption...)
    title: Java SE 25 API, java.nio.file.Files
    locator: walk(Path, int, FileVisitOption...), walkFileTree (FileSystemLoopException)
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.io.enumerationoptions.attributestoskip?view=net-10.0
    title: .NET API reference, EnumerationOptions.AttributesToSkip
    locator: Property Value
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.io.enumerationoptions.ignoreinaccessible?view=net-10.0
    title: .NET API reference, EnumerationOptions.IgnoreInaccessible
    locator: Property Value
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.io.fileattributes?view=net-10.0
    title: .NET API reference, FileAttributes
    locator: Remarks, the Hidden attribute on Unix
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.io.filesysteminfo.linktarget?view=net-10.0
    title: .NET API reference, FileSystemInfo.LinkTarget
    locator: Property Value
  - url: https://pkg.go.dev/os#ReadDir
    title: Go standard library, package os
    locator: ReadDir, File.ReadDir
  - url: https://doc.rust-lang.org/std/fs/fn.read_dir.html
    title: The Rust Standard Library, std::fs::read_dir
    locator: Platform-specific behavior
  - url: https://doc.rust-lang.org/std/fs/struct.DirEntry.html#method.file_type
    title: The Rust Standard Library, std::fs::DirEntry
    locator: file_type
  - url: https://en.cppreference.com/w/cpp/filesystem/recursive_directory_iterator.html
    title: cppreference.com, std::filesystem::recursive_directory_iterator
  - url: https://en.cppreference.com/w/cpp/filesystem/directory_iterator.html
    title: cppreference.com, std::filesystem::directory_iterator
  - url: https://en.cppreference.com/w/cpp/filesystem/directory_entry/is_directory.html
    title: cppreference.com, std::filesystem::directory_entry::is_directory
  - url: https://www.gnu.org/software/bash/manual/bash.html#Filename-Expansion
    title: GNU Bash Reference Manual, Filename Expansion
    locator: Filename Expansion; The Shopt Builtin, dotglob, globskipdots and nullglob
---

## Task

List the entries of a directory. With the recursive option, list the entries of the whole tree under it.

Each entry is a path relative to the directory, with `/` between the parts. A directory has a `/` at the end, so the reader can see the type. The list does not hold `.` and `..`. It holds names that start with `.`, because they are entries too.

The list is in ascending order of Unicode code points. The file system gives the entries in an order of its own, which can change between machines and file systems. A program that prints the raw order is not repeatable, so the solution sorts. The sort is on the whole relative path. So `a.txt` comes before `a/` and `a/x`, because `.` (U+002E) is less than `/` (U+002F). The order does not depend on the locale, so `B.txt` comes before `a.txt`.

A symbolic link is an entry, and it is listed by its own name with no `/`. The solution never follows a link, also when the link points to a directory. This gives the same result for a broken link, and a link that points to its own parent cannot make the walk loop.

If the path does not exist, or it names a file and not a directory, the solution reports an error and returns no list.

The tests make each tree in a new temporary directory, so the result is the same on each machine. The node has no fixtures.

## Solutions

Each solution reads one directory at a time, asks the type of each entry without following links, and recurses into the real directories only. Then it sorts the whole list once.

- **Python.** `os.scandir` gives `DirEntry` objects in arbitrary order. `entry.is_dir(follow_symlinks=False)` is false for a link. The default `follow_symlinks=True` is true for a link to a directory.
- **JavaScript and TypeScript (Node.js).** `readdir` from `node:fs/promises` with `withFileTypes: true` gives `Dirent` objects. `isDirectory()` is false for a link and `isSymbolicLink()` is true, so the recursion stops at links. The default `sort()` compares UTF-16 code units. That is code point order for names in the Basic Multilingual Plane.
- **Java.** `Files.walk(root, depth)` does not follow links unless it gets `FOLLOW_LINKS`. Depth 1 lists one directory. The stream starts with the root itself, so the code skips it. `Files.walk` on a file returns the file, so the code checks the type of the root first and throws `NotDirectoryException`.
- **C#.** `DirectoryInfo.EnumerateFileSystemInfos` with an `EnumerationOptions` object lists one directory, and the code recurses itself. `RecurseSubdirectories = true` follows a link to a directory, as the pitfall below shows. Set `AttributesToSkip = 0` and `IgnoreInaccessible = false`, because the defaults skip hidden entries and hide errors. `LinkTarget` is not null for a link. A path that names a file gives `DirectoryNotFoundException`, so the code checks `File.Exists` first. Sort with `StringComparer.Ordinal`.
- **Go.** `os.ReadDir` returns the entries of one directory sorted by file name. `IsDir` is false for a link. The code still sorts at the end, because the order of each directory is not the order of the whole list.
- **Rust.** `fs::read_dir` gives the entries in an order that depends on the platform. `DirEntry::file_type` does not follow links. The code returns an error of kind `InvalidData` for a name that is not UTF-8, and `NotADirectory` comes from `read_dir`.
- **C.** POSIX `opendir` and `readdir` in a loop, with `errno` set to 0 before each call to tell the end from an error. `readdir` returns `.` and `..`, so the code skips them. `fstatat` with `AT_SYMLINK_NOFOLLOW` gives the type of the link itself. `qsort` with `strcmp` sorts by byte, and for UTF-8 that is code point order.
- **C++.** `std::filesystem::recursive_directory_iterator` does not follow links by default. `directory_iterator` lists one directory. Both skip `.` and `..`, and the order is unspecified. `directory_entry::is_directory` follows a link, so the code tests `is_symlink` first.
- **Bash (4.4 or later).** A loop over `"$dir"/*` in a subshell with `shopt -s dotglob nullglob`, so the options do not leak. `[[ -d $p && ! -L $p ]]` finds a real directory, because `-d` follows a link. `LC_ALL=C sort` sorts by byte. A name with a newline would break the output, so the contract excludes it.

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`. Each case gives the tree as a list: a name that ends in `/` is a directory, and `link -> target` is a symbolic link. The cases that must give an error are in `cases/errors.jsonl`.

- `empty` and `empty-subdir`: an empty directory gives an empty list. In Bash without `nullglob`, the pattern `*` stays as the literal word `*`.
- `hidden`: `.env` and `.git/` are in the list. Bash globs and C# with a new `EnumerationOptions` skip them by default.
- `case-order`: `B.txt` comes before `a.txt` in code point order. Bash globs sort by the collation order of the locale (`LC_COLLATE`), so their order changes with the locale.
- `dot-before-slash`: the list is `a-b`, `a.txt`, `a/`, `a/x`. A walk that prints each directory in order gives `a/`, `a/x`, `a.txt`, so the sort must be on the whole path.
- `symlink-to-dir`: `link` points to `real/`. It is listed once, with no `/`, and its contents are not listed again. C# with `RecurseSubdirectories` lists them again.
- `symlink-to-file` and `broken-symlink`: a link is listed also when its target is a file or does not exist. A check that follows links fails on the broken link.
- `symlink-loop`: `d/up` points to `..`. A walk that follows links never ends, or stops with an error.
- `spaces` and `non-ascii`: `été.txt` comes after `zebra.txt`, because `é` is U+00E9.
- Errors: a path that does not exist, and a path that names a file.

## Pitfalls

- **Bash globs skip dot files and keep a pattern that matches nothing.** Without `dotglob`, `*` does not match `.env`. Without `nullglob`, an empty directory gives the word `empty/*`:

```bash wrong id=bash-glob-defaults
mkdir -p tree/empty && touch tree/.env tree/main.go
for p in tree/*; do echo "entry: $p"; done
for p in tree/empty/*; do echo "entry: $p"; done
```

```output for=bash-glob-defaults
entry: tree/empty
entry: tree/main.go
entry: tree/empty/*
```

Set both options:

```bash run id=bash-glob-options
mkdir -p tree/empty && touch tree/.env tree/main.go
shopt -s dotglob nullglob
for p in tree/*; do echo "entry: $p"; done
for p in tree/empty/*; do echo "entry: $p"; done
```

```output for=bash-glob-options
entry: tree/.env
entry: tree/empty
entry: tree/main.go
```

- **The type check follows the link.** In C++, `directory_entry::is_directory` reports the type of the target. Python `DirEntry.is_dir()` does the same by default, and `os.walk` puts a link to a directory in its list of directory names:

```cpp run id=cpp-is-directory
#include <filesystem>
#include <print>

namespace fs = std::filesystem;

int main() {
    fs::create_directories("tree/real");
    fs::create_directory_symlink("real", "tree/link");
    fs::directory_entry link{"tree/link"};
    std::println("is_directory: {}", link.is_directory());
    std::println("is_symlink: {}", link.is_symlink());
}
```

```output for=cpp-is-directory
is_directory: true
is_symlink: true
```

```python run id=python-walk-dirnames
import os

os.makedirs("tree/real", exist_ok=True)
os.symlink("real", "tree/link")
root, dirnames, filenames = next(os.walk("tree"))
print("dirnames:", sorted(dirnames))
link = next(e for e in os.scandir("tree") if e.name == "link")
print("is_dir():", link.is_dir())
print("is_dir(follow_symlinks=False):", link.is_dir(follow_symlinks=False))
```

```output for=python-walk-dirnames
dirnames: ['link', 'real']
is_dir(): True
is_dir(follow_symlinks=False): False
```

- **A new `EnumerationOptions` in C# skips hidden entries, and its recursion follows links.** Its `AttributesToSkip` is `Hidden | System` by default. On Unix, .NET marks a name that starts with `.` as `Hidden`. With `RecurseSubdirectories = true`, the output also holds `tree/link/f`, so the files under `real` are listed twice:

```csharp wrong id=csharp-enumeration-options
Directory.CreateDirectory("tree/real");
File.WriteAllText("tree/real/f", "");
File.WriteAllText("tree/.env", "");
File.CreateSymbolicLink("tree/link", "real");
var options = new EnumerationOptions { RecurseSubdirectories = true };
foreach (string path in Directory.GetFileSystemEntries("tree", "*", options).Order(StringComparer.Ordinal))
{
    Console.WriteLine(path);
}
```

```output for=csharp-enumeration-options
tree/link
tree/link/f
tree/real
tree/real/f
```

- **Following links can loop.** Java `Files.walk` with `FOLLOW_LINKS` detects a link to an ancestor and throws `FileSystemLoopException`, wrapped in an `UncheckedIOException`. Do not follow links unless the task needs it:

```java run id=java-follow-links
import java.io.UncheckedIOException;
import java.nio.file.FileVisitOption;
import java.nio.file.Files;
import java.nio.file.Path;

public class Main {
  public static void main(String[] args) throws Exception {
    Path d = Files.createDirectories(Path.of("tree/d"));
    Files.createSymbolicLink(d.resolve("up"), Path.of(".."));
    try (var paths = Files.walk(Path.of("tree"), FileVisitOption.FOLLOW_LINKS)) {
      paths.forEach(p -> {});
    } catch (UncheckedIOException e) {
      System.out.println(e.getCause().getClass().getSimpleName());
    }
    try (var paths = Files.walk(Path.of("tree"))) {
      System.out.println(paths.count() + " paths without FOLLOW_LINKS");
    }
  }
}
```

```output for=java-follow-links
FileSystemLoopException
3 paths without FOLLOW_LINKS
```

- **The raw order of the file system.** Python `os.listdir` and `os.scandir`, Rust `read_dir`, Go `File.ReadDir` and the C++ iterators give no fixed order. A test that compares the raw order can pass on one machine and fail on another. Sort before you print or compare.

## Related tasks

- [[read-a-file-line-by-line]]: read each file that the listing finds.
- [[write-a-file]]: make the files that a test lists.
- [[sort-a-list-of-records]]: sort the entries by size or by time instead of by name.
- [[path-traversal]]: check that a name from outside stays inside the directory before you join it to a path.

## References
