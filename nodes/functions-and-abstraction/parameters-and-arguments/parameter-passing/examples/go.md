```go run id=values
package main

import "fmt"

func update(items []int, count int, p *int) {
	items[0] = 100
	count++
	*p++
}

func sum(values ...int) int {
	total := 0
	for _, v := range values {
		total += v
	}
	return total
}

func main() {
	numbers := []int{1, 2, 3}
	count, other := 1, 1
	update(numbers, count, &other)
	fmt.Println(numbers, count, other)
	fmt.Println(sum(), sum(numbers...))
}
```

```output for=values
[100 2 3] 1 2
0 105
```

- Verdict: native. "Everything in Go is passed by value." A function gets a copy of each argument. A pointer is copied, but it points to the same variable, so `*p++` changes the variable of the caller.
- A slice value is a small descriptor with a pointer, a length and a capacity. The copy shares the array, so a change to an element is visible to the caller.
- A variadic parameter `...int` receives a slice. `sum(numbers...)` passes an existing slice without a copy.
- Go has no default values and no named arguments.

```go wrong id=append
package main

import "fmt"

func add(items []int, v int) {
	items = append(items, v)
}

func main() {
	numbers := make([]int, 0, 4)
	add(numbers, 1)
	fmt.Println(numbers, len(numbers))
}
```

```output for=append
[] 0
```

`append` returns a new slice header. The function changes its own copy of the header, and the length of the caller's slice stays 0. Return the new slice, `numbers = add(numbers, 1)`, or pass a pointer to the slice.
