```go run id=function-signature-go
package main

import "fmt"

func average(a, b int) float64 {
	return float64(a+b) / 2
}

func main() {
	var f func(int, int) float64 = average
	fmt.Println(f(3, 4))
}
```

```output for=function-signature-go
3.5
```

- Verdict: native. The parameter types follow the names, and the result type follows the parameter list. The type of `average` is `func(int, int) float64`.
