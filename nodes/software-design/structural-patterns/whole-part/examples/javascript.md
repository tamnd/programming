```javascript run id=car
class Starter {
  crank() {
    return "crank the engine";
  }
}

class NewStarter {
  crank() {
    return "crank the engine with the new starter";
  }
}

class Engine {
  fire() {
    return "fire the cylinders";
  }
}

class Wheels {
  turn() {
    return "turn the wheels";
  }
}

class Car {
  // the parts: private fields that no client can reach
  #starter = new Starter();
  #engine = new Engine();
  #wheels = new Wheels();

  drive() {
    return [this.#starter.crank(), this.#engine.fire(), this.#wheels.turn()].join(", ");
  }
  replaceStarter(starter) {
    this.#starter = starter;
  }
}

const car = new Car();
console.log(`drive: ${car.drive()}`);
car.replaceStarter(new NewStarter());
console.log(`drive: ${car.drive()}`);
```

```output
drive: crank the engine, fire the cylinders, turn the wheels
drive: crank the engine with the new starter, fire the cylinders, turn the wheels
```

- Verdict: idiom. Private `#` fields hold the parts, so code outside the class cannot reach them.
