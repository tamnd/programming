```go run id=graph
package main

import (
	"fmt"
	"slices"
	"strings"
)

func main() {
	edges := [][2]string{{"a", "b"}, {"a", "c"}, {"b", "c"}, {"c", "d"}, {"d", "a"}}

	graph := map[string][]string{}
	var order []string
	for _, e := range edges {
		for _, node := range e {
			if _, ok := graph[node]; !ok {
				graph[node] = []string{}
				order = append(order, node)
			}
		}
		graph[e[0]] = append(graph[e[0]], e[1])
	}

	for _, node := range order {
		fmt.Println(node, "->", strings.Join(graph[node], " "))
	}

	hasEdge := func(x, y string) bool {
		return slices.Contains(graph[x], y)
	}
	yesNo := func(b bool) string {
		if b {
			return "yes"
		}
		return "no"
	}
	fmt.Println("edge a->c:", yesNo(hasEdge("a", "c")))
	fmt.Println("edge c->a:", yesNo(hasEdge("c", "a")))

	inDegree, edgeCount := 0, 0
	for _, neighbors := range graph {
		for _, n := range neighbors {
			if n == "c" {
				inDegree++
			}
		}
		edgeCount += len(neighbors)
	}
	fmt.Println("in-degree of c:", inDegree)
	fmt.Println("edges:", edgeCount)
}
```

```output for=graph
a -> b c
b -> c
c -> d
d -> a
edge a->c: yes
edge c->a: no
in-degree of c: 2
edges: 5
```

- Verdict: idiom. A `map[string][]string` is an adjacency list. The program keeps the nodes in a slice to print them in a fixed order.
- `slices.Contains` tests for an edge.
