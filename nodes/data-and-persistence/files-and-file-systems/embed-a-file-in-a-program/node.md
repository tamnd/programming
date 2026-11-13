---
tag: EHSVGA
kind: task
slug: embed-a-file-in-a-program
name: Embed a file in a program
aliases: [embed a resource, include a file at compile time, go:embed, include_str, include_bytes, "#embed", embedded resource, package data, resource file, bundle a file]
area: data-and-persistence/files-and-file-systems
level: core
summary: Put the bytes of a data file into a program when it is built, so that the program reads them from memory at run time and does not depend on the working directory or on a file next to it.
contract: contract.yaml
category: file-input
languages: [python, c, cpp, go, rust, bash]
sources:
  - url: https://pkg.go.dev/embed
    title: Go standard library, package embed
    locator: Overview, Directives, Strings and Bytes
  - url: https://doc.rust-lang.org/std/macro.include_bytes.html
    title: The Rust Standard Library, macro include_bytes
    locator: Description
  - url: https://doc.rust-lang.org/std/macro.include_str.html
    title: The Rust Standard Library, macro include_str
    locator: Description
  - url: https://en.cppreference.com/w/c/preprocessor/embed.html
    title: cppreference, Binary resource inclusion (C23)
    locator: Syntax, Explanation, Parameters
  - url: https://www.open-std.org/jtc1/sc22/wg14/www/docs/n3017.htm
    title: "N3017: #embed - a scannable, tooling-friendly binary resource inclusion mechanism"
    locator: 3.1 Motivation, 3.2.1 Speed, 3.2.2 Memory Size
  - url: https://docs.python.org/3/library/importlib.resources.html
    title: The Python Standard Library, importlib.resources, Package resource reading, opening and access
    locator: introduction, files
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/lang/Class.html
    title: Java SE 25 API, Class
    locator: getResourceAsStream(String)
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.reflection.assembly.getmanifestresourcestream?view=net-10.0
    title: .NET API reference, Assembly.GetManifestResourceStream Method
    locator: GetManifestResourceStream(String), Returns
  - url: https://learn.microsoft.com/en-us/visualstudio/msbuild/common-msbuild-project-items?view=visualstudio
    title: MSBuild documentation, Common MSBuild project items
    locator: EmbeddedResource
  - url: https://nodejs.org/api/single-executable-applications.html
    title: Node.js documentation, Single executable applications
    locator: introduction, sea.getAsset(key[, encoding])
  - url: https://pubs.opengroup.org/onlinepubs/9799919799/utilities/V3_chap02.html
    title: The Open Group Base Specifications Issue 8 (POSIX.1-2024), Shell Command Language
    locator: 2.7.4 Here-Document
---

## Task

Put the text file `lines.txt` into the program when the program is built. Then return line `n` of it, with no line end, from the copy in memory. Line 1 is the first line after the license header. Return no value when `n` is less than 1 or more than the number of lines.

The file is next to the source of each solution, and it is UTF-8. Its first line is the license header `# SPDX-License-Identifier: MIT`, because each file of a solution on this site starts with one. The four lines after it are `alpha`, `café`, an empty line and `gamma`. Each line ends with `\n`, so the file has 50 bytes. Each solution skips the header line.

A program that opens `lines.txt` at run time does not do the task. Such a program fails when it starts in another directory, or when a person copies only the program to another computer. With an embedded file, the program is the only thing to install.

SQL, regular expressions, HTML and CSS do not apply, because they do not build programs.

## Solutions

The compiled languages read the file in the build, and put its bytes in the program as a constant. The other languages do not have a build step that can do this, so they ship the file with the code and find it through the loader, not through the working directory.

