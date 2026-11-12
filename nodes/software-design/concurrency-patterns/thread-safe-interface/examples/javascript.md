```javascript run id=interface
// A lock for async tasks: each body waits for the one before it.
class TaskMutex {
  tail = Promise.resolve();
  async run(body) {
    const previous = this.tail;
    let release;
    this.tail = new Promise((resolve) => (release = resolve));
    await previous;
    try {
      return await body();
    } finally {
      release();
    }
  }
}

class FileCache {
  lock = new TaskMutex(); // not reentrant
  files = new Map();
  acquires = 0;

  // Interface methods: take the lock, then call an implementation method.
  find(path) {
    return this.lock.run(async () => {
      this.acquires++;
      return this.findI(path);
    });
  }

  bind(path) {
    return this.lock.run(async () => {
      this.acquires++;
      this.bindI(path);
    });
  }

  // Implementation methods: trust that the lock is held, never take it.
  async findI(path) {
    let file = this.checkCacheI(path);
    if (file === undefined) {
      await null; // another task can run here, but it waits for the lock
      this.bindI(path); // not bind(): that would wait for itself
      file = this.checkCacheI(path);
    }
    return file;
  }

  checkCacheI(path) {
    return this.files.get(path);
  }

  bindI(path) {
    this.files.set(path, "mapped " + path);
  }
}

async function main() {
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

- Verdict: idiom. The example uses plain methods, so the `I` suffix marks the implementation methods by convention. The promise lock is not reentrant: a call from `findI` to `find` would wait for its own body to end.
