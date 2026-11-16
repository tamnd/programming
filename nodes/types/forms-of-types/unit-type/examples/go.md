```go run id=empty-struct
package main

import (
	"fmt"
	"unsafe"
)

func main() {
	// struct{} has one value, struct{}{}, and it needs no storage.
	var u struct{}
	fmt.Println(unsafe.Sizeof(u), u == struct{}{})

	// A map to struct{} is a set: only the keys carry information.
	seen := map[string]struct{}{}
	for _, w := range []string{"b", "a", "b"} {
		seen[w] = struct{}{}
	}
	_, ok := seen["a"]
	fmt.Println(len(seen), ok)

	// A channel of struct{} carries only the fact that something happened.
	done := make(chan struct{})
	go func() {
		fmt.Println("work")
		close(done)
	}()
	<-done
	fmt.Println("done")

	// A function can have no result at all, so its call is not a value.
	f := func() { fmt.Println("no result") }
	f()
	fmt.Printf("%T\n", f)
}
```

```output for=empty-struct
0 true
2 true
work
done
no result
func()
```

- Verdict: idiom. Go has no named unit type, but the empty struct `struct{}` has one value and the size 0. A `map[string]struct{}` is the usual set, and a `chan struct{}` signals an event without data. A function can have no result, as the type `func()` shows, and a call to it cannot appear where a value is needed.
