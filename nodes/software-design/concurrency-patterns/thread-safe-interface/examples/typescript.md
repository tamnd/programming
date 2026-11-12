```typescript run id=interface
// A lock for async tasks: each body waits for the one before it.
class TaskMutex {
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
  private lock = new TaskMutex(); // not reentrant
  files = new Map<string, string>();
  acquires = 0;

  // Interface methods: take the lock, then call an implementation method.
  find(path: string): Promise<string> {
    return this.lock.run(async () => {
      this.acquires++;
      return this.findI(path);
    });
  }

  bind(path: string): Promise<void> {
    return this.lock.run(async () => {
      this.acquires++;
      this.bindI(path);
    });
  }

  // Implementation methods: trust that the lock is held, never take it.
  private async findI(path: string): Promise<string> {
    let file = this.checkCacheI(path);
    if (file === undefined) {
      await null; // another task can run here, but it waits for the lock
      this.bindI(path); // not bind(): that would wait for itself
      file = this.checkCacheI(path)!;
    }
    return file;
  }

  private checkCacheI(path: string): string | undefined {
    return this.files.get(path);
  }

  private bindI(path: string): void {
    this.files.set(path, "mapped " + path);
  }
}

async function main(): Promise<void> {
  const cache = new FileCache();
  await cache.bind("/index.html");
  const paths = ["/a.html", "/b.html", "/c.html"];
  const worker = async () => {
    for (let i = 0; i < 1000; i++) await cache.find(paths[i % 3]);
  };
  await Promise.all([worker(), worker(), worker(), worker()]);
  console.log("find /a.html:", await cache.find("/a.html"));
  console.log("files cached:", cache.files.size);
  console.log("lock acquisitions:", cache.acquires);
}

main();
```

```output
find /a.html: mapped /a.html
files cached: 4
lock acquisitions: 4002
```

- Verdict: idiom. `private` marks the implementation methods for the compiler. The promise lock is not reentrant: a call from `findI` to `find` would wait for its own body to end.
