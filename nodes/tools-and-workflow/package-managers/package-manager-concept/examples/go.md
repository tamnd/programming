```go run id=tiny-package-manager
// A tiny package manager: install with dependencies, refuse to break them, and remove unused ones.
package main

import (
	"fmt"
	"slices"
	"sort"
	"strings"
)

var registry = map[string][]string{
	"app":  {"web", "log"},
	"web":  {"http", "log"},
	"http": {},
	"log":  {},
	"cli":  {"log"},
}

// installed maps a name to true if the user asked for it, false if it came as a dependency.
var installed = map[string]bool{}

func names() []string {
	var list []string
	for name := range installed {
		list = append(list, name)
	}
	sort.Strings(list)
	return list
}

func install(name string, asked bool) {
	if was, ok := installed[name]; ok {
		installed[name] = was || asked
		return
	}
	for _, dep := range registry[name] {
		install(dep, false)
	}
	installed[name] = asked
	fmt.Println("  install", name, label(asked))
}

func label(asked bool) string {
	if asked {
		return "(asked)"
	}
	return "(dependency)"
}

func neededBy(name string) []string {
	var users []string
	for _, p := range names() {
		if slices.Contains(registry[p], name) {
			users = append(users, p)
		}
	}
	return users
}

func remove(name string) {
	if users := neededBy(name); len(users) > 0 {
		fmt.Println("  error:", name, "is needed by", strings.Join(users, ", "))
		return
	}
	delete(installed, name)
	fmt.Println("  remove", name)
}

func autoremove() {
	for changed := true; changed; {
		changed = false
		for _, name := range names() {
			if _, ok := installed[name]; ok && !installed[name] && len(neededBy(name)) == 0 {
				remove(name)
				changed = true
			}
		}
	}
}

func main() {
	steps := [][2]string{{"install", "app"}, {"install", "cli"}, {"remove", "log"},
		{"remove", "app"}, {"autoremove", ""}, {"list", ""}}
	for _, s := range steps {
		fmt.Println(strings.TrimSpace("$ " + s[0] + " " + s[1]))
		switch s[0] {
		case "install":
			install(s[1], true)
		case "remove":
			remove(s[1])
		case "autoremove":
			autoremove()
		default:
			for _, name := range names() {
				fmt.Println(" ", name, label(installed[name]))
			}
		}
	}
}
```

```output
$ install app
  install http (dependency)
  install log (dependency)
  install web (dependency)
  install app (asked)
$ install cli
  install cli (asked)
$ remove log
  error: log is needed by app, cli, web
$ remove app
  remove app
$ autoremove
  remove web
  remove http
$ list
  cli (asked)
  log (dependency)
```

- Verdict: idiom. A `map[string][]string` is the registry, and a `map[string]bool` holds the installed packages.
- Go does not keep the order of a map, so `names` sorts the keys before each loop. Without the sort, the output order changes from run to run.
- `slices.Contains` checks whether a package lists a dependency.
