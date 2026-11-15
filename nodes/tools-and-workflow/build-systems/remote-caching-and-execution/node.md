---
tag: SM46N3
kind: technique
slug: remote-caching-and-execution
name: Remote caching and execution
aliases: [remote cache, remote build cache, remote execution, remote build execution, RBE, action cache, content-addressable storage, distributed builds]
area: tools-and-workflow/build-systems
level: advanced
summary: >-
  Key each build action by a hash of its command and inputs, keep the results in a shared action cache and content-addressable store, and run missing actions on remote workers.
see_also: [hermetic-builds, reproducible-builds, build-systems-a-la-carte, incremental-builds, build-graph, compute-a-sha-256-hash]
sources:
  - url: https://bazel.build/remote/caching
    title: Bazel documentation, Remote Caching
    locator: the introduction; Overview; Known issues
  - url: https://bazel.build/basics/distributed-builds
    title: Bazel documentation, Distributed Builds
    locator: Remote caching; Remote execution
  - url: https://github.com/bazelbuild/remote-apis/blob/v2.12.0/build/bazel/remote/execution/v2/remote_execution.proto
    title: Remote Execution API v2.12.0, remote_execution.proto
    locator: the services Execution, ActionCache and ContentAddressableStorage; the messages Action and Digest
  - url: https://abseil.io/resources/swe-book/html/ch18.html
    title: Erik Kuefler, Build Systems and Build Philosophy, chapter 18 of Software Engineering at Google, edited by Lisa Carey
    locator: Remote caching; Remote execution; Distributed builds at Google
  - url: https://bazel.build/remote/rbe
    title: Bazel documentation, Remote Execution Overview
    locator: the introduction
  - url: https://bazel.build/docs/sandboxing
    title: Bazel documentation, Sandboxing
    locator: the introduction (undeclared inputs and remote caching)
  - url: https://docs.gradle.org/current/userguide/build_cache.html
    title: Gradle User Manual, Build Cache
    locator: Overview; Enable the Build Cache; Configuration use cases
  - url: https://ccache.dev/manual/latest.html
    title: Ccache manual
    locator: Description; remote_storage; HTTP storage backend
  - url: https://github.com/golang/go/blob/go1.26.5/src/cmd/go/alldocs.go
    title: Go 1.26.5 source, cmd/go documentation (alldocs.go)
    locator: Build and test caching; Environment variables (GOCACHEPROG)
---

## Purpose

A team builds the same code on many machines: the workstation of each developer and each continuous integration (CI) job. Without a shared cache, each machine compiles every file that it has not compiled before, even if another machine compiled the same file from the same inputs a minute earlier. A remote cache lets the team share build outputs, and "If your build is reproducible, the outputs from one machine can be safely reused on another machine".[^1] Remote execution goes one step further and runs the actions themselves on a pool of workers, not on the machine of the user.[^2]

## Steps

1. **Break the build into actions.** Bazel calls each step of the build an action. Each action has inputs, output names, a command line and environment variables, and declares its inputs and outputs explicitly.[^1]
2. **Compute a key for each action.** The key is a hash of the command and of the contents of all inputs. A key on both the target and a hash of its inputs lets different engineers change the same target at the same time without conflicts in the cache.[^2]
3. **Look up the key in the action cache.** The remote cache stores two types of data: the action cache, "a map of action hashes to action result metadata", and a content-addressable store (CAS) of output files.[^1] In the Remote Execution API, the CAS addresses each blob by its own content, and the action cache addresses each result by the digest of the action that produced it.[^3]
4. **On a hit, download the outputs.** The action result lists the digests of the outputs, and the client gets them from the CAS.[^1]
5. **On a miss, run the action and upload the result.** The system builds the artifact and uploads it to the cache, so that low-level dependencies that seldom change are built once and shared.[^2]
6. **For remote execution, send the action to a worker.** The client sends its requests to a central service, which splits them into actions and schedules them on a pool of workers. The workers share their outputs until the final output goes back to the user.[^2]

## Example

The program below models two machines, A and B. Each machine has the same three source files and builds `greet.o`, `main.o` and `app`. The shared remote cache is a directory with an action cache `ac` and a CAS `cas`. For each action, the program prints `run` when the machine runs the action and `hit` when it gets the output from the cache.

