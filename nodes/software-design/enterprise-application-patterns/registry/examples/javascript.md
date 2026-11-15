```javascript run id=person-registry
class PersonFinder {
  constructor(rows) {
    this.rows = rows;
  }

  find(personId) {
    return this.rows.get(personId);
  }
}

// A well-known object with one explicit method for each service.
class Registry {
  static #sole = null;

  constructor({ personFinder = null } = {}) {
    this.personFinder = personFinder;
  }

  // The start of the program, or a test, sets the registry.
  static install(registry) {
    Registry.#sole = registry;
  }

  static personFinder() {
    const finder = Registry.#sole?.personFinder;
    if (!finder) throw new Error("no person finder is registered");
    return finder;
  }
}

// Code deep in the program, with no reference to a finder.
const greet = (personId) => `hello, ${Registry.personFinder().find(personId)}`;

class StubFinder {
  find(personId) {
    return `test person ${personId}`;
  }
}

Registry.install(new Registry({ personFinder: new PersonFinder(new Map([[1, "ann"], [2, "bob"]])) }));
console.log(greet(1));
console.log(greet(2));

Registry.install(new Registry({ personFinder: new StubFinder() })); // a test installs a stub
console.log("in a test: " + greet(1));

Registry.install(new Registry());
try {
  console.log(greet(1));
} catch (e) {
  console.log("with an empty registry:", e.message);
}
```

```output
hello, ann
hello, bob
in a test: hello, test person 1
with an empty registry: no person finder is registered
```

- Verdict: idiom. A static private field holds the sole instance, and static methods give the services. A module-level variable that the module exports through functions is the same design.
- The optional chaining `?.` makes an empty registry give a clear error, not a `TypeError`.
