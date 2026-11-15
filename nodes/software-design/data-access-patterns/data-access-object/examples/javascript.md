```javascript run id=two-daos-one-interface
// The transfer object: plain data that goes in and out of the DAO.
class Player {
  constructor(id, name, country) {
    Object.assign(this, { id, name, country });
  }
}

// JavaScript has no interfaces. The interface of the DAO is the set of
// methods below, and both classes have all of them:
// insert(p), find(id), findByCountry(country), findAll(), update(p), delete(id)

// The data source is a map from key to record.
class MemoryPlayerDao {
  constructor() {
    this.rows = new Map();
  }
  insert(p) {
    this.rows.set(p.id, [p.name, p.country]);
  }
  find(id) {
    const [name, country] = this.rows.get(id);
    return new Player(id, name, country);
  }
  findByCountry(country) {
    return this.findAll().filter((p) => p.country === country);
  }
  findAll() {
    return [...this.rows.keys()].sort((a, b) => a - b).map((id) => this.find(id));
  }
  update(p) {
    this.rows.set(p.id, [p.name, p.country]);
  }
  delete(id) {
    this.rows.delete(id);
  }
}

// The data source is a flat file: one line "id;name;country" for each player.
class TextPlayerDao {
  constructor() {
    this.lines = [];
  }
  parse(line) {
    const [id, name, country] = line.split(";");
    return new Player(Number(id), name, country);
  }
  index(id) {
    return this.lines.findIndex((line) => this.parse(line).id === id);
  }
  insert(p) {
    this.lines.push(`${p.id};${p.name};${p.country}`);
  }
  find(id) {
    return this.parse(this.lines[this.index(id)]);
  }
  findByCountry(country) {
    return this.findAll().filter((p) => p.country === country);
  }
  findAll() {
    return this.lines.map((line) => this.parse(line)).sort((a, b) => a.id - b.id);
  }
  update(p) {
    this.lines[this.index(p.id)] = `${p.id};${p.name};${p.country}`;
  }
  delete(id) {
    this.lines.splice(this.index(id), 1);
  }
}

// The one place that knows which storage the program uses.
function daoFactory(kind) {
  return kind === "memory" ? new MemoryPlayerDao() : new TextPlayerDao();
}

// The business object. It uses only the DAO interface.
class PlayerService {
  constructor(dao) {
    this.dao = dao;
  }
  transfer(id, country) {
    const p = this.dao.find(id);
    p.country = country;
    this.dao.update(p);
  }
}

const names = (players) => players.map((p) => `${p.name} ${p.country}`).join(" | ");

const daos = {};
for (const kind of ["memory", "text"]) {
  const dao = (daos[kind] = daoFactory(kind));
  const service = new PlayerService(dao);
  console.log("dao:", dao.constructor.name);
  for (const p of [new Player(1, "Ann", "NZ"), new Player(2, "Bo", "NZ"), new Player(3, "Cy", "GB")]) {
    dao.insert(p);
  }
  console.log("all:", names(dao.findAll()));
  console.log("find 1:", names([dao.find(1)]));
  service.transfer(1, "AU");
  console.log("from NZ:", names(dao.findByCountry("NZ")));
  console.log("from AU:", names(dao.findByCountry("AU")));
  dao.delete(3);
  console.log("all:", names(dao.findAll()));
}

// The business code never saw this format; only TextPlayerDao knows it.
console.log("lines in the text store:", daos.text.lines.join(" | "));
```

```output
dao: MemoryPlayerDao
all: Ann NZ | Bo NZ | Cy GB
find 1: Ann NZ
from NZ: Bo NZ
from AU: Ann AU
all: Ann AU | Bo NZ
dao: TextPlayerDao
all: Ann NZ | Bo NZ | Cy GB
find 1: Ann NZ
from NZ: Bo NZ
from AU: Ann AU
all: Ann AU | Bo NZ
lines in the text store: 1;Ann;AU | 2;Bo;NZ
```

- Verdict: idiom. JavaScript has no interfaces, so the DAO interface is a set of method names that both classes have. A TypeScript version would declare it as an `interface`.
- `daoFactory` is the only code that names a class of DAO, and `PlayerService` gets the DAO in its constructor.
