```go run id=fall-through-go
package main

import "fmt"

func rights(level int) []string {
	var out []string
	switch level {
	case 3:
		out = append(out, "delete")
		fallthrough
	case 2:
		out = append(out, "write")
		fallthrough
	case 1:
		out = append(out, "read")
	default:
		out = append(out, "none")
	}
	return out
}

func main() {
	fmt.Println(rights(3))
	fmt.Println(rights(1))
	fmt.Println(rights(0))
}
```

```output for=fall-through-go
[delete write read]
[read]
[none]
```

- Verdict: native. A Go case ends by itself, so case 1 does not run on into `default`. The `fallthrough` statement, as the last statement of a case, moves control into the next case on request. It does not test the expression of the next case.
