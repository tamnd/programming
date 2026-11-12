```go run id=thread-confinement
package main

import "fmt"

func main() {
	words := []string{"a", "b", "c", "d", "e"}
	const chunks = 4
	results := make(chan map[string]int)
	for i := 0; i < chunks; i++ {
		go func() {
			counts := map[string]int{} // local: no lock
			for r := 0; r < 2000; r++ {
				for _, w := range words {
					counts[w]++
				}
			}
			results <- counts // send the map and stop using it
		}()
	}

	total := map[string]int{}
	for i := 0; i < chunks; i++ {
		for w, n := range <-results {
			total[w] += n
		}
	}
	sum := 0
	for _, n := range total {
		sum += n
	}
	fmt.Println("chunks:", chunks)
	fmt.Println("words counted:", sum)
	fmt.Println("distinct words:", len(total))
	fmt.Println("count of 'a':", total["a"])
}
```

```output
chunks: 4
words counted: 40000
distinct words: 5
count of 'a': 8000
```

- Verdict: idiom. Each goroutine fills a local map and sends it on a channel, so only one goroutine has the map at a time.
