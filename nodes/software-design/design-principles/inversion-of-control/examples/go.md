```go run id=who-calls
package main

import "fmt"

// eachLine is the framework: it runs the loop and calls the handler.
func eachLine(source []string, handler func(string)) {
	for _, line := range source {
		handler(line)
	}
}

func main() {
	lines := []string{"alpha", "beta", "gamma"}

	// Your code is in control: it runs the loop.
	for _, line := range lines {
		fmt.Printf("plain: %d\n", len(line))
	}

	// Inverted: the framework calls your handler.
	eachLine(lines, func(line string) {
		fmt.Printf("inverted: %d\n", len(line))
	})
}
```

```output
plain: 5
plain: 4
plain: 5
inverted: 5
inverted: 4
inverted: 5
```
