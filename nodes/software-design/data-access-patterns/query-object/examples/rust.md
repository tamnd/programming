```rust run id=query-object-to-sql
struct Player {
    name: &'static str,
    country: &'static str,
    goals: i64,
}

#[derive(Clone, PartialEq)]
enum Value {
    Int(i64),
    Text(String),
}

impl Player {
    fn get(&self, field: &str) -> Value {
        match field {
            "name" => Value::Text(self.name.to_string()),
            "country" => Value::Text(self.country.to_string()),
            _ => Value::Int(self.goals),
        }
    }
}

// The metadata: the table and, for each field, its column, in order.
struct DataMap {
    table: &'static str,
    columns: Vec<(&'static str, &'static str)>,
}

impl DataMap {
    fn column(&self, field: &str) -> &'static str {
        self.columns.iter().find(|(f, _)| *f == field).unwrap().1
    }
}

// A node of the query object: it can write itself as SQL or test an object.
enum Condition {
    Eq(&'static str, Value),
    Gt(&'static str, i64),
    Starts(&'static str, &'static str),
    AnyOf(Vec<Condition>),
}

impl Condition {
    fn sql(&self, map: &DataMap, params: &mut Vec<Value>) -> String {
        match self {
            Condition::Eq(f, v) => {
                params.push(v.clone());
                format!("{} = ?", map.column(f))
            }
            Condition::Gt(f, v) => {
                params.push(Value::Int(*v));
                format!("{} > ?", map.column(f))
            }
            Condition::Starts(f, v) => {
                params.push(Value::Text(format!("{}%", v)));
                format!("{} like ?", map.column(f))
            }
            Condition::AnyOf(parts) => {
                let s: Vec<String> = parts.iter().map(|c| c.sql(map, params)).collect();
                format!("({})", s.join(" or "))
            }
        }
    }

    fn matches(&self, p: &Player) -> bool {
        match self {
            Condition::Eq(f, v) => p.get(f) == *v,
            Condition::Gt(f, v) => matches!(p.get(f), Value::Int(x) if x > *v),
            Condition::Starts(f, v) => matches!(p.get(f), Value::Text(s) if s.starts_with(v)),
            Condition::AnyOf(parts) => parts.iter().any(|c| c.matches(p)),
        }
    }
}

// A query on fields. It knows nothing of tables and columns
// until to_sql reads the data map.
struct Query {
    criteria: Vec<Condition>,
    order: &'static str,
}

impl Query {
    fn to_sql(&self, map: &DataMap) -> (String, Vec<Value>) {
        let cols: Vec<&str> = map.columns.iter().map(|(_, c)| *c).collect();
        let mut params = Vec::new();
        let mut sql = format!("select {} from {}", cols.join(", "), map.table);
        if !self.criteria.is_empty() {
            let s: Vec<String> = self.criteria.iter().map(|c| c.sql(map, &mut params)).collect();
            sql += &format!(" where {}", s.join(" and "));
        }
        sql += &format!(" order by {}", map.column(self.order));
        (sql, params)
    }

    fn matches(&self, p: &Player) -> bool {
        self.criteria.iter().all(|c| c.matches(p))
    }
}

fn run(title: &str, q: &Query, map: &DataMap, players: &[Player]) {
    let (sql, params) = q.to_sql(map);
    println!("query: {}", title);
    println!("sql: {}", sql);
    let shown: Vec<String> = params
        .iter()
        .map(|p| match p {
            Value::Text(s) => format!("'{}'", s),
            Value::Int(i) => i.to_string(),
        })
        .collect();
    println!("params: [{}]", shown.join(", "));
    // No database here: the same query object is run on the objects in memory.
    let mut found: Vec<&Player> = players.iter().filter(|p| q.matches(p)).collect();
    found.sort_by_key(|p| p.name);
    let rows: Vec<String> = found.iter().map(|p| format!("{} ({}, {})", p.name, p.country, p.goals)).collect();
    let names: Vec<&str> = found.iter().map(|p| p.name).collect();
    println!("rows: {}", rows.join(" | "));
    println!("in memory: {}", names.join(" | "));
}

fn main() {
    let players = [
        Player { name: "Ann", country: "NZ", goals: 14 },
        Player { name: "Bo", country: "NZ", goals: 3 },
        Player { name: "Cy", country: "NZ", goals: 21 },
        Player { name: "Dee", country: "AU", goals: 9 },
        Player { name: "Eli", country: "GB", goals: 12 },
    ];
    let mut columns = vec![("id", "id"), ("name", "name"), ("country", "nation"), ("goals", "goals_scored")];
    let player_map = DataMap { table: "player", columns: columns.clone() };

    let q1 = Query {
        criteria: vec![Condition::Eq("country", Value::Text("NZ".to_string())), Condition::Gt("goals", 10)],
        order: "name",
    };
    let q2 = Query {
        criteria: vec![Condition::AnyOf(vec![Condition::Starts("name", "B"), Condition::Eq("country", Value::Text("AU".to_string()))])],
        order: "name",
    };
    run("from NZ and more than 10 goals", &q1, &player_map, &players);
    run("name starts with B or from AU", &q2, &player_map, &players);

    // The column nation is renamed. Only the data map changes; the query does not.
    columns[2] = ("country", "country_code");
    let new_map = DataMap { table: "player", columns };
    run("from NZ and more than 10 goals, after the rename", &q1, &new_map, &players);
}
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

- Verdict: idiom. `Condition` is an enum, and `sql` and `matches` are two `match` expressions over it, so the compiler checks that each kind of node is handled.
- The data map is a vector of pairs, which keeps the column order fixed.
- The example has no database, so the rows come from the same query object run on the objects in memory. Diesel and SeaORM are Rust query builders with this design.
