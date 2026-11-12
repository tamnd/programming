```go run id=counter-module
package main

import "fmt"

type Counter struct {
	Increment func()
	Decrement func()
	Value     func() int
}

var counter = func() Counter {
	privateCounter := 0
	changeBy := func(val int) { privateCounter += val }
	return Counter{
		Increment: func() { changeBy(1) },
		Decrement: func() { changeBy(-1) },
		Value:     func() int { return privateCounter },
	}
}()

func main() {
	fmt.Println("value:", counter.Value())
	counter.Increment()
	counter.Increment()
	fmt.Println("value:", counter.Value())
	counter.Decrement()
	fmt.Println("value:", counter.Value())
}
```

```output
value: 0
value: 2
value: 1
```

- Verdict: native. The Go form is a package `counter` with an unexported variable `privateCounter` and exported functions `Increment`, `Decrement` and `Value`; a name is exported only if it starts with an upper-case letter. The closure here keeps the example in one file.
