```javascript run id=lazy
let loads = 0;

function loadConfig() {
  console.log("loading config"); // stands for a slow read of a file
  loads += 1;
  return { timeout: 30 };
}

const settings = {
  // A getter that replaces itself with a plain property on first access.
  get config() {
    const value = loadConfig();
    Object.defineProperty(this, "config", { value, writable: false });
    return value;
  },
};

console.log("settings created, loads =", loads);
console.log("timeout =", settings.config.timeout);
console.log("timeout =", settings.config.timeout);
console.log("loads =", loads);

// For an async value, cache the promise, not the result, so that callers
// that ask at the same time share one load.
let asyncLoads = 0;
let pending = null;
function config() {
  pending ??= new Promise((resolve) => {
    asyncLoads += 1;
    setTimeout(() => resolve({ timeout: 30 }), 10);
  });
  return pending;
}

const callers = Array.from({ length: 8 }, () => config());
Promise.all(callers).then(() => console.log("8 callers: loads =", asyncLoads));
```

```output
settings created, loads = 0
loading config
timeout = 30
timeout = 30
loads = 1
8 callers: loads = 1
```

- Verdict: idiom. MDN calls this form a smart, self-overwriting or lazy getter: the getter computes the value, then defines a data property with the same name.
- JavaScript runs one thread per agent, so a synchronous lazy value needs no lock. An asynchronous load does have a race: a second caller can ask before the first load ends. Store the promise to close it.
- ES modules are evaluated once, at the first import, and dynamic `import()` loads a module only when the program asks for it.
