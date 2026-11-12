```rust run id=car
mod car {
    pub trait StarterPart {
        fn crank(&self) -> String;
    }

    pub struct Starter;

    impl StarterPart for Starter {
        fn crank(&self) -> String {
            "crank the engine".to_string()
        }
    }

    pub struct NewStarter;

    impl StarterPart for NewStarter {
        fn crank(&self) -> String {
            "crank the engine with the new starter".to_string()
        }
    }

    struct Engine; // private to the module
    struct Wheels;

    impl Engine {
        fn fire(&self) -> &'static str {
            "fire the cylinders"
        }
    }

    impl Wheels {
        fn turn(&self) -> &'static str {
            "turn the wheels"
        }
    }

    pub struct Car {
        starter: Box<dyn StarterPart>,
        engine: Engine,
        wheels: Wheels,
    }

    impl Car {
        pub fn new() -> Car {
            Car { starter: Box::new(Starter), engine: Engine, wheels: Wheels }
        }
        pub fn drive(&self) -> String {
            format!("{}, {}, {}", self.starter.crank(), self.engine.fire(), self.wheels.turn())
        }
        pub fn replace_starter(&mut self, starter: Box<dyn StarterPart>) {
            self.starter = starter;
        }
    }
}

fn main() {
    let mut car = car::Car::new();
    println!("drive: {}", car.drive());
    car.replace_starter(Box::new(car::NewStarter));
    println!("drive: {}", car.drive());
}
```

```output
drive: crank the engine, fire the cylinders, turn the wheels
drive: crank the engine with the new starter, fire the cylinders, turn the wheels
```

- Verdict: idiom. The `Car` struct owns its parts, its fields are private to the module, and `Engine` and `Wheels` are private types.
