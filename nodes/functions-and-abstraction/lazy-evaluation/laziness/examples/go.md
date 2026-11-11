```go run id=seq
package main

import (
	"fmt"
	"iter"
	"sync"
)

func naturals() iter.Seq[int] {
	return func(yield func(int) bool) {
		for n := 1; ; n++ {
			if !yield(n) {
				return
			}
		}
	}
}

var config = sync.OnceValue(func() string {
	fmt.Println("reading config")
	return "debug"
})

func main() {
	for n := range naturals() {
		if n*n > 50 {
			fmt.Println(n)
			break
		}
	}
	fmt.Println(config(), config())
}
```

```output for=seq
8
reading config
debug debug
```

- Verdict: idiom. Go evaluates eagerly. An iterator function of type `iter.Seq` produces values only while the loop asks for them, and `break` stops the producer.
- `sync.OnceValue` returns a function that computes its value on the first call, and it is safe for use by many goroutines.
