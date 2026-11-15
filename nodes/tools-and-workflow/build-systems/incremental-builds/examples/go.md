```go run id=rebuild
// Two rebuilders on one graph: file modification times (as in Make) and content hashes.
package main

import (
	"crypto/sha256"
	"fmt"
	"strings"
)

var rules = map[string][]string{
	"main.o": {"main.c", "util.h"},
	"util.o": {"util.c", "util.h"},
	"app":    {"main.o", "util.o"},
}
var order = []string{"main.o", "util.o", "app"}
var sources = map[string]string{
	"main.c": "int main(void) { return util(); }",
	"util.c": "int util(void) { return 1; }",
	"util.h": "int util(void);",
}
var clock = 0

func tick() int {
	clock++
	return clock
}

// action drops comments, so an object file depends only on the code.
func action(target string, content map[string]string) string {
	var parts []string
	for _, dep := range rules[target] {
		code, _, _ := strings.Cut(content[dep], "//")
		parts = append(parts, strings.TrimRight(code, " "))
	}
	return target + "(" + strings.Join(parts, " | ") + ")"
}

func digest(text string) [32]byte {
	return sha256.Sum256([]byte(text))
}

func fromScratch(content map[string]string) string {
	values := map[string]string{}
	for name := range sources {
		values[name] = content[name]
	}
	for _, target := range order {
		values[target] = action(target, values)
	}
	return values["app"]
}

type builder struct {
	byHash  bool
	content map[string]string
	mtime   map[string]int
	// traces maps a target to the hashes of its dependencies at the last build.
	traces map[string]map[string][32]byte
}

func newBuilder(byHash bool) *builder {
	return &builder{byHash, map[string]string{}, map[string]int{}, map[string]map[string][32]byte{}}
}

func (b *builder) write(name, text string, mtime int) {
	b.content[name] = text
	if mtime == 0 {
		mtime = tick()
	}
	b.mtime[name] = mtime
}

func (b *builder) dirty(target string) bool {
	if _, ok := b.content[target]; !ok {
		return true
	}
	for _, dep := range rules[target] {
		if b.byHash && b.traces[target][dep] != digest(b.content[dep]) {
			return true
		}
		if !b.byHash && b.mtime[dep] > b.mtime[target] {
			return true
		}
	}
	return false
}

func (b *builder) build() string {
	var ran []string
	for _, target := range order {
		if b.dirty(target) {
			b.write(target, action(target, b.content), 0)
			seen := map[string][32]byte{}
			for _, dep := range rules[target] {
				seen[dep] = digest(b.content[dep])
			}
			b.traces[target] = seen
			ran = append(ran, target)
		}
	}
	if len(ran) == 0 {
		return "-"
	}
	return strings.Join(ran, " ")
}

func main() {
	steps := []struct {
		label string
		edit  func(b *builder)
	}{
		{"clean build", func(b *builder) {
			for _, name := range []string{"main.c", "util.c", "util.h"} {
				b.write(name, sources[name], 0)
			}
		}},
		{"touch util.h", func(b *builder) {
			b.mtime["util.h"] = tick()
		}},
		{"comment in main.c", func(b *builder) {
			b.write("main.c", sources["main.c"]+" // entry point", 0)
		}},
		// A copy from a backup keeps its old time, older than util.o.
		{"old util.c, old time", func(b *builder) {
			b.write("util.c", "int util(void) { return 2; }", 1)
		}},
	}
	byTime, byHash := newBuilder(false), newBuilder(true)
	fmt.Printf("%-24s%-20s%s\n", "step", "mtime", "hash")
	for _, s := range steps {
		s.edit(byTime)
		s.edit(byHash)
		fmt.Printf("%-24s%-20s%s\n", s.label, byTime.build(), byHash.build())
	}
	fmt.Println("mtime: app matches a clean build:", byTime.content["app"] == fromScratch(byTime.content))
	fmt.Println("hash: app matches a clean build:", byHash.content["app"] == fromScratch(byHash.content))
}
```

```output
step                    mtime               hash
clean build             main.o util.o app   main.o util.o app
touch util.h            main.o util.o app   -
comment in main.c       main.o app          main.o
old util.c, old time    -                   util.o app
mtime: app matches a clean build: false
hash: app matches a clean build: true
```

- Verdict: idiom. `sha256.Sum256` returns a `[32]byte` array. Arrays in Go are comparable with `!=`, so the traces store the arrays as they are.
- `strings.Cut` splits the text at the first `//` and returns the part before it, which is how the "compiler" drops a comment.
- The steps are a slice of structs with a label and a function, and each step runs on both builders.
