```go run id=simple-factory
package main

import "fmt"

type Notifier interface {
	Send(user, text string) string
}

type EmailNotifier struct{}

func (EmailNotifier) Send(user, text string) string {
	return fmt.Sprintf("mail to %s@example.com: %s", user, text)
}

type SmsNotifier struct{}

func (SmsNotifier) Send(user, text string) string {
	return fmt.Sprintf("text to the phone of %s: %s", user, text)
}

type PushNotifier struct{}

func (PushNotifier) Send(user, text string) string {
	return fmt.Sprintf("push to the app of %s: %s", user, text)
}

// NewNotifier is the one place that knows the concrete types.
func NewNotifier(channel string) (Notifier, error) {
	switch channel {
	case "email":
		return EmailNotifier{}, nil
	case "sms":
		return SmsNotifier{}, nil
	case "push":
		return PushNotifier{}, nil
	}
	return nil, fmt.Errorf("unknown channel: %s", channel)
}

func main() {
	for _, channel := range []string{"email", "sms", "push", "fax"} {
		n, err := NewNotifier(channel)
		if err != nil {
			fmt.Println(err)
			continue
		}
		fmt.Printf("%T: %s\n", n, n.Send("ann", "order shipped"))
	}
}
```

```output
main.EmailNotifier: mail to ann@example.com: order shipped
main.SmsNotifier: text to the phone of ann: order shipped
main.PushNotifier: push to the app of ann: order shipped
unknown channel: fax
```

- Verdict: idiom. A function named `New...` that returns an interface and an `error` is the Go form. The `%T` verb prints the dynamic type, with its package name.
- The function returns an interface, not a concrete type, because the concrete type depends on the argument.
