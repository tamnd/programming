```go run id=forms
package main

import (
	"fmt"
	"math"
)

func toSignMagnitude(n int) int {
	if n < 0 {
		return 0x80 | -n
	}
	return n
}

func toOnes(n int) int {
	if n < 0 {
		return ^(-n) & 0xFF
	}
	return n
}

func fromSignMagnitude(b int) int {
	if b&0x80 != 0 {
		return -(b & 0x7F)
	}
	return b
}

func fromOnes(b int) int {
	if b&0x80 != 0 {
		return -(^b & 0xFF)
	}
	return b
}

func yesNo(b bool) string {
	if b {
		return "yes"
	}
	return "no"
}

func main() {
	fmt.Printf("-43 in sign-magnitude: %08b\n", toSignMagnitude(-43))
	fmt.Printf("-43 in ones' complement: %08b\n", toOnes(-43))
	fmt.Println("10101011 in sign-magnitude:", fromSignMagnitude(0b10101011))
	fmt.Println("11010100 in ones' complement:", fromOnes(0b11010100))
	fmt.Println("10000000 in sign-magnitude:", fromSignMagnitude(0b10000000))
	fmt.Println("11111111 in ones' complement:", fromOnes(0b11111111))

	z := math.Copysign(0, -1)
	fmt.Println("z == 0:", yesNo(z == 0))
	fmt.Println("math.Signbit(z):", yesNo(math.Signbit(z)))
	fmt.Println("1 / z:", 1/z)
	lit := -0.0
	fmt.Println("math.Signbit(-0.0 literal):", yesNo(math.Signbit(lit)))
}
```

```output for=forms
-43 in sign-magnitude: 10101011
-43 in ones' complement: 11010100
10101011 in sign-magnitude: -43
11010100 in ones' complement: -43
10000000 in sign-magnitude: 0
11111111 in ones' complement: 0
z == 0: yes
math.Signbit(z): yes
1 / z: -Inf
math.Signbit(-0.0 literal): no
```

- Verdict: idiom. Go's signed integers are two's complement, so a program makes the other two forms by hand. Go writes bitwise NOT as the unary `^`.
- The constant `-0.0` is an exact constant, and exact zero has no sign, so `lit` is positive zero. `math.Copysign(0, -1)` makes a real negative zero.
