```go run id=acceptor
package main

import "fmt"

type Channel struct{ messages []string }

type StatusHandler struct{ ch *Channel }

func (h *StatusHandler) Open(ch *Channel) { h.ch = ch; fmt.Println("accept -> status handler open") }
func (h *StatusHandler) Poll() {
	fmt.Println("status: got", h.ch.messages[0])
	h.ch.messages = h.ch.messages[1:]
}

type CommandHandler struct{ ch *Channel }

func (h *CommandHandler) Open(ch *Channel) { h.ch = ch; fmt.Println("connect done -> command handler open") }
func (h *CommandHandler) Send(text string) {
	fmt.Println("command: send", text)
	h.ch.messages = append(h.ch.messages, text)
}

type Acceptor struct {
	make     func() *StatusHandler
	handlers []*StatusHandler
}

var listeners = map[string]*Acceptor{} // stands in for the network

func Listen(address string, make func() *StatusHandler) *Acceptor {
	a := &Acceptor{make: make}
	listeners[address] = a
	fmt.Println("listen", address)
	return a
}

func (a *Acceptor) Accept(ch *Channel) {
	h := a.make()
	h.Open(ch)
	a.handlers = append(a.handlers, h)
}

func Connect(address string, make func() *CommandHandler) *CommandHandler {
	fmt.Println("connect", address)
	ch := &Channel{}
	listeners[address].Accept(ch)
	h := make() // complete: activate when the connection is up
	h.Open(ch)
	return h
}

func main() {
	acceptor := Listen("gateway:9000", func() *StatusHandler { return &StatusHandler{} })
	command := Connect("gateway:9000", func() *CommandHandler { return &CommandHandler{} })
	command.Send("reset")
	acceptor.handlers[0].Poll()
}
```

```output
listen gateway:9000
connect gateway:9000
accept -> status handler open
connect done -> command handler open
command: send reset
status: got reset
```

- Verdict: idiom. A function value that returns a new handler is the factory.
