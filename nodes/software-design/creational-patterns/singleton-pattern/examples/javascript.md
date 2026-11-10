```javascript norun reason=fragment file=settings.mjs group=modules
console.log("creating the settings");

export const settings = { level: "info" };
```

```javascript run file=main.js group=modules id=modules
import { settings } from "./settings.mjs";

async function worker() {
  // A second import of the same specifier gets the same module.
  const { settings: again } = await import("./settings.mjs");
  console.log("worker sets debug");
  again.level = "debug";
  return again;
}

console.log(`main: ${settings.level}`);
const seen = await worker();
console.log(`main: ${settings.level}`);
console.log(`same instance: ${seen === settings}`);
```

```output
creating the settings
main: info
worker sets debug
main: debug
same instance: true
```

- Verdict: native. An ES module is evaluated once, and every import of the same specifier from the same place gets the same module. So the exported object is the single instance.
- The module runs when the program first imports it, not on first use. A module that must create its object lazily exports a function that creates and stores it on the first call.
- JavaScript runs one thread for this code, so there is no race on creation. Worker threads each load their own copy of the module, with their own instance.
- The `settings` binding is `const`, but the object is not frozen. `Object.freeze` gives a read-only singleton.
