```go run id=overloading-go
package main

import "fmt"

func showInt(x int) string {
	return fmt.Sprintf("int %d", x)
}

func showString(s string) string {
	return "string " + s
}

func showTwoInts(x, y int) string {
	return fmt.Sprintf("two ints %d %d", x, y)
}

func main() {
	fmt.Println(showInt(42))
	fmt.Println(showString("hi"))
	fmt.Println(showTwoInts(3, 4))
}
```

```output for=overloading-go
int 42
string hi
two ints 3 4
```

- Verdict: absent. Go has no overloading, so each function gets its own name.
