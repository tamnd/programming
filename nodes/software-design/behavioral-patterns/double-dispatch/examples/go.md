```go run id=collisions
package main

import "fmt"

type Body interface {
	Name() string
	Collide(other Body) string
	CollideWithShip(s Ship) string
	CollideWithAsteroid(a Asteroid) string
}

type Ship struct{}

func (Ship) Name() string                     { return "ship" }
func (s Ship) Collide(other Body) string      { return other.CollideWithShip(s) }
func (Ship) CollideWithShip(Ship) string      { return "ships bounce" }
func (Ship) CollideWithAsteroid(Asteroid) string { return "ship takes damage" }

type Asteroid struct{}

func (Asteroid) Name() string                     { return "asteroid" }
func (a Asteroid) Collide(other Body) string      { return other.CollideWithAsteroid(a) }
func (Asteroid) CollideWithShip(Ship) string      { return "ship takes damage" }
func (Asteroid) CollideWithAsteroid(Asteroid) string { return "asteroids break apart" }

func main() {
	pairs := [][2]Body{{Ship{}, Asteroid{}}, {Asteroid{}, Ship{}}, {Asteroid{}, Asteroid{}}, {Ship{}, Ship{}}}
	for _, p := range pairs {
		fmt.Printf("%s + %s: %s\n", p[0].Name(), p[1].Name(), p[0].Collide(p[1]))
	}
}
```

```output
ship + asteroid: ship takes damage
asteroid + ship: ship takes damage
asteroid + asteroid: asteroids break apart
ship + ship: ships bounce
```

- Verdict: idiom. Go has no overloading, so each callback has its own name. Another form is a type switch on both values.
