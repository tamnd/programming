```javascript run id=twin-ball
class Item {
  constructor(name) {
    this.name = name;
  }
  show() {
    console.log(`${this.name} at ${this.position()}`);
  }
}

class Ticker {
  constructor() {
    this.count = 0;
  }
  tick() {
    this.count += 1;
    this.onTick();
  }
}

class BallItem extends Item {
  constructor(name) {
    super(name);
    this.x = 0;
    this.twin = new BallTicker(this);
  }
  position() {
    return this.x;
  }
  move() {
    this.x += 2;
  }
}

class BallTicker extends Ticker {
  constructor(twin) {
    super();
    this.twin = twin;
  }
  onTick() {
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

- Verdict: idiom. A class has one `extends` clause, so the two parents need two linked objects.
