```go run id=deep
package main

import "fmt"

func depth(n int) int {
	if n == 0 {
		return 0
	}
	return 1 + depth(n-1) // one frame for each step
}

func loop(n int) int {
	count := 0
	for n > 0 { // one frame for the whole loop
		n--
		count++
	}
	return count
}

func main() {
	fmt.Println("depth(100) =", depth(100))
	fmt.Println("depth(1000000) =", depth(1_000_000))
	fmt.Println("loop(1000000) =", loop(1_000_000))
}
```

```output for=deep
depth(100) = 100
depth(1000000) = 1000000
loop(1000000) = 1000000
```

- Verdict: native. A goroutine stack starts small and grows as the calls need it, so a million frames fit. Past the limit that `debug.SetMaxStack` sets, the program crashes.
