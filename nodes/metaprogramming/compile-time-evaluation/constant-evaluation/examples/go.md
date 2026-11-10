```go run id=exact
package main

import "fmt"

const Huge = 1 << 100
const Four int8 = Huge >> 98

type Weekday int

const (
	Sunday Weekday = iota
	Monday
	Tuesday
)

var table [len("abcd")]int

func main() {
	fmt.Println("Four:", Four)
	fmt.Println("Huge >> 90:", Huge>>90)
	fmt.Println("Tuesday:", Tuesday)
	fmt.Println("table:", len(table))
}
```

```output for=exact
Four: 4
Huge >> 90: 1024
Tuesday: 2
table: 4
```

`Huge` is 2 to the power 100. It has no type, and the compiler keeps it as an exact value. Constants do not overflow. Only `Four` and the printed shift must fit in a machine type. `iota` numbers the constants in a group. `len` of a string constant is a constant, so it can size an array.

```go fails expect=compile id=overflow
package main

import "fmt"

const Big = int32(1) << 33

func main() {
	fmt.Println(Big)
}
```

A typed constant must fit in its type. `int32(1) << 33` overflows `int32`, and the compiler reports the error.

```go fails expect=compile id=call
package main

import "fmt"

func square(x int) int {
	return x * x
}

const Nine = square(3)

func main() {
	fmt.Println(Nine)
}
```

A Go constant expression has only constant operands and a few built-in functions, such as `len`. A call to a user function is never constant, even when the function is pure. Use a `var` that the program initializes at run time.

- Verdict: native. Constants are exact values that the compiler computes. The language has no compile-time functions, and no static assertion keyword.
- A constant with no type takes a type when the program uses it. The value must then fit in that type.
