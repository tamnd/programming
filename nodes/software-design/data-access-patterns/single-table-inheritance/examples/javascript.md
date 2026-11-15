```javascript run id=one-table-for-hierarchy
// A stand-in for the database: one table for the whole hierarchy.
// Columns: type, name, club, runs, wickets. A missing value is null.
const playerTable = new Map();
let lastId = 0;

class Player {
  constructor(name, id = null) {
    this.id = id;
    this.name = name;
  }
}

class Footballer extends Player {
  constructor(name, club, id = null) {
    super(name, id);
    this.club = club;
  }
  describe() {
    return `Footballer ${this.name}, club ${this.club}`;
  }
}

class Cricketer extends Player {
  constructor(name, runs, id = null) {
    super(name, id);
    this.runs = runs;
  }
  describe() {
    return `Cricketer ${this.name}, runs ${this.runs}`;
  }
}

class Bowler extends Cricketer {
  constructor(name, runs, wickets, id = null) {
    super(name, runs, id);
    this.wickets = wickets;
  }
  describe() {
    return `Bowler ${this.name}, runs ${this.runs}, wickets ${this.wickets}`;
  }
}

// The discriminator: one value for each concrete class, kept in the type column.
function typeOf(p) {
  if (p instanceof Bowler) return "bowler";
  if (p instanceof Cricketer) return "cricketer";
  return "footballer";
}
const SUBTYPES = { cricketer: ["cricketer", "bowler"] };

function insert(p) {
  // Each class writes its own fields; the columns of the other classes stay null.
  p.id = ++lastId;
  playerTable.set(p.id, {
    type: typeOf(p),
    name: p.name,
    club: p.club ?? null,
    runs: p.runs ?? null,
    wickets: p.wickets ?? null,
  });
}

function load(id, r) {
  if (r.type === "footballer") return new Footballer(r.name, r.club, id);
  if (r.type === "cricketer") return new Cricketer(r.name, r.runs, id);
  return new Bowler(r.name, r.runs, r.wickets, id);
}

function findAll(kind = null) {
  const kinds = kind === null ? null : SUBTYPES[kind] ?? [kind];
  return [...playerTable]
    .filter(([, r]) => kinds === null || kinds.includes(r.type))
    .map(([id, r]) => load(id, r));
}

const show = (v) => (v === null ? "null" : String(v));

insert(new Footballer("Ann", "Lions"));
insert(new Cricketer("Bo", 412));
insert(new Bowler("Cy", 95, 31));
for (const [id, r] of playerTable) {
  console.log(`row: ${[id, r.type, r.name, r.club, r.runs, r.wickets].map(show).join(" ")}`);
}
for (const p of findAll()) console.log(`loaded: ${p.describe()}`);
console.log(`cricketers: ${findAll("cricketer").map((p) => p.name).join(", ")}`);
let nulls = 0;
for (const r of playerTable.values()) {
  nulls += [r.club, r.runs, r.wickets].filter((v) => v === null).length;
}
console.log(`null cells in subclass columns: ${nulls} of ${3 * playerTable.size}`);
```

```output
row: 1 footballer Ann Lions null null
row: 2 cricketer Bo null 412 null
row: 3 bowler Cy null 95 31
loaded: Footballer Ann, club Lions
loaded: Cricketer Bo, runs 412
loaded: Bowler Cy, runs 95, wickets 31
cricketers: Bo, Cy
null cells in subclass columns: 5 of 9
```

- Verdict: idiom. `typeOf` uses `instanceof` and tests `Bowler` before `Cricketer`, because a bowler is also an instance of `Cricketer`. `p.club ?? null` writes null for a field that the class does not have.
- The table is a `Map` of plain objects with one property for each column.
