```go run id=decompose
package main

import (
	"fmt"
	"sort"
	"strings"
)

type wordCount struct {
	word string
	n    int
}

func splitWords(text string) []string {
	return strings.Fields(strings.ToLower(text))
}

func countWords(words []string) map[string]int {
	counts := map[string]int{}
	for _, w := range words {
		counts[w]++
	}
	return counts
}

func top(counts map[string]int, n int) []wordCount {
	var all []wordCount
	for w, c := range counts {
		all = append(all, wordCount{w, c})
	}
	sort.Slice(all, func(i, j int) bool {
		if all[i].n != all[j].n {
			return all[i].n > all[j].n
		}
		return all[i].word < all[j].word
	})
	return all[:n]
}

func report(text string, n int) {
	for _, wc := range top(countWords(splitWords(text)), n) {
		fmt.Printf("%-5s %d\n", wc.word, wc.n)
	}
}

func main() {
	report("The cat and the hat and the bat", 2)
}
```

```output for=decompose
the   3
and   2
```

- Verdict: native. Go programs are mostly package-level functions and types. A name that starts with a capital letter is exported from its package, and other names are private to it.
