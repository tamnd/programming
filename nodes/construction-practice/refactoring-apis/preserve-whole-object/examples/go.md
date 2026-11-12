```go run id=whole-object
package main

import "fmt"

type TempRange struct{ Low, High int }

type Plan struct{ Low, High int }

func withinRangeBefore(p Plan, bottom, top int) bool {
	return bottom >= p.Low && top <= p.High
}

// withinRange gets a copy of the range, so it cannot change the range of the room.
func withinRange(p Plan, r TempRange) bool {
	return r.Low >= p.Low && r.High <= p.High
}

func main() {
	days := TempRange{Low: 18, High: 23}
	plan := Plan{Low: 16, High: 24}
	if withinRange(plan, days) != withinRangeBefore(plan, days.Low, days.High) || withinRange(plan, TempRange{12, 20}) {
		panic("mismatch")
	}
	fmt.Println(withinRange(plan, days))
}
```

```output
true
```