```python run id=python-remote-cache
import hashlib
import json
import os
import tempfile


def digest(data):
    return hashlib.sha256(data).hexdigest()


class RemoteCache:
    """An action cache and a content-addressable store (CAS) in one directory."""

    def __init__(self, root):
        self.ac = os.path.join(root, "ac")
        self.cas = os.path.join(root, "cas")
        os.makedirs(self.ac)
        os.makedirs(self.cas)

    def get_result(self, key):
        path = os.path.join(self.ac, key)
        if not os.path.exists(path):
            return None
        with open(path) as f:
            return json.load(f)

    def put_result(self, key, result):
        with open(os.path.join(self.ac, key), "w") as f:
            json.dump(result, f)

    def get_blob(self, d):
        with open(os.path.join(self.cas, d), "rb") as f:
            return f.read()

    def put_blob(self, data):
        d = digest(data)
        with open(os.path.join(self.cas, d), "wb") as f:
            f.write(data)
        return d


class Machine:
    def __init__(self, cache, compiler=b"gcc 15", key_has_tool=True):
        self.cache = cache
        self.compiler = compiler
        self.key_has_tool = key_has_tool
        self.files = {}

    def action(self, command, inputs, output, work):
        # The key covers the command and the digest of each input file.
        spec = {"command": command,
                "inputs": {n: digest(self.files[n]) for n in sorted(inputs)}}
        if self.key_has_tool:
            spec["tool"] = self.compiler.decode()
        key = digest(json.dumps(spec, sort_keys=True).encode())
        result = self.cache.get_result(key) if self.cache else None
        if result is not None:
            self.files[output] = self.cache.get_blob(result[output])
            return "hit"
        self.files[output] = work(*(self.files[n] for n in inputs))
        if self.cache:
            self.cache.put_result(key, {output: self.cache.put_blob(self.files[output])})
        return "run"

    def compile(self, src, hdr):
        return b"obj(" + self.compiler + b": " + src + hdr + b")"

    def build(self):
        return " ".join([
            self.action("cc -c greet.c", ["greet.c", "greet.h"], "greet.o", self.compile),
            self.action("cc -c main.c", ["main.c", "greet.h"], "main.o", self.compile),
            self.action("cc -o app", ["greet.o", "main.o"], "app", lambda a, b: a + b),
        ])


SOURCES = {"greet.c": b"greet", "greet.h": b"decl", "main.c": b"main"}


def checkout(machine):
    machine.files = dict(SOURCES)
    return machine


def report(label, machine):
    print(f"  {label:<24}{machine.build()}")


with tempfile.TemporaryDirectory() as root:
    print("no cache")
    a, b = checkout(Machine(None)), checkout(Machine(None))
    report("A, clean build", a)
    report("B, clean build", b)

    print("shared remote cache")
    cache = RemoteCache(os.path.join(root, "shared"))
    a, b = checkout(Machine(cache)), checkout(Machine(cache))
    report("A, clean build", a)
    report("B, clean build", b)
    b.files["main.c"] = b"main v2"
    report("B, edit main.c", b)
    a.files["main.c"] = b"main v2"
    report("A, same edit", a)
    print("  same app on A and B:", a.files["app"] == b.files["app"])
    print("  entries:", len(os.listdir(cache.ac)), "action results,",
          len(os.listdir(cache.cas)), "blobs")

    for has_tool in [False, True]:
        print("compiler in the key:", has_tool)
        cache = RemoteCache(os.path.join(root, "tool-%s" % has_tool))
        a = checkout(Machine(cache, b"gcc 14", has_tool))
        c = checkout(Machine(cache, b"gcc 15", has_tool))
        report("A (gcc 14), clean build", a)
        report("C (gcc 15), clean build", c)
        print("  main.o on C:", c.files["main.o"].decode())
```

