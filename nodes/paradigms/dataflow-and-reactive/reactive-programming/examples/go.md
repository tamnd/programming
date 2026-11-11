```go run id=pipeline
package main

import "fmt"

func source(values ...int) <-chan int {
	out := make(chan int)
	go func() {
		defer close(out)
		for _, v := range values {
			out <- v
		}
	}()
	return out
}

func filter(in <-chan int, keep func(int) bool) <-chan int {
	out := make(chan int)
	go func() {
		defer close(out)
		for v := range in {
			if keep(v) {
				out <- v
			}
		}
	}()
	return out
}

func runningSum(in <-chan int) <-chan [2]int {
	out := make(chan [2]int)
	go func() {
		defer close(out)
		sum := 0
		for v := range in {
			sum += v
			out <- [2]int{v, sum}
		}
	}()
	return out
}

func main() {
	hot := filter(source(18, 22, 19, 25, 30), func(t int) bool { return t > 20 })
	for r := range runningSum(hot) {
		fmt.Printf("hot %d, running sum %d\n", r[0], r[1])
	}
}
```

```output for=pipeline
hot 22, running sum 22
hot 25, running sum 47
hot 30, running sum 77
```

- Verdict: idiom. Each stage is a goroutine that reads one channel and writes another, so the program is a dataflow graph. The stages run at the same time, and an unbuffered channel gives back-pressure: a stage waits until the next one takes its value. Closing a channel is the signal that the stream is complete.
