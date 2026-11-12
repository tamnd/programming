```typescript run id=twin-ball
abstract class Item {
  name: string;
  constructor(name: string) {
    this.name = name;
  }
  abstract position(): number;
  show(): void {
    console.log(`${this.name} at ${this.position()}`);
  }
}

abstract class Ticker {
  count = 0;
  abstract onTick(): void;
  tick(): void {
    this.count += 1;
    this.onTick();
  }
}

class BallItem extends Item {
  x = 0;
  twin: BallTicker;
  constructor(name: string) {
    super(name);
    this.twin = new BallTicker(this);
  }
  position(): number {
    return this.x;
  }
  move(): void {
    this.x += 2;
  }
}

class BallTicker extends Ticker {
  twin: BallItem;
  constructor(twin: BallItem) {
    super();
    this.twin = twin;
  }
  onTick(): void {
    this.twin.move();
  }
}

const ball = new BallItem("ball");
ball.show();
for (let i = 0; i < 2; i++) {
  ball.twin.tick();
  ball.show();
}
console.log(`ticks: ${ball.twin.count}`);
```

```output
ball at 0
ball at 2
ball at 4
ticks: 2
```

- Verdict: idiom. A class extends one class and can implement several interfaces, so the code of a second parent needs a twin.
