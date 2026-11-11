---
tag: 7D4GF5
kind: language
slug: go
name: Go
area: languages
level: intro
summary: >-
  Go is a general-purpose, compiled language with static, strong typing, garbage collection and goroutines and channels for concurrency. The gc toolchain of the go command is its main implementation.
class: programming
tier: 1
paradigms: [imperative, procedural, concurrent, object-oriented]
typing: {discipline: static, strength: strong, checking: [static, runtime], inference: local}
memory: garbage-collected
execution: [compiled-native]
appeared: 2009
designers: [robert-griesemer, rob-pike, ken-thompson]
influenced_by: [c, pascal, modula, oberon, newsqueak, limbo]
standard: null
reference: {url: "https://go.dev/ref/spec", licence: cc-by-4.0}
licence_of_reference_impl: bsd-3-clause
file_extensions: [go]
hello_world: examples/go.md#hello
same_as: {wikidata: Q37227, wikipedia: Go_(programming_language), pldb: go, linguist: Go, rosetta: null, hopl: null}
sources:
  - url: https://go.dev/ref/spec#Introduction
    title: The Go Programming Language Specification, language version go1.27
    locator: Introduction
    accessed: 2026-11-10
  - url: https://go.dev/doc/faq#creating_a_new_language
    title: Frequently Asked Questions (FAQ), The Go Programming Language
    locator: Why did you create a new language?
    accessed: 2026-11-10
  - url: https://go.dev/doc/faq#principles
    title: Frequently Asked Questions (FAQ), The Go Programming Language
    locator: What are the guiding principles in the design?
    accessed: 2026-11-10
  - url: https://go.dev/doc/faq#What_is_the_purpose_of_the_project
    title: Frequently Asked Questions (FAQ), The Go Programming Language
    locator: What is the purpose of the project?
    accessed: 2026-11-10
  - url: https://go.dev/ref/mod#modules-overview
    title: Go Modules Reference
    locator: Modules, packages, and versions; go.mod files
    accessed: 2026-11-10
  - url: https://pkg.go.dev/slices#Chunk
    title: slices package, Go standard library
    locator: func Chunk
    accessed: 2026-11-10
  - url: https://pkg.go.dev/sync#WaitGroup.Go
    title: sync package, Go standard library
    locator: type WaitGroup; func (*WaitGroup) Go
    accessed: 2026-11-10
  - url: https://go.dev/ref/spec#For_range
    title: The Go Programming Language Specification, language version go1.27
    locator: For statements with range clause
    accessed: 2026-11-10
  - url: https://github.com/github-linguist/linguist/blob/main/lib/linguist/languages.yml
    title: GitHub Linguist, languages.yml
    locator: Go, extensions; Go Module; Go Checksums; Go Workspace
    accessed: 2026-11-10
  - url: https://pkg.go.dev/testing#pkg-overview
    title: testing package, Go standard library
    locator: Overview
    accessed: 2026-11-10
  - url: https://go.dev/ref/spec#Variables
    title: The Go Programming Language Specification, language version go1.27
    locator: Variables
    accessed: 2026-11-10
  - url: https://go.dev/ref/spec#Variable_declarations
    title: The Go Programming Language Specification, language version go1.27
    locator: Variable declarations; Short variable declarations
    accessed: 2026-11-10
  - url: https://go.dev/doc/faq#implements_interface
    title: Frequently Asked Questions (FAQ), The Go Programming Language
    locator: Why doesn't Go have "implements" declarations?
    accessed: 2026-11-10
  - url: https://go.dev/ref/spec#Type_assertions
    title: The Go Programming Language Specification, language version go1.27
    locator: Type assertions
    accessed: 2026-11-10
  - url: https://go.dev/doc/faq#conversions
    title: Frequently Asked Questions (FAQ), The Go Programming Language
    locator: Why does Go not provide implicit numeric conversions?
    accessed: 2026-11-10
  - url: https://go.dev/doc/faq#generics
    title: Frequently Asked Questions (FAQ), The Go Programming Language
    locator: When did Go get generic types?
    accessed: 2026-11-10
  - url: https://go.dev/doc/go1.27#language
    title: Go 1.27 Release Notes
    locator: Changes to the language
  - url: https://go.dev/doc/faq#garbage_collection
    title: Frequently Asked Questions (FAQ), The Go Programming Language
    locator: Why do garbage collection? Won't it be too expensive?
    accessed: 2026-11-10
  - url: https://go.dev/doc/gc-guide#Where_Go_Values_Live
    title: A Guide to the Go Garbage Collector
    locator: Where Go Values Live; Tracing Garbage Collection
    accessed: 2026-11-10
  - url: https://go.dev/doc/faq#no_pointer_arithmetic
    title: Frequently Asked Questions (FAQ), The Go Programming Language
    locator: Why is there no pointer arithmetic?
    accessed: 2026-11-10
  - url: https://go.dev/ref/spec#Package_unsafe
    title: The Go Programming Language Specification, language version go1.27
    locator: Package unsafe
    accessed: 2026-11-10
  - url: https://go.dev/doc/faq#runtime
    title: Frequently Asked Questions (FAQ), The Go Programming Language
    locator: Does Go have a runtime?
    accessed: 2026-11-10
  - url: https://go.dev/doc/faq#Is_Go_an_object-oriented_language
    title: Frequently Asked Questions (FAQ), The Go Programming Language
    locator: Is Go an object-oriented language?
    accessed: 2026-11-10
  - url: https://go.dev/ref/spec#Function_literals
    title: The Go Programming Language Specification, language version go1.27
    locator: Function literals
    accessed: 2026-11-10
  - url: https://go.dev/ref/spec#Go_statements
    title: The Go Programming Language Specification, language version go1.27
    locator: Go statements
    accessed: 2026-11-10
  - url: https://go.dev/doc/faq#csp
    title: Frequently Asked Questions (FAQ), The Go Programming Language
    locator: Why build concurrency on the ideas of CSP?
    accessed: 2026-11-10
  - url: https://go.dev/ref/mem#introduction
    title: The Go Memory Model
    locator: Introduction; Advice; Channel communication
    accessed: 2026-11-10
  - url: https://go.dev/doc/devel/release#policy
    title: Release History, The Go Programming Language
    locator: Release Policy
    accessed: 2026-11-10
  - url: https://go.dev/doc/devel/release
    title: Release History, The Go Programming Language
    locator: go1.27.0 to go1.23.0 and their minor revisions; go1; go1.5; go1.18
    accessed: 2026-11-10
  - url: https://endoflife.date/api/v1/products/go/
    title: endoflife.date API v1, Go
    locator: result.releases
    accessed: 2026-11-10
  - url: https://go.dev/doc/go1compat#introduction
    title: Go 1 and the Future of Go Programs
    locator: Introduction; Expectations
    accessed: 2026-11-10
  - url: https://go.dev/doc/faq#What_compiler_technology_is_used_to_build_the_compilers
    title: Frequently Asked Questions (FAQ), The Go Programming Language
    locator: What compiler technology is used to build the compilers?
    accessed: 2026-11-10
  - url: https://go.dev/doc/install/source#environment
    title: Installing Go from source
    locator: Optional environment variables, $GOOS and $GOARCH
    accessed: 2026-11-10
  - url: https://go.dev/doc/install/gccgo#Releases
    title: Setting up and using gccgo
    locator: Releases
    accessed: 2026-11-10
  - url: https://gcc.gnu.org/gcc-13/changes.html#go
    title: GCC 13 Release Series, Changes, New Features, and Fixes
    locator: New Languages and Language specific improvements, Go
  - url: https://gcc.gnu.org/releases.html
    title: GCC Releases
    locator: GCC Timeline
    accessed: 2026-11-10
  - url: https://gcc.gnu.org/gcc-16/changes.html
    title: GCC 16 Release Series, Changes, New Features, and Fixes
    locator: New Languages and Language specific improvements (no Go entry); GCC 16.2
    accessed: 2026-11-10
  - url: https://gcc.gnu.org/onlinedocs/gcc/Copying.html
    title: Using the GNU Compiler Collection (GCC)
    locator: GNU General Public License, Version 3
    accessed: 2026-11-10
  - url: https://github.com/tinygo-org/tinygo/blob/v0.44.0/README.md
    title: TinyGo README, v0.44.0
    locator: the introduction; License
    accessed: 2026-11-10
  - url: https://github.com/tinygo-org/tinygo/releases/tag/v0.44.0
    title: TinyGo release v0.44.0
    locator: the release notes
    accessed: 2026-11-10
  - url: https://github.com/tinygo-org/tinygo/blob/v0.44.0/compileopts/config.go#L22-L23
    title: TinyGo source, compileopts/config.go at v0.44.0
    locator: MinimumGoMinorVersion and MaximumGoMinorVersion, lines 22 to 23
  - url: https://github.com/gopherjs/gopherjs/releases/tag/v1.21.0
    title: GopherJS release v1.21.0
    locator: the release notes
    accessed: 2026-11-10
  - url: https://github.com/traefik/yaegi/blob/v0.16.3/README.md
    title: Yaegi README, v0.16.3
    locator: the introduction; Features
    accessed: 2026-11-10
  - url: https://pkg.go.dev/cmd/go#pkg-overview
    title: go command documentation
    locator: Overview; Compile packages and dependencies; Test packages
    accessed: 2026-11-10
  - url: https://proxy.golang.org/#services
    title: Go modules services
    locator: the introduction; Services
    accessed: 2026-11-10
  - url: https://go.dev/ref/mod#checksum-database
    title: Go Modules Reference
    locator: Checksum database
    accessed: 2026-11-10
  - url: https://pkg.go.dev/about#adding-a-package
    title: About, Go Packages
    locator: Adding a package
    accessed: 2026-11-10
  - url: https://pkg.go.dev/cmd/gofmt#pkg-overview
    title: gofmt command documentation
    locator: Overview
    accessed: 2026-11-10
  - url: https://pkg.go.dev/cmd/vet#pkg-overview
    title: vet command documentation
    locator: Overview
    accessed: 2026-11-10
  - url: https://go.dev/doc/faq#history
    title: Frequently Asked Questions (FAQ), The Go Programming Language
    locator: What is the history of the project?
    accessed: 2026-11-10
  - url: https://go.dev/doc/go1.5#c
    title: Go 1.5 Release Notes
    locator: No more C
  - url: https://go.dev/doc/go1.18#generics
    title: Go 1.18 Release Notes
    locator: Changes to the language, Generics
  - url: https://go.dev/doc/go1.22#language
    title: Go 1.22 Release Notes
    locator: Changes to the language
  - url: https://github.com/golang/proposal/blob/master/README.md
    title: Proposing Changes to Go, golang/proposal
    locator: The Proposal Process; Language changes; Proposal Review; Consensus and Disagreement
    accessed: 2026-11-10
  - url: https://go.dev/doc/faq#ancestors
    title: Frequently Asked Questions (FAQ), The Go Programming Language
    locator: What are Go's ancestors?
    accessed: 2026-11-10
  - url: https://go.dev/doc/faq#exceptions
    title: Frequently Asked Questions (FAQ), The Go Programming Language
    locator: Why does Go not have exceptions?
    accessed: 2026-11-10
  - url: https://go.dev/ref/spec#Errors
    title: The Go Programming Language Specification, language version go1.27
    locator: Errors; Handling panics
    accessed: 2026-11-10
  - url: https://go.dev/doc/faq#inheritance
    title: Frequently Asked Questions (FAQ), The Go Programming Language
    locator: Why is there no type inheritance?
    accessed: 2026-11-10
  - url: https://go.dev/ref/spec#Index_expressions
    title: The Go Programming Language Specification, language version go1.27
    locator: Map types; Index expressions
    accessed: 2026-11-10
  - url: https://go.dev/doc/faq#nil_error
    title: Frequently Asked Questions (FAQ), The Go Programming Language
    locator: Why is my nil error value not equal to nil?
    accessed: 2026-11-10
  - url: https://go.dev/ref/spec#The_zero_value
    title: The Go Programming Language Specification, language version go1.27
    locator: The zero value
    accessed: 2026-11-10
