```go run id=car
package main

import (
	"fmt"
	"strings"
)

type StarterPart interface{ Crank() string }

type Starter struct{}

func (Starter) Crank() string { return "crank the engine" }

type NewStarter struct{}

func (NewStarter) Crank() string { return "crank the engine with the new starter" }

type engine struct{}

func (engine) fire() string { return "fire the cylinders" }

type wheels struct{}

func (wheels) turn() string { return "turn the wheels" }

// Car is the whole. In a package of its own, its unexported fields and the
// unexported engine and wheels types are hidden from other packages.
type Car struct {
	starter StarterPart
	engine  engine
	wheels  wheels
}

func NewCar() *Car { return &Car{starter: Starter{}} }

func (c *Car) Drive() string {
	return strings.Join([]string{c.starter.Crank(), c.engine.fire(), c.wheels.turn()}, ", ")
}

func (c *Car) ReplaceStarter(s StarterPart) { c.starter = s }

func main() {
	car := NewCar()
	fmt.Println("drive:", car.Drive())
	car.ReplaceStarter(NewStarter{})
	fmt.Println("drive:", car.Drive())
}
```

```output
drive: crank the engine, fire the cylinders, turn the wheels
drive: crank the engine with the new starter, fire the cylinders, turn the wheels
```

- Verdict: idiom. Unexported fields and types hide the parts from other packages. Inside the package of the whole, they are visible.
