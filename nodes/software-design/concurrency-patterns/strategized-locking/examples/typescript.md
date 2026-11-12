```typescript run id=strategy
interface Lockable {
  run<T>(body: () => Promise<T>): Promise<T>;
}

// A lock for one task: it runs the body at once.
class NullMutex implements Lockable {
  run<T>(body: () => Promise<T>): Promise<T> {
    return body();
  }
}

// A lock for many tasks: each body waits for the one before it.
class TaskMutex implements Lockable {
  private tail: Promise<void> = Promise.resolve();
  async run<T>(body: () => Promise<T>): Promise<T> {
    const previous = this.tail;
    let release!: () => void;
    this.tail = new Promise<void>((resolve) => (release = resolve));
    await previous;
    try {
      return await body();
    } finally {
      release();
    }
  }
}

class FileCache {
  files = new Map<string, string>();
  lookups = 0;
  private lock: Lockable;
  constructor(lock: Lockable) {
    this.lock = lock; // the locking strategy is a parameter
  }

  find(path: string): Promise<string> {
    return this.lock.run(async () => {
      this.lookups++;
      if (!this.files.has(path)) {
        await null; // another task can run here
        this.files.set(path, "mapped " + path);
      }
      return this.files.get(path)!;
    });
  }
}

async function main(): Promise<void> {
  const single = new FileCache(new NullMutex());
  for (const path of ["/a.html", "/b.html", "/a.html"]) await single.find(path);
  console.log(`Null_Mutex cache: ${single.files.size} files, ${single.lookups} lookups`);

  const multi = new FileCache(new TaskMutex());
  const paths = ["/a.html", "/b.html", "/c.html"];
  const worker = async () => {
    for (let i = 0; i < 1000; i++) await multi.find(paths[i % 3]);
  };
  await Promise.all([worker(), worker(), worker(), worker()]);
  console.log(`Thread_Mutex cache: ${multi.files.size} files, ${multi.lookups} lookups`);
}

main();
```

```output
Null_Mutex cache: 2 files, 3 lookups
Thread_Mutex cache: 3 files, 4000 lookups
```

- Verdict: idiom. JavaScript runs the tasks on one thread, so the second strategy orders async tasks, not threads. The `Lockable` interface is the strategy type.