- **Go.** A `//go:embed` directive above a variable at package scope. "Package embed provides access to files embedded in the running Go program."[^1] The directive can "initialize a variable of type string, []byte, or FS with the contents of files read from the package directory or subdirectories at compile time"[^1]. For a `string`, "The //go:embed line for a variable of type string or []byte can have only a single pattern, and that pattern can match only a single file."[^1] Only "Go source files that import "embed" can use the //go:embed directive"[^1], so the code has `import _ "embed"`, also though it uses only a `string`.
- **Rust.** `include_str!("lines.txt")` "Includes a UTF-8 encoded file as a string."[^3], and `include_bytes!` gives "an expression of type &'static [u8; N] which is the contents of the file"[^2]. For each macro, "The file is located relative to the current file (similarly to how modules are found)."[^3] The code uses `str::lines`, and returns `None` for `n` less than 1 before it converts `n` to an index.
- **C.** The `#embed` directive in an initializer: "#embed is a preprocessor directive to include (binary) resources in the build"[^4]. It "replaces the directive by a comma separated list of integers corresponding to the data of the resource"[^4]. A compiler with no `#embed` does not define `__has_embed`, so the code uses `#if defined(__has_embed)`, and else includes `lines_txt.h`, the same list of bytes that `xxd -i` made. The native test checks that the header agrees with `lines.txt`. The array has no 0 byte at its end, so the function returns a pointer and a length, not a C string.
- **C++.** The same two forms. The code uses `#embed` only when `__cplusplus` is more than `202302L` and the compiler has `__has_embed`. Else it includes `lines_txt.h`, because `#embed` in an earlier mode is an extension, and a build with warnings as errors stops on it. The function returns `std::optional<std::string_view>`, a view into the array, so it does not copy the line.
- **Python.** Python has no build step that puts a file into a program. The file ships next to the module, and `importlib.resources` finds it. "This module leverages Python’s import system to provide access to resources within packages ."[^6] The resources "may be contained directly in a package, within a subdirectory contained in that package, or adjacent to modules outside a package"[^6]. So `resources.files(__name__)` works for a module that is not in a package. The loader finds the file, so the working directory does not matter, and "a package and its resources can be imported from a zip file using zipimport"[^6]. The code reads the file one time, and keeps the lines.
- **Bash.** A script has no build step, so the text is in the script as a here-document. "The redirection operators "<<" and "<<-" both allow redirection of subsequent lines read by the shell to the input of a command."[^11] The delimiter is quoted (`<<'END_OF_LINES'`), and "If any part of word is quoted, not counting double-quotes outside a command substitution if the here-document is inside one, the delimiter shall be formed by performing quote removal on word , and the here-document lines shall not be expanded."[^11] So a `$` or a backquote in the text stays as it is. The native test compares the text with `lines.txt`.

Java, C#, JavaScript and TypeScript have no solution here, because each one embeds a file through its build tool or packager, and the test harness of this site compiles only the source files:

- **Java.** A file in a JAR, next to the classes, is a resource. `Class.getResourceAsStream(name)` "Finds a resource with a given name."[^7] A name with no `/` at the start is relative to the package of the class: "the absolute name is of the following form: modified_package_name/name"[^7]. The method returns "null if no resource with this name is found"[^7], so check for null.
- **C#.** An `EmbeddedResource` item in the project file "Represents resources to be embedded in the generated assembly."[^9] At run time, `Assembly.GetManifestResourceStream(name)` "Loads the specified manifest resource from this assembly."[^8] It returns "null if no resources were specified during compilation or if the resource is not visible to the caller"[^8]. The item has a `LogicalName`, "The logical name of the embedded resource."[^9], and the code must use that name.
- **JavaScript and TypeScript (Node.js).** A module can read a file next to it at run time, with a path from the location of the module. To ship one file, a single executable application "allows the distribution of a Node.js application conveniently to a system that does not have Node.js installed"[^10], and `sea.getAsset(key)` can "retrieve the assets configured to be bundled into the single-executable application at build time"[^10]. A bundler can also put the text of a file into the code of a module.

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`.

- `first-line` and `last-line`: lines 1 and 4 are `alpha` and `gamma`. The text after the last `\n` is empty, and it is not a fifth line.
- `non-ascii-line`: line 2 is `café`, 5 bytes in UTF-8. The embedded data has the bytes of the file, not characters. Rust's `include_str!` and Go's `string` keep the UTF-8 bytes, and C and C++ get the values of the bytes.
- `empty-line`: line 3 is an empty string. That is a line, not the end of the data.
- `zero`, `negative` and `past-the-end`: lines 0, -1 and 5 do not exist, and the result is no value. In Rust, a negative `n` must not become a large `usize` index.

The native tests also check that the embedded bytes are the 50 bytes of `lines.txt`, and, in Python, Go, Rust and Bash, that the result does not change when the working directory changes. C and C++ also check that `lines_txt.h` has the same bytes as `lines.txt`.

## Pitfalls

- **A path from the working directory.** A program that opens `lines.txt` by a relative path looks in the working directory, not in the directory of the code. It works when a person starts it in the directory of the project, and fails in other places:

```python wrong id=python-relative-path
import os
import tempfile

