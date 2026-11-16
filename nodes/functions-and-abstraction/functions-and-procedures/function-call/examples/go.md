```go run id=function-call-go
package main

import "fmt"

func rectangleArea(width, height int) int {
	fmt.Println("computing", width, "by", height)
	return width * height
}

func main() {
	area := rectangleArea(3, 4)
	fmt.Println("area", area)
}
```

```output for=function-call-go
computing 3 by 4
area 12
```

- Verdict: native. A call writes the arguments in parentheses after the function. Go evaluates the arguments, makes new storage for the parameters and runs the body.
