```go run id=package-main
package main

import (
	"fmt"
	"os"
)

func main() {
	fmt.Println("Hello, world!")
	fmt.Printf("%s has %d letters\n", "world", len("world"))
	fmt.Println("arguments:", len(os.Args)-1)
}
```

```output for=package-main
Hello, world!
world has 5 letters
arguments: 0
```

- Verdict: native. A Go program is a package named `main` with a function `main` that takes no arguments and returns nothing. `fmt.Println` writes its arguments and a newline, `os.Args` holds the program name and the arguments, and `os.Exit` sets a status other than 0. The program ends when `main` returns, even if other goroutines still run.
