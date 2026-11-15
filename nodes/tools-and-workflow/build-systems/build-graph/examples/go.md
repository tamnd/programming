```go run id=graph
// A build graph: a topological order, the waves of a parallel build, and a cycle check.
package main

import (
	"fmt"
	"slices"
	"strings"
)

var rules = map[string][]string{
	"app":     {"main.o", "parse.o", "lex.o"},
	"main.o":  {"main.c", "parse.h"},
	"parse.o": {"parse.c", "parse.h", "lex.h"},
	"lex.o":   {"lex.c", "lex.h"},
	"parse.c": {"grammar.y"},
	"parse.h": {"grammar.y"},
}

func nodes(rules map[string][]string) []string {
	seen := map[string]bool{}
	for target, deps := range rules {
		seen[target] = true
		for _, dep := range deps {
			seen[dep] = true
		}
	}
	var names []string
	for n := range seen {
		names = append(names, n)
	}
	slices.Sort(names)
	return names
}

// topologicalOrder uses Kahn's algorithm: a node is ready when all its dependencies are done.
func topologicalOrder(rules map[string][]string) []string {
	waiting := map[string]int{}
	users := map[string][]string{}
	var ready []string
	for _, n := range nodes(rules) {
		waiting[n] = len(rules[n])
		if waiting[n] == 0 {
			ready = append(ready, n)
		}
	}
	for target, deps := range rules {
		for _, dep := range deps {
			users[dep] = append(users[dep], target)
		}
	}
	var order []string
	for len(ready) > 0 {
		slices.Sort(ready)
		n := ready[0]
		ready = ready[1:]
		order = append(order, n)
		for _, user := range users[n] {
			waiting[user]--
			if waiting[user] == 0 {
				ready = append(ready, user)
			}
		}
	}
	return order
}

func waves(rules map[string][]string) [][]string {
	level := map[string]int{}
	var out [][]string
	for _, n := range topologicalOrder(rules) {
		for _, dep := range rules[n] {
			level[n] = max(level[n], level[dep]+1)
		}
		if k := level[n]; k > 0 {
			for len(out) < k {
				out = append(out, nil)
			}
			out[k-1] = append(out[k-1], n)
		}
	}
	for _, wave := range out {
		slices.Sort(wave)
	}
	return out
}

func findCycle(rules map[string][]string) []string {
	state := map[string]int{}
	var path []string
	var visit func(n string) []string
	visit = func(n string) []string {
		state[n] = 1
		path = append(path, n)
		for _, dep := range rules[n] {
			if state[dep] == 1 {
				i := slices.Index(path, dep)
				return append(slices.Clone(path[i:]), dep)
			}
			if state[dep] == 0 {
				if found := visit(dep); found != nil {
					return found
				}
			}
		}
		path = path[:len(path)-1]
		state[n] = 2
		return nil
	}
	for _, n := range nodes(rules) {
		if state[n] == 0 {
			if found := visit(n); found != nil {
				return found
			}
		}
	}
	return nil
}

func main() {
	fmt.Println("order:", strings.Join(topologicalOrder(rules), " "))
	for i, wave := range waves(rules) {
		fmt.Printf("wave %d: %s\n", i+1, strings.Join(wave, " "))
	}
	if found := findCycle(rules); found == nil {
		fmt.Println("cycle: none")
	}
	bad := map[string][]string{"lex.h": {"lex.o"}}
	for k, v := range rules {
		bad[k] = v
	}
	fmt.Println("after lex.h <- lex.o:")
	order := topologicalOrder(bad)
	fmt.Printf("order: %s (%d of %d nodes)\n", strings.Join(order, " "), len(order), len(nodes(bad)))
	fmt.Println("cycle:", strings.Join(findCycle(bad), " -> "))
}
```

```output
order: grammar.y lex.c lex.h lex.o main.c parse.c parse.h main.o parse.o app
wave 1: lex.o parse.c parse.h
wave 2: main.o parse.o
wave 3: app
cycle: none
after lex.h <- lex.o:
order: grammar.y lex.c main.c parse.c parse.h main.o (6 of 10 nodes)
cycle: lex.h -> lex.o -> lex.h
```

- Verdict: idiom. A `map[string][]string` holds the rules. The order of a Go map is not fixed, so `nodes` sorts the names, and the ready list is sorted before each step.
- `waves` reads the nodes in topological order, so the level of each dependency is known before the level of the node that needs it.
- `findCycle` is a closure that calls itself, declared with `var visit func(n string) []string` first. `slices.Clone` copies the cycle out of the path.
