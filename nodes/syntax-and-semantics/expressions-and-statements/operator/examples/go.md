```go run id=no-overloading-and-bit-clear
package main

import "fmt"

type Money int64

// Go has no operator overloading, so a type uses methods.
func (m Money) Add(o Money) Money { return m + o }

func main() {
	// A defined type keeps the operators of its underlying type.
	var a, b Money = 150, 275
	fmt.Println(a+b, a.Add(b))

	// Integer / truncates toward zero, and % has the sign of the left operand.
	fmt.Println(-7/2, -7%2)

	// &^ is AND NOT: it clears the bits that are set in the right operand.
	fmt.Printf("%04b\n", 0b1111&^0b0101)

	// <- sends to a channel and receives from it.
	ch := make(chan int, 1)
	ch <- 5
	fmt.Println(<-ch)

	// & takes an address and * follows a pointer.
	x := 10
	p := &x
	*p += 1
	fmt.Println(x)

	// ++ is a statement, not an expression, so y := x++ does not compile.
	x++
	fmt.Println(x)
}
```

```output for=no-overloading-and-bit-clear
425 425
-3 -1
1010
5
11
12
```

- Verdict: native. Go has no operator overloading, but a defined type such as `Money` keeps the operators of `int64`. Integer `/` truncates toward zero. `&^` clears bits. `<-` sends to a channel and receives from it. `&` and `*` take an address and follow a pointer. `++` is a statement, not an expression.
