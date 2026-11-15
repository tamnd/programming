---
tag: NDCT0C
kind: practice
slug: hermetic-builds
name: Hermetic builds
aliases: [hermetic build, hermeticity, sandboxed build, build sandboxing, pinned toolchain]
area: tools-and-workflow/build-systems
level: advanced
summary: >-
  Isolate each build step from the host, so that it sees only the inputs and tools that it declares, and the same inputs and configuration always give the same output.
history:
  year: 2015
  people: []
  note: Google built its internal build system Blaze around declared inputs and sandboxed actions, and released an implementation of it as Bazel in 2015.
see_also: [reproducible-builds, remote-caching-and-execution, build-system-concept, build-graph, incremental-builds]
sources:
  - url: https://bazel.build/basics/hermeticity
    title: Bazel documentation, Hermeticity
    locator: Overview; Benefits; Identifying non-hermeticity; Troubleshooting non-hermetic builds
  - url: https://abseil.io/resources/swe-book/html/ch18.html
    title: Erik Kuefler, Build Systems and Build Philosophy, chapter 18 of Software Engineering at Google, edited by Lisa Carey
    locator: the introduction; Sandboxing; Making external dependencies deterministic
  - url: https://bazel.build/docs/sandboxing
    title: Bazel documentation, Sandboxing
    locator: the introduction; the sandboxing strategies; Sandboxing overhead
  - url: https://github.com/containers/bubblewrap/blob/v0.9.0/bwrap.xml
    title: bubblewrap 0.9.0, the bwrap manual page source
    locator: the options --ro-bind, --bind, --symlink, --proc, --dev, --unshare-all, --die-with-parent, --clearenv and --setenv
  - url: https://bazel.build/remote/caching
    title: Bazel documentation, Remote Caching
    locator: Known issues (Environment variables leaking into an action; Bazel does not track tools outside a workspace)
  - url: https://go.dev/doc/toolchain
    title: Go documentation, Go Toolchains
    locator: Introduction; Go toolchain selection
  - url: https://rust-lang.github.io/rustup/overrides.html
    title: The rustup book, Overrides
    locator: the override order; The toolchain file
  - url: https://learn.microsoft.com/en-us/dotnet/core/tools/global-json
    title: Microsoft Learn, global.json overview
    locator: the introduction; rollForward
  - url: https://docs.npmjs.com/cli/v11/commands/npm-ci
    title: npm Docs, npm ci
    locator: Description
  - url: https://github.com/containers/bubblewrap/blob/v0.9.0/README.md
    title: bubblewrap 0.9.0, README
    locator: Sandbox security
---

## Statement

"When given the same input source code and product configuration, a hermetic build system always returns the same output by isolating the build from changes to the host system."[^1] A hermetic build does not depend on the libraries and other software that happen to be on the host. It depends on specific versions of the build tools and of the dependencies.[^1]

## Meaning

The Bazel documentation names two parts of hermeticity.[^1]

- **Isolation.** The build treats its tools as source code. It downloads copies of the tools and keeps them in file trees that it manages, apart from the languages and tools that the user installed on the host.[^1]
- **Source identity.** The build identifies its inputs by content, for example by the hash that a repository such as Git gives to each set of changes.[^1]

In Google's build system, each action runs in a sandbox. The action sees only the files that it declares as inputs and the outputs that it makes, and the system discards any file that it writes but does not declare. The sandbox also keeps actions off the network.[^2] Files from outside the workspace, such as libraries from a package registry, are a risk, because they can change without a change to the source. The answer of the chapter is a manifest for the whole workspace with a cryptographic hash of each external dependency; a download with a different hash stops the build.[^2]

A build step that reads an undeclared input is wrong in a way that is hard to see. Without a sandbox, Bazel does not know that a tool reads the file, so a change to the file does not cause a rebuild, and the incremental build is wrong.[^3] With a shared cache, the wrong result reaches every developer of the project.[^3]

Hermeticity is a means, and a [[reproducible-builds|reproducible build]] is one of the results. Bazel lists four benefits: speed, because outputs can come from a cache; parallel execution; several builds with different tools on one machine; and reproducibility.[^1]

## Examples

The script below runs a build step three times. The step declares one input, `hello.txt`, but it also reads a file of the host. It uses bubblewrap (`bwrap`), a tool that builds a sandbox from the options on its command line. `--ro-bind` mounts a host path read-only at a path in the sandbox, `--bind` mounts it read-write, `--unshare-all` unshares all the namespaces that it can, which include the network namespace, and `--clearenv` removes all environment variables except `PWD`.[^4] The sandbox sees `/usr`, the inputs at `/in`, the output directory at `/out`, and nothing else.

