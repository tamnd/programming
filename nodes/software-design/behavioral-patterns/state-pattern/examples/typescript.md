```typescript run id=player
type StateName = "stopped" | "playing" | "paused";
type EventName = "play" | "pause" | "stop";
type Transition = { next: StateName; action: string };

// The transition table. Record<StateName, ...> makes the compiler ask for a row for each state.
const table: Record<StateName, Partial<Record<EventName, Transition>>> = {
  stopped: {
    play: { next: "playing", action: "start at 0:00" },
  },
  playing: {
    pause: { next: "paused", action: "hold the position" },
    stop: { next: "stopped", action: "rewind" },
  },
  paused: {
    play: { next: "playing", action: "resume" },
    stop: { next: "stopped", action: "rewind" },
  },
};

class Player {
  state: StateName = "stopped";

  handle(event: EventName): void {
    const transition = table[this.state][event] ?? { next: this.state, action: "ignored" };
    console.log(`${event}: ${transition.action} (${this.state} -> ${transition.next})`);
    this.state = transition.next;
  }
}

const player = new Player();
const events: EventName[] = ["play", "pause", "play", "stop", "pause"];
for (const event of events) {
  player.handle(event);
}
```

```output
play: start at 0:00 (stopped -> playing)
pause: hold the position (playing -> paused)
play: resume (paused -> playing)
stop: rewind (playing -> stopped)
pause: ignored (stopped -> stopped)
```

- Verdict: idiom. This is the table-driven form. The states and events are unions of string literals, and the transitions are data in one table.
- `Record<StateName, ...>` needs a key for each state, so `tsc` rejects a table that leaves out a state. `Partial` lets a row leave out events, and the `??` gives those events the default.
- A table fits when the behaviour of a state is only "go to the next state". When each state needs its own code, a function or an object for each state fits better.
