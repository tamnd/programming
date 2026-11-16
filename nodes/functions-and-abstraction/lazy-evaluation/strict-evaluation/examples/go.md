```go run id=strict
package main

import "fmt"

func trace[T any](label string, value T) T {
	fmt.Println("evaluating", label)
	return value
}

func pick(flag bool, left, right int) int {
	fmt.Println("inside pick")
	if flag {
		return left
	}
	return right
}

func main() {
	fmt.Println(pick(trace("flag", true), trace("left", 1), trace("right", 2)))
	fmt.Println(trace("x", false) && trace("y", true))
	defer fmt.Println("deferred value:", trace("deferred", 42))
	fmt.Println("end of main")
}
```

```output for=strict
evaluating flag
evaluating left
evaluating right
inside pick
1
evaluating x
false
evaluating deferred
end of main
deferred value: 42
```

- Verdict: native. Arguments are evaluated before the call, and function calls inside an expression run in lexical left-to-right order.
- A `defer` statement evaluates its arguments at once, when the statement runs, but delays the call until the function returns. The trace shows `evaluating deferred` before `end of main`.
