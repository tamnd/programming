```rust run id=connection-state
#[derive(Clone, Copy, PartialEq)]
enum State {
    Connecting,
    Connected,
    Disconnected,
}

fn name(s: State) -> &'static str {
    match s {
        State::Connecting => "connecting",
        State::Connected => "connected",
        State::Disconnected => "disconnected",
    }
}

// Before: a state flag and five optional fields.
#[derive(Default)]
struct FlagSet {
    state: Option<State>,
    last_ping_time: Option<u64>,
    last_ping_id: Option<u32>,
    session_id: Option<String>,
    when_initiated: Option<u64>,
    when_disconnected: Option<u64>,
}

// The fields that each state must have, and no others.
fn legal(c: &FlagSet) -> bool {
    let present = [
        c.last_ping_time.is_some(),
        c.last_ping_id.is_some(),
        c.session_id.is_some(),
        c.when_initiated.is_some(),
        c.when_disconnected.is_some(),
    ];
    match c.state {
        Some(State::Connecting) => present == [false, false, false, true, false],
        Some(State::Connected) => {
            present == [false, false, true, false, false]
                || present == [true, true, true, false, false]
        }
        Some(State::Disconnected) => present == [false, false, false, false, true],
        None => false,
    }
}

// After: each state carries its own fields.
enum ConnectionState {
    Connecting { when_initiated: u64 },
    Connected { session_id: String, last_ping: Option<(u64, u32)> },
    Disconnected { when_disconnected: u64 },
}

fn to_flags(s: &ConnectionState) -> FlagSet {
    match s {
        ConnectionState::Connecting { when_initiated } => FlagSet {
            state: Some(State::Connecting),
            when_initiated: Some(*when_initiated),
            ..FlagSet::default()
        },
        ConnectionState::Connected { session_id, last_ping } => FlagSet {
            state: Some(State::Connected),
            session_id: Some(session_id.clone()),
            last_ping_time: last_ping.map(|(time, _)| time),
            last_ping_id: last_ping.map(|(_, id)| id),
            ..FlagSet::default()
        },
        ConnectionState::Disconnected { when_disconnected } => FlagSet {
            state: Some(State::Disconnected),
            when_disconnected: Some(*when_disconnected),
            ..FlagSet::default()
        },
    }
}

fn main() {
    let mut flag_sets = Vec::new();
    for state in [State::Connecting, State::Connected, State::Disconnected] {
        for mask in 0..32u32 {
            let on = |bit: u32| mask & (1 << bit) != 0;
            flag_sets.push(FlagSet {
                state: Some(state),
                last_ping_time: on(0).then_some(12),
                last_ping_id: on(1).then_some(7),
                session_id: on(2).then(|| "s1".to_string()),
                when_initiated: on(3).then_some(10),
                when_disconnected: on(4).then_some(20),
            });
        }
    }
    let count = flag_sets.iter().filter(|c| legal(c)).count();
    println!("flag set: {} forms, {} legal", flag_sets.len(), count);

    // One value of each form: the optional last_ping gives Connected two forms.
    let sums = [
        ConnectionState::Connecting { when_initiated: 10 },
        ConnectionState::Connected { session_id: "s1".to_string(), last_ping: None },
        ConnectionState::Connected { session_id: "s1".to_string(), last_ping: Some((12, 7)) },
        ConnectionState::Disconnected { when_disconnected: 20 },
    ];
    let count = sums.iter().filter(|s| legal(&to_flags(s))).count();
    println!("sum type: {} forms, {} legal", sums.len(), count);

    // No session id, but the loose type accepts it. The precise type has
    // no way to write a Connected value without a session_id.
    let bad = FlagSet { state: Some(State::Connected), ..FlagSet::default() };
    let state = bad.state.map_or("none", name);
    let ok = if legal(&bad) { "yes" } else { "no" };
    println!("built: {state} without a session id, legal: {ok}");
}
```

```output
flag set: 96 forms, 4 legal
sum type: 4 forms, 4 legal
built: connected without a session id, legal: no
```
