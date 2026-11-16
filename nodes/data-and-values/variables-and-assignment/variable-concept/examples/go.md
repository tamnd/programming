```go run id=storage
package main

import "fmt"

func main() {
	var count int
	fmt.Println(count)
	count = 3
	name := "Ada"
	p := &count
	*p = *p + 1
	fmt.Println(count, name)
}
```

```output for=storage
0
4 Ada
```

- Verdict: native. `var count int` declares a variable with the zero value of its type, and `name := "Ada"` declares a variable and takes its type from the value. A Go variable is a storage location, so `&count` gives its address, and a write through the pointer `p` changes `count`.
