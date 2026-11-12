```go run id=surprise
package main

import "fmt"

func main() {
	// Since Go 1.22, each iteration has its own i,
	// so each closure prints the value it saw.
	var prints []func()
	for i := 1; i <= 3; i++ {
		prints = append(prints, func() { fmt.Println(i) })
	}
	for _, p := range prints {
		p()
	}
}
```

```output
1
2
3
```
