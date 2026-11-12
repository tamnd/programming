```go run id=active
package main

import (
	"fmt"
	"sync"
)

// The servant: only the scheduler goroutine touches it, so it has no lock.
type accountServant struct{ total int }

// A method request carries the call and the future for its result.
type request struct {
	call   func(*accountServant) int
	future chan int
}

// The proxy: each method makes a request and returns a future at once.
type AccountProxy struct {
	activation chan request
	gate       chan struct{} // lets the demo hold the scheduler
	done       chan struct{}
}

func NewAccountProxy() *AccountProxy {
	p := &AccountProxy{
		activation: make(chan request, 2000),
		gate:       make(chan struct{}),
		done:       make(chan struct{}),
	}
	go p.schedule(&accountServant{})
	return p
}

func (p *AccountProxy) schedule(s *accountServant) {
	<-p.gate
	for r := range p.activation {
		r.future <- r.call(s)
	}
	close(p.done)
}

func (p *AccountProxy) send(call func(*accountServant) int) chan int {
	future := make(chan int, 1)
	p.activation <- request{call, future}
	return future
}

func (p *AccountProxy) Deposit(amount int) chan int {
	return p.send(func(s *accountServant) int { s.total += amount; return s.total })
}

func (p *AccountProxy) Balance() chan int {
	return p.send(func(s *accountServant) int { return s.total })
}

func main() {
	account := NewAccountProxy()
	perClient := make([][]chan int, 4)
	var wg sync.WaitGroup
	for c := range perClient {
		wg.Add(1)
		go func() {
			defer wg.Done()
			for i := 0; i < 250; i++ {
				perClient[c] = append(perClient[c], account.Deposit(1)) // returns at once
			}
		}()
	}
	wg.Wait()
	queued, done := 0, 0
	for _, futures := range perClient {
		for _, f := range futures {
			queued++
			done += len(f)
		}
	}
	fmt.Println("deposits queued:", queued)
	fmt.Println("deposits done before the scheduler starts:", done)
	close(account.gate)
	fmt.Println("balance after 1000 deposits:", <-account.Balance())
	close(account.activation)
	<-account.done
}
```

```output
deposits queued: 1000
deposits done before the scheduler starts: 0
balance after 1000 deposits: 1000
```

- Verdict: idiom. A buffered channel is the activation queue, a goroutine is the scheduler, and a channel with room for one value is the future.
