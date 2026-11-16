```go run id=iota-consts
package main

import "fmt"

type Weekday int

const (
	Sunday Weekday = iota
	Monday
	Tuesday
)

func (d Weekday) String() string {
	switch d {
	case Sunday:
		return "Sunday"
	case Monday:
		return "Monday"
	case Tuesday:
		return "Tuesday"
	}
	return fmt.Sprintf("Weekday(%d)", int(d))
}

func main() {
	fmt.Println(Sunday, Monday, Tuesday)
	var d Weekday = 9 // an untyped constant converts without a cast
	fmt.Println(d, d == Tuesday)
	n := 1
	fmt.Println(Weekday(n))
}
```

```output for=iota-consts
Sunday Monday Tuesday
Weekday(9) false
Monday
```

- Verdict: idiom. Go has no literal types and no closed enums. `iota` gives the constants 0, 1 and 2 of the named type `Weekday`, but any integer of that type is allowed: the untyped constant 9 converts without a cast, and a conversion turns any `int` into a `Weekday`.
