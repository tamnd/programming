```javascript run id=query-object-to-sql
class Player {
  constructor(id, name, country, goals) {
    Object.assign(this, { id, name, country, goals });
  }
}

// The metadata: each field of Player and its column in the player table.
const PLAYER_MAP = {
  table: "player",
  columns: { id: "id", name: "name", country: "nation", goals: "goals_scored" },
};

// One condition on a field. The query object is a tree of these.
class Criterion {
  constructor(op, field, value) {
    Object.assign(this, { op, field, value });
  }
  sql(cols, params) {
    const col = cols[this.field];
    if (this.op === "starts") {
      params.push(this.value + "%");
      return `${col} like ?`;
    }
    params.push(this.value);
    return `${col} ${this.op} ?`;
  }
  matches(obj) {
    const v = obj[this.field];
    if (this.op === "=") return v === this.value;
    if (this.op === ">") return v > this.value;
    return v.startsWith(this.value);
  }
}

// A group of conditions joined with "or".
class AnyOf {
  constructor(...parts) {
    this.parts = parts;
  }
  sql(cols, params) {
    return "(" + this.parts.map((p) => p.sql(cols, params)).join(" or ") + ")";
  }
  matches(obj) {
    return this.parts.some((p) => p.matches(obj));
  }
}

const eq = (f, v) => new Criterion("=", f, v);
const gt = (f, v) => new Criterion(">", f, v);
const starts = (f, v) => new Criterion("starts", f, v);

// A query on fields of a class. It knows nothing of tables and columns
// until toSql reads the data map.
class Query {
  constructor() {
    this.criteria = [];
    this.order = null;
  }
  where(c) {
    this.criteria.push(c);
    return this;
  }
  orderBy(field) {
    this.order = field;
    return this;
  }
  toSql(map) {
    const cols = map.columns;
    const params = [];
    let sql = `select ${Object.values(cols).join(", ")} from ${map.table}`;
    if (this.criteria.length > 0) {
      sql += " where " + this.criteria.map((c) => c.sql(cols, params)).join(" and ");
    }
    if (this.order) sql += ` order by ${cols[this.order]}`;
    return [sql, params];
  }
  matches(obj) {
    return this.criteria.every((c) => c.matches(obj));
  }
}

const PLAYERS = [
  new Player(1, "Ann", "NZ", 14), new Player(2, "Bo", "NZ", 3), new Player(3, "Cy", "NZ", 21),
  new Player(4, "Dee", "AU", 9), new Player(5, "Eli", "GB", 12),
];

function run(title, query, map) {
  const [sql, params] = query.toSql(map);
  console.log("query:", title);
  console.log("sql:", sql);
  const shown = params.map((p) => (typeof p === "string" ? `'${p}'` : String(p)));
  console.log(`params: [${shown.join(", ")}]`);
  // No database here: the same query object is run on the objects in memory.
  const found = PLAYERS.filter((p) => query.matches(p))
    .sort((a, b) => (a[query.order] < b[query.order] ? -1 : 1));
  console.log("rows:", found.map((p) => `${p.name} (${p.country}, ${p.goals})`).join(" | "));
  console.log("in memory:", found.map((p) => p.name).join(" | "));
}

const q1 = new Query().where(eq("country", "NZ")).where(gt("goals", 10)).orderBy("name");
const q2 = new Query().where(new AnyOf(starts("name", "B"), eq("country", "AU"))).orderBy("name");
run("from NZ and more than 10 goals", q1, PLAYER_MAP);
run("name starts with B or from AU", q2, PLAYER_MAP);

// The column nation is renamed. Only the data map changes; the query does not.
const NEW_MAP = { table: "player", columns: { ...PLAYER_MAP.columns, country: "country_code" } };
run("from NZ and more than 10 goals, after the rename", q1, NEW_MAP);
```

```output
query: from NZ and more than 10 goals
sql: select id, name, nation, goals_scored from player where nation = ? and goals_scored > ? order by name
params: ['NZ', 10]
rows: Ann (NZ, 14) | Cy (NZ, 21)
in memory: Ann | Cy
query: name starts with B or from AU
sql: select id, name, nation, goals_scored from player where (name like ? or nation = ?) order by name
params: ['B%', 'AU']
rows: Bo (NZ, 3) | Dee (AU, 9)
in memory: Bo | Dee
query: from NZ and more than 10 goals, after the rename
sql: select id, name, country_code, goals_scored from player where country_code = ? and goals_scored > ? order by name
params: ['NZ', 10]
rows: Ann (NZ, 14) | Cy (NZ, 21)
in memory: Ann | Cy
```

- Verdict: idiom. Each criterion is a class with `sql` and `matches`, and `where` returns `this` so that the calls chain.
- The example has no database, so the rows come from the same query object run on the objects in memory. The SQL and the parameters are the same as in the Python file.
- Query builders for JavaScript, such as Knex, have this design.
