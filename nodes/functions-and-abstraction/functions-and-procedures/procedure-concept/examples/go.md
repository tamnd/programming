```go run id=procedure-go
package main

import "fmt"

func greet(name string) {
	fmt.Println("Hello, " + name + "!")
}

func main() {
	greet("Ada")
	greet("Alan")
}
```

```output for=procedure-go
Hello, Ada!
Hello, Alan!
```

- Verdict: native. A function with no result list is a procedure. Its call can only be a statement.