---

## Overview

Go is a general-purpose language. Its specification says that Go is "designed with systems programming in mind". The language is strongly typed and garbage-collected, and it has explicit support for concurrent programming.[^1]

The designers started Go at Google. They wanted efficient compilation, efficient execution and ease of programming in one language. Go tries to join the ease of programming of a dynamically typed language with the efficiency and safety of a statically typed, compiled language. It also aimed at multicore computers, and at a build of a large program in a few seconds.[^2]

The FAQ names the guiding principles of the design. Go tries to reduce clutter and complexity. It has no forward declarations and no header files. There is no type hierarchy, and the concepts of the language are kept orthogonal.[^3] A further goal was tool support, such as automatic code formatting. Go also tries to remove obstacles to work on large code bases.[^4]

A program is built from packages. A module is a set of packages that are released and versioned together. A `go.mod` file in its root directory declares its path and its dependencies.[^5]

## A first program

```go run id=hello
package main

import "fmt"

func main() {
	fmt.Println("Hello, world!")
}
```

```output for=hello
Hello, world!
```

The program below adds up a list of numbers in parallel. `slices.Chunk` returns an iterator over parts of four elements.[^6] `WaitGroup.Go` calls each function in its own goroutine, and `Wait` blocks until all of them have returned.[^7] Each goroutine sends its partial sum on a buffered channel, and the main goroutine adds up the values that it receives.

