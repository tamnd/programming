```go run id=labels
package main

import "fmt"

func main() {
	grid := [][]int{{3, 1}, {4, -1}, {-5, 9}}
	row, col := -1, -1
search:
	for r, line := range grid {
		for c, value := range line {
			if value < 0 {
				row, col = r, c
				break search
			}
		}
	}
	fmt.Println(row, col)
}
```

```output for=labels
1 1
```

- Verdict: native. Go has labeled `break` and `continue`, `defer` for cleanup, and a restricted `goto`. A `goto` must not jump into a block or over a variable declaration.

```go fails expect=compile id=goto-over-declaration
package main

import "fmt"

func main() {
	goto end
	message := "skipped"
	fmt.Println(message)
end:
	fmt.Println("end")
}
```

The compiler rejects the program: `goto end` jumps over the declaration of `message`.
