```go run id=twin-ball
package main

import "fmt"

type Item struct {
	name     string
	position func() int
}

func (i *Item) Show() { fmt.Printf("%s at %d\n", i.name, i.position()) }

type Ticker struct {
	count  int
	onTick func()
}

func (t *Ticker) Tick() {
	t.count++
	t.onTick()
}

// Ball embeds both parts, so it has the methods of both.
type Ball struct {
	Item
	Ticker
	x int
}

func NewBall(name string) *Ball {
	b := &Ball{}
	b.Item = Item{name: name, position: func() int { return b.x }}
	b.Ticker = Ticker{onTick: func() { b.x += 2 }}
	return b
}

func main() {
	ball := NewBall("ball")
	ball.Show()
	for i := 0; i < 2; i++ {
		ball.Tick()
		ball.Show()
	}
	fmt.Printf("ticks: %d\n", ball.count)
}
```

```output
ball at 0
ball at 2
ball at 4
ticks: 2
```

- Verdict: idiom. Go has no inheritance. A struct can embed several structs, and their methods are promoted to it, so one value has the methods of both parts. The hooks are function fields, because an embedded struct does not call methods of the outer struct.