with tempfile.TemporaryDirectory() as project, tempfile.TemporaryDirectory() as other:
    with open(os.path.join(project, "lines.txt"), "w", encoding="utf-8") as f:
        f.write("alpha\n")
    start = os.getcwd()
    os.chdir(other)  # the program starts in another directory
    try:
        with open("lines.txt", encoding="utf-8") as f:
            print(f.readline())
    except FileNotFoundError as e:
        print("error:", e.strerror)
    finally:
        os.chdir(start)
```

```output
error: No such file or directory
```

Embed the file, or find it through the loader. A path from `__file__` works only while the module is a file on the disk. `importlib.resources` also works for a module in a zip file, where no file `lines.txt` exists:

```python run id=python-resource-in-a-zip
import os
import sys
import tempfile
import zipfile
from importlib import resources

with tempfile.TemporaryDirectory() as d:
    archive = os.path.join(d, "app.zip")
    with zipfile.ZipFile(archive, "w") as z:
        z.writestr("app/__init__.py", "")
        z.writestr("app/lines.txt", "alpha\ncafé\n")
    sys.path.insert(0, archive)
    import app

    print("loader:", type(app.__loader__).__name__)
    text = resources.files("app").joinpath("lines.txt").read_text(encoding="utf-8")
    print("lines:", text.splitlines())
    print("a file on the disk:", os.path.exists(os.path.join(d, "app", "lines.txt")))
```

```output
loader: zipimporter
lines: ['alpha', 'café']
a file on the disk: False
```

- **A large file in source code.** Before `#embed`, C and C++ programs used a list of bytes in source code, and a tool such as `xxd` made it. The author of `#embed` writes that users reach "for utilities such as xxd , writing python scripts, or engaging in highly platform-specific linker calls to set up extern variables pointing at their data"[^5]. Such a list is slow to compile. In the measurements of N3017, a file of 40 megabytes took 1.069 s with `#embed` in GCC, and 225.290 s as an `xxd` list in GCC, 83.250 s in Clang, and "Out of Memory" in MSVC[^5]. GCC used 341.72 MB of memory with `#embed`, and 12,622.00 MB for the `xxd` list[^5]. Keep the generated list for small files, or for a compiler with no `#embed`.
- **A copy that is old.** The program has the bytes that the file had at build time. After a change of the file, build again. The build system must know that the program depends on the data file, or it does not build the program again.
- **The rules of the directive.** In Go, "It can only be used with variables at package scope, not with local variables."[^1] And when a pattern names a directory, "files with names beginning with ‘.’ or ‘_’ are excluded"[^1]. "If any patterns are invalid or have invalid matches, the build will fail."[^1] In Rust, "an invocation with a Windows path containing backslashes \ would not compile correctly on Unix"[^3], so use `/` in the path.
- **An empty or missing file in C.** `#embed` of an empty file gives no values, and an array of unknown size with no values is an error. `__has_embed` "evaluates to __STDC_EMBED_FOUND__ if the search for the resource succeeds, the resource is non empty and all the parameters are supported"[^4], and the `if_empty` parameter gives values for an empty file[^4].
- **A resource that is not there.** In Java and .NET, a wrong name, or a file that the build did not include, gives null at run time, not an error at build time[^7][^8]. Check for null, and test the packaged program.
- **The size of the program.** Each embedded byte is in the program file, so a large file makes a large program. Embed small files, such as templates, schemas and test data. For large data, ship a file and find it from the path of the program.

## Related tasks

- [[read-a-whole-file]]
- [[read-a-file-line-by-line]]
- [[read-a-binary-file]]
- [[check-whether-a-file-exists]]
- [[create-a-temporary-file]]

## References
