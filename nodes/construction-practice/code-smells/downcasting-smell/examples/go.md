```go run id=give-the-base-the-method
package main

import "fmt"

// Before: an interface with no methods, and type assertions in the client.
type NoticeBefore interface{}

type EmailBefore struct{ Address string }
type SmsBefore struct{ Phone string }
type PushBefore struct{ Device string }

func logLineBefore(n NoticeBefore) (string, error) {
	if e, ok := n.(EmailBefore); ok {
		return "email to " + e.Address, nil
	}
	if s, ok := n.(SmsBefore); ok {
		return "sms to " + s.Phone, nil
	}
	return "", fmt.Errorf("unknown notice %T", n)
}

// After: the interface has the operation, and each type implements it.
// A type without Describe does not satisfy Notice, and the compiler says so.
type Notice interface{ Describe() string }

type Email struct{ Address string }
type Sms struct{ Phone string }
type Push struct{ Device string }

func (e Email) Describe() string { return "email to " + e.Address }
func (s Sms) Describe() string   { return "sms to " + s.Phone }
func (p Push) Describe() string  { return "push to " + p.Device }

func logLine(n Notice) string { return n.Describe() }

func main() {
	before, err := logLineBefore(EmailBefore{"ada@example.com"})
	if err != nil || before != logLine(Email{"ada@example.com"}) {
		panic("email")
	}
	if logLine(Sms{"+15550100"}) != "sms to +15550100" || logLine(Push{"tablet-7"}) != "push to tablet-7" {
		panic("after")
	}
	if _, err := logLineBefore(PushBefore{"tablet-7"}); err == nil {
		panic("the chain should not know push notices")
	}

	// The single-value form of a type assertion panics when it is false.
	func() {
		defer func() {
			if recover() == nil {
				panic("expected a panic")
			}
		}()
		var n NoticeBefore = SmsBefore{"+15550100"}
		_ = n.(EmailBefore).Address
	}()
	fmt.Println("notices: tests pass")
}
```

```output
notices: tests pass
```

An empty interface accepts any value, so the before version compiles whatever the caller passes, and finds out at run time. The interface with a `Describe` method moves that check to the compiler: a call `logLine(Fax{})` with a type that has no `Describe` does not compile.
