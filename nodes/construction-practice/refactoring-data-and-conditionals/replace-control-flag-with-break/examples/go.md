```go run id=stop-where-you-know
package main

import "fmt"

// findSeatBefore uses a flag to leave two nested loops.
func findSeatBefore(taken [][]bool) (int, int) {
	row, col := -1, -1
	done := false
	for r := 0; r < len(taken) && !done; r++ {
		for c := 0; c < len(taken[r]) && !done; c++ {
			if !taken[r][c] {
				row, col = r, c
				done = true
			}
		}
	}
	return row, col
}

// findSeat leaves both loops with a labeled break.
func findSeat(taken [][]bool) (int, int) {
	row, col := -1, -1
rows:
	for r := range taken {
		for c := range taken[r] {
			if !taken[r][c] {
				row, col = r, c
				break rows
			}
		}
	}
	return row, col
}

func main() {
	halls := [][][]bool{
		{{true, true}, {true, false}, {false, false}},
		{{true}, {true}},
		{},
	}
	for _, h := range halls {
		r1, c1 := findSeat(h)
		r2, c2 := findSeatBefore(h)
		if r1 != r2 || c1 != c2 {
			panic(fmt.Sprint("differs: ", h))
		}
	}
	fmt.Println("replace control flag with break: tests pass")
}
```

```output
replace control flag with break: tests pass
```
