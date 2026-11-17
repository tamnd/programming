```go run id=frames
package main

import "fmt"

func count(n int) {
	here := n * 10 // a local: one copy in each call
	fmt.Printf("enter n=%d here=%d\n", n, here)
	if n > 1 {
		count(n - 1)
	}
	fmt.Printf("leave n=%d here=%d\n", n, here)
}

func main() {
	count(3)
}
```

```output for=frames
enter n=3 here=30
enter n=2 here=20
enter n=1 here=10
leave n=1 here=10
leave n=2 here=20
leave n=3 here=30
```

- Verdict: native. Each call has its own `n` and `here`. The compiler keeps a local in the frame when it can prove that nothing uses it after the return, and moves it to the heap if not.
- The example does not print addresses: to take the address of `here` can change where the compiler puts it.
