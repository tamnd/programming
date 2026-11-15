```javascript run id=key-generator-plugin
class CounterKeyGenerator {
  // for unit tests: a counter in memory
  last = 0;
  nextKey() {
    return ++this.last;
  }
}

class SequenceKeyGenerator {
  // for production: it stands for a database sequence
  last = 1000;
  nextKey() {
    return ++this.last;
  }
}

// JavaScript cannot find a class by its name, so each plugin registers itself under a name.
// In a real program, the configuration can also name a module that import() loads.
const plugins = new Map();
plugins.set("CounterKeyGenerator", () => new CounterKeyGenerator());
plugins.set("SequenceKeyGenerator", () => new SequenceKeyGenerator());

// One configuration for each deployment. Only the configuration names the classes.
const CONFIGS = {
  test: { KeyGenerator: "CounterKeyGenerator" },
  production: { KeyGenerator: "SequenceKeyGenerator" },
  broken: { KeyGenerator: "FileKeyGenerator" },
};

class PluginFactory {
  constructor(config) {
    this.config = config;
  }

  get(iface) {
    const name = this.config[iface];
    const make = plugins.get(name);
    if (!make) throw new Error(`no plugin named ${name}`);
    return make();
  }
}

for (const [name, config] of Object.entries(CONFIGS)) {
  let gen;
  try {
    gen = new PluginFactory(config).get("KeyGenerator");
  } catch (e) {
    console.log(`${name}: error: ${e.message}`);
    continue;
  }
  const keys = [gen.nextKey(), gen.nextKey(), gen.nextKey()].join(", ");
  console.log(`${name}: keys ${keys} from ${gen.constructor.name}`);
}
```

```output
test: keys 1, 2, 3 from CounterKeyGenerator
production: keys 1001, 1002, 1003 from SequenceKeyGenerator
broken: error: no plugin named FileKeyGenerator
```

- Verdict: idiom. JavaScript cannot create a class from a string name, so each plugin registers a constructor in a map. A configuration can also name a module path that a dynamic `import()` loads.
- `gen.constructor.name` gives the name of the class of the loaded object.
