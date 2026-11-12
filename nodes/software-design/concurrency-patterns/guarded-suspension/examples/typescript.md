```typescript run id=guarded-suspension
// The tasks share one thread. A waiting task keeps a promise in a list.
class Tank {
  level: number;
  lowest: number;
  filled: number;
  taken: number;
  served: number;
  waiters: Array<() => void>;

  constructor() {
    this.level = 0;
    this.lowest = 0; // the lowest level seen after a take
    this.filled = 0;
    this.taken = 0;
    this.served = 0;
    this.waiters = [];
  }

  async take(amount: number): Promise<void> {
    // Guarded suspension: wait in a loop until the guard is true.
    while (this.level < amount) {
      await new Promise<void>((resolve) => this.waiters.push(resolve));
    }
    this.level -= amount;
    this.lowest = Math.min(this.lowest, this.level);
    this.taken += amount;
    this.served++;
  }

  fill(amount: number): void {
    this.level += amount;
    this.filled += amount;
    const waiters = this.waiters; // each waiting taker checks its guard again
    this.waiters = [];
    for (const resolve of waiters) resolve();
  }
}

const tick = (): Promise<void> => new Promise<void>((resolve) => setImmediate(resolve));

async function main(): Promise<void> {
  const tank = new Tank();
  const takers = [5, 3, 8].map((a) => tank.take(a));
  for (let i = 0; i < 10; i++) {
    tank.fill(2);
    await tick();
  }
  await Promise.all(takers);

  console.log("units filled:", tank.filled);
  console.log("units taken:", tank.taken);
  console.log("takers served:", tank.served);
  console.log("level never below 0:", tank.lowest >= 0 ? "True" : "False");
  console.log("units left:", tank.level);
}

main();
```

```output
units filled: 20
units taken: 16
takers served: 3
level never below 0: True
units left: 4
```

- Verdict: idiom. The tasks share one thread, and a waiting task keeps a promise in a list that `fill` resolves; the task then tests its guard again.
