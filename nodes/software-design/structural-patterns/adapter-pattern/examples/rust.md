```rust run id=thermometer
trait Thermometer {
    fn celsius(&self) -> f64;
}

fn report(name: &str, sensor: &dyn Thermometer) {
    println!("{name}: {:.1} C", sensor.celsius());
}

struct RoomSensor(f64);

impl Thermometer for RoomSensor {
    fn celsius(&self) -> f64 {
        self.0
    }
}

// The vendor type. In a real program it is in another crate.
mod vendor {
    pub struct LegacyThermometer {
        tenths: i32,
    }

    impl LegacyThermometer {
        pub fn new(tenths: i32) -> Self {
            LegacyThermometer { tenths }
        }

        pub fn fahrenheit_tenths(&self) -> i32 {
            self.tenths
        }
    }
}

use vendor::LegacyThermometer;

// A newtype: a tuple struct with one field, which wraps the vendor type.
struct LegacyAdapter(LegacyThermometer);

impl Thermometer for LegacyAdapter {
    fn celsius(&self) -> f64 {
        (f64::from(self.0.fahrenheit_tenths()) / 10.0 - 32.0) * 5.0 / 9.0
    }
}

fn main() {
    report("lab", &RoomSensor(21.5));
    report("patient", &LegacyAdapter(LegacyThermometer::new(986)));
    report("hall", &LegacyAdapter(LegacyThermometer::new(704)));
}
```

```output
lab: 21.5 C
patient: 37.0 C
hall: 21.3 C
```

- Verdict: idiom. The adapter is a newtype that owns the vendor value and implements the trait. The Rust book describes this form and states that it has no run-time cost.
- Here the trait is local, so `impl Thermometer for LegacyThermometer` would also compile. When both the trait and the type come from other crates, the orphan rule forbids that impl, and the newtype is the way to adapt.
- `f64::from` converts the `i32` with no loss, because every `i32` value fits in an `f64`. The standard library has no `From<i64>` for `f64`, so the compiler rejects the same call for a type that could lose a value.
