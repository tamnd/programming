```go run id=escape
package main

import "fmt"

type Counter struct {
	n int
}

var total int

func newCounter() *Counter {
	c := Counter{}
	return &c
}

func main() {
	p := newCounter()
	p.n++
	p.n++
	total += p.n
	fmt.Println(p.n, total)
}
```

```output for=escape
2 2
```

- Verdict: native. A variable lives as long as the program can reach it. `c` is a local variable of `newCounter`, but the function returns its address, so the compiler puts `c` on the heap and it lives on. `total` is a package variable, which lives for the whole run.
