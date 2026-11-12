```rust run id=typestate-door
use std::marker::PhantomData;

struct Open;
struct Closed;

// The state is a type parameter, so the compiler knows it.
struct Door<S> {
    name: String,
    state: PhantomData<S>,
}

impl Door<Closed> {
    fn new(name: &str) -> Self {
        Door { name: name.to_string(), state: PhantomData }
    }
    fn open(self) -> Door<Open> {
        println!("{} opens", self.name);
        Door { name: self.name, state: PhantomData }
    }
}

impl Door<Open> {
    fn close(self) -> Door<Closed> {
        println!("{} closes", self.name);
        Door { name: self.name, state: PhantomData }
    }
}

fn main() {
    let door = Door::new("front");
    let door = door.open();
    let door = door.close();
    let _ = door.open();
}
```

```output for=typestate-door
front opens
front closes
front opens
```

```rust fails expect=error:E0599 id=close-a-closed-door
use std::marker::PhantomData;

struct Open;
struct Closed;

struct Door<S> {
    state: PhantomData<S>,
}

impl Door<Closed> {
    fn open(self) -> Door<Open> {
        Door { state: PhantomData }
    }
}

impl Door<Open> {
    fn close(self) -> Door<Closed> {
        Door { state: PhantomData }
    }
}

fn main() {
    let door: Door<Closed> = Door { state: PhantomData };
    let door = door.close(); // close is defined only for Door<Open>
    let _ = door.open();
}
```

- Verdict: native. A binding changes only when it is declared `mut`. The typestate idiom puts the state in a type parameter, so `Door<Closed>` has `open` and `Door<Open>` has `close`, and each transition consumes the old value and returns a new one. `PhantomData` marks the type parameter and takes no memory. The fails block calls `close` on a closed door, and the compiler reports E0599 because `Door<Closed>` has no such method.
