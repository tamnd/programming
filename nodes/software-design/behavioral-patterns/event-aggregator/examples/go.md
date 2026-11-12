```go run id=consultants
package main

import "fmt"

type Handler func(c *Consultant)

type Signal []Handler

func (s *Signal) Add(h Handler) { *s = append(*s, h) }
func (s Signal) Fire(c *Consultant) {
	for _, h := range s {
		h(c)
	}
}

type Consultant struct {
	Name, Home          string
	AvailableAtHome     bool
	Location            string
	LocationChanged     Signal
	AvailabilityChanged Signal
}

func (c *Consultant) AvailableAt(place string) bool {
	if place == c.Home {
		return c.AvailableAtHome
	}
	return true
}

func (c *Consultant) Move(place string) {
	fmt.Printf("%s moves to %s\n", c.Name, place)
	if c.Location != place {
		c.LocationChanged.Fire(c)
		if c.AvailableAt(place) != c.AvailableAt(c.Location) {
			c.AvailabilityChanged.Fire(c)
		}
		c.Location = place
	}
}

type EventAggregator struct {
	Changed, Location, Availability Signal
}

func (a *EventAggregator) Listen(c *Consultant) {
	c.LocationChanged.Add(func(x *Consultant) { a.signal(a.Location, x) })
	c.AvailabilityChanged.Add(func(x *Consultant) { a.signal(a.Availability, x) })
}

func (a *EventAggregator) signal(specific Signal, c *Consultant) {
	specific.Fire(c)  // the specific event
	a.Changed.Fire(c) // and the general one
}

func main() {
	agg := &EventAggregator{}
	alice := &Consultant{Name: "alice", Home: "London", AvailableAtHome: false, Location: "London"}
	bob := &Consultant{Name: "bob", Home: "Paris", AvailableAtHome: true, Location: "Lyon"}
	agg.Listen(alice)
	agg.Listen(bob)

	agg.Changed.Add(func(c *Consultant) { fmt.Printf("screen: %s changed\n", c.Name) })
	agg.Availability.Add(func(c *Consultant) { fmt.Printf("phone: availability of %s changed\n", c.Name) })

	alice.Move("Leeds")
	bob.Move("Paris")
}
```

```output for=consultants
alice moves to Leeds
screen: alice changed
phone: availability of alice changed
screen: alice changed
bob moves to Paris
screen: bob changed
```

- Verdict: idiom. Slices of functions are the events.
