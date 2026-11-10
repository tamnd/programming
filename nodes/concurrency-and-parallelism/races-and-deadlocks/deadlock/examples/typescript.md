```typescript run id=ordered
class Mutex {
  #locked = false;
  #waiters: Array<() => void> = [];

  lock(): Promise<void> {
    if (!this.#locked) {
      this.#locked = true;
      return Promise.resolve();
    }
    return new Promise((resolve) => this.#waiters.push(resolve));
  }

  unlock(): void {
    const next = this.#waiters.shift();
    if (next) next();
    else this.#locked = false;
  }
}

interface Account {
  id: number;
  balance: number;
  mutex: Mutex;
}

const tick = (): Promise<void> => new Promise((r) => setTimeout(r, 1));

async function transfer(from: Account, to: Account, amount: number) {
  const [first, second] = from.id < to.id ? [from, to] : [to, from];
  await first.mutex.lock();
  await tick();
  await second.mutex.lock();
  from.balance -= amount;
  to.balance += amount;
  second.mutex.unlock();
  first.mutex.unlock();
}

const a: Account = { id: 1, balance: 100, mutex: new Mutex() };
const b: Account = { id: 2, balance: 100, mutex: new Mutex() };
const jobs: Promise<void>[] = [];
for (let i = 0; i < 20; i++) {
  jobs.push(transfer(a, b, 1), transfer(b, a, 2));
}
await Promise.all(jobs);
console.log(`a: ${a.balance}, b: ${b.balance}, total: ${a.balance + b.balance}`);
```

```output for=ordered
a: 120, b: 80, total: 200
```

- Verdict: native. TypeScript runs as JavaScript, so async functions can wait for each other in a cycle as in the JavaScript example.
- The fix is the same as with threads: every task takes the two locks in the order of the account ids. Without the order, the `await tick()` between the locks would let the tasks deadlock.
- The type checker does not see the order of locks, and it accepts a cycle of awaits.
