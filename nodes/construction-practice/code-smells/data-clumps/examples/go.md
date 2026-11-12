```go run id=date-range
package main

import "fmt"

// Before: each period is two loose numbers.
func overlapsBefore(aStart, aEnd, bStart, bEnd int) bool {
	return aStart <= bEnd && bStart <= aEnd
}

func containsBefore(start, end, day int) bool {
	return start <= day && day <= end
}

// After: the clump is a type, and the rules live in it.
type DateRange struct {
	start, end int
}

func NewDateRange(start, end int) (DateRange, error) {
	if end < start {
		return DateRange{}, fmt.Errorf("range %d..%d ends before it starts", start, end)
	}
	return DateRange{start, end}, nil
}

func (r DateRange) Contains(day int) bool {
	return r.start <= day && day <= r.end
}

func (r DateRange) Overlaps(other DateRange) bool {
	return r.start <= other.end && other.start <= r.end
}

func mustRange(start, end int) DateRange {
	r, err := NewDateRange(start, end)
	if err != nil {
		panic(err)
	}
	return r
}

func check(ok bool, what string) {
	if !ok {
		panic(what)
	}
}

func main() {
	check(overlapsBefore(1, 5, 4, 9), "before overlap")
	check(!overlapsBefore(1, 4, 5, 9), "before gap")
	check(!overlapsBefore(1, 5, 9, 4), "before reversed") // no one is told
	fmt.Println("before: a reversed period gives a wrong answer")

	week := mustRange(1, 5)
	check(week.Overlaps(mustRange(4, 9)), "overlap")
	check(!week.Overlaps(mustRange(6, 9)), "gap")
	check(week.Contains(5) && !week.Contains(6), "contains")
	check(containsBefore(1, 5, 5) == week.Contains(5), "same result")
	check(week == mustRange(1, 5), "equal by value")
	_, err := NewDateRange(9, 4)
	check(err != nil, "a reversed range was accepted")
	fmt.Println("after: tests pass")
}
```

```output
before: a reversed period gives a wrong answer
after: tests pass
```

- The fields are unexported, so code in other packages must call `NewDateRange`, which checks the order. Code in the same package can still write `DateRange{9, 4}`.
- A struct whose fields are all comparable can be compared with `==`, so two ranges with the same fields are equal.
