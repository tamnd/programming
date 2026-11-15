```go run id=tiny-build
// A tiny build system: rules, dependencies first, and a skip when a target is up to date.
package main

import (
	"fmt"
	"strings"
)

type rule struct {
	deps   []string
	action string
}

type entry struct {
	value string
	time  int
}

var rules = map[string]rule{
	"app":    {[]string{"main.o", "util.o"}, "link"},
	"main.o": {[]string{"main.c", "util.h"}, "compile"},
	"util.o": {[]string{"util.c", "util.h"}, "compile"},
}

var store = map[string]entry{}
var clock = 0

func write(key, value string) {
	clock++
	store[key] = entry{value, clock}
}

func build(target string, done map[string]bool, ran *[]string) error {
	if done[target] {
		return nil
	}
	done[target] = true
	r, ok := rules[target]
	if !ok {
		if _, found := store[target]; !found {
			return fmt.Errorf("no rule to make %s", target)
		}
		return nil
	}
	for _, dep := range r.deps {
		if err := build(dep, done, ran); err != nil {
			return err
		}
	}
	old, built := store[target]
	dirty := !built
	for _, dep := range r.deps {
		if store[dep].time > old.time {
			dirty = true
		}
	}
	if dirty {
		write(target, r.action+"("+strings.Join(r.deps, ", ")+")")
		*ran = append(*ran, r.action+" "+target)
	}
	return nil
}

func run(label string) {
	var ran []string
	if err := build("app", map[string]bool{}, &ran); err != nil {
		fmt.Println(label + ": error: " + err.Error())
		return
	}
	if len(ran) == 0 {
		fmt.Println(label + ": nothing to do")
		return
	}
	fmt.Println(label + ": " + strings.Join(ran, ", "))
}

func main() {
	for _, name := range []string{"main.c", "util.c", "util.h"} {
		write(name, "source of "+name)
	}
	run("build 1")
	run("build 2")
	fmt.Println("edit util.h")
	write("util.h", "edited")
	run("build 3")
	fmt.Println("edit main.c")
	write("main.c", "edited")
	run("build 4")
	fmt.Println("delete util.c")
	delete(store, "util.c")
	run("build 5")
}
```

```output
build 1: compile main.o, compile util.o, link app
build 2: nothing to do
edit util.h
build 3: compile main.o, compile util.o, link app
edit main.c
build 4: compile main.o, link app
delete util.c
build 5: error: no rule to make util.c
```

- Verdict: idiom. A map of `rule` structs holds the dependencies and the action of each target, and a map of `entry` structs is the store.
- `build` returns an `error` for an input that has no rule and no value, and each caller passes the error up.
- The targets that ran go into a slice through a pointer, so the caller can print them after the build.