```output for=python-remote-cache
no cache
  A, clean build          run run run
  B, clean build          run run run
shared remote cache
  A, clean build          run run run
  B, clean build          hit hit hit
  B, edit main.c          hit run run
  A, same edit            hit hit hit
  same app on A and B: True
  entries: 5 action results, 5 blobs
compiler in the key: False
  A (gcc 14), clean build run run run
  C (gcc 15), clean build hit hit hit
  main.o on C: obj(gcc 14: maindecl)
compiler in the key: True
  A (gcc 14), clean build run run run
  C (gcc 15), clean build run run run
  main.o on C: obj(gcc 15: maindecl)
```

Before, with no cache, B runs all three actions again. After, with a shared cache, B gets all three outputs from the cache. When B edits `main.c`, the key of `greet.o` does not change, so only `main.o` and `app` run. A then makes the same edit and gets both changed outputs from B's results. The cache holds five action results and five blobs, one for each distinct output.

The last two parts show the main pitfall. When the key does not include the compiler, machine C, which has a different compiler, gets the object that machine A compiled with `gcc 14`. When the key includes the compiler, C runs its own actions.

The same model in Go, without the last two parts:

```go run id=go-remote-cache
package main

import (
	"crypto/sha256"
	"encoding/hex"
	"encoding/json"
	"fmt"
	"os"
	"path/filepath"
	"sort"
	"strings"
)

func digest(data []byte) string {
	sum := sha256.Sum256(data)
	return hex.EncodeToString(sum[:])
}

// RemoteCache is an action cache and a content-addressable store in one directory.
type RemoteCache struct {
	ac, cas string
}

func NewRemoteCache(root string) *RemoteCache {
	c := &RemoteCache{ac: filepath.Join(root, "ac"), cas: filepath.Join(root, "cas")}
	must(os.MkdirAll(c.ac, 0o755))
	must(os.MkdirAll(c.cas, 0o755))
	return c
}

func (c *RemoteCache) GetResult(key string) (map[string]string, bool) {
	data, err := os.ReadFile(filepath.Join(c.ac, key))
	if err != nil {
		return nil, false
	}
	var result map[string]string
	must(json.Unmarshal(data, &result))
	return result, true
}

func (c *RemoteCache) PutResult(key string, result map[string]string) {
	data, err := json.Marshal(result)
	must(err)
	must(os.WriteFile(filepath.Join(c.ac, key), data, 0o644))
}

func (c *RemoteCache) GetBlob(d string) []byte {
	data, err := os.ReadFile(filepath.Join(c.cas, d))
	must(err)
	return data
}

func (c *RemoteCache) PutBlob(data []byte) string {
	d := digest(data)
	must(os.WriteFile(filepath.Join(c.cas, d), data, 0o644))
	return d
}

type Machine struct {
	cache *RemoteCache
	files map[string][]byte
}

func (m *Machine) action(command string, inputs []string, output string, work func(in ...[]byte) []byte) string {
	// The key covers the command and the digest of each input file.
	names := append([]string(nil), inputs...)
	sort.Strings(names)
	spec := command
	for _, n := range names {
		spec += "\n" + n + " " + digest(m.files[n])
	}
	key := digest([]byte(spec))
	if m.cache != nil {
		if result, ok := m.cache.GetResult(key); ok {
			m.files[output] = m.cache.GetBlob(result[output])
			return "hit"
		}
	}
	var in [][]byte
	for _, n := range inputs {
		in = append(in, m.files[n])
	}
	m.files[output] = work(in...)
	if m.cache != nil {
		m.cache.PutResult(key, map[string]string{output: m.cache.PutBlob(m.files[output])})
	}
	return "run"
}

func compile(in ...[]byte) []byte {
	return []byte("obj(" + string(in[0]) + string(in[1]) + ")")
}

func link(in ...[]byte) []byte {
	return append(append([]byte(nil), in[0]...), in[1]...)
}

func (m *Machine) build() string {
	return strings.Join([]string{
		m.action("cc -c greet.c", []string{"greet.c", "greet.h"}, "greet.o", compile),
		m.action("cc -c main.c", []string{"main.c", "greet.h"}, "main.o", compile),
		m.action("cc -o app", []string{"greet.o", "main.o"}, "app", link),
	}, " ")
}

func checkout(cache *RemoteCache) *Machine {
	files := map[string][]byte{"greet.c": []byte("greet"), "greet.h": []byte("decl"), "main.c": []byte("main")}
	return &Machine{cache: cache, files: files}
}

func report(label string, m *Machine) {
	fmt.Printf("  %-24s%s\n", label, m.build())
}

func must(err error) {
	if err != nil {
		panic(err)
	}
}

func count(dir string) int {
	entries, err := os.ReadDir(dir)
	must(err)
	return len(entries)
}

func main() {
	root, err := os.MkdirTemp("", "cache")
	must(err)
	defer os.RemoveAll(root)

	fmt.Println("no cache")
	a, b := checkout(nil), checkout(nil)
	report("A, clean build", a)
	report("B, clean build", b)

	fmt.Println("shared remote cache")
	cache := NewRemoteCache(filepath.Join(root, "shared"))
	a, b = checkout(cache), checkout(cache)
	report("A, clean build", a)
	report("B, clean build", b)
	b.files["main.c"] = []byte("main v2")
	report("B, edit main.c", b)
	a.files["main.c"] = []byte("main v2")
	report("A, same edit", a)
	fmt.Println("  same app on A and B:", string(a.files["app"]) == string(b.files["app"]))
	fmt.Println("  entries:", count(cache.ac), "action results,", count(cache.cas), "blobs")
}
```

