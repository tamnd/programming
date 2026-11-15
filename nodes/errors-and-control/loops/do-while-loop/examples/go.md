```go run id=for-with-a-break-at-the-end
package main

import "fmt"

func main() {
	// for with the test at the end of the body is the do-while loop of Go.
	n, digits := 0, 0
	for {
		n /= 10
		digits++
		if n == 0 {
			break
		}
	}
	fmt.Println("digits of 0:", digits)

	// Reverse the digits of a number.
	m, rev := 1200, 0
	for {
		rev = rev*10 + m%10
		m /= 10
		if m == 0 {
			break
		}
	}
	fmt.Println("reversed 1200:", rev)

	// A three-part for whose condition is set at the end of each pass.
	x := 10
	for ok := true; ok; ok = x < 5 {
		fmt.Println("x =", x)
		x++
	}
}
```

```output for=for-with-a-break-at-the-end
digits of 0: 1
reversed 1200: 21
x = 10
```

- Verdict: idiom. Go has only `for`, and no do-while form. An endless `for` with `if ... { break }` as its last statement is the post-test loop. The last loop uses a Boolean that starts as `true` and gets the real condition in the post statement, so the body runs once before the first real test.
