```go run id=align-the-senders
package main

import (
	"fmt"
	"slices"
)

type user struct{ email, phone string }

func first(s string, n int) string {
	if len(s) > n {
		return s[:n]
	}
	return s
}

// Before: two types for one job, with two method sets.
type emailSenderBefore struct{ sent []string }

func (e *emailSenderBefore) send(to, subject, body string) {
	e.sent = append(e.sent, "email to "+to+": "+subject+" | "+body)
}

type smsGatewayBefore struct{ sent []string }

func (s *smsGatewayBefore) dispatch(phone, text string) {
	s.sent = append(s.sent, "sms to "+phone+": "+first(text, 40))
}

func alertBefore(channel any, u user, text string) {
	switch c := channel.(type) {
	case *emailSenderBefore:
		c.send(u.email, "Alert", text)
	case *smsGatewayBefore:
		c.dispatch(u.phone, text)
	default:
		panic("unknown channel")
	}
}

// After: one interface. Both types satisfy it because they have the method.
type channel interface{ deliver(u user, text string) }

type emailSender struct{ sent []string }

func (e *emailSender) deliver(u user, text string) {
	e.sent = append(e.sent, "email to "+u.email+": Alert | "+text)
}

type smsGateway struct{ sent []string }

func (s *smsGateway) deliver(u user, text string) {
	s.sent = append(s.sent, "sms to "+u.phone+": "+first(text, 40))
}

func alert(c channel, u user, text string) { c.deliver(u, text) }

func main() {
	ana := user{"ana@example.org", "+31600000000"}
	msg := "Your backup finished with 2 warnings."
	e0, s0 := &emailSenderBefore{}, &smsGatewayBefore{}
	e1, s1 := &emailSender{}, &smsGateway{}
	alertBefore(e0, ana, msg)
	alertBefore(s0, ana, msg)
	for _, c := range []channel{e1, s1} {
		alert(c, ana, msg)
	}
	if !slices.Equal(e1.sent, e0.sent) || !slices.Equal(s1.sent, s0.sent) {
		panic("sent")
	}
	fmt.Println("deliver: tests pass")
}
```

```output
deliver: tests pass
```

- Go interfaces are satisfied implicitly. Once both types have a `deliver` method with the same signature, both are a `channel`, and neither type needs to mention it.
- The type switch in `alertBefore` is the Go form of the branch on the class. The aligned interface removes it.
- `first` cuts by bytes, which is correct here because the text is ASCII. Text with other characters needs a cut by runes.
