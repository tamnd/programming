```go run id=notifier-interface
package main

import (
	"fmt"
	"strings"
)

// ---- package domain: it defines the interface that it needs, and imports nothing below.

// Notifier is the interface that the domain needs.
type Notifier interface {
	Send(customer, text string)
}

type OrderService struct{ notifier Notifier }

func (s OrderService) Confirm(number int, customer string) {
	s.notifier.Send(customer, fmt.Sprintf("order %d is confirmed", number))
}

// ---- package infrastructure: it implements the interface. In Go it does not even
// import the domain, because a type satisfies an interface by its methods alone.

type EmailNotifier struct{}

func (EmailNotifier) Send(customer, text string) { fmt.Printf("email to %s: %s\n", customer, text) }

// ---- package tests: another implementation of the same interface.

type RecordingNotifier struct{ sent []string }

func (r *RecordingNotifier) Send(customer, text string) {
	r.sent = append(r.sent, customer+": "+text)
}

// ---- main: the only code that knows both the domain and the implementations.

func main() {
	OrderService{EmailNotifier{}}.Confirm(7, "ann")

	recorder := &RecordingNotifier{}
	OrderService{recorder}.Confirm(8, "bob")
	fmt.Println("the test notifier recorded:", strings.Join(recorder.sent, "; "))
}
```

```output
email to ann: order 7 is confirmed
the test notifier recorded: bob: order 8 is confirmed
```

- Verdict: native. Go interfaces are satisfied by the method set alone, so `EmailNotifier` does not import the package that defines `Notifier`. The Go code review guide puts interfaces in the package that uses them, which is this pattern.
- `RecordingNotifier` has a pointer receiver, so the service gets `&RecordingNotifier{}` and the test can read `sent` after the call.
