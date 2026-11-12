```rust run id=wrappers
use std::cell::{Cell, OnceCell};

trait Thermometer {
    fn read(&self) -> i32;
}

struct Sensor {
    reads: Cell<u32>,
}

impl Thermometer for Sensor {
    fn read(&self) -> i32 {
        self.reads.set(self.reads.get() + 1);
        20
    }
}

struct FahrenheitAdapter<T: Thermometer>(T);

impl<T: Thermometer> FahrenheitAdapter<T> {
    fn fahrenheit(&self) -> i32 {
        self.0.read() * 9 / 5 + 32
    }
}

struct Calibrated<T: Thermometer> {
    inner: T,
    offset: i32,
}

impl<T: Thermometer> Thermometer for Calibrated<T> {
    fn read(&self) -> i32 {
        self.inner.read() + self.offset
    }
}

struct CachingProxy<'a, T: Thermometer> {
    inner: &'a T,
    value: OnceCell<i32>,
}

impl<T: Thermometer> Thermometer for CachingProxy<'_, T> {
    fn read(&self) -> i32 {
        *self.value.get_or_init(|| self.inner.read())
    }
}

fn sensor() -> Sensor {
    Sensor { reads: Cell::new(0) }
}

fn main() {
    println!("adapter: {} F", FahrenheitAdapter(sensor()).fahrenheit());
    println!("decorator: {}", Calibrated { inner: sensor(), offset: 1 }.read());
    let real = sensor();
    let proxy = CachingProxy { inner: &real, value: OnceCell::new() };
    let (first, second) = (proxy.read(), proxy.read());
    println!("proxy: {} {}, real reads: {}", first, second, real.reads.get());
}
```

```output
adapter: 68 F
decorator: 21
proxy: 20 20, real reads: 1
```

- Verdict: idiom. Generic wrapper structs implement the same trait as the value they hold, and calls through a generic type parameter are dispatched statically. `Deref` is meant for smart pointers, not for decorators.
