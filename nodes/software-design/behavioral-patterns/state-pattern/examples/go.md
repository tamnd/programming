```go run id=player
package main

import "fmt"

// A State reacts to each event. A nil result means "stay in this state".
type State interface {
	Name() string
	Play() (State, string)
	Pause() (State, string)
	Stop() (State, string)
}

// ignore gives the default for each event. A state embeds it and overrides what it handles.
type ignore struct{}

func (ignore) Play() (State, string)  { return nil, "ignored" }
func (ignore) Pause() (State, string) { return nil, "ignored" }
func (ignore) Stop() (State, string)  { return nil, "ignored" }

type stopped struct{ ignore }
type playing struct{ ignore }
type paused struct{ ignore }

func (stopped) Name() string           { return "stopped" }
func (stopped) Play() (State, string)  { return playing{}, "start at 0:00" }
func (playing) Name() string           { return "playing" }
func (playing) Pause() (State, string) { return paused{}, "hold the position" }
func (playing) Stop() (State, string)  { return stopped{}, "rewind" }
func (paused) Name() string            { return "paused" }
func (paused) Play() (State, string)   { return playing{}, "resume" }
func (paused) Stop() (State, string)   { return stopped{}, "rewind" }

type Player struct{ state State }

func (p *Player) handle(event string, react func(State) (State, string)) {
	next, action := react(p.state)
	if next == nil {
		next = p.state
	}
	fmt.Printf("%s: %s (%s -> %s)\n", event, action, p.state.Name(), next.Name())
	p.state = next
}

func main() {
	p := &Player{state: stopped{}}
	p.handle("play", State.Play)
	p.handle("pause", State.Pause)
	p.handle("play", State.Play)
	p.handle("stop", State.Stop)
	p.handle("pause", State.Pause)
}
```

```output
play: start at 0:00 (stopped -> playing)
pause: hold the position (playing -> paused)
play: resume (paused -> playing)
stop: rewind (playing -> stopped)
pause: ignored (stopped -> stopped)
```

- Verdict: idiom. Each state is an empty struct that satisfies the `State` interface. An empty struct has size zero, so making a state value costs nothing.
- A state struct embeds `ignore`, so it gets the default methods. The methods that the state declares itself win over the embedded ones.
- An embedded method cannot return "this state", because it does not know the outer struct. The default methods therefore return `nil`, and the player reads `nil` as "stay".
- `State.Play` is a method expression. It is a function that takes the receiver as its first argument, so one `handle` method serves all three events.
