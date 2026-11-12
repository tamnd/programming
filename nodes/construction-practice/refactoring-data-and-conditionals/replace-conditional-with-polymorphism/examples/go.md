```go run id=each-case-its-code
package main

import "fmt"

// Message is the interface that each channel type implements.
type Message interface {
	Cost() int
	DelaySeconds() int
}

type Email struct{ Text string }
type Sms struct {
	Text   string
	Urgent bool
}
type Push struct{ Urgent bool }

func (Email) Cost() int         { return 0 }
func (Email) DelaySeconds() int { return 60 }

func (s Sms) Cost() int { return (len(s.Text) + 159) / 160 * 7 }
func (s Sms) DelaySeconds() int {
	if s.Urgent {
		return 0
	}
	return 30
}

func (p Push) Cost() int {
	if p.Urgent {
		return 1
	}
	return 0
}
func (Push) DelaySeconds() int { return 0 }

func costBefore(channel, text string, urgent bool) int {
	switch channel {
	case "email":
		return 0
	case "sms":
		return (len(text) + 159) / 160 * 7
	default: // "push"
		if urgent {
			return 1
		}
		return 0
	}
}

func main() {
	long := string(make([]byte, 161))
	for _, urgent := range []bool{false, true} {
		for _, text := range []string{"hi", long} {
			msgs := map[string]Message{"email": Email{text}, "sms": Sms{text, urgent}, "push": Push{urgent}}
			for ch, m := range msgs {
				if m.Cost() != costBefore(ch, text, urgent) {
					panic(ch)
				}
			}
		}
	}
	fmt.Println("replace conditional with polymorphism: tests pass")
}
```

```output
replace conditional with polymorphism: tests pass
```
