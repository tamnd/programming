```go run id=proactor
package main

import "fmt"

type Handler func(name string, size int)

type operation struct {
	name    string
	handler Handler
}

type completion struct {
	handler Handler
	name    string
	size    int
}

func main() {
	files := map[string]string{"a.txt": "hello", "b.txt": "abc"}
	operations := make(chan operation)
	completions := make(chan completion, 2)
	go func() { // the Asynchronous Operation Processor
		for op := range operations {
			completions <- completion{op.handler, op.name, len(files[op.name])}
		}
	}()
	onRead := func(name string, size int) { fmt.Printf("%s done: %d bytes\n", name, size) }
	for _, name := range []string{"a.txt", "b.txt"} {
		fmt.Println("start read", name)
		operations <- operation{name, onRead}
	}
	close(operations)
	for i := 0; i < 2; i++ { // the Completion Dispatcher
		c := <-completions
		c.handler(c.name, c.size)
	}
}
```

```output
start read a.txt
start read b.txt
a.txt done: 5 bytes
b.txt done: 3 bytes
```

- Verdict: idiom. A goroutine is the processor, and channels are the request queue and the completion queue.
