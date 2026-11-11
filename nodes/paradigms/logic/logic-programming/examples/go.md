```go run id=fixpoint
package main

import (
	"fmt"
	"sort"
)

type pair struct{ x, y string }

func main() {
	parent := []pair{{"zeus", "ares"}, {"ares", "harmonia"}, {"harmonia", "semele"}, {"semele", "dionysus"}}

	ancestor := map[pair]bool{}
	for _, p := range parent {
		ancestor[p] = true
	}
	for changed := true; changed; {
		changed = false
		for _, p := range parent {
			for a := range ancestor {
				if p.y == a.x && !ancestor[pair{p.x, a.y}] {
					ancestor[pair{p.x, a.y}] = true
					changed = true
				}
			}
		}
	}

	var below []string
	for a := range ancestor {
		if a.x == "harmonia" {
			below = append(below, a.y)
		}
	}
	sort.Strings(below)
	fmt.Println(len(ancestor), below)
}
```

```output for=fixpoint
10 [dionysus semele]
```

- Verdict: absent. Go has no logic engine. A map used as a set holds the facts, and the loop is a naive bottom-up evaluation of the rules.
- Adding to a map while ranging over it is allowed in Go, but the new entries may or may not be visited in the same loop. The outer loop repeats until nothing changes, so the result is still complete.
