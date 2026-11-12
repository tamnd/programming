```go run id=parameter-object
package main

import "fmt"

// Before: six positional parameters.
func stayPriceBefore(startDay, endDay, nightlyRate, breakfastRate, guests int, withBreakfast bool) int {
	nights := endDay - startDay
	price := nights * nightlyRate
	if withBreakfast {
		price += nights * guests * breakfastRate
	}
	return price
}

// After: a date range, the room as a whole, and a stay that holds the rest.
type dateRange struct{ start, end int }

func (r dateRange) nights() int { return r.end - r.start }

type room struct{ nightlyRate, breakfastRate int }

type stay struct {
	dates     dateRange
	guests    int
	breakfast bool
}

func stayPrice(s stay, r room) int {
	n := s.dates.nights()
	price := n * r.nightlyRate
	if s.breakfast {
		price += n * s.guests * r.breakfastRate
	}
	return price
}

func main() {
	if stayPriceBefore(10, 13, 8000, 1500, 2, true) != 33000 ||
		stayPriceBefore(10, 13, 1500, 8000, 2, true) != 52500 { // swapped, and no error
		panic("before")
	}
	fmt.Println("stayPriceBefore: tests pass")

	r := room{nightlyRate: 8000, breakfastRate: 1500}
	s := stay{dates: dateRange{start: 10, end: 13}, guests: 2, breakfast: true}
	noBreakfast := s
	noBreakfast.breakfast = false
	if stayPrice(s, r) != 33000 || stayPrice(noBreakfast, r) != 24000 {
		panic("after")
	}
	fmt.Println("stayPrice: tests pass")
}
```

```output
stayPriceBefore: tests pass
stayPrice: tests pass
```

- Go has no named arguments, but a composite literal can name its fields: `room{nightlyRate: 8000, breakfastRate: 1500}`. The names at the call site do the work that keyword arguments do in Python.
- A struct is a value in Go, so `noBreakfast := s` copies the stay, and the change to the copy does not change `s`.
- The old signature has five parameters of type `int`, so the compiler accepts them in any order. The structs remove that risk.
