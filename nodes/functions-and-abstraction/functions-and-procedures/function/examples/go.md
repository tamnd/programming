```go run id=function-go
package main

import "fmt"

func rectangleArea(width, height int) int {
	return width * height
}

func greeting(name string) string {
	return "Hello, " + name + "!"
}

func main() {
	fmt.Println(rectangleArea(3, 4))
	fmt.Println(greeting("Ada"))
}
```

```output for=function-go
12
Hello, Ada!
```

- Verdict: native. `func` declares a function. The result type comes after the parameter list.
