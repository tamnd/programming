```go run id=move
package main

import "fmt"

type Position struct{ X, Y int }

type Movable interface {
	Position() Position
	SetPosition(p Position)
}

type MoveServant struct{}

func (MoveServant) MoveTo(m Movable, where Position) { m.SetPosition(where) }

func (MoveServant) MoveBy(m Movable, dx, dy int) {
	p := m.Position()
	m.SetPosition(Position{p.X + dx, p.Y + dy})
}

type Shape struct {
	name string
	pos  Position
}

func (s *Shape) Position() Position     { return s.pos }
func (s *Shape) SetPosition(p Position) { s.pos = p }
func (s *Shape) String() string         { return fmt.Sprintf("%s at (%d, %d)", s.name, s.pos.X, s.pos.Y) }

type Rectangle struct{ Shape }
type Ellipse struct{ Shape }
type Triangle struct{ Shape }

func main() {
	var servant MoveServant
	r := &Rectangle{Shape{"rectangle", Position{0, 0}}}
	e := &Ellipse{Shape{"ellipse", Position{10, 0}}}
	t := &Triangle{Shape{"triangle", Position{0, 10}}}
	for _, m := range []Movable{r, e, t} {
		servant.MoveBy(m, 3, 4)
		fmt.Println(m)
	}
	servant.MoveTo(t, Position{1, 1})
	fmt.Println(t)
}
```

```output for=move
rectangle at (3, 4)
ellipse at (13, 4)
triangle at (3, 14)
triangle at (1, 1)
```

- Verdict: idiom. A type is served when it has the methods of `Movable`. The package `sort` uses this form.
