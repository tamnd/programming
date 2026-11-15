---
tag: 595JTP
kind: practice
slug: reproducible-builds
name: Reproducible builds
aliases: [reproducible build, deterministic build, bit-for-bit reproducible, SOURCE_DATE_EPOCH, verifiable builds]
area: tools-and-workflow/build-systems
level: advanced
summary: >-
  Make the build give bit-by-bit identical artifacts from the same source, environment and instructions, so that anyone can rebuild a published binary and check it against the source.
history:
  year: 2013
  people: [Mike Perry, Lunar]
  note: GNU tools had reproducible builds in the early 1990s; the Debian effort started at DebConf13 in July 2013, after Mike Perry made the Tor Browser build reproducibly.
see_also: [build-system-concept, hermetic-builds, remote-caching-and-execution, incremental-builds, build-systems-a-la-carte, compute-a-sha-256-hash]
sources:
  - url: https://reproducible-builds.org/docs/definition/
    title: reproducible-builds.org, Definitions
    locator: When is a build reproducible?; Explanations
  - url: https://go.dev/blog/rebuild
    title: Russ Cox, Perfectly Reproducible, Verified Go Toolchains, The Go Blog, 28 August 2023
    locator: the introduction; Making a Build Reproducible; the aside on Ken Thompson; Conclusion
  - paper: doi:10.1145/358198.358210
    title: Ken Thompson, Reflections on trusting trust, Communications of the ACM 27(8), pages 761 to 763, 1984
    locator: the abstract
  - url: https://reproducible-builds.org/docs/timestamps/
    title: reproducible-builds.org, Timestamps
    locator: the introduction and the second section
  - url: https://reproducible-builds.org/docs/stable-outputs/
    title: reproducible-builds.org, Stable order for outputs
    locator: the introduction; Perl; Python
  - url: https://www.gnu.org/software/tar/manual/html_node/Reproducibility.html
    title: GNU tar manual, 8.4 Making tar Archives More Reproducible
    locator: the list of options
  - url: https://reproducible-builds.org/docs/build-path/
    title: reproducible-builds.org, Build path
    locator: the compiler flags -fdebug-prefix-map, -fmacro-prefix-map and -ffile-prefix-map
  - url: https://reproducible-builds.org/specs/source-date-epoch/
    title: Chris Lamb and Ximin Luo, SOURCE_DATE_EPOCH specification, revision 1.1
    locator: Revision History; Specification
  - url: https://github.com/golang/go/blob/go1.26.5/src/cmd/go/alldocs.go
    title: Go 1.26.5 source, cmd/go documentation (alldocs.go)
    locator: Compile packages and dependencies, the -trimpath flag
  - url: https://doc.rust-lang.org/rustc/command-line-arguments.html
    title: The rustc book, Command-line Arguments
    locator: --remap-path-prefix
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/compiler-options/code-generation
    title: Microsoft Learn, C# compiler options that control code generation
    locator: Deterministic
  - url: https://maven.apache.org/guides/mini/guide-reproducible-builds.html
    title: Apache Maven, Configuring for Reproducible Builds
    locator: project.build.outputTimestamp
  - url: https://docs.python.org/3/library/py_compile.html
    title: Python documentation, py_compile, Compile Python source files
    locator: py_compile.compile, the invalidation_mode parameter
  - url: https://reproducible-builds.org/docs/history/
    title: reproducible-builds.org, History
    locator: An old idea; Private property + Snowden effect; Kick-off
---

## Statement

"A build is reproducible if given the same source code, build environment and build instructions, any party can recreate bit-by-bit identical copies of all specified artifacts."[^1] The authors or distributors of the software decide which attributes of the environment, the instructions and the source are relevant, and which artifacts must be identical.[^1]

## Meaning

Most people install software as compiled binaries, which are much harder to inspect than source code. An attacker who replaces the binaries on a download server and leaves the source alone makes a supply chain attack that is hard to see.[^2] When the build is reproducible, anyone can build the published source and compare the result with the published binary. If the bytes are identical, the binary has no changes that are not in the source, and nobody had to disassemble it.[^2] The problem is old: in 1984, Ken Thompson asked "To what extent should one trust a statement that a program is free of Trojan horses?"[^3]

