```go run id=invoices
package main

import "fmt"

// The consumer declares the small interfaces it needs.
type Clock interface{ Today() int }

type Mailer interface{ Send(to, text string) }

type Invoice struct {
	Number   int
	Customer string
	Due      int // day number
}

type ReminderService struct {
	clock  Clock
	mailer Mailer
}

func NewReminderService(clock Clock, mailer Mailer) *ReminderService {
	return &ReminderService{clock: clock, mailer: mailer}
}

func (s *ReminderService) Remind(inv Invoice) bool {
	late := s.clock.Today() - inv.Due
	if late <= 0 {
		return false
	}
	s.mailer.Send(inv.Customer, fmt.Sprintf("invoice %d is %d days overdue", inv.Number, late))
	return true
}

type FixedClock int

func (c FixedClock) Today() int { return int(c) }

type ConsoleMailer struct{}

func (ConsoleMailer) Send(to, text string) { fmt.Printf("to %s: %s\n", to, text) }

type RecordingMailer struct{ Sent []string }

func (m *RecordingMailer) Send(to, text string) {
	m.Sent = append(m.Sent, fmt.Sprintf("to %s: %s", to, text))
}

func main() {
	// The composition root.
	service := NewReminderService(FixedClock(40), ConsoleMailer{})
	invoices := []Invoice{{17, "ana@example.com", 31}, {18, "bo@example.com", 45}}
	sent := 0
	for _, inv := range invoices {
		if service.Remind(inv) {
			sent++
		}
	}
	fmt.Printf("reminders sent: %d\n", sent)

	// A test wires the same type to a recording double.
	recorder := &RecordingMailer{}
	NewReminderService(FixedClock(50), recorder).Remind(invoices[1])
	fmt.Printf("recorded: %s\n", recorder.Sent[0])
}
```

```output
to ana@example.com: invoice 17 is 9 days overdue
reminders sent: 1
recorded: to bo@example.com: invoice 18 is 5 days overdue
```

- Verdict: idiom. A struct holds interface values, and a constructor function fills them. Go has no container in the standard library, and the usual Go program wires its objects by hand in `main`.
- Interfaces are satisfied implicitly. `FixedClock` and `RecordingMailer` do not name `Clock` or `Mailer`, so the package that consumes a role can declare the interface, with only the methods it calls.
- `FixedClock` is a named `int`. A role with one method does not need a struct.
- `RecordingMailer.Send` has a pointer receiver because it changes the slice. So `*RecordingMailer` satisfies `Mailer`, and `RecordingMailer` does not.
