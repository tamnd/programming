```go run id=for-as-while-and-scanner
package main

import (
	"bufio"
	"fmt"
	"strings"
)

func main() {
	// for with only a condition is the while loop of Go.
	n, digits := 9876543210, 0
	for n > 0 {
		n /= 10
		digits++
	}
	fmt.Println("digits:", digits)

	// Scan returns false at the end of the input.
	sc := bufio.NewScanner(strings.NewReader("alpha beta\ngamma"))
	sc.Split(bufio.ScanWords)
	words := 0
	for sc.Scan() {
		words++
		fmt.Println(words, sc.Text())
	}

	// Euclid's algorithm.
	a, b := 1071, 462
	for b != 0 {
		a, b = b, a%b
	}
	fmt.Println("gcd:", a)
}
```

```output for=for-as-while-and-scanner
digits: 10
1 alpha
2 beta
3 gamma
gcd: 21
```

- Verdict: native. Go has no `while` keyword, and `for n > 0 { }` is the while loop. `for sc.Scan()` reads words until the input ends. Euclid's algorithm uses a parallel assignment in the body.
