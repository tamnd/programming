```go run id=around
package main

import "fmt"

func withConnection(name string, block func(conn string) int) int {
	fmt.Println("open", name)
	defer fmt.Println("close", name) // runs even if block panics
	return block(name)               // the caller's work
}

func main() {
	result := withConnection("db", func(conn string) int {
		rows := []string{"a", "b", "c"}
		fmt.Printf("query: %d rows\n", len(rows))
		return len(rows)
	})
	fmt.Println("result:", result)
}
```

```output for=around
open db
query: 3 rows
close db
result: 3
```

- Verdict: idiom. A function takes the block as a function value, and `defer` does the cleanup.
