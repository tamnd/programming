```javascript run id=table-per-class
// A stand-in for the database: one table for each class, all keyed by the same id.
const tables = { player: new Map(), footballer: new Map(), cricketer: new Map(), bowler: new Map() };
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

function insert(p) {
  // One row in the table of each class on the path from the root, all with the same key.
  p.id = ++lastId;
  tables.player.set(p.id, [p.name]);
  if (p instanceof Footballer) tables.footballer.set(p.id, [p.club]);
  if (p instanceof Cricketer) tables.cricketer.set(p.id, [p.runs]);
  if (p instanceof Bowler) tables.bowler.set(p.id, [p.wickets]);
}

function find(id) {
  // The root row has no type, so the mapper looks for the key in the tables of the subclasses.
  const [name] = tables.player.get(id);
  if (tables.footballer.has(id)) {
    return [new Footballer(name, tables.footballer.get(id)[0], id), ["player", "footballer"]];
  }
  const [runs] = tables.cricketer.get(id);
  if (tables.bowler.has(id)) {
    return [new Bowler(name, runs, tables.bowler.get(id)[0], id), ["player", "cricketer", "bowler"]];
  }
  return [new Cricketer(name, runs, id), ["player", "cricketer"]];
}

function remove(p) {
  let count = 0;
  for (const t of ["bowler", "cricketer", "footballer", "player"]) {
    if (tables[t].delete(p.id)) count++; // Children first.
  }
  return count;
}

function show(t) {
  return [...tables[t]].map(([id, cols]) => [id, ...cols].join(" ")).join(" | ");
}

const ann = new Footballer("Ann", "Lions");
const bo = new Cricketer("Bo", 412);
const cy = new Bowler("Cy", 95, 31);
for (const p of [ann, bo, cy]) insert(p);
for (const t of ["player", "footballer", "cricketer", "bowler"]) console.log(`${t}: ${show(t)}`);
for (const id of [1, 2, 3]) {
  const [p, used] = find(id);
  console.log(`loaded: ${p.describe()} (tables: ${used.join(", ")})`);
}
const cricketers = [...tables.cricketer.keys()].map((id) => tables.player.get(id)[0]);
console.log(`cricketers: ${cricketers.join(", ")}`);
console.log(`delete Cy: rows deleted ${remove(cy)}`);
```

```output
player: 1 Ann | 2 Bo | 3 Cy
footballer: 1 Lions
cricketer: 2 412 | 3 95
bowler: 3 31
loaded: Footballer Ann, club Lions (tables: player, footballer)
loaded: Cricketer Bo, runs 412 (tables: player, cricketer)
loaded: Bowler Cy, runs 95, wickets 31 (tables: player, cricketer, bowler)
cricketers: Bo, Cy
delete Cy: rows deleted 3
```

- Verdict: idiom. Each table is a `Map` from the shared id to the columns that its class declares. `insert` uses `instanceof`, so a `Bowler` gets a row in the cricketer table too.
- `remove` deletes the rows from the bowler table first and from the player table last, in the order that the foreign keys need.
