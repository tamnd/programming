```go run id=copies
package main

import (
	"fmt"
	"slices"
)

type Point struct{ X, Y int }

func main() {
	word := "abc"
	bytes := []byte(word)
	bytes[0] = 'X'
	fmt.Println(word, string(bytes))

	p := Point{1, 2}
	q := p
	q.Y = 5
	fmt.Println(p, q)

	numbers := []int{1, 2, 3}
	alias := numbers
	alias[0] = 9
	clone := slices.Clone(numbers)
	clone[1] = 0
	fmt.Println(numbers, clone)
}
```

```output for=copies
abc Xbc
{1 2} {1 5}
[9 2 3] [9 0 3]
```

- Verdict: idiom. Strings are immutable. `const` declares constants of boolean, numeric and string types only.
- There is no read-only struct, slice or map. The usual form is a value copy, an unexported field with getter methods, or a copy that a function returns.
- An assignment copies a struct value. An assignment of a slice copies only the slice header, so both slices share one array. `slices.Clone` makes a separate array.
- The conversion of a string to `[]byte` makes a fresh array, so a change to the bytes does not change the string.
