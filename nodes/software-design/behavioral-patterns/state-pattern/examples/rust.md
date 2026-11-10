```rust run id=player
#[derive(Clone, Copy, Debug)]
enum State {
    Stopped,
    Playing,
    Paused,
}

#[derive(Clone, Copy, Debug)]
enum Event {
    Play,
    Pause,
    Stop,
}

fn next(state: State, event: Event) -> (State, &'static str) {
    use Event::*;
    use State::*;
    // No wildcard arm: each pair is listed, so a new state or event needs new arms.
    match (state, event) {
        (Stopped, Play) => (Playing, "start at 0:00"),
        (Stopped, Pause | Stop) => (Stopped, "ignored"),
        (Playing, Pause) => (Paused, "hold the position"),
        (Playing, Play) => (Playing, "ignored"),
        (Paused, Play) => (Playing, "resume"),
        (Paused, Pause) => (Paused, "ignored"),
        (Playing | Paused, Stop) => (Stopped, "rewind"),
    }
}

fn name<T: std::fmt::Debug>(value: T) -> String {
    format!("{value:?}").to_lowercase()
}

fn main() {
    let mut state = State::Stopped;
    for event in [Event::Play, Event::Pause, Event::Play, Event::Stop, Event::Pause] {
        let (after, action) = next(state, event);
        println!("{}: {action} ({} -> {})", name(event), name(state), name(after));
        state = after;
    }
}
```

```output
play: start at 0:00 (stopped -> playing)
pause: hold the position (playing -> paused)
play: resume (paused -> playing)
stop: rewind (playing -> stopped)
pause: ignored (stopped -> stopped)
```

- Verdict: native. An enum and a `match` on the pair `(state, event)` hold the whole machine in one function.
- The `match` has no `_` arm. The compiler checks that the arms cover every pair, so a state added to the enum stops the build until each event has an arm for it. The node page shows this error.
- The function is pure. It takes a state and an event and returns the next state, so a test can call it with no player object.
- The Rust book builds the same kind of machine with trait objects and then with a distinct type for each state. With distinct types, a call that is wrong for the state is a compile error.
