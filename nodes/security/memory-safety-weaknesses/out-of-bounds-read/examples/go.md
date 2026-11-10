```go wrong id=capacity
package main

import "fmt"

// echo returns the first claimed bytes of the payload.
func echo(payload []byte, claimed int) []byte {
	return payload[:claimed]
}

func main() {
	buf := make([]byte, 0, 32)
	buf = append(buf, "user=alice;pin=4321"...) // an earlier request
	buf = buf[:0]                                // reuse the buffer
	buf = append(buf, "hello"...)

	fmt.Printf("%q\n", echo(buf, 5))
	fmt.Printf("%q\n", echo(buf, 19))
}
```

```output for=capacity
"hello"
"helloalice;pin=4321"
```

The slice has a length of 5 and a capacity of 32. The Go specification allows the upper bound of a slice expression to go up to the capacity, so `buf[:19]` does not panic. It returns the old bytes of the earlier request. A claimed length past the capacity panics.

```go run id=check
package main

import (
	"errors"
	"fmt"
)

// echo returns the first claimed bytes of the payload.
func echo(payload []byte, claimed int) ([]byte, error) {
	if claimed < 0 || claimed > len(payload) {
		return nil, errors.New("the claimed length does not match the payload")
	}
	return payload[:claimed], nil
}

func main() {
	buf := make([]byte, 0, 32)
	buf = append(buf, "user=alice;pin=4321"...)
	buf = buf[:0]
	buf = append(buf, "hello"...)

	for _, claimed := range []int{5, 19} {
		reply, err := echo(buf, claimed)
		if err != nil {
			fmt.Printf("claimed %d: %v\n", claimed, err)
			continue
		}
		fmt.Printf("claimed %d: %q\n", claimed, reply)
	}
}
```

```output for=check
claimed 5: "hello"
claimed 19: the claimed length does not match the payload
```

- Verdict: native. Indices and slice expressions are checked at run time, and a bound past the capacity panics.
- The check uses the capacity, not the length. Code that reuses buffers must compare a length from input with `len`, as the second example does.
- The packages `unsafe` and `cgo` skip the checks.
