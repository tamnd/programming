```go run id=widgets
package main

import "fmt"

type Widget interface{ Render() string }

type WidgetFactory interface {
	Button(label string) Widget
	Checkbox(label string, checked bool) Widget
}

type plainButton struct{ label string }
type plainCheckbox struct {
	label   string
	checked bool
}
type roundButton struct{ label string }
type roundCheckbox struct {
	label   string
	checked bool
}

func (b plainButton) Render() string { return "[ " + b.label + " ]" }
func (c plainCheckbox) Render() string {
	mark := " "
	if c.checked {
		mark = "x"
	}
	return "[" + mark + "] " + c.label
}
func (b roundButton) Render() string { return "( " + b.label + " )" }
func (c roundCheckbox) Render() string {
	mark := " "
	if c.checked {
		mark = "*"
	}
	return "(" + mark + ") " + c.label
}

type Plain struct{}
type Round struct{}

func (Plain) Button(label string) Widget                 { return plainButton{label} }
func (Plain) Checkbox(label string, checked bool) Widget { return plainCheckbox{label, checked} }
func (Round) Button(label string) Widget                 { return roundButton{label} }
func (Round) Checkbox(label string, checked bool) Widget { return roundCheckbox{label, checked} }

func loginForm(widgets WidgetFactory) string {
	return widgets.Button("Save").Render() + " " + widgets.Checkbox("Remember me", true).Render()
}

func main() {
	kits := []struct {
		name string
		kit  WidgetFactory
	}{{"plain", Plain{}}, {"round", Round{}}}
	for _, k := range kits {
		fmt.Printf("%s: %s\n", k.name, loginForm(k.kit))
	}
}
```

```output for=widgets
plain: [ Save ] [x] Remember me
round: ( Save ) (*) Remember me
```

- Verdict: idiom. A Go interface lists the factory methods, and a type satisfies it by having them. The concrete factories are empty structs, so they cost nothing to pass.
- The product types are not exported. A caller outside the package can get a widget only from a factory, so it cannot mix families by calling a constructor.
- The `database/sql/driver` package has this shape. A `Driver` opens a `Conn`, and the connection prepares statements and begins transactions of the same driver.
