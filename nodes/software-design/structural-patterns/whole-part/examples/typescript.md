```typescript run id=car
interface StarterPart {
  crank(): string;
}

class Starter implements StarterPart {
  crank(): string {
    return "crank the engine";
  }
}

class NewStarter implements StarterPart {
  crank(): string {
    return "crank the engine with the new starter";
  }
}

class Engine {
  fire(): string {
    return "fire the cylinders";
  }
}

class Wheels {
  turn(): string {
    return "turn the wheels";
  }
}

class Car {
  #starter: StarterPart = new Starter();
  readonly #engine = new Engine();
  readonly #wheels = new Wheels();

  drive(): string {
    return [this.#starter.crank(), this.#engine.fire(), this.#wheels.turn()].join(", ");
  }
  replaceStarter(starter: StarterPart): void {
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

- Verdict: idiom. The `StarterPart` interface says what a replacement part must do, and private fields hide the parts.
