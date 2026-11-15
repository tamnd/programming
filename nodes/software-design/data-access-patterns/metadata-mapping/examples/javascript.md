```javascript run id=generic-mapper-from-metadata
// A stand-in for the database: each table maps an id to a row of columns.
const db = { player: new Map(), club: new Map() };

class Player {
  constructor(id = null, name = null, country = null) {
    Object.assign(this, { id, name, country });
  }
  toString() {
    return `Player(id=${this.id}, name=${this.name}, country=${this.country})`;
  }
}

class Club {
  constructor(id = null, name = null, city = null) {
    Object.assign(this, { id, name, city });
  }
  toString() {
    return `Club(id=${this.id}, name=${this.name}, city=${this.city})`;
  }
}

// The metadata: the class, the table and the pairs of field and column.
class DataMap {
  constructor(cls, table, columns) {
    Object.assign(this, { cls, table, columns });
  }
  describe() {
    const pairs = this.columns.map(([f, c]) => `${f} -> ${c}`).join(", ");
    return `${this.cls.name} -> table ${this.table}: ${pairs}`;
  }
}

const MAPS = new Map([
  [Player, new DataMap(Player, "player", [["id", "id"], ["name", "name"], ["country", "nation"]])],
  [Club, new DataMap(Club, "club", [["id", "id"], ["name", "title"], ["city", "city"]])],
]);

// Generic code: it reads the metadata and reads and writes the fields by name.
class Mapper {
  insert(obj) {
    const m = MAPS.get(obj.constructor);
    const cols = m.columns.map(([, c]) => c).join(", ");
    const marks = m.columns.map(() => "?").join(", ");
    const row = new Map(m.columns.map(([f, c]) => [c, obj[f]]));
    db[m.table].set(obj.id, row);
    return `insert into ${m.table} (${cols}) values (${marks})`;
  }
  find(cls, id) {
    const m = MAPS.get(cls);
    const row = db[m.table].get(id);
    const obj = new cls();
    for (const [f, c] of m.columns) obj[f] = row.get(c);
    return obj;
  }
  update(obj) {
    const m = MAPS.get(obj.constructor);
    const rest = m.columns.filter(([f]) => f !== "id");
    const row = db[m.table].get(obj.id);
    for (const [f, c] of rest) row.set(c, obj[f]);
    return `update ${m.table} set ${rest.map(([, c]) => `${c} = ?`).join(", ")} where id = ?`;
  }
}

const mapper = new Mapper();
for (const m of MAPS.values()) console.log(`map ${m.describe()}`);
console.log(`insert: ${mapper.insert(new Player(1, "Ann", "NZ"))}`);
console.log(`insert: ${mapper.insert(new Club(1, "Lions", "Leeds"))}`);
const p = mapper.find(Player, 1);
console.log(`find player 1: ${p}`);
console.log(`find club 1: ${mapper.find(Club, 1)}`);
p.country = "AU";
console.log(`update: ${mapper.update(p)}`);
for (const table of ["player", "club"]) {
  for (const row of db[table].values()) console.log(`row in ${table}: ${[...row.values()].join(" ")}`);
}
```

```output
map Player -> table player: id -> id, name -> name, country -> nation
map Club -> table club: id -> id, name -> title, city -> city
insert: insert into player (id, name, nation) values (?, ?, ?)
insert: insert into club (id, title, city) values (?, ?, ?)
find player 1: Player(id=1, name=Ann, country=NZ)
find club 1: Club(id=1, name=Lions, city=Leeds)
update: update player set name = ?, nation = ? where id = ?
row in player: 1 Ann AU
row in club: 1 Lions Leeds
```

- Verdict: idiom. Property access with `obj[f]` is the reflection of JavaScript, and `new cls()` creates an empty object of the mapped class. `MAPS` uses the class itself as the key.
- The rows are kept in maps from column name to value, so the generic code also writes the columns by name.
