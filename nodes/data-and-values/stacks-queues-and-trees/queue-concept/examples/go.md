```go run id=queue
package main

import "fmt"

func main() {
	// A slice as a queue: append at the back, slice off the front.
	var queue []string
	for _, item := range []string{"a", "b", "c"} {
		queue = append(queue, item)
	}
	fmt.Println(queue, "front", queue[0])

	first := queue[0]
	queue = queue[1:]
	fmt.Println(first, "next", queue[0], "size", len(queue))
	queue = queue[2:]
	fmt.Println("empty:", map[bool]string{true: "yes", false: "no"}[len(queue) == 0])

	// A buffered channel is a FIFO queue between goroutines.
	jobs := make(chan string, 3)
	go func() {
		for _, job := range []string{"x", "y", "z"} {
			jobs <- job
		}
		close(jobs)
	}()
	for job := range jobs {
		fmt.Print(job, " ")
	}
	fmt.Println("done")
}
```

```output for=queue
[a b c] front a
a next b size 2
empty: yes
x y z done
```

- Verdict: idiom. Go has no queue type. A slice works: `append` adds at the back and `queue[1:]` drops the front. The old items stay in the backing array until `append` makes a new one.
- A buffered channel is a queue between goroutines, and values arrive in the order they were sent.