```bash run id=sandbox
w=$(mktemp -d)
mkdir -p "$w/src" "$w/host" "$w/out"
echo "hello" >"$w/src/hello.txt"
echo "a header from the host" >"$w/host/extra.txt"
# The step declares one input, IN/hello.txt, but it also reads a file of the host.
printf 'cat "$1/hello.txt" %s >"$2/result.txt"\n' "$w/host/extra.txt" >"$w/src/step.sh"

# sandbox [OPTIONS]: run the step with the declared inputs only.
sandbox() {
  bwrap --ro-bind /usr /usr --symlink usr/bin /bin --symlink usr/lib /lib \
    --symlink usr/lib64 /lib64 --proc /proc --dev /dev \
    --unshare-all --die-with-parent --clearenv --setenv PATH /usr/bin \
    --ro-bind "$w/src" /in --bind "$w/out" /out "$@" bash /in/step.sh /in /out
}

bash "$w/src/step.sh" "$w/src" "$w/out" && echo "host: $(paste -sd' ' "$w/out/result.txt")"
rm -f "$w/out/result.txt"
sandbox 2>&1 | sed "s#$w#\$w#g; s/^/sandbox: /"
echo "sandbox exit status: ${PIPESTATUS[0]}"
sandbox --ro-bind "$w/host/extra.txt" "$w/host/extra.txt" &&
  echo "declared: $(paste -sd' ' "$w/out/result.txt")"
rm -rf "${w:?}"
```

```output for=sandbox
host: hello a header from the host
sandbox: cat: $w/host/extra.txt: No such file or directory
sandbox exit status: 1
declared: hello a header from the host
```

On the host, the step works and nobody sees the extra input. In the sandbox, the step fails, which shows the hidden input. When the build declares the file, the step works again, and the build now knows the file as an input. Bazel's `linux-sandbox` uses Linux namespaces in the same way to isolate each action from the host.[^3]

Environment variables are inputs too. The step below prints `CFLAGS`. On the host, it sees the value from the shell of the user. In a sandbox with a clear environment, it sees no value, or only the value that the build sets with `--setenv`.

```bash run id=environment
# A step that reads an environment variable that nobody declared.
step='echo "flags: ${CFLAGS:-none}"'
export CFLAGS=-O0
bash -c "$step"
bwrap --ro-bind /usr /usr --symlink usr/bin /bin --symlink usr/lib /lib \
  --symlink usr/lib64 /lib64 --unshare-all --clearenv --setenv PATH /usr/bin \
  bash -c "$step"
bwrap --ro-bind /usr /usr --symlink usr/bin /bin --symlink usr/lib /lib \
  --symlink usr/lib64 /lib64 --unshare-all --clearenv --setenv PATH /usr/bin \
  --setenv CFLAGS -O2 bash -c "$step"
```

```output for=environment
flags: -O0
flags: none
flags: -O2
```

Bazel puts only the environment variables that `--action_env` allows into an action definition, so that machines with different values of a variable such as `PATH` can share cache hits.[^5]

Most language tools have a file in the repository that pins the tool or the dependencies:

| Tool | File | Effect |
| --- | --- | --- |
| Go | `go.mod`, the `go` and `toolchain` lines | the `go` line sets the minimum Go version, the `toolchain` line sets a preferred toolchain, and the `go` command can download that toolchain[^6] |
| Rust | `rust-toolchain.toml` | the `channel` key of the `[toolchain]` table names the toolchain for the directory tree[^7] |
| .NET | `global.json` | selects the version of the .NET SDK, and `rollForward` sets the range of versions that it accepts[^8] |
| npm | `package-lock.json` with `npm ci` | `npm ci` stops with an error if the lock file does not match `package.json`, and "It will never write to package.json or any of the package-locks"[^9] |

The last script asks for a Go version that is not installed. With `GOTOOLCHAIN=local`, the `go` command uses only its bundled toolchain.[^6] It does not build with the wrong version: it stops.

```bash run id=go-toolchain
d=$(mktemp -d)
cd "$d"
printf 'module example.com/hello\n\ngo 1.99\n' >go.mod
printf 'package main\n\nfunc main() {}\n' >main.go
GOTOOLCHAIN=local go build . 2>&1
echo "exit status: $?"
cd /
rm -rf "${d:?}"
```

```output for=go-toolchain
go: go.mod requires go >= 1.99 (running go 1.26.5; GOTOOLCHAIN=local)
exit status: 1
```

With the default setting `GOTOOLCHAIN=auto`, the same `go` command looks for the requested toolchain in the `PATH`, or downloads and caches a copy of it.[^6]

## Criticism and limits

- **Hermeticity is a matter of degree.** Bazel does not track tools outside the workspace, such as a compiler in `/usr/bin/`. Two users with different compilers can then share cache hits wrongly, because the outputs differ but the action hash is the same.[^5] The example above also binds the `/usr` of the host into the sandbox, so the tools there are undeclared inputs.
- **A sandbox costs time and caches.** On Linux, sandboxed builds are rarely more than a few percent slower, but sandboxing effectively turns off any cache that a tool keeps for itself.[^3]
- **Sandboxes depend on the platform.** `linux-sandbox` and `darwin-sandbox` do not work when they are nested, because of limits in the mechanisms of the operating system.[^3] bubblewrap is not a complete sandbox with a security policy; the program that calls it must choose the options.[^10]
- **Every tool and dependency must be declared.** The Bazel documentation says that a move to Bazel is easier when the existing build is made more hermetic first. Common sources of non-hermeticity are actions that write build IDs or timestamps, system binaries that differ between hosts, and builds that write to the source tree.[^1]

## History

At Google, Blaze is the main component of the build system. In 2015, Google released an implementation of Blaze as open source, with the name Bazel.[^2] The book chapter describes how its sandboxed actions and its hashed external dependencies make builds hermetic.[^2] Starting with Go 1.21, the Go distribution consists of a `go` command and a bundled toolchain, and the `go` command selects a toolchain from the `go` and `toolchain` lines.[^6]

## References
