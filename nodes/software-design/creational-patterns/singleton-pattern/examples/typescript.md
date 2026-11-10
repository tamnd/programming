```typescript run id=settings
class Settings {
  static #instance: Settings | undefined;

  level = "info";

  private constructor() {
    console.log("creating the settings");
  }

  static get instance(): Settings {
    Settings.#instance ??= new Settings();
    return Settings.#instance;
  }
}

function worker(): Settings {
  const settings = Settings.instance;
  console.log("worker sets debug");
  settings.level = "debug";
  return settings;
}

const first = Settings.instance;
console.log(`main: ${first.level}`);
const seen = worker();
console.log(`main: ${Settings.instance.level}`);
console.log(`same instance: ${seen === first}`);
```

```output
creating the settings
main: info
worker sets debug
main: debug
same instance: true
```

- Verdict: idiom. The classic form: a `private` constructor and a static accessor that creates the instance on first use with `??=`.
- `private` is a compile-time check. After the types are stripped, JavaScript code can still call `new Settings()`. The `#instance` field is private at run time too.
- An exported object from an ES module is the more usual TypeScript form, as the JavaScript example shows.