```go run id=sum
package main

import (
	"fmt"
	"slices"
	"sync"
)

func main() {
	nums := []int{3, 1, 4, 1, 5, 9, 2, 6, 5, 3}
	sums := make(chan int, len(nums)) // room for every partial sum
	var wg sync.WaitGroup
	for part := range slices.Chunk(nums, 4) {
		wg.Go(func() {
			s := 0
			for _, n := range part {
				s += n
			}
			sums <- s
		})
	}
	wg.Wait()
	close(sums)
	total := 0
	for s := range sums {
		total += s
	}
	fmt.Println("total:", total)
}
```

```output for=sum
total: 39
```

The three goroutines can finish in any order, but the total does not depend on the order. Each closure sees its own `part`, because each iteration of a `for` loop with a `range` clause has its own variables from Go 1.22.[^8]

## The language at a glance

A source file has the extension `.go`. Linguist lists no other extension for Go. The module files `go.mod`, `go.sum` and `go.work` have fixed names. Linguist lists them as the data languages Go Module, Go Checksums and Go Workspace.[^9] A test file has a name that ends in `_test.go`. A normal build leaves it out, and `go test` compiles it.[^10]

**Types.** Each variable has a static type. A variable of interface type also has a dynamic type, the type of the value that it holds.[^11] A declaration with no type gives the variable the type of its initial value. The short form `x := v` does this inside a function.[^12] A type implements an interface when it has the methods of the interface. There is no `implements` declaration, so the FAQ calls this a kind of structural typing.[^13] A type assertion checks the dynamic type of an interface value at run time, and a failed assertion causes a run-time panic.[^14] Go has no implicit numeric conversions, so `int` and `int64` are distinct types.[^15] The Go 1.18 release added type parameters, so functions and types can be generic.[^16] From Go 1.27, a method can declare its own type parameters.[^17] See [[interfaces]], [[parametric-polymorphism]] and [[type-conversion]].

