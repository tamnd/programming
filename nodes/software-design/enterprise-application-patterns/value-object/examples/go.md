```go run id=date-range-value
package main

import (
	"errors"
	"fmt"
)

// Day is a calendar day. A struct of comparable fields is comparable with ==.
type Day struct{ Y, M, D int }

func (t Day) before(u Day) bool {
	if t.Y != u.Y {
		return t.Y < u.Y
	}
	if t.M != u.M {
		return t.M < u.M
	}
	return t.D < u.D
}

func (t Day) String() string { return fmt.Sprintf("%04d-%02d-%02d", t.Y, t.M, t.D) }

// DateRange is a value: unexported fields, no setters, and methods with value receivers.
type DateRange struct{ start, end Day }

func NewDateRange(start, end Day) (DateRange, error) {
	if end.before(start) {
		return DateRange{}, errors.New("the end is before the start")
	}
	return DateRange{start, end}, nil
}

func (r DateRange) Contains(t Day) bool { return !t.before(r.start) && !r.end.before(t) }

// ExtendTo returns a new value; r does not change.
func (r DateRange) ExtendTo(end Day) (DateRange, error) { return NewDateRange(r.start, end) }

func (r DateRange) String() string { return r.start.String() + ".." + r.end.String() }

func yn(b bool) string {
	if b {
		return "yes"
	}
	return "no"
}

func main() {
	a, _ := NewDateRange(Day{2026, 3, 1}, Day{2026, 3, 10})
	b, _ := NewDateRange(Day{2026, 3, 1}, Day{2026, 3, 10})
	fmt.Println("a equals b:", yn(a == b))
	fmt.Println("a and b are the same object:", yn(&a == &b))

	c, _ := a.ExtendTo(Day{2026, 3, 15})
	fmt.Println("a:", a)
	fmt.Println("c:", c)

	rooms := map[DateRange]string{a: "room 4"}
	fmt.Println("the room for b:", rooms[b])
	fmt.Println("a contains 2026-03-05:", yn(a.Contains(Day{2026, 3, 5})))
	fmt.Println("a contains 2026-03-12:", yn(a.Contains(Day{2026, 3, 12})))

	if _, err := NewDateRange(Day{2026, 3, 10}, Day{2026, 3, 1}); err != nil {
		fmt.Println("error:", err)
	}
}
```

```output
a equals b: yes
a and b are the same object: no
a: 2026-03-01..2026-03-10
c: 2026-03-01..2026-03-15
the room for b: room 4
a contains 2026-03-05: yes
a contains 2026-03-12: no
error: the end is before the start
```

- Verdict: native. A struct whose fields are all comparable can be compared with `==` and used as a map key. Unexported fields and a constructor function keep the rule of the value.
- A Go struct is copied when it is assigned or passed, so two variables never share one `DateRange`. `&a == &b` compares the addresses of the two variables.
