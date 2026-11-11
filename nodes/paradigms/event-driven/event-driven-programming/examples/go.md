```go run id=select
package main

import "fmt"

type event struct {
	kind string
	data string
}

func main() {
	events := make(chan event)
	done := make(chan struct{})
	handlers := map[string][]func(string){
		"key":    {func(d string) { fmt.Println("key pressed:", d) }},
		"resize": {func(d string) { fmt.Println("window size:", d) }},
	}

	go func() {
		for _, e := range []event{{"key", "a"}, {"resize", "80x24"}, {"key", "q"}} {
			events <- e
		}
		close(done)
	}()

	for {
		select {
		case e := <-events:
			for _, h := range handlers[e.kind] {
				h(e.data)
			}
		case <-done:
			fmt.Println("no more events")
			return
		}
	}
}
```

```output for=select
key pressed: a
window size: 80x24
key pressed: q
no more events
```

- Verdict: idiom. Go has no event loop in the language, because goroutines can block cheaply. A loop over `select` on several channels is the Go form of an event loop: each case is a source of events, and the body of the case is the handler.
- The runtime itself is event-driven: when a goroutine waits for the network, the scheduler parks it and uses epoll, kqueue or IOCP to learn when the socket is ready.