```output for=go-remote-cache
no cache
  A, clean build          run run run
  B, clean build          run run run
shared remote cache
  A, clean build          run run run
  B, clean build          hit hit hit
  B, edit main.c          hit run run
  A, same edit            hit hit hit
  same app on A and B: true
  entries: 5 action results, 5 blobs
```

## When to use

- Many machines build the same code: developers, CI jobs, or both. At Google, many artifacts come from a cache and are not built from scratch.[^2]
- The build is hermetic, so that each action declares all of its inputs. For a remote cache to work, the build system must guarantee that the same set of inputs gives exactly the same output on any machine.[^2]
- The build is large enough that the network is faster than a local compile. The latency of the network and the distance to the cache server decide whether the cache helps.[^4]
- Remote execution helps when one machine is too slow for the whole build. It also gives the team one consistent environment in which actions run.[^5]

## Pitfalls

- **Undeclared inputs.** An action that reads an undeclared file gives a key that does not cover the file. In a shared cache, a bad entry affects every developer on the project.[^6] The example shows this with the compiler: Bazel does not track tools outside a workspace, so two users with different compilers in `/usr/bin/` can wrongly share cache hits.[^1]
- **Environment variables.** An action definition contains its environment variables, so machines with different values of `PATH` do not share cache hits. Bazel includes only the variables that `--action_env` allows.[^1]
- **Writes from untrusted machines.** The Gradle manual describes a setup in which CI builds fill the remote cache and developers only load from it.[^7] Bazel can read from the remote cache without writing to it, with `--remote_upload_local_results=false`.[^1]
- **Files that change during the build.** When an input file changes during a build, Bazel might upload invalid results to the remote cache.[^1]
- **A lost cache.** A remote cache alone is not a distributed build. If the cache is lost, or a low-level change invalidates everything, each machine must build everything again.[^2]

## Tools that do it

| Tool | Cache | Remote use |
| --- | --- | --- |
| Bazel | action cache and CAS; `--disk_cache` uses a local directory[^1] | `--remote_cache` with any HTTP/1.1 server that supports `PUT` and `GET`; remote execution over gRPC[^1][^5] |
| Remote Execution API | the services `ActionCache` and `ContentAddressableStorage`[^3] | the service `Execution`, which runs an action on remote workers[^3] |
| Gradle | task outputs, under a key from the task inputs; enable with `--build-cache`[^7] | a remote HTTP build cache[^7] |
| ccache | results of previous C and C++ compilations[^8] | `remote_storage`, which ccache queries after the local cache[^8] |
| Go | `go` caches build outputs in `GOCACHE`[^9] | `GOCACHEPROG` names a command that implements an external build cache[^9] |
| Google internal | ObjFS stores build outputs; Forge runs actions[^4] | the Forge Scheduler keeps a cache of action results and queues other actions for executors[^4] |

## References
