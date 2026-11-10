```javascript run id=player
// The prototype of every state: each event is ignored unless a state overrides it.
const base = {
  play() {
    return [this, "ignored"];
  },
  pause() {
    return [this, "ignored"];
  },
  stop() {
    return [this, "ignored"];
  },
};

const stopped = Object.create(base);
const playing = Object.create(base);
const paused = Object.create(base);

Object.assign(stopped, {
  name: "stopped",
  play: () => [playing, "start at 0:00"],
});
Object.assign(playing, {
  name: "playing",
  pause: () => [paused, "hold the position"],
  stop: () => [stopped, "rewind"],
});
Object.assign(paused, {
  name: "paused",
  play: () => [playing, "resume"],
  stop: () => [stopped, "rewind"],
});

class Player {
  state = stopped;

  handle(event) {
    const [after, action] = this.state[event]();
    console.log(`${event}: ${action} (${this.state.name} -> ${after.name})`);
    this.state = after;
  }
}

const player = new Player();
for (const event of ["play", "pause", "play", "stop", "pause"]) {
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

- Verdict: idiom. Each state is a plain object. `Object.create(base)` makes `base` its prototype, so a state that has no method for an event finds the default in `base`.
- The states are made first and filled in later with `Object.assign`, because each state refers to the others.
- `this.state[event]()` looks up the method by the name of the event. A name with no method throws a `TypeError`, so the caller must send only known events.
