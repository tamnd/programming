```go run id=trace
package main

import "fmt"

func f(name string, value int) int {
	fmt.Println("evaluate", name)
	return value
}

func main() {
	fmt.Println(f("a", 1) + f("b", 2)*f("c", 3))

	x := []int{0, 0}
	i := 0
	i, x[i] = 1, 9
	fmt.Println(i, x)
}
```

```output for=trace
evaluate a
evaluate b
evaluate c
7
1 [9 0]
```

- Verdict: native. The Go specification says that function calls, method calls, receive operations and the logical operators `&&` and `||` are evaluated in lexical left-to-right order.
- An assignment has two phases. First, the index expressions and pointer indirections on the left and the expressions on the right are evaluated in the usual order. Second, the assignments are done from left to right. So `x[i]` uses the old `i`, which is 0.
- The order of other operations, such as reading a variable, compared to these calls is not specified.

```go wrong id=read-then-call
package main

import "fmt"

func main() {
	a := 1
	next := func() int {
		a++
		return a
	}
	x := []int{a, next()}
	fmt.Println(x)
}
```

```output for=read-then-call
[2 2]
```

The specification gives this case: `x` may be `[1, 2]` or `[2, 2]`, because the read of `a` is not ordered compared to the call. The gc compiler reads `a` after the call. Put the call in its own statement before the literal.
