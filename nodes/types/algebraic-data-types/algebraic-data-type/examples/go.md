```go run id=count
package main

import (
	"fmt"
	"strings"
)

type Light int

const (
	Red Light = iota
	Amber
	Green
)

var lights = []Light{Red, Amber, Green}

func (l Light) String() string {
	return [...]string{"red", "amber", "green"}[l]
}

// Product: a flag AND a light.
type Pair struct {
	Flag  bool
	Light Light
}

// Sum: a flag OR a light. The unexported method closes the set of cases.
type Choice interface{ isChoice() }

type Flag struct{ Flag bool }
type Lamp struct{ Light Light }

func (Flag) isChoice() {}
func (Lamp) isChoice() {}

func yn(b bool) string {
	if b {
		return "yes"
	}
	return "no"
}

func show(c Choice) string {
	switch c := c.(type) {
	case Flag:
		return "flag:" + yn(c.Flag)
	case Lamp:
		return "light:" + c.Light.String()
	}
	panic("unknown case")
}

func main() {
	flags := []bool{false, true}
	var pairs []Pair
	var choices []Choice
	for _, f := range flags {
		for _, l := range lights {
			pairs = append(pairs, Pair{f, l})
		}
		choices = append(choices, Flag{f})
	}
	for _, l := range lights {
		choices = append(choices, Lamp{l})
	}

	fmt.Println("pairs:", len(pairs))
	var p []string
	for _, pair := range pairs {
		p = append(p, yn(pair.Flag)+"/"+pair.Light.String())
	}
	fmt.Println(strings.Join(p, " "))
	fmt.Println("choices:", len(choices))
	var c []string
	for _, choice := range choices {
		c = append(c, show(choice))
	}
	fmt.Println(strings.Join(c, " "))
}
```

```output for=count
pairs: 6
no/red no/amber no/green yes/red yes/amber yes/green
choices: 5
flag:no flag:yes light:red light:amber light:green
```

- Verdict: idiom. A `struct` is a product. Go has no sum types, so an interface with an unexported method and a type switch stand for one.
- The type switch does not check that it covers every case. The `panic` at the end makes a missing case fail at once.