```go run id=types
package main

import (
	"cmp"
	"fmt"
)

type Celsius float64

func (c Celsius) String() string { return fmt.Sprintf("%.1f C", float64(c)) }

func Largest[T cmp.Ordered](xs []T) T {
	m := xs[0]
	for _, x := range xs[1:] {
		m = max(m, x)
	}
	return m
}

func main() {
	var s fmt.Stringer = Celsius(21.5)
	fmt.Println(s)
	n := 3
	f := float64(n) / 2
	fmt.Printf("%T %T %T\n", n, f, s)
	fmt.Println(Largest([]int{3, 9, 4}), Largest([]string{"go", "c", "rust"}))
}
```

```output for=types
21.5 C
int float64 main.Celsius
9 rust
```

`Celsius` has a `String` method, so a `Celsius` value can be stored in a `fmt.Stringer` variable with no declaration. `%T` prints `main.Celsius`, the dynamic type of `s`. The compiler infers the type argument of each call to `Largest` from the slice.

**Memory.** Go manages memory with a garbage collector, and a program never frees memory by hand. The FAQ describes a mark-and-sweep collector that runs in parallel with the program.[^18] The compiler keeps a value on the goroutine stack when it can prove the lifetime of the value. Other values escape to the heap, and the collector finds the live ones from roots such as global variables.[^19] Go has pointers but no pointer arithmetic.[^20] The `unsafe` package gives operations that step outside the type system, and code that uses it must be checked by hand.[^21] See [[garbage-collection]] and [[pointers-and-references]].

**Execution.** Go programs are compiled ahead of time to native machine code. Some variant implementations emit JavaScript or WebAssembly instead. Each program contains the Go runtime, a library that implements garbage collection, concurrency and stack management. The runtime is not a virtual machine.[^22]

**Paradigms.** Go is an imperative, procedural language with built-in concurrency. It has methods and allows an object-oriented style, but it has no type hierarchy. A method can belong to any defined type, not only to a struct.[^23] Function literals are closures.[^24] A `go` statement starts a function call as a goroutine, an independent concurrent thread of control in the same address space.[^25] Goroutines communicate over channels, an idea from the languages that grew out of Hoare's CSP.[^26] The Go memory model says when a read in one goroutine sees a write in another. It asks programs to serialize shared access with channels or with the `sync` and `sync/atomic` packages.[^27] See [[closures]], [[channels]], [[threads]], [[data-races]] and [[mutual-exclusion]].

