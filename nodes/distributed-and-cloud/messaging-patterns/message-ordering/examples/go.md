```go run id=resequence
package main

import (
	"fmt"
	"sort"
	"strings"
)

type msg struct {
	key string
	seq int
}

func main() {
	// Messages arrive with a key and a sequence number for each key, out of order and with one copy.
	arrivals := []msg{{"A", 1}, {"A", 3}, {"B", 1}, {"A", 2}, {"B", 3}, {"A", 3}, {"B", 2}, {"A", 4}}

	// expected maps a key to the next sequence number to deliver.
	expected := map[string]int{}
	// held maps a key to the messages that wait, by sequence number.
	held := map[string]map[int]string{}
	for _, m := range arrivals {
		next, ok := expected[m.key]
		if !ok {
			next = 1
		}
		name := fmt.Sprintf("%s%d", m.key, m.seq)
		if held[m.key] == nil {
			held[m.key] = map[int]string{}
		}
		if _, dup := held[m.key][m.seq]; dup || m.seq < next {
			fmt.Printf("%s: copy, dropped\n", name)
			continue
		}
		held[m.key][m.seq] = name
		var out []string
		for {
			v, ok := held[m.key][next]
			if !ok {
				break
			}
			out = append(out, v)
			delete(held[m.key], next)
			next++
		}
		expected[m.key] = next
		if len(out) > 0 {
			fmt.Printf("%s: deliver %s\n", name, strings.Join(out, " "))
		} else {
			fmt.Printf("%s: hold, waiting for %s%d\n", name, m.key, next)
		}
	}
	left := 0
	for _, h := range held {
		left += len(h)
	}
	keys := make([]string, 0, len(expected))
	for k := range expected {
		keys = append(keys, k)
	}
	sort.Strings(keys)
	parts := make([]string, len(keys))
	for i, k := range keys {
		parts[i] = fmt.Sprintf("%s%d", k, expected[k])
	}
	fmt.Printf("held at the end: %d; next: %s\n", left, strings.Join(parts, ", "))
}
```

```output
A1: deliver A1
A3: hold, waiting for A2
B1: deliver B1
A2: deliver A2 A3
B3: hold, waiting for B2
A3: copy, dropped
B2: deliver B2 B3
A4: deliver A4
held at the end: 0; next: A5, B4
```

- Verdict: idiom. A map of maps holds the early messages for each key; the two-value form `next, ok := expected[key]` tells a new key from a key whose next number is stored.
- The order of iteration over a Go map is not specified, so the program sorts the keys with `sort.Strings` before it prints the next numbers.
- `delete` removes each held message as the loop delivers it.
