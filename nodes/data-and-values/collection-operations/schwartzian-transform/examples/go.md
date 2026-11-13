```go run id=struct-decorate
package main

import (
	"cmp"
	"fmt"
	"slices"
	"strings"
)

var calls int

func vowels(w string) int {
	calls++
	n := 0
	for _, c := range w {
		if strings.ContainsRune("aeiou", c) {
			n++
		}
	}
	return n
}

type decorated struct {
	key  int
	word string
}

func main() {
	words := []string{"banana", "kiwi", "apple", "fig", "cherry", "date", "plum", "mango"}

	// Without the technique: two key calls at each comparison.
	calls = 0
	slow := slices.Clone(words)
	slices.SortStableFunc(slow, func(a, b string) int { return cmp.Compare(vowels(a), vowels(b)) })
	fmt.Println(slow, calls > len(words))

	// Decorate: one key call for each word.
	calls = 0
	d := make([]decorated, len(words))
	for i, w := range words {
		d[i] = decorated{vowels(w), w}
	}
	// Sort by the stored key. The stable sort keeps the input order of ties.
	slices.SortStableFunc(d, func(a, b decorated) int { return cmp.Compare(a.key, b.key) })
	// Undecorate.
	fast := make([]string, len(d))
	for i, x := range d {
		fast[i] = x.word
	}
	fmt.Println(fast, calls)
}
```

```output
[fig cherry plum kiwi apple date mango banana] true
[fig cherry plum kiwi apple date mango banana] 8
```

A compare function that calls `vowels` computes the keys many times. The slice of `decorated` structs holds each key once, `slices.SortStableFunc` compares the stored keys and keeps ties in their input order, and a loop takes out the words.
