```go run id=pointers
package main

import "fmt"

func reverseInPlace(xs []int) {
	for i, j := 0, len(xs)-1; i < j; i, j = i+1, j-1 {
		xs[i], xs[j] = xs[j], xs[i]
	}
}

func addTo(total *int, amount int) {
	*total += amount
}

func main() {
	data := []int{1, 2, 3, 4, 5}
	reverseInPlace(data)
	fmt.Println(data)

	total := 0
	addTo(&total, 5)
	addTo(&total, 7)
	fmt.Println(total)

	count := 0
	for _, ch := range "imperative" {
		switch ch {
		case 'a', 'e', 'i', 'o', 'u':
			count++
		}
	}
	fmt.Println(count)
}
```

```output for=pointers
[5 4 3 2 1]
12
5
```

- Verdict: native. The Go specification says that "An assignment replaces the current value stored in a variable with a new value specified by an expression." Go has one loop keyword, `for`, and tuple assignment.
- A slice shares its array with the caller, so `reverseInPlace` changes `data`. To change an integer of the caller, the function takes a pointer.
