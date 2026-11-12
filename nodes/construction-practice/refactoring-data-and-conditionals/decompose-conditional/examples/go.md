```go run id=name-the-parts
package main

import "fmt"

const graceDays = 3

type Member struct {
	Age  int
	Tier string
}

// PaysFullFee is the condition, with a name that says the rule.
func (m Member) PaysFullFee() bool {
	return !(m.Age < 18 || m.Tier == "staff")
}

func lateFeeBefore(daysLate, priceCents int, m Member) int {
	var fee int
	if daysLate > 3 && !(m.Age < 18 || m.Tier == "staff") {
		fee = min(daysLate*25, priceCents)
	} else if daysLate > 3 {
		fee = min(daysLate*10, 500)
	}
	return fee
}

func lateFee(daysLate, priceCents int, m Member) int {
	if daysLate <= graceDays {
		return 0
	}
	if m.PaysFullFee() {
		return fullFee(daysLate, priceCents)
	}
	return reducedFee(daysLate)
}

func fullFee(daysLate, priceCents int) int { return min(daysLate*25, priceCents) }
func reducedFee(daysLate int) int          { return min(daysLate*10, 500) }

func main() {
	for _, age := range []int{12, 40} {
		for _, tier := range []string{"staff", "public"} {
			for _, d := range []int{0, 3, 4, 30, 200} {
				m := Member{age, tier}
				if lateFee(d, 1999, m) != lateFeeBefore(d, 1999, m) {
					panic(fmt.Sprint("differs: ", m, d))
				}
			}
		}
	}
	fmt.Println("decompose conditional: tests pass")
}
```

```output
decompose conditional: tests pass
```
