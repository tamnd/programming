```go run id=convert
package main

import "fmt"

func yesNo(b bool) string {
	if b {
		return "yes"
	}
	return "no"
}

func main() {
	var a int32 = -1
	var b uint32 = 1
	// a < b does not compile: the types differ.
	fmt.Println("uint32(a):", uint32(a))
	fmt.Println("uint32(a) < b:", yesNo(uint32(a) < b))
	fmt.Println("int64(a) < int64(b):", yesNo(int64(a) < int64(b)))

	var x, y uint32 = 3, 5
	fmt.Println("x - y:", x-y)
}
```

```output for=convert
uint32(a): 4294967295
uint32(a) < b: no
int64(a) < int64(b): yes
x - y: 4294967294
```

- Verdict: native. Go has `uint8` to `uint64`, `uint` and `uintptr`, and `byte` is another name for `uint8`.
- Go does not convert between numeric types by itself. The program must choose the conversion, and the choice decides the answer.
- `int64` holds all values of `int32` and `uint32`, so a comparison after the conversion to `int64` is correct.
- Unsigned arithmetic wraps, so `3 - 5` in `uint32` is 4294967294.
