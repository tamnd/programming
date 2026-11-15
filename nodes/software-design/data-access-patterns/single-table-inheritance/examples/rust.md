```rust run id=one-table-for-hierarchy
use std::collections::BTreeMap;

// One row of the single player table. None is a null column.
struct Row {
    kind: &'static str,
    name: String,
    club: Option<String>,
    runs: Option<i64>,
    wickets: Option<i64>,
}

// Rust has no class inheritance, so the hierarchy is an enum with one variant
// for each concrete class.
enum Player {
    Footballer { name: String, club: String },
    Cricketer { name: String, runs: i64 },
    Bowler { name: String, runs: i64, wickets: i64 },
}

impl Player {
    fn name(&self) -> &str {
        match self {
            Player::Footballer { name, .. } | Player::Cricketer { name, .. } | Player::Bowler { name, .. } => name,
        }
    }

    fn describe(&self) -> String {
        match self {
            Player::Footballer { name, club } => format!("Footballer {}, club {}", name, club),
            Player::Cricketer { name, runs } => format!("Cricketer {}, runs {}", name, runs),
            Player::Bowler { name, runs, wickets } => format!("Bowler {}, runs {}, wickets {}", name, runs, wickets),
        }
    }
}

#[derive(Default)]
struct Db {
    player: BTreeMap<i64, Row>,
    last_id: i64,
}

impl Db {
    // Each variant writes its own fields; the columns of the other classes stay None.
    fn insert(&mut self, p: &Player) -> i64 {
        let row = match p {
            Player::Footballer { name, club } => Row { kind: "footballer", name: name.clone(), club: Some(club.clone()), runs: None, wickets: None },
            Player::Cricketer { name, runs } => Row { kind: "cricketer", name: name.clone(), club: None, runs: Some(*runs), wickets: None },
            Player::Bowler { name, runs, wickets } => Row { kind: "bowler", name: name.clone(), club: None, runs: Some(*runs), wickets: Some(*wickets) },
        };
        self.last_id += 1;
        self.player.insert(self.last_id, row);
        self.last_id
    }

    fn load(r: &Row) -> Player {
        let name = r.name.clone();
        match r.kind {
            "footballer" => Player::Footballer { name, club: r.club.clone().unwrap() },
            "cricketer" => Player::Cricketer { name, runs: r.runs.unwrap() },
            _ => Player::Bowler { name, runs: r.runs.unwrap(), wickets: r.wickets.unwrap() },
        }
    }

    fn find_all(&self, kind: Option<&str>) -> Vec<Player> {
        let kinds: Option<Vec<&str>> = kind.map(|k| if k == "cricketer" { vec!["cricketer", "bowler"] } else { vec![k] });
        self.player
            .values()
            .filter(|r| kinds.as_ref().map_or(true, |ks| ks.contains(&r.kind)))
            .map(Db::load)
            .collect()
    }
}

fn show<T: ToString>(v: &Option<T>) -> String {
    v.as_ref().map_or("null".to_string(), |x| x.to_string())
}

fn main() {
    let mut db = Db::default();
    db.insert(&Player::Footballer { name: "Ann".into(), club: "Lions".into() });
    db.insert(&Player::Cricketer { name: "Bo".into(), runs: 412 });
    db.insert(&Player::Bowler { name: "Cy".into(), runs: 95, wickets: 31 });
    for (id, r) in &db.player {
        println!("row: {} {} {} {} {} {}", id, r.kind, r.name, show(&r.club), show(&r.runs), show(&r.wickets));
    }
    for p in db.find_all(None) {
        println!("loaded: {}", p.describe());
    }
    let names: Vec<String> = db.find_all(Some("cricketer")).iter().map(|p| p.name().to_string()).collect();
    println!("cricketers: {}", names.join(", "));
    let nulls: usize = db.player.values().map(|r| r.club.is_none() as usize + r.runs.is_none() as usize + r.wickets.is_none() as usize).sum();
    println!("null cells in subclass columns: {} of {}", nulls, 3 * db.player.len());
}
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

- Verdict: idiom. Rust has no class inheritance, so the hierarchy is an `enum` with one variant for each concrete class, and the type code comes from a `match` on the variant. A column that can be null is an `Option`.
- `find_all` turns the kind `cricketer` into the codes `cricketer` and `bowler`, as a polymorphic query does.
