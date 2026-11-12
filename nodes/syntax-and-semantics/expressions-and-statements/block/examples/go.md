```go run id=implicit-blocks-and-defer
package main

import "fmt"

func main() {
	x := 1
	{
		x := 2 // a new x in the inner block hides the outer one
		y := x * 10
		fmt.Println("inner x =", x, "y =", y)
	}
	fmt.Println("outer x =", x)

	// The if statement is an implicit block, so v is visible only in it.
	if v := x + 5; v > 3 {
		fmt.Println("v =", v)
	}

	// Each case of a switch is an implicit block.
	switch x {
	case 1:
		msg := "one"
		fmt.Println(msg)
	default:
		msg := "other"
		fmt.Println(msg)
	}

	// defer runs at the end of the function, not at the end of the block.
	for i := range 2 {
		defer fmt.Println("deferred", i)
	}
	fmt.Println("end of main")
}
```

```output for=implicit-blocks-and-defer
inner x = 2 y = 20
outer x = 1
v = 6
one
end of main
deferred 1
deferred 0
```

- Verdict: native. A `:=` in an inner block declares a new `x` that hides the outer one. An `if` and each `switch` case are implicit blocks, so `v` and `msg` are local to them. `defer` runs when the function returns, not at the end of the loop body, and the deferred calls run in reverse order.