## The guide

The guide to Go has the 25 parts of the language template. Each row says what the part covers for Go. Only this page exists so far, so the table has no links yet.

| Part | What it covers in Go |
|---|---|
| Overview and history | this page: the design goals, the versions, the implementations, the history |
| Getting started | installing Go, `go run` and `go build`, `go mod init`, the Go Playground |
| Lexical structure and syntax | UTF-8 source, automatic semicolons, exported names, literals, operator precedence |
| Values and types | `bool`, the sized numeric types, `string`, `byte` and `rune`, untyped constants, zero values |
| Variables, binding and scope | `var`, `:=`, `const` and `iota`, blocks, shadowing, the blank identifier |
| Control flow | `if` and `switch` with an init statement, type switches, the forms of `for` and `range`, labels |
| Functions | multiple and named results, variadic parameters, closures, `defer` |
| Built-in collections | arrays, slices, maps, `append`, `copy`, `clear`, the `slices`, `maps` and `iter` packages |
| Strings and text | immutable byte strings, `rune` and UTF-8, `fmt` verbs, `strings`, `strconv`, `regexp` |
| User-defined types | structs and embedding, methods, interfaces and type sets, type parameters, generic methods |
| Errors and exceptions | the `error` interface, `errors.Is` and `errors.As`, wrapping with `%w`, `panic` and `recover` |
| Memory and resources | the garbage collector, escape analysis, `GOGC`, `defer` for cleanup, `unsafe` |
| Modules, packages and builds | packages and imports, `go.mod` and `go.sum`, minimal version selection, workspaces |
| Concurrency and parallelism | goroutines, channels, `select`, `sync`, `sync/atomic`, `context`, the memory model, the race detector |
| Metaprogramming | `reflect`, struct tags, `go generate`, build constraints, compiler directives |
| The standard library | a map of the packages; the api cards hang here |
| I/O, files and the OS | `io`, `bufio`, `os`, `path/filepath`, `os/exec`, `time`, `net/http` |
| Testing | `go test`, the `testing` package, table-driven tests, benchmarks, fuzzing, coverage |
| Tooling | `gofmt`, `go vet`, `go doc`, `pprof`, the gopls language server |
| Ecosystem | pkg.go.dev, the module proxy and the checksum database, the proposal process, libraries by need |
| Performance | the cost model, profiling, escape analysis, inlining, profile-guided optimization |
| Interoperability | cgo and calling C, WebAssembly, `syscall` |
| Idioms and style | gofmt layout, Effective Go, naming, early returns, small interfaces |
| Pitfalls | a nil pointer in a non-nil interface, writes to a nil map, map iteration order, goroutine leaks |
| Versions and migration | the release pages, the Go 1 compatibility promise, the `go` line in `go.mod`, GODEBUG |

## Versions and support

The Go team supports each major release until there are two newer major releases. A supported release gets minor revisions for critical problems, security problems included.[^28] The pages of this guide state facts for Go 1.26 and 1.27, the releases that the Go team supports on 2026-11-10. A fact that holds only from a later version gives that version. Go does not publish a date for its next release.

| Version | Status | First release | Newest release | End of life |
|---|---|---|---|---|
| 1.27 | supported | 2026-08-19 | 1.27.3 (2026-10-19) | at the release of Go 1.29 |
| 1.26 | supported | 2026-02-10 | 1.26.10 (2026-10-19) | at the release of Go 1.28 |
| 1.25 | end of life | 2025-08-12 | 1.25.14 (2026-08-19) | 2026-08-19 |
| 1.24 | end of life | 2025-02-11 | 1.24.13 (2026-02-04) | 2026-02-10 |
| 1.23 | end of life | 2024-08-13 | 1.23.12 (2025-08-06) | 2025-08-12 |

The dates of the releases and of the minor revisions come from the release history.[^29] The end-of-life dates follow from the policy, and endoflife.date gives the same dates.[^30] The Go 1 compatibility promise says that programs written to the Go 1 specification continue to compile and run. The promise is at the source level, not the binary level.[^31]

## Implementations

The FAQ names two production compilers. gc is the default compiler and comes with the `go` command. gccgo is a front end, written in C++, for the GCC back end.[^32] The table gives the newest release of each implementation on 2026-11-10.

