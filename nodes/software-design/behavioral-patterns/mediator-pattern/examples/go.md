```go run id=checkout
package main

import "fmt"

// Mediator is told when a widget changes.
type Mediator interface {
	Changed(widget any)
}

// Select, TextField and Button are colleagues. Each knows its mediator and no other widget.
type Select struct {
	Name     string
	Value    string
	mediator Mediator
}

func (s *Select) Choose(value string) {
	s.Value = value
	fmt.Printf("%s = %q\n", s.Name, value)
	s.mediator.Changed(s)
}

type TextField struct {
	Name     string
	Text     string
	mediator Mediator
}

// Type is a user edit: it notifies the mediator.
func (t *TextField) Type(text string) {
	t.SetText(text)
	t.mediator.Changed(t)
}

// SetText is a program edit: no notification.
func (t *TextField) SetText(text string) {
	t.Text = text
	fmt.Printf("%s = %q\n", t.Name, text)
}

type Button struct {
	Name     string
	Enabled  bool
	mediator Mediator
}

func (b *Button) SetEnabled(enabled bool) {
	if enabled == b.Enabled {
		return
	}
	b.Enabled = enabled
	state := "disabled"
	if enabled {
		state = "enabled"
	}
	fmt.Println(b.Name, state)
}

func (b *Button) Click() {
	if b.Enabled {
		b.mediator.Changed(b)
	}
}

// CheckoutForm is the mediator. All rules that join the widgets are here.
type CheckoutForm struct {
	Country  *Select
	Postcode *TextField
	Submit   *Button
}

var postcodeLength = map[string]int{"US": 5, "NL": 6}

func NewCheckoutForm() *CheckoutForm {
	f := &CheckoutForm{}
	f.Country = &Select{Name: "country", mediator: f}
	f.Postcode = &TextField{Name: "postcode", mediator: f}
	f.Submit = &Button{Name: "submit", mediator: f}
	return f
}

func (f *CheckoutForm) Changed(widget any) {
	switch widget {
	case f.Country:
		f.Postcode.SetText("")
		f.Submit.SetEnabled(false)
	case f.Postcode:
		needed, ok := postcodeLength[f.Country.Value]
		f.Submit.SetEnabled(ok && len(f.Postcode.Text) == needed)
	case f.Submit:
		fmt.Println("order sent to", f.Country.Value, f.Postcode.Text)
	}
}

func main() {
	form := NewCheckoutForm()
	form.Country.Choose("US")
	form.Postcode.Type("1234")
	form.Postcode.Type("12345")
	form.Country.Choose("NL")
	form.Postcode.Type("1234AB")
	form.Submit.Click()
}
```

```output for=checkout
country = "US"
postcode = ""
postcode = "1234"
postcode = "12345"
submit enabled
country = "NL"
postcode = ""
submit disabled
postcode = "1234AB"
submit enabled
order sent to NL 1234AB
```

- Verdict: idiom. A `Mediator` interface, widgets that hold it in an unexported field, and a form whose `Changed` method switches on the widget.
- `switch widget` with `case f.Country` compares an `any` with a `*Select`. The comparison is true when the dynamic type and the pointer are both equal, so it is an identity test.
- The pointers form a cycle: the form points to the widgets and each widget points back to the form. A garbage collector frees such a cycle when nothing else refers to it.
- A channel of events is the other Go form. The widgets send to the channel, and one goroutine runs the rules. This also makes the mediator the only goroutine that changes the widgets.
