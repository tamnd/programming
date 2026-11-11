```go run id=scopes
package main

import "fmt"

var level = "package"

func readsPackage() string {
	return level
}

func caller() string {
	level := "caller"
	return readsPackage() + ", local " + level
}

func main() {
	fmt.Println(caller())

	x := 1
	if x := 2; x > 1 {
		fmt.Println("x in the if:", x)
	}
	fmt.Println("x after the if:", x)
}
```

```output for=scopes
package, local caller
x in the if: 2
x after the if: 1
```

- Verdict: native. Go is lexically scoped using blocks. Each `if`, `for` and `switch` statement is its own implicit block, so the `x` declared in the `if` header is a new variable.
- A short variable declaration `:=` in an inner block declares new variables, even when variables with the same names exist outside. In the same block, `:=` can reuse existing variables if at least one variable on the left is new.

```go wrong id=shadow
package main

import (
	"fmt"
	"strconv"
)

func parsePort(s string) (int, error) {
	port := 8080
	var err error
	if s != "" {
		port, err := strconv.Atoi(s)
		if err != nil {
			return 0, err
		}
		fmt.Println("parsed", port)
	}
	return port, err
}

func main() {
	port, err := parsePort("9090")
	fmt.Println(port, err)
}
```

```output for=shadow
parsed 9090
8080 <nil>
```

Inside the `if` block, `:=` declares a new `port` and a new `err`. The parsed value goes to the inner `port`, and the function returns the outer one, `8080`. Use `=` to assign to the outer variables: `port, err = strconv.Atoi(s)`.
