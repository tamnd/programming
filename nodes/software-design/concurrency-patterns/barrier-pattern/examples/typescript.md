```typescript run id=barrier-pattern
// A cyclic barrier for async tasks on one event loop.
class Barrier {
  parties: number;
  action: () => void;
  waiting: (() => void)[];

  constructor(parties: number, action: () => void) {
    this.parties = parties;
    this.action = action;
    this.waiting = []; // the resolve functions of the tasks that wait
  }

  wait(): Promise<void> {
    return new Promise<void>((resolve) => {
      this.waiting.push(resolve);
      if (this.waiting.length === this.parties) {
        this.action(); // runs once per trip, before the tasks are released
        const released = this.waiting;
        this.waiting = []; // the barrier can be used again
        released.forEach((r) => r());
      }
    });
  }
}

const N = 8;
const values: number[] = Array.from({ length: N }, (_, i) => i + 1);
const distances: number[] = [1, 2, 4]; // each step adds the value that is d places to the left
console.log("input: [" + values.join(", ") + "]");
let trips: number = 0;
const barrier = new Barrier(N, () => trips++);

async function worker(i: number): Promise<void> {
  for (const d of distances) {
    // Phase 1: read a value that the last step wrote.
    const add = i >= d ? values[i - d] : 0;
    await barrier.wait(); // nobody writes until every worker has read
    // Phase 2: write the new value.
    values[i] += add;
    await barrier.wait(); // nobody reads until every worker has written
  }
}

async function main(): Promise<void> {
  await Promise.all(Array.from({ length: N }, (_, i) => worker(i)));
  const matches = values.every((v, i) => v === ((i + 1) * (i + 2)) / 2);
  console.log("steps:", distances.length);
  console.log("barrier trips:", trips);
  console.log("prefix sums: [" + values.join(", ") + "]");
  console.log("matches a running total:", matches ? "True" : "False");
}

main();
```

```output
input: [1, 2, 3, 4, 5, 6, 7, 8]
steps: 3
barrier trips: 6
prefix sums: [1, 3, 6, 10, 15, 21, 28, 36]
matches a running total: True
```

- Verdict: idiom. The tasks share one thread, so the barrier is a list of `resolve` functions that the last task to arrive calls. It shows the phases, not parallel work.
