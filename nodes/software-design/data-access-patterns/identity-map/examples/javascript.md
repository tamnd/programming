```javascript run id=one-object-per-row
class Person {
  constructor(id, name, city) {
    Object.assign(this, { id, name, city });
  }
}

// A stand-in for the people table, because Node has no SQL driver in its standard library.
// Each call of select() stands for one SELECT statement, and returns fresh row objects.
class Database {
  rows = [[1, "Ann", "Oslo"], [2, "Bo", "Oslo"], [3, "Cy", "Bergen"]];
  selects = 0;

  select(where) {
    this.selects++;
    return this.rows.filter(where).map((row) => [...row]);
  }
}

class PersonMapper {
  #identityMap = new Map(); // primary key -> the one object for that row

  constructor(db) {
    this.db = db;
  }

  find(id) {
    if (this.#identityMap.has(id)) return this.#identityMap.get(id); // no query
    const [row] = this.db.select(([rowId]) => rowId === id);
    return row === undefined ? undefined : this.#load(row);
  }

  findByCity(city) {
    // The map is keyed by id, so a query by city must run.
    return this.db.select(([, , rowCity]) => rowCity === city).map((row) => this.#load(row));
  }

  #load([id, name, city]) {
    let person = this.#identityMap.get(id);
    if (person === undefined) {
      person = new Person(id, name, city);
      this.#identityMap.set(id, person);
    }
    return person;
  }
}

const db = new Database();
const mapper = new PersonMapper(db); // one mapper, and one map, for each session
const a = mapper.find(1);
const b = mapper.find(1);
console.log(`a === b: ${a === b} | selects: ${db.selects}`);

const oslo = mapper.findByCity("Oslo");
console.log(`Oslo: ${oslo.map((p) => p.name).join(", ")} | oslo[0] === a: ${oslo[0] === a} | selects: ${db.selects}`);

a.city = "Rome"; // a change in memory, not saved yet
console.log(`b.city: ${b.city}`);
console.log(`Oslo again: ${mapper.findByCity("Oslo").map((p) => `${p.name} in ${p.city}`).join(", ")}`);
console.log(`another session: same object: ${new PersonMapper(db).find(1) === a}`);
```

```output
a === b: true | selects: 1
Oslo: Ann, Bo | oslo[0] === a: true | selects: 2
b.city: Rome
Oslo again: Ann in Rome, Bo in Oslo
another session: same object: false
```

- Verdict: idiom. A `Map` from the primary key to the object is the identity map, and `===` on two objects compares references.
- The query by city runs, because the map knows objects only by key, but `#load` returns the object that is already in the map, with its unsaved city.
