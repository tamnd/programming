```go run id=rename-in-place
package main

import "fmt"

type call struct{ minutes int }

func calc(l []call, x int) float64 {
	n := 0
	for _, e := range l {
		if e.minutes > x {
			n += e.minutes
		}
	}
	return float64(n) / 60
}

func billableHours(calls []call, freeMinutes int) float64 {
	billedMinutes := 0
	for _, c := range calls {
		if c.minutes > freeMinutes {
			billedMinutes += c.minutes
		}
	}
	return float64(billedMinutes) / 60
}

func main() {
	calls := []call{{5}, {30}, {45}}
	for _, f := range []struct {
		name string
		fn   func([]call, int) float64
	}{{"calc", calc}, {"billableHours", billableHours}} {
		if f.fn(calls, 10) != 1.25 || f.fn(nil, 10) != 0 {
			panic(f.name)
		}
		fmt.Println(f.name + ": tests pass")
	}
}
```

```output
calc: tests pass
billableHours: tests pass
```

- The loop variable `c` stays short. Its scope is two lines, and the Go code review guide asks for short names in small scopes. The parameter `freeMinutes` is used further from its declaration, so it gets the longer name.
- gopls implements the rename request of the Language Server Protocol, so any editor that uses gopls can rename a Go symbol in all the files of a module.
- In Go, the first letter of a name decides if it is exported. A rename from `billableHours` to `BillableHours` changes the API of the package, not only the name.
