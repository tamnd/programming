```go run id=order-sessions
package main

import (
	"fmt"
	"strconv"
	"strings"
)

type Op struct{ Name, Arg string }

const maxDeliveries = 3

func main() {
	arrivals := []struct{ Session, Name, Arg string }{
		{"A", "create", ""}, {"B", "create", ""}, {"C", "create", ""}, {"A", "add", "10"},
		{"B", "add", "x"}, {"A", "add", "5"}, {"C", "add", "3"}, {"B", "add", "7"},
	}
	var order []string // the sessions in the order they first appear
	sessions := map[string][]Op{}
	for _, a := range arrivals {
		if _, ok := sessions[a.Session]; !ok {
			order = append(order, a.Session)
		}
		sessions[a.Session] = append(sessions[a.Session], Op{a.Name, a.Arg})
	}

	workers := []string{"w1", "w2"}
	holding := map[string]string{} // worker -> session id
	locked := map[string]bool{}
	deliveries := map[string]int{}
	totals := map[string]int{}
	var dead []string
	outOfOrder := 0
	pending := func() bool {
		for _, q := range sessions {
			if len(q) > 0 {
				return true
			}
		}
		return false
	}

	for rnd := 1; pending(); rnd++ {
		for _, w := range workers {
			if holding[w] == "" {
				for _, s := range order {
					if len(sessions[s]) > 0 && !locked[s] {
						locked[s], holding[w] = true, s
						fmt.Printf("%d %s locks %s\n", rnd, w, s)
						break
					}
				}
				if holding[w] == "" {
					continue
				}
			}
			sid := holding[w]
			op := sessions[sid][0]
			label := strings.TrimSpace(fmt.Sprintf("%s %s %s", sid, op.Name, op.Arg))
			ok := true
			switch _, created := totals[sid]; {
			case op.Name == "create":
				totals[sid] = 0
			case !created:
				outOfOrder++
			default:
				n, err := strconv.Atoi(op.Arg)
				if err != nil {
					ok = false
				} else {
					totals[sid] += n
				}
			}
			if ok {
				fmt.Printf("%d %s %s\n", rnd, w, label)
				sessions[sid] = sessions[sid][1:]
				delete(deliveries, sid)
			} else {
				deliveries[sid]++
				if deliveries[sid] < maxDeliveries {
					fmt.Printf("%d %s %s failed, delivery %d\n", rnd, w, label, deliveries[sid])
				} else {
					fmt.Printf("%d %s %s failed, delivery %d: to the dead letter queue\n", rnd, w, label, deliveries[sid])
					dead = append(dead, label)
					sessions[sid] = sessions[sid][1:]
					delete(deliveries, sid)
				}
			}
			if len(sessions[sid]) == 0 {
				delete(locked, sid)
				holding[w] = ""
			}
		}
	}

	var parts []string
	for _, s := range order {
		parts = append(parts, fmt.Sprintf("%s %d", s, totals[s]))
	}
	fmt.Printf("totals: %s; dead letter: %s; out of order: %d\n", strings.Join(parts, ", "), strings.Join(dead, ", "), outOfOrder)
}
```

```output
1 w1 locks A
1 w1 A create
1 w2 locks B
1 w2 B create
2 w1 A add 10
2 w2 B add x failed, delivery 1
3 w1 A add 5
3 w2 B add x failed, delivery 2
4 w1 locks C
4 w1 C create
4 w2 B add x failed, delivery 3: to the dead letter queue
5 w1 C add 3
5 w2 B add 7
totals: A 15, B 7, C 3; dead letter: B add x; out of order: 0
```

- Verdict: idiom. A map of slices holds the sessions, and a separate slice `order` keeps the order of first arrival, because the order of a `range` over a map is not fixed in Go.
- The map `locked` is the session lock; a worker keeps its session until the slice is empty.
- `strconv.Atoi` returns an error for `x`, and the message stays at the head of its session until the third delivery.
