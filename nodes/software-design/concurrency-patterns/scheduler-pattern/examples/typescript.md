```typescript run id=scheduler-pattern
interface Entry {
  priority: number;
  arrival: number;
  name: string;
  wake: () => void;
}

// Decides which waiting task uses the printer next.
class Scheduler {
  busy: boolean;
  waiting: Entry[];
  arrivals: number;

  constructor() {
    this.busy = false;
    this.waiting = []; // entries { priority, arrival, name, wake }
    this.arrivals = 0;
  }

  enter(name: string, priority: number): Promise<void> {
    if (!this.busy && this.waiting.length === 0) {
      this.busy = true;
      return Promise.resolve();
    }
    // The task sleeps until the scheduler chooses it.
    return new Promise<void>((wake) => {
      this.waiting.push({ priority, arrival: ++this.arrivals, name, wake });
    });
  }

  done(): void {
    if (this.waiting.length === 0) {
      this.busy = false;
      return;
    }
    // The policy: the lowest priority number first, then the first to arrive.
    let best = 0;
    this.waiting.forEach((e, i) => {
      const b = this.waiting[best];
      if (e.priority < b.priority || (e.priority === b.priority && e.arrival < b.arrival)) best = i;
    });
    const [entry] = this.waiting.splice(best, 1);
    entry.wake(); // the printer stays busy and passes to that task
  }
}

const scheduler = new Scheduler();
const order: string[] = [];
let onPrinter = 0;
let mostOnPrinter = 0;

async function usePrinter(name: string): Promise<void> {
  onPrinter++;
  mostOnPrinter = Math.max(mostOnPrinter, onPrinter);
  order.push(name);
  await null; // let other tasks run while the job prints
  onPrinter--;
}

async function printJob(name: string, priority: number): Promise<void> {
  await scheduler.enter(name, priority);
  await usePrinter(name);
  scheduler.done();
}

async function main(): Promise<void> {
  await scheduler.enter("first", 0); // the first job takes the free printer
  await usePrinter("first");
  const jobs: [string, number][] = [["report", 2], ["invoice", 1], ["photo", 3], ["label", 1]];
  // Each call puts the job in the waiting list before it returns, in this order.
  const tasks = jobs.map(([name, priority]) => printJob(name, priority));
  console.log("jobs waiting when the first job ends:", scheduler.waiting.length);
  scheduler.done();
  await Promise.all(tasks);

  console.log("print order:", order.join(", "));
  console.log("jobs on the printer at once, at most:", mostOnPrinter);
}

main();
```

```output
jobs waiting when the first job ends: 4
print order: first, invoice, label, report, photo
jobs on the printer at once, at most: 1
```

- Verdict: idiom. The tasks share one thread. `enter` returns a `Promise<void>` and keeps its `resolve` function in the waiting list, and `done` resolves the chosen one.
