```go run id=map-of-counts
package main

import (
	"fmt"
	"maps"
	"slices"
	"strings"
)

func count(items []string) map[string]int {
	m := map[string]int{}
	for _, x := range items {
		m[x]++ // a missing key reads as 0
	}
	return m
}

func main() {
	c := count(strings.Fields("the cat and the hat and the bat"))
	keys := slices.Sorted(maps.Keys(c))
	for _, k := range keys {
		fmt.Printf("%s=%d ", k, c[k])
	}
	fmt.Println("end")
	fmt.Println(c["the"], c["dog"])

	// maps.Equal compares two multisets of letters.
	letters := func(s string) map[string]int { return count(strings.Split(s, "")) }
	fmt.Println(maps.Equal(letters("listen"), letters("silent")), maps.Equal(letters("aab"), letters("ab")))

	// Remove one copy, and delete the key when the count reaches 0.
	for _, w := range []string{"and", "cat"} {
		c[w]--
		if c[w] == 0 {
			delete(c, w)
		}
	}
	total := 0
	for _, n := range c {
		total += n
	}
	_, hasCat := c["cat"]
	fmt.Println(c["and"], hasCat, len(c), total)
}
```

```output
and=2 bat=1 cat=1 hat=1 the=3 end
3 0
true false
1 false 4 6
```

- Verdict: idiom. A map[K]int. A missing key reads as 0, so m[k]++ needs no check. maps.Equal compares two multisets. The program deletes a key when its count reaches 0.
