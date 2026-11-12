```go run id=strconv-and-bits
package main

import (
	"fmt"
	"math/big"
	"math/bits"
	"strconv"
)

func main() {
	n := 0b1010_1100 // binary literal with a digit separator
	fmt.Printf("%d %b %o %x %08b\n", n, n, n, n, 5)
	fmt.Println(strconv.FormatInt(-5, 2), strconv.FormatUint(uint64(uint8(251)), 2))

	v, err := strconv.ParseInt("101101", 2, 64)
	fmt.Println(v, err)
	_, err = strconv.ParseInt("102", 2, 64)
	fmt.Println(err)
	w, _ := strconv.ParseUint("ff", 16, 8)
	fmt.Println(w)

	fmt.Println(bits.OnesCount(uint(n)), bits.LeadingZeros8(uint8(n)), bits.Len(200))
	x := new(big.Int).Lsh(big.NewInt(1), 70)
	fmt.Println(x.BitLen(), x.Text(16))
}
```

```output for=strconv-and-bits
172 10101100 254 ac 00000101
-101 11111011
45 <nil>
strconv.ParseInt: parsing "102": invalid syntax
255
4 0 8
71 400000000000000000
```

- Verdict: native. The verb `%b` prints binary, and `strconv.FormatInt` prints a negative number with a minus sign; converting to an unsigned type first shows its bits. `strconv.ParseInt` takes a base and a bit size and returns an error for a digit that is not valid, so the program does not silently accept `102`. `math/bits` counts bits, and `math/big` handles integers of any size.
