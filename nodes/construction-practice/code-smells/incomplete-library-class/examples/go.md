```go run id=write-it-once
package main

import (
	"fmt"
	"time"
)

func day(s string) time.Time {
	t, err := time.Parse(time.DateOnly, s)
	if err != nil {
		panic(err)
	}
	return t
}

// Before: each client writes the missing operation inline.
func dueDateBefore(invoice time.Time) time.Time {
	d := invoice.AddDate(0, 0, 1)
	for d.Weekday() == time.Saturday || d.Weekday() == time.Sunday {
		d = d.AddDate(0, 0, 1)
	}
	return d
}

func shipDateBefore(order time.Time) time.Time {
	d := order.AddDate(0, 0, 1)
	for d.Weekday() == time.Sunday { // forgot Saturday
		d = d.AddDate(0, 0, 1)
	}
	return d
}

// After, one or two operations: a function that takes the library value first.
// Go does not allow a method on time.Time outside the package time.
func NextWorkday(t time.Time) time.Time {
	d := t.AddDate(0, 0, 1)
	for d.Weekday() == time.Saturday || d.Weekday() == time.Sunday {
		d = d.AddDate(0, 0, 1)
	}
	return d
}

// After, many operations: a local extension as a struct that embeds time.Time.
// The methods of time.Time are promoted, and the added methods sit next to them.
type Workday struct{ time.Time }

func (w Workday) IsWorkday() bool {
	return w.Weekday() != time.Saturday && w.Weekday() != time.Sunday
}

func (w Workday) Next() Workday { return Workday{NextWorkday(w.Time)} }

func main() {
	cases := [][2]string{{"2026-11-13", "2026-11-16"}, {"2026-11-14", "2026-11-16"}, {"2026-11-17", "2026-11-18"}}
	for _, c := range cases {
		from, want := day(c[0]), day(c[1])
		if !NextWorkday(from).Equal(want) || !NextWorkday(from).Equal(dueDateBefore(from)) {
			panic("NextWorkday " + c[0])
		}
		if !(Workday{from}).Next().Equal(want) {
			panic("Workday.Next " + c[0])
		}
	}
	if !shipDateBefore(day("2026-11-13")).Equal(day("2026-11-14")) {
		panic("the copy ships on a Saturday")
	}
	// A time.Time from the library must be wrapped before the extra methods
	// can be called, and unwrapped with .Time where the library expects a time.Time.
	w := Workday{day("2026-11-14")}
	if w.IsWorkday() || w.Format(time.DateOnly) != "2026-11-14" || !w.Time.Equal(day("2026-11-14")) {
		panic("Workday")
	}
	fmt.Println("NextWorkday: tests pass")
}
```

```output
NextWorkday: tests pass
```

The struct that embeds `time.Time` is a local extension. The methods of `time.Time`, such as `Format` and `Weekday`, are promoted, so a `Workday` can call them. But a `Workday` is not a `time.Time`: a function that takes a `time.Time` needs `w.Time`, and a value from the library needs to be wrapped. A defined type such as `type Day time.Time` would be worse, because a defined type does not get the methods of its underlying type.
