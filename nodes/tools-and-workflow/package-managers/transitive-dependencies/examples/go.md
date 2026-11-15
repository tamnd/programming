```go run id=dependency-tree
// Print a dependency tree, then find the packages that are shared (a diamond) and the packages with two versions.
package main

import (
	"fmt"
	"maps"
	"slices"
	"strings"
)

var deps = map[string][]string{
	"app":      {"cli 1.0", "web 2.0"},
	"cli 1.0":  {"json 0.9", "log 1.2"},
	"web 2.0":  {"http 1.4", "json 1.1", "log 1.2"},
	"http 1.4": {"log 1.2"},
	"json 0.9": {},
	"json 1.1": {},
	"log 1.2":  {},
}

var seen = map[string]bool{}

func show(pkg, prefix string) {
	children := deps[pkg]
	for i, child := range children {
		last := i == len(children)-1
		branch, indent := "+-- ", "|   "
		if last {
			branch, indent = "`-- ", "    "
		}
		if seen[child] {
			fmt.Println(prefix + branch + child + " (*)")
			continue
		}
		fmt.Println(prefix + branch + child)
		seen[child] = true
		show(child, prefix+indent)
	}
}

func main() {
	fmt.Println("app")
	show("app", "")
	var transitive []string
	for _, p := range slices.Sorted(maps.Keys(seen)) {
		if !slices.Contains(deps["app"], p) {
			transitive = append(transitive, p)
		}
	}
	fmt.Println("direct:", strings.Join(deps["app"], ", "))
	fmt.Println("transitive:", strings.Join(transitive, ", "))

	users := map[string][]string{}
	for pkg, children := range deps {
		for _, child := range children {
			users[child] = append(users[child], pkg)
		}
	}
	versions := map[string][]string{}
	for _, p := range slices.Sorted(maps.Keys(seen)) {
		name, version, _ := strings.Cut(p, " ")
		versions[name] = append(versions[name], version)
	}
	fmt.Println("needed by more than one package:")
	for _, p := range slices.Sorted(maps.Keys(users)) {
		if len(users[p]) > 1 {
			slices.Sort(users[p])
			fmt.Printf("  %s: %s\n", p, strings.Join(users[p], ", "))
		}
	}
	fmt.Println("more than one version:")
	for _, name := range slices.Sorted(maps.Keys(versions)) {
		if len(versions[name]) > 1 {
			fmt.Printf("  %s: %s\n", name, strings.Join(versions[name], ", "))
		}
	}
}
```

```output
app
+-- cli 1.0
|   +-- json 0.9
|   `-- log 1.2
`-- web 2.0
    +-- http 1.4
    |   `-- log 1.2 (*)
    +-- json 1.1
    `-- log 1.2 (*)
direct: cli 1.0, web 2.0
transitive: http 1.4, json 0.9, json 1.1, log 1.2
needed by more than one package:
  log 1.2: cli 1.0, http 1.4, web 2.0
more than one version:
  json: 0.9, 1.1
```

- Verdict: idiom. A map of slices holds the graph, and a `map[string]bool` is the set of shown packages.
- Go map order is random, so `slices.Sorted(maps.Keys(...))` sorts the keys before each loop that prints.
- `strings.Cut` splits a package into its name and its version at the first space.