Russ Cox calls an input relevant when the output of a build can change with it, and he calls a build reproducible when it can run again with all the same relevant inputs.[^2] The hard part is the unintentional inputs, which the build uses but nobody meant it to use. "The most common unintentional input in build systems is the current time."[^2] The file system records a time for each file that a build writes, and most archive formats record those times again.[^4] Other unintentional inputs are:

- **Order.** Python sets and dictionaries, Perl hashes, Rust `HashMap` and Ruby `Hash` objects can list their keys in a different order on each run.[^5] The order of entries in a directory is another source, and GNU tar has an option to remove it from an archive.[^6]
- **Paths.** Compilers can record the build directory in debug information, and the C macro `__FILE__` expands to a path.[^7]
- **The environment.** The relevant attributes of the environment usually include the dependencies and their versions, the build flags, and the environment variables that the build uses.[^1]

## Examples

The script below makes two checkouts of the same source with different file times, as two clones made on different days have. It packs each checkout three times with GNU tar and compares each pair of archives with `cmp`.

```bash run id=tar-archives
w=$(mktemp -d)
# checkout DIR TIME: the same two source files, with a different file time.
checkout() {
  mkdir -p "$1/src"
  printf 'print("hello")\n' >"$1/src/main.py"
  printf 'NAME = "hello"\n' >"$1/src/names.py"
  touch -d "@$2" "$1/src/main.py" "$1/src/names.py"
}
checkout "$w/a" 1700000000
checkout "$w/b" 1700086400
export SOURCE_DATE_EPOCH=1700000000
opts=(--sort=name "--mtime=@$SOURCE_DATE_EPOCH" --owner=0 --group=0 --numeric-owner)
for d in a b; do
  cd "$w/$d"
  # 1. A plain archive.
  tar -cf "../$d-1.tar" src
  # 2. The options of GNU tar, but the build records its path and time.
  printf 'path = %s\nbuilt = %s\n' "$PWD" "$(date -u +%FT%TZ)" >src/build-info.txt
  tar "${opts[@]}" -cf "../$d-2.tar" src
  # 3. The options of GNU tar, and the build records SOURCE_DATE_EPOCH.
  printf 'built = %s\n' "$(date -u -d "@$SOURCE_DATE_EPOCH" +%FT%TZ)" >src/build-info.txt
  tar "${opts[@]}" -cf "../$d-3.tar" src
done
cd /
for n in 1 2 3; do
  if cmp -s "$w/a-$n.tar" "$w/b-$n.tar"; then r=identical; else r=differ; fi
  echo "archive $n: $r"
done
rm -rf "${w:?}"
```

```output for=tar-archives
archive 1: differ
archive 2: differ
archive 3: identical
```

Archive 1 differs because tar records the modification time of each file. Archive 2 uses options from the list that the GNU tar manual gives for reproducible archives. `--sort=name` removes the order of the directory entries, `--mtime` sets one time for all members, and `--owner=0 --group=0 --numeric-owner` remove the owner and the user and group names.[^6] The archives still differ, because the build wrote its path and the current time into `build-info.txt`. Archive 3 is identical, because the build took its date from `SOURCE_DATE_EPOCH`. The specification defines this variable as a UNIX timestamp, and says that its value "MUST be reproducible (deterministic) across different executions of the build, depending only on the source code."[^8] It "SHOULD be set to the last modification time of the source", and build processes must use it in place of the current date and time for timestamps that they embed.[^8]

The second example runs one build step twice in a child process, with the hash seed `PYTHONHASHSEED` set to 1 and to 2. The step writes a manifest of package names, first in the order of a set and then sorted.

