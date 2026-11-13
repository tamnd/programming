```go run id=arrays-and-slices-of-slices
package main

import "fmt"

func main() {
	// [2][3]int is an array of arrays. It is one value in one block, and assignment copies it.
	var a [2][3]int
	a[0][1] = 5
	b := a
	b[1][2] = 9
	fmt.Println(a, b, len(a), len(a[0]))

	// A slice of slices: allocate each row on its own, so rows can grow and differ in length.
	rows, cols := 2, 3
	grid := make([][]int, rows)
	for i := range grid {
		grid[i] = make([]int, cols)
	}
	grid[1] = append(grid[1], 4)
	fmt.Println(grid, len(grid[0]), len(grid[1]))

	// One allocation: the rows are slices of a single backing array.
	cells := make([]int, rows*cols)
	pic := make([][]int, rows)
	for i := range pic {
		pic[i], cells = cells[:cols], cells[cols:]
	}
	pic[1][0] = 7
	fmt.Println(pic)

	// A flat slice with an index formula is a dense matrix in row-major order.
	flat := make([]int, rows*cols)
	for r := range rows {
		for c := range cols {
			flat[r*cols+c] = 10*r + c
		}
	}
	fmt.Println(flat, flat[1*cols+2])
}
```

```output
[[0 5 0] [0 0 0]] [[0 5 0] [0 0 9]] 2 3
[[0 0 0] [0 0 0 4]] 3 4
[[0 0 0] [7 0 0]]
[0 1 2 10 11 12] 12
```

- Verdict: native. [2][3]int is one value, so assignment copies all items. A slice of slices with one allocation per row lets one row grow with append. The single-allocation method from Effective Go points the rows into one backing slice. A flat slice with an index formula is a dense matrix.
