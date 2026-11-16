```go run id=packed
package main

import (
	"errors"
	"fmt"
	"strconv"
)

func toPacked(n int64) []byte {
	sign := byte(0xC)
	if n < 0 {
		sign = 0xD
		n = -n
	}
	s := strconv.FormatInt(n, 10)
	nibbles := make([]byte, 0, len(s)+2)
	if len(s)%2 == 0 {
		nibbles = append(nibbles, 0)
	}
	for _, c := range s {
		nibbles = append(nibbles, byte(c-'0'))
	}
	nibbles = append(nibbles, sign)
	out := make([]byte, len(nibbles)/2)
	for i := range out {
		out[i] = nibbles[2*i]<<4 | nibbles[2*i+1]
	}
	return out
}

func fromPacked(b []byte) (int64, error) {
	var v int64
	for i, x := range b {
		hi, lo := x>>4, x&0x0F
		last := i == len(b)-1
		if hi > 9 || (!last && lo > 9) {
			return 0, errors.New("not packed BCD")
		}
		v = v*10 + int64(hi)
		if !last {
			v = v*10 + int64(lo)
		} else if lo == 0xD {
			v = -v
		}
	}
	return v, nil
}

func main() {
	p := toPacked(-12345)
	fmt.Printf("packed -12345: % X\n", p)
	v, _ := fromPacked(p)
	fmt.Println("back:", v)
	fmt.Printf("packed 2024: % X\n", toPacked(2024))
	_, err := fromPacked([]byte{0x1A, 0x3C})
	fmt.Println("1A 3C:", err)
}
```

```output for=packed
packed -12345: 12 34 5D
back: -12345
packed 2024: 02 02 4C
1A 3C: not packed BCD
```

- Verdict: idiom. Go has no BCD type. The block builds a slice of nibbles, then joins each pair into a byte.
- The `% X` verb prints the bytes in hexadecimal with spaces, which shows the decimal digits and the sign nibble.
