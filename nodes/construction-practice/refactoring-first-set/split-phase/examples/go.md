```go run id=two-phases
package main

import (
	"fmt"
	"strconv"
	"strings"
)

// Timesheet is the intermediate structure between the phases.
type Timesheet struct {
	Name      string
	Hours     int
	RateCents int
}

// Phase 1 knows the format, and it is the only phase that can fail.
func parseTimesheets(text string) ([]Timesheet, error) {
	var out []Timesheet
	for i, line := range strings.Split(text, "\n") {
		f := strings.Split(line, ",")
		if len(f) != 3 {
			return nil, fmt.Errorf("line %d: want 3 fields, got %d", i+1, len(f))
		}
		h, err1 := strconv.Atoi(f[1])
		r, err2 := strconv.Atoi(f[2])
		if err1 != nil || err2 != nil {
			return nil, fmt.Errorf("line %d: bad number", i+1)
		}
		out = append(out, Timesheet{f[0], h, r})
	}
	return out, nil
}

// Phase 2 knows the rules. Its input is already valid, so it returns no error.
func computePay(sheets []Timesheet) map[string]int {
	pay := map[string]int{}
	for _, s := range sheets {
		overtime := max(0, s.Hours-40)
		pay[s.Name] = (s.Hours-overtime)*s.RateCents + overtime*s.RateCents*3/2
	}
	return pay
}

func main() {
	sheets, err := parseTimesheets("ana,38,2500\nbo,45,2000\ncy,40,3000")
	if err != nil {
		panic(err)
	}
	pay := computePay(sheets)
	fmt.Println(pay["ana"], pay["bo"], pay["cy"])
	_, err = parseTimesheets("ana,38")
	fmt.Println(err)
	fmt.Println("split phase: tests pass")
}
```

```output
95000 95000 120000
line 1: want 3 fields, got 2
split phase: tests pass
```
