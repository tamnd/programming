```javascript run id=await-cycle
class Mutex {
  #locked = false;
  #waiters = [];

  lock() {
    if (!this.#locked) {
      this.#locked = true;
      return Promise.resolve();
    }
    return new Promise((resolve) => this.#waiters.push(resolve));
  }

  unlock() {
    const next = this.#waiters.shift();
    if (next) next();
    else this.#locked = false;
  }
}

const tick = () => new Promise((resolve) => setTimeout(resolve, 10));
const a = new Mutex();
const b = new Mutex();

async function transfer(name, first, second) {
  await first.lock();
  await tick();
  await second.lock();
  console.log(name, "has both locks");
  second.unlock();
  first.unlock();
}

const work = Promise.all([transfer("t1", a, b), transfer("t2", b, a)]);
const timeout = new Promise((resolve) =>
  setTimeout(() => resolve("timeout"), 200),
);
console.log(await Promise.race([work.then(() => "done"), timeout]));
```

```output for=await-cycle
timeout
```

- Verdict: native. JavaScript has no threads that share a heap, but two async functions can wait for each other. Each `await` here waits for a lock that the other function holds.
- The cycle is silent. A promise that never settles uses no CPU and gives no error. The timer in `Promise.race` turns the wait into a result.
- In Node.js, an ES module whose top-level `await` never settles ends with exit code 13:

```javascript fails expect=exit:13 id=unsettled
const never = new Promise(() => {});
await never;
console.log("not printed");
```

Node.js prints a warning about an unsettled top-level await, then exits with code 13.
