```javascript run id=mapper-for-each-class
// A stand-in for the database: one player table with a type column.
const playerTable = new Map();
let nextId = 1;

class Player {
  constructor(name) {
    this.id = null;
    this.name = name;
  }
}

class Footballer extends Player {
  constructor(name, club) {
    super(name);
    this.club = club;
  }
  describe() {
    return `Footballer ${this.name}, club ${this.club}`;
  }
}

class Cricketer extends Player {
  constructor(name, runs) {
    super(name);
    this.runs = runs;
  }
  describe() {
    return `Cricketer ${this.name}, runs ${this.runs}`;
  }
}

class Bowler extends Cricketer {
  constructor(name, runs, wickets) {
    super(name, runs);
    this.wickets = wickets;
  }
  describe() {
    return `Bowler ${this.name}, runs ${this.runs}, wickets ${this.wickets}`;
  }
}

// Each mapper class saves and loads the fields that its domain class declares,
// and calls the mapper of the superclass for the inherited fields.
class AbstractPlayerMapper {
  save(p, row, log) {
    row.name = p.name;
    log.push("name (Player)");
  }
  load(p, row) {
    p.name = row.name;
  }
  insert(p) {
    const row = { type: this.constructor.typeCode };
    const log = [];
    this.save(p, row, log);
    p.id = nextId++;
    playerTable.set(p.id, row);
    return log;
  }
  find(id, row) {
    const p = this.create();
    p.id = id;
    this.load(p, row);
    return p;
  }
}

class FootballerMapper extends AbstractPlayerMapper {
  static typeCode = "footballer";
  create() {
    return new Footballer(null, null);
  }
  save(p, row, log) {
    super.save(p, row, log);
    row.club = p.club;
    log.push("club (Footballer)");
  }
  load(p, row) {
    super.load(p, row);
    p.club = row.club;
  }
}

class CricketerMapper extends AbstractPlayerMapper {
  static typeCode = "cricketer";
  create() {
    return new Cricketer(null, null);
  }
  save(p, row, log) {
    super.save(p, row, log);
    row.runs = p.runs;
    log.push("runs (Cricketer)");
  }
  load(p, row) {
    super.load(p, row);
    p.runs = row.runs;
  }
}

class BowlerMapper extends CricketerMapper {
  static typeCode = "bowler";
  create() {
    return new Bowler(null, null, null);
  }
  save(p, row, log) {
    super.save(p, row, log);
    row.wickets = p.wickets;
    log.push("wickets (Bowler)");
  }
  load(p, row) {
    super.load(p, row);
    p.wickets = row.wickets;
  }
}

// The mapper for the whole hierarchy: it chooses the concrete mapper and delegates to it.
class PlayerMapper {
  constructor() {
    this.byType = new Map([new FootballerMapper(), new CricketerMapper(), new BowlerMapper()].map((m) => [m.constructor.typeCode, m]));
    this.byClass = new Map([
      [Footballer, this.byType.get("footballer")],
      [Cricketer, this.byType.get("cricketer")],
      [Bowler, this.byType.get("bowler")],
    ]);
  }
  insert(p) {
    const mapper = this.byClass.get(p.constructor);
    return [mapper, mapper.insert(p)];
  }
  find(id) {
    const row = playerTable.get(id);
    const mapper = this.byType.get(row.type);
    return [mapper, mapper.find(id, row)];
  }
  findAll() {
    return [...playerTable].map(([id, row]) => this.byType.get(row.type).find(id, row));
  }
}

function chain(mapper) {
  const names = [];
  for (let c = mapper.constructor; c !== Function.prototype && c.name; c = Object.getPrototypeOf(c)) names.push(c.name);
  return names.join(", ");
}

const players = new PlayerMapper();
for (const [t, m] of players.byType) console.log(`mapper chain for ${t}: ${chain(m)}`);
for (const p of [new Footballer("Ann", "Lions"), new Cricketer("Bo", 412), new Bowler("Cy", 95, 31)]) {
  const [mapper, log] = players.insert(p);
  console.log(`insert ${p.name}: ${mapper.constructor.name} saved ${log.join(", ")}`);
}
const [mapper, p] = players.find(3);
console.log(`find 3: ${p.describe()} (mapper ${mapper.constructor.name})`);
console.log(`find all: ${players.findAll().map((q) => q.describe()).join(" | ")}`);
```

```output
mapper chain for footballer: FootballerMapper, AbstractPlayerMapper
mapper chain for cricketer: CricketerMapper, AbstractPlayerMapper
mapper chain for bowler: BowlerMapper, CricketerMapper, AbstractPlayerMapper
insert Ann: FootballerMapper saved name (Player), club (Footballer)
insert Bo: CricketerMapper saved name (Player), runs (Cricketer)
insert Cy: BowlerMapper saved name (Player), runs (Cricketer), wickets (Bowler)
find 3: Bowler Cy, runs 95, wickets 31 (mapper BowlerMapper)
find all: Footballer Ann, club Lions | Cricketer Bo, runs 412 | Bowler Cy, runs 95, wickets 31
```

- Verdict: idiom. The mapper classes use `extends` and `super.save`, as the domain classes do. Each concrete mapper has a static `typeCode`, and `PlayerMapper` finds the mapper for an object from `p.constructor`.
- `chain` walks the prototype chain of the mapper class with `Object.getPrototypeOf`.
