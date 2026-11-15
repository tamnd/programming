```go run id=models
// Two build systems from the same parts: a scheduler and a rebuilder.
// The tasks are a small spreadsheet. B3 has a dynamic dependency.
package main

import (
	"fmt"
	"strconv"
	"strings"
)

type fetchFunc func(key string) int

var tasks = map[string]func(fetch fetchFunc) int{
	"B1": func(fetch fetchFunc) int { return fetch("A1") + fetch("A2") },
	"B2": func(fetch fetchFunc) int { return fetch("B1") * 2 },
	"B3": func(fetch fetchFunc) int {
		if fetch("C1") == 1 {
			return fetch("B2")
		}
		return fetch("A2")
	},
}

// A topological scheduler must know the dependencies before the build.
// For B3 it must list both branches.
var staticDeps = map[string][]string{"B1": {"A1", "A2"}, "B2": {"B1"}, "B3": {"C1", "B2", "A2"}}
var targets = []string{"B2", "B3"}

type dep struct {
	key, hash string
}

type trace struct {
	deps   []dep
	result string
}

// makeLike is a topological scheduler with a dirty bit.
type makeLike struct {
	store map[string]int
	dirty map[string]bool
}

func (m *makeLike) setInput(k string, v int) {
	m.store[k] = v
	// Writing a file sets its time, even with the same content.
	m.dirty[k] = true
}

func (m *makeLike) build() []string {
	var order []string
	seen := map[string]bool{}
	var visit func(k string)
	visit = func(k string) {
		if _, isTask := tasks[k]; seen[k] || !isTask {
			return
		}
		seen[k] = true
		for _, d := range staticDeps[k] {
			visit(d)
		}
		order = append(order, k)
	}
	for _, t := range targets {
		visit(t)
	}
	var ran []string
	read := func(d string) int { return m.store[d] }
	for _, k := range order {
		_, built := m.store[k]
		changed := false
		for _, d := range staticDeps[k] {
			changed = changed || m.dirty[d]
		}
		if !built || changed {
			m.store[k] = tasks[k](read)
			m.dirty[k] = true
			ran = append(ran, k)
		}
	}
	m.dirty = map[string]bool{}
	return ran
}

// shakeLike is a suspending scheduler with verifying traces.
type shakeLike struct {
	store  map[string]int
	traces map[string]trace
}

func (s *shakeLike) setInput(k string, v int) {
	s.store[k] = v
}

func (s *shakeLike) build() []string {
	done := map[string]bool{}
	var ran []string
	var fetch func(k string) int
	fetch = func(k string) int {
		task, isTask := tasks[k]
		if !isTask || done[k] {
			return s.store[k]
		}
		if t, ok := s.traces[k]; ok && strconv.Itoa(s.store[k]) == t.result {
			upToDate := true
			for _, d := range t.deps {
				if strconv.Itoa(fetch(d.key)) != d.hash {
					upToDate = false
					break
				}
			}
			if upToDate {
				done[k] = true
				return s.store[k]
			}
		}
		var deps []dep
		tracked := func(d string) int {
			// The task waits here until d is up to date: this is the suspending scheduler.
			v := fetch(d)
			deps = append(deps, dep{d, strconv.Itoa(v)})
			return v
		}
		v := task(tracked)
		s.store[k] = v
		s.traces[k] = trace{deps, strconv.Itoa(v)}
		done[k] = true
		ran = append(ran, k)
		return v
	}
	for _, t := range targets {
		fetch(t)
	}
	return ran
}

func show(ran []string) string {
	if len(ran) == 0 {
		return "-"
	}
	return strings.Join(ran, " ")
}

func main() {
	inputs := map[string]int{"A1": 10, "A2": 20, "C1": 1}
	m := &makeLike{store: map[string]int{}, dirty: map[string]bool{}}
	s := &shakeLike{store: map[string]int{}, traces: map[string]trace{}}
	for k, v := range inputs {
		m.setInput(k, v)
		s.setInput(k, v)
	}
	type change struct {
		key   string
		value int
	}
	steps := []struct {
		name    string
		changes []change
	}{
		{"clean build", nil},
		{"A2 = 20 again", []change{{"A2", 20}}},
		{"A1 = 11, A2 = 19", []change{{"A1", 11}, {"A2", 19}}},
		{"C1 = 2", []change{{"C1", 2}}},
		{"A1 = 12", []change{{"A1", 12}}},
	}
	fmt.Printf("%-18s%-14s%s\n", "step", "make-like", "shake-like")
	for _, st := range steps {
		for _, c := range st.changes {
			m.setInput(c.key, c.value)
			s.setInput(c.key, c.value)
		}
		fmt.Printf("%-18s%-14s%s\n", st.name, show(m.build()), show(s.build()))
	}
	same := true
	for k := range tasks {
		same = same && m.store[k] == s.store[k]
	}
	fmt.Println("same values:", same)
	fmt.Println("B1, B2, B3 =", s.store["B1"], s.store["B2"], s.store["B3"])
}
```

```output
step              make-like     shake-like
clean build       B1 B2 B3      B1 B2 B3
A2 = 20 again     B1 B2 B3      -
A1 = 11, A2 = 19  B1 B2 B3      B1
C1 = 2            B3            B3
A1 = 12           B1 B2 B3      B1 B2
same values: true
B1, B2, B3 = 31 62 19
```

- Verdict: idiom. A map from each key to a `func(fetch fetchFunc) int` holds the tasks, and two structs hold the two build systems.
- `fetch` is a closure that calls itself, so it is declared with `var fetch func(k string) int` first. The closure `tracked` appends each dependency and its hash to the slice `deps`.
- The order of a Go map is not fixed, so the examples keep the targets and the steps in slices, and the topological order comes from a depth-first search over a slice of dependencies.
