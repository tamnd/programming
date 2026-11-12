```go run id=name-the-group
package main

import (
	"fmt"
	"slices"
)

type meeting struct {
	title string
	start int // minutes from midnight
}

// Before: the pair (open, close) travels as two parameters.
func outsideHoursBefore(list []meeting, open, close int) []string {
	var out []string
	for _, m := range list {
		if m.start < open || m.start >= close {
			out = append(out, m.title)
		}
	}
	return out
}

// After: a value type. The fields are unexported, so a window
// from another package can only be made by NewTimeWindow.
type TimeWindow struct{ open, close int }

func NewTimeWindow(open, close int) (TimeWindow, error) {
	if open >= close {
		return TimeWindow{}, fmt.Errorf("open %d is not before close %d", open, close)
	}
	return TimeWindow{open, close}, nil
}

func (w TimeWindow) Includes(minute int) bool { return minute >= w.open && minute < w.close }

func outsideHours(list []meeting, hours TimeWindow) []string {
	var out []string
	for _, m := range list {
		if !hours.Includes(m.start) {
			out = append(out, m.title)
		}
	}
	return out
}

func main() {
	meetings := []meeting{{"standup", 8 * 60}, {"review", 10 * 60}, {"planning", 16 * 60}, {"retro", 18 * 60}}
	office, err := NewTimeWindow(9*60, 17*60)
	if err != nil {
		panic(err)
	}
	got := outsideHours(meetings, office)
	if !slices.Equal(got, outsideHoursBefore(meetings, 9*60, 17*60)) {
		panic("behaviour changed")
	}
	fmt.Println(got)
	_, err = NewTimeWindow(17*60, 9*60)
	fmt.Println(err)
	fmt.Println("introduce parameter object: tests pass")
}
```

```output
[standup retro]
open 1020 is not before close 540
introduce parameter object: tests pass
```