```python run id=hash-order
import os
import subprocess
import sys

# The build step writes the dependencies of a package to a manifest.
STEP = """
import sys
deps = {"zlib", "openssl", "libffi", "sqlite", "readline", "ncurses", "bzip2", "xz",
        "expat", "gdbm", "tcl", "tk", "uuid", "gettext", "mpdecimal", "libxcrypt"}
names = sorted(deps) if sys.argv[1] == "sorted" else deps
print(",".join(names))
"""


def build(order, seed):
    env = dict(os.environ, PYTHONHASHSEED=str(seed))
    step = subprocess.run([sys.executable, "-c", STEP, order], env=env,
                          capture_output=True, text=True, check=True)
    return step.stdout


for order in ["set", "sorted"]:
    same = build(order, 1) == build(order, 2)
    print(f"{order:<6} seed 1 and seed 2: {'identical' if same else 'differ'}")
print(build("sorted", 1).strip())
```

```output for=hash-order
set    seed 1 and seed 2: differ
sorted seed 1 and seed 2: identical
bzip2,expat,gdbm,gettext,libffi,libxcrypt,mpdecimal,ncurses,openssl,readline,sqlite,tcl,tk,uuid,xz,zlib
```

A fixed `PYTHONHASHSEED` makes the order of dictionaries and sets the same on every run.[^5] For Perl, reproducible-builds.org gives the sort of the keys as the first fix, because then the output does not depend on the seed at all.[^5]

Compilers and build tools have settings for these inputs:

| Tool | Setting | Effect |
| --- | --- | --- |
| GCC and Clang | `-ffile-prefix-map=OLD=NEW` | rewrites path prefixes in debug information and in `__FILE__`[^7] |
| Go | `go build -trimpath` | "remove all file system paths from the resulting executable"[^9] |
| rustc | `--remap-path-prefix FROM=TO` | rewrites source path prefixes in all output, including diagnostics and debug information[^10] |
| C# compiler | `Deterministic` | "Produce byte-for-byte equivalent output from the same input source."[^11] |
| Maven | the property `project.build.outputTimestamp` | turns on the Reproducible Builds mode of plugins[^12] |
| Python `py_compile` | `SOURCE_DATE_EPOCH` in the environment | the default invalidation mode of `.pyc` files becomes `CHECKED_HASH`, not `TIMESTAMP`[^13] |

## Criticism and limits

- **The environment is part of the definition.** A reproducible build does not promise the same bytes on every machine. It promises them for the environment that the authors define, and that environment includes the dependencies and their versions.[^1] Two parties with different compilers can get different outputs.
- **Perfect reproducibility is rare.** Cox calls a build perfectly reproducible when the source files are its only relevant input, and he writes that this is only possible for programs that build themselves, such as compiler toolchains.[^2]
- **Identical bytes do not prove that the code is safe.** A rebuild shows that the binary matches the source. If the tools that a distribution uses to build a package are malicious, a rebuild from clean sources with the same tools gives the same malicious binaries, much like Thompson's compiler attack.[^2] Thompson's own answer was about people: "Perhaps it is more important to trust the people who wrote the software."[^3]
- **The build date has no meaning.** Builds often recorded the time to show which source and tools were used. In a reproducible build, that time is meaningless, and reproducible-builds.org recommends a date that comes from the source instead.[^4]
- **The work spreads over many tools.** Each tool in the chain must stop recording times, paths and orders. Go 1.21.0 was the first Go toolchain with perfectly reproducible builds, after many changes to Go.[^2]

## History

GNU tools had reproducible builds in the early 1990s. In Debian, the idea came up in 2000 and again in 2007, and both times it got little support.[^14] Interest came back with Bitcoin, whose users needed to trust their downloads: the first versions of Gitian, which drives builds with virtual machines and Git, date from 2011.[^14] After the surveillance disclosures of 2013, Mike Perry made the Tor Browser build reproducibly, and Lunar then organized a discussion at DebConf13 in July 2013.[^14] A first mass rebuild found 24% of 5240 Debian source packages reproducible. A second one, made before FOSDEM 2014 with binutils in deterministic mode, found 67% of 6887.[^14] Version 1.0 of the `SOURCE_DATE_EPOCH` specification is dated 1 September 2015.[^8] Cox reports that Thompson told him that his compiler attack was found, because the build of the compiler stopped being reproducible.[^2]

## References
