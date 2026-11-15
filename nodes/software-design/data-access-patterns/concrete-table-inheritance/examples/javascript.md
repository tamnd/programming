```javascript run id=table-per-concrete-class
// A stand-in for the database: one table for each concrete class, with all its fields.
const tables = { footballer: new Map(), cricketer: new Map(), bowler: new Map() };
let nextId = 1; // One key source for all the tables.

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

function insert(p) {
  // Each concrete class has one table with all its fields, the inherited ones too.
  p.id = nextId++;
  if (p instanceof Bowler) tables.bowler.set(p.id, [p.name, p.runs, p.wickets]);
  else if (p instanceof Cricketer) tables.cricketer.set(p.id, [p.name, p.runs]);
  else tables.footballer.set(p.id, [p.name, p.club]);
}

const MAKERS = [
  ["footballer", (id, [name, club]) => new Footballer(name, club, id)],
  ["cricketer", (id, [name, runs]) => new Cricketer(name, runs, id)],
  ["bowler", (id, [name, runs, wickets]) => new Bowler(name, runs, wickets, id)],
];

function find(id) {
  // A key alone does not say which table holds the row, so the mapper tries each table.
  const searched = [];
  for (const [table, make] of MAKERS) {
    searched.push(table);
    if (tables[table].has(id)) return [make(id, tables[table].get(id)), searched];
  }
  return [null, searched];
}

function show(t) {
  return [...tables[t]].map(([id, cols]) => [id, ...cols].join(" ")).join(" | ");
}

for (const p of [new Footballer("Ann", "Lions"), new Cricketer("Bo", 412), new Bowler("Cy", 95, 31)]) {
  insert(p);
}
for (const t of ["footballer", "cricketer", "bowler"]) console.log(`${t}: ${show(t)}`);
const [p, searched] = find(3);
console.log(`find 3: ${p.describe()} (tables searched: ${searched.join(", ")})`);
// The union of all the tables, as a polymorphic query does it.
const everyone = MAKERS.flatMap(([t]) => [...tables[t]].map(([id, cols]) => [id, cols[0], t]));
everyone.sort((a, b) => a[0] - b[0]);
console.log(`all players: ${everyone.map((r) => r.join(" ")).join(" | ")}`);
const cricketers = [...tables.cricketer.values(), ...tables.bowler.values()].map((c) => c[0]).sort();
console.log(`cricketers: ${cricketers.join(", ")}`);
console.log(`next key: ${nextId}`);
```

```output
footballer: 1 Ann Lions
cricketer: 2 Bo 412
bowler: 3 Cy 95 31
find 3: Bowler Cy, runs 95, wickets 31 (tables searched: footballer, cricketer, bowler)
all players: 1 Ann footballer | 2 Bo cricketer | 3 Cy bowler
cricketers: Bo, Cy
next key: 4
```

- Verdict: idiom. Each table is a `Map` from the key to all the columns of its class, and one `nextId` counter gives the keys for all the tables.
- `find` walks the `MAKERS` list, because the key alone does not say which table holds the row. A query for cricketers reads the cricketer map and the bowler map.
