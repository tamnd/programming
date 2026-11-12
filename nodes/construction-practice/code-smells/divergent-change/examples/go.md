```go run id=split-phase-report
package main

import (
	"fmt"
	"strings"
)

type sale struct {
	region string
	amount int
}

// Before: the computation and the format are mixed.
func reportBefore(sales []sale, format string) string {
	csv := format == "csv"
	total := 0
	byRegion := map[string]int{}
	var order []string
	var out strings.Builder
	if csv {
		out.WriteString("region,amount\n")
	}
	for _, s := range sales {
		total += s.amount
		if _, ok := byRegion[s.region]; !ok {
			order = append(order, s.region)
		}
		byRegion[s.region] += s.amount
	}
	for _, r := range order {
		if csv {
			fmt.Fprintf(&out, "%s,%d\n", r, byRegion[r])
		} else {
			fmt.Fprintf(&out, "%s: %d\n", r, byRegion[r])
		}
	}
	if csv {
		fmt.Fprintf(&out, "total,%d\n", total)
	} else {
		fmt.Fprintf(&out, "Total: %d\n", total)
	}
	return out.String()
}

// After: phase 1 computes a summary, phase 2 renders it.
type regionTotal struct {
	region string
	amount int
}

type summary struct {
	byRegion []regionTotal
	total    int
}

func summarize(sales []sale) summary {
	index := map[string]int{}
	var s summary
	for _, x := range sales {
		i, ok := index[x.region]
		if !ok {
			i = len(s.byRegion)
			index[x.region] = i
			s.byRegion = append(s.byRegion, regionTotal{x.region, 0})
		}
		s.byRegion[i].amount += x.amount
		s.total += x.amount
	}
	return s
}

var renderers = map[string]func(summary) string{
	"text": func(s summary) string {
		var b strings.Builder
		for _, r := range s.byRegion {
			fmt.Fprintf(&b, "%s: %d\n", r.region, r.amount)
		}
		fmt.Fprintf(&b, "Total: %d\n", s.total)
		return b.String()
	},
	"csv": func(s summary) string {
		var b strings.Builder
		b.WriteString("region,amount\n")
		for _, r := range s.byRegion {
			fmt.Fprintf(&b, "%s,%d\n", r.region, r.amount)
		}
		fmt.Fprintf(&b, "total,%d\n", s.total)
		return b.String()
	},
}

func report(sales []sale, format string) string { return renderers[format](summarize(sales)) }

func main() {
	sales := []sale{{"north", 100}, {"south", 50}, {"north", 25}}
	for _, f := range []string{"text", "csv"} {
		if report(sales, f) != reportBefore(sales, f) {
			panic(f)
		}
	}
	if report(sales, "text") != "north: 125\nsouth: 50\nTotal: 175\n" || summarize(sales).total != 175 {
		panic("after")
	}
	fmt.Println("report: tests pass")
}
```

```output
report: tests pass
```

- A Go map has no defined order of iteration, so both versions keep the regions in a slice in the order they first appear. The summary stores that order, and the renderers do not need to know about it.
- The `if csv` checks in the "before" version appear three times. Each new format would add a branch to each of them.
- The renderers are plain functions in a map. A new format needs no new type.
