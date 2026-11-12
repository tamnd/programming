```go run id=channels-and-select
package main

import (
	"fmt"
	"sort"
)

type request struct {
	n     int
	reply chan int
}

func main() {
	// A goroutine owns the total; other goroutines send it messages.
	adds := make(chan int)
	gets := make(chan chan int)
	go func() {
		total := 0
		for {
			select {
			case n := <-adds:
				total += n
			case r := <-gets:
				r <- total
			}
		}
	}()
	done := make(chan bool)
	for i := 0; i < 4; i++ {
		go func() {
			for k := 0; k < 1000; k++ {
				adds <- 1
			}
			done <- true
		}()
	}
	for i := 0; i < 4; i++ {
		<-done
	}
	r := make(chan int)
	gets <- r
	fmt.Println("total", <-r)

	// A buffered channel holds messages until a receiver takes them.
	buf := make(chan string, 2)
	buf <- "a"
	buf <- "b"
	fmt.Println(len(buf), cap(buf))
	select {
	case buf <- "c":
		fmt.Println("sent c")
	default:
		fmt.Println("buffer full")
	}
	close(buf)
	var got []string
	for s := range buf {
		got = append(got, s)
	}
	fmt.Println(got)

	// A receive from a closed channel gives the zero value and false.
	v, ok := <-buf
	fmt.Printf("%q %v\n", v, ok)

	// Three workers answer requests; each request carries its reply channel.
	jobs := make(chan request)
	for w := 0; w < 3; w++ {
		go func() {
			for req := range jobs {
				req.reply <- req.n * req.n
			}
		}()
	}
	replies := make(chan int, 5)
	for n := 1; n <= 5; n++ {
		jobs <- request{n, replies}
	}
	close(jobs)
	var results []int
	for i := 0; i < 5; i++ {
		results = append(results, <-replies)
	}
	sort.Ints(results)
	fmt.Println(results)
}
```

```output for=channels-and-select
total 4000
2 2
buffer full
[a b]
"" false
[1 4 9 16 25]
```

- Verdict: native. A goroutine owns the total and uses `select` to take an add or a request with a reply channel. A buffered channel with capacity 2 holds two strings, and `select` with `default` sees that it is full. `range` reads until `close`, and a receive after that gives `""` and `false`. Three workers square numbers sent on a jobs channel.
