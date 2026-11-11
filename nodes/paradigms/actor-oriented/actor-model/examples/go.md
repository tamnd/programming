```go run id=goroutine
package main

import (
	"fmt"
	"sync"
)

type msg struct {
	op     string
	amount int
	reply  chan int
}

func account(mailbox <-chan msg) {
	balance := 0 // only this goroutine uses balance
	for m := range mailbox {
		switch m.op {
		case "deposit":
			balance += m.amount
		case "withdraw":
			if m.amount <= balance {
				balance -= m.amount
			}
		case "balance":
			m.reply <- balance
		}
	}
}

func main() {
	mailbox := make(chan msg, 16)
	go account(mailbox)

	var wg sync.WaitGroup
	for i := 0; i < 100; i++ {
		wg.Add(1)
		go func() {
			defer wg.Done()
			mailbox <- msg{op: "deposit", amount: 10}
		}()
	}
	wg.Wait()

	mailbox <- msg{op: "withdraw", amount: 1}
	reply := make(chan int)
	mailbox <- msg{op: "balance", reply: reply}
	fmt.Println("balance", <-reply)
	close(mailbox)
}
```

```output for=goroutine
balance 999
```

- Verdict: idiom. A goroutine that owns its state and reads a channel is an actor. Go's model is CSP, not actors: channels are values that any goroutine can use, and an unbuffered send waits for the receiver. A buffered channel gives a mailbox. Go does not stop goroutines from sharing memory, so the rule that only the actor uses `balance` is a convention.
