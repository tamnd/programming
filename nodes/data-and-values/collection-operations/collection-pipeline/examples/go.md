```go run id=iterator-adapters
package main

import (
	"fmt"
	"iter"
	"slices"
)

type article struct {
	title string
	words int
	tags  []string
}

// Map, Filter and Take are lazy steps. Each one wraps an iterator in a new iterator.
func Map[T, U any](seq iter.Seq[T], f func(T) U) iter.Seq[U] {
	return func(yield func(U) bool) {
		for v := range seq {
			if !yield(f(v)) {
				return
			}
		}
	}
}

func Filter[T any](seq iter.Seq[T], keep func(T) bool) iter.Seq[T] {
	return func(yield func(T) bool) {
		for v := range seq {
			if keep(v) && !yield(v) {
				return
			}
		}
	}
}

func Take[T any](seq iter.Seq[T], n int) iter.Seq[T] {
	return func(yield func(T) bool) {
		if n <= 0 {
			return
		}
		i := 0
		for v := range seq {
			if !yield(v) {
				return
			}
			i++
			if i == n {
				return
			}
		}
	}
}

func main() {
	articles := []article{
		{"Intro to NoSQL", 1200, []string{"nosql", "databases"}},
		{"Event sourcing", 800, []string{"architecture"}},
		{"Graph stores", 1500, []string{"nosql", "graphs"}},
		{"Refactoring loops", 600, []string{"refactoring"}},
		{"Key-value stores", 900, []string{"nosql"}},
	}

	// filter, sort, take and map, built from the steps above and the slices package.
	isNoSQL := func(a article) bool { return slices.Contains(a.tags, "nosql") }
	byWordsDesc := func(a, b article) int { return b.words - a.words }
	nosql := slices.Collect(Filter(slices.Values(articles), isNoSQL))
	sorted := slices.SortedFunc(slices.Values(nosql), byWordsDesc)
	titles := slices.Collect(Map(Take(slices.Values(sorted), 2), func(a article) string { return a.title }))
	fmt.Printf("%q\n", titles)

	// map and reduce: a range loop over the iterator does the reduce.
	total := 0
	for w := range Map(slices.Values(nosql), func(a article) int { return a.words }) {
		total += w
	}
	fmt.Println(total)

	// flat map and group by: nested loops fill a map. Println prints the keys in order.
	counts := map[string]int{}
	for _, a := range articles {
		for _, t := range a.tags {
			counts[t]++
		}
	}
	fmt.Println(counts)

	calls := 0
	score := func(a article) int {
		calls++
		return a.words
	}
	long := func(s int) bool { return s > 1000 }

	// A slice between the steps makes the pipeline eager: every score is computed.
	all := slices.Collect(Map(slices.Values(articles), score))
	eager := slices.Collect(Take(Filter(slices.Values(all), long), 1))
	fmt.Println("eager", eager, calls)

	// Iterators make the pipeline lazy: Take stops the work after the first match.
	calls = 0
	lazy := slices.Collect(Take(Filter(Map(slices.Values(articles), score), long), 1))
	fmt.Println("lazy", lazy, calls)
}
```

```output
["Graph stores" "Intro to NoSQL"]
3600
map[architecture:1 databases:1 graphs:1 nosql:3 refactoring:1]
eager [1200] 5
lazy [1200] 1
```

- Verdict: idiom. The Go standard library has iterators (iter.Seq) and functions such as slices.Values, slices.Collect and slices.SortedFunc, but no map or filter. The program writes Map, Filter and Take as functions that wrap one iterator in another. A slice between the steps makes the pipeline eager, so score runs 5 times. The iterator chain is lazy, and Take stops it after the first match.
