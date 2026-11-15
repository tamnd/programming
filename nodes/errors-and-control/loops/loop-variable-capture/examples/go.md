```go run id=per-iteration-variables-and-addresses
package main

import (
	"fmt"
	"sync"
)

func main() {
	// Go 1.22 and later: each pass has its own i.
	var prints []func() int
	for i := 1; i <= 3; i++ {
		prints = append(prints, func() int { return i })
	}
	for _, p := range prints {
		fmt.Print(p(), ";")
	}
	fmt.Println()

	// Each pass also has its own v, at its own address.
	var ptrs []*string
	for _, v := range []string{"a", "b", "c"} {
		ptrs = append(ptrs, &v)
	}
	fmt.Println(*ptrs[0], *ptrs[1], *ptrs[2], ptrs[0] != ptrs[1])

	// A variable declared before the loop is still shared.
	var x int
	var shared []func() int
	for x = 0; x < 3; x++ {
		shared = append(shared, func() int { return x })
	}
	fmt.Println(shared[0](), shared[1](), shared[2]())

	// Goroutines get the value of their pass.
	out := make([]string, 3)
	var wg sync.WaitGroup
	for i, name := range []string{"ann", "bob", "cy"} {
		wg.Add(1)
		go func() {
			defer wg.Done()
			out[i] = fmt.Sprint(i, "=", name)
		}()
	}
	wg.Wait()
	fmt.Println(out)
}
```

```output for=per-iteration-variables-and-addresses
1;2;3;
a b c true
3 3 3
[0=ann 1=bob 2=cy]
```

- Verdict: native. With `go 1.22` or later in `go.mod`, the closures of the three-part loop print 1, 2 and 3, and the pointers `&v` point to three different variables. A variable declared before the loop with `var x` is not a loop variable, so the closures share it and see 3. The goroutines write to their own slots, so the output has a fixed order. Before Go 1.22, the first loop printed 4 three times.