| Implementation | Runs on | Go version | Newest release | Licence |
|---|---|---|---|---|
| gc | the `GOOS` and `GOARCH` pairs of the Go release, among them Linux, macOS, Windows, the BSDs and WebAssembly | 1.27 | 1.27.3 (2026-10-19) | BSD-3-Clause |
| gccgo | the targets of GCC | the Go 1.18 standard library, without generics | GCC 16.2 (2026-08-07) | GPL-3.0 |
| TinyGo | microcontrollers, WebAssembly and command-line tools, through LLVM | 1.25 to 1.27 | 0.44.0 (2026-10-28) | BSD-3-Clause |
| GopherJS | browsers; Go compiled to JavaScript | 1.21 | 1.21.0 (2026-06-26) | BSD-2-Clause |
| Yaegi | an interpreter on top of the Go runtime | 1.21 and 1.22, as its README states | 0.16.3 (2026-11-05) | Apache-2.0 |

Sources: gc[^33], gccgo[^34][^35][^36][^37][^38], TinyGo[^39][^40][^41], GopherJS[^42] and Yaegi[^43]. The release notes of GCC 14 to 16 have no Go entry. The Go level of gccgo is therefore the one stated for GCC 12 and 13. The compat tables of the feature pages have columns for gc, gccgo and TinyGo.

## Ecosystem

The `go` command builds, tests, formats and vets code, and it manages modules.[^44] A module declares its path, its Go version and its requirements in `go.mod`.[^5] By default, the `go` command downloads modules through proxy.golang.org. It is a module mirror that Google runs for the Go team.[^45] The `go` command checks each download against the checksum database at sum.golang.org, a transparent log of `go.sum` lines.[^46] pkg.go.dev shows the documentation of public modules, with data from proxy.golang.org.[^47]

| Need | Tool |
|---|---|
| build and run | `go build`, `go run`[^44] |
| manage dependencies | modules, `go get`, `go mod tidy`[^5] |
| download modules | proxy.golang.org and sum.golang.org[^45][^46] |
| find packages and read their documentation | pkg.go.dev[^47] |
| run tests | `go test` and the `testing` package[^10] |
| format | `gofmt`[^48] |
| lint | `go vet`[^49] |

The guide's Ecosystem part lists the main libraries by need, as library cards.

## History and influence

Robert Griesemer, Rob Pike and Ken Thompson started to sketch the goals of the language on a whiteboard on 2007-09-21. By January 2008, Ken Thompson had started work on a compiler that generated C code. Ian Taylor started a GCC front end in May 2008, and Russ Cox joined in late 2008.[^50]

| Date | Event |
|---|---|
| 2007-09-21 | the first sketch of the goals[^50] |
| 2009-11-10 | Go became a public open-source project[^50] |
| 2012-03-28 | Go 1, with the compatibility promise[^29][^31] |
| 2015-08-19 | Go 1.5: the compiler and the runtime written in Go, with no C[^29][^51] |
| 2022-03-15 | Go 1.18: type parameters[^29][^52] |
| 2024-02-06 | Go 1.22: a fresh set of loop variables for each iteration, and `range` over integers[^29][^53] |

The Go project decides changes through the proposal process. An author opens an issue, and a discussion on the issue tracker accepts the proposal, declines it or asks for a design document. A proposal review group of Go team members records when the discussion has reached consensus. When the group finds no consensus, the decision passes to the Go architects. Language changes use the same process with their own template.[^54]

The FAQ says that Go is mostly in the C family for its basic syntax. Its declarations and packages owe much to the Pascal, Modula and Oberon family. Its concurrency takes ideas from Newsqueak and Limbo, languages inspired by Tony Hoare's CSP.[^55] The notion of channels as first-class objects comes from that part of the CSP family.[^26]

## Coming from another language

- Go has no exceptions. A function returns an `error` value as its last result, and the caller checks it. `panic` and `recover` handle truly exceptional conditions.[^56][^57] See [[exception-handling]].
- Go has no classes and no inheritance. A type has methods, and it satisfies every interface whose methods it has, with no declaration.[^58]
- The zero value of a pointer, function, interface, slice, channel or map is `nil`. A nil map reads like an empty map, but an assignment to one of its elements causes a run-time panic.[^59] An interface that holds a nil pointer is not itself nil.[^60] See [[null-references]].
- Every variable starts with the zero value of its type: `false`, `0`, `""` or `nil`, and so on recursively for arrays and structs.[^61]
- The order of a `range` loop over a map is not specified, and it can differ from one loop to the next.[^8]
- From Go 1.22, each iteration of a `for` loop has its own loop variables. A closure no longer shares one variable across iterations.[^53]

## References
