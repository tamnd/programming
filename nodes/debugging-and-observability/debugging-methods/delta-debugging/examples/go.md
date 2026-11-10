```go run id=ddmin
package main

import (
	"fmt"
	"strings"
)

// ddmin returns a 1-minimal part of chars on which fails is still true.
func ddmin(chars string, fails func(string) bool) string {
	n := 2
	for len(chars) >= 2 {
		size := len(chars)
		var subsets, complements []string
		for i := range n {
			lo, hi := i*size/n, (i+1)*size/n
			subsets = append(subsets, chars[lo:hi])
			complements = append(complements, chars[:lo]+chars[hi:])
		}
		if s, ok := first(subsets, fails); ok { // reduce to subset
			chars, n = s, 2
			fmt.Println("subset    ", len(chars))
			continue
		}
		if c, ok := first(complements, fails); ok { // reduce to complement
			chars, n = c, max(n-1, 2)
			fmt.Println("complement", len(chars))
			continue
		}
		if n == size { // each single character was removed
			break
		}
		n = min(size, 2*n) // increase granularity
	}
	return chars
}

// first returns the first candidate on which fails is true.
func first(candidates []string, fails func(string) bool) (string, bool) {
	for _, c := range candidates {
		if fails(c) {
			return c, true
		}
	}
	return "", false
}

func main() {
	tests := 0
	// browserCrashes stands for one run of the browser:
	// it crashes on a <SELECT ...> tag.
	browserCrashes := func(text string) bool {
		tests++
		start := strings.Index(text, "<SELECT")
		return start >= 0 && strings.Contains(text[start:], ">")
	}
	page := `<td align=left><SELECT NAME="priority" MULTIPLE SIZE=7>`
	minimal := ddmin(page, browserCrashes)
	fmt.Printf("%d characters reduced to %s in %d tests\n", len(page), minimal, tests)
}
```

```output for=ddmin
complement 42
complement 28
complement 21
complement 17
complement 14
complement 13
complement 12
complement 10
complement 9
complement 8
55 characters reduced to <SELECT> in 147 tests
```

- Verdict: idiom. The standard library has no delta debugging.
- The test is a closure that counts its own calls in the variable `tests`. See [[closures]].
- A Go string is a sequence of bytes, and `chars[lo:hi]` slices bytes. For input that is not ASCII, convert it to `[]rune` first, or a part can hold half of a character.
- The helper `first` stops at the first part that fails, so the program runs no extra tests.
