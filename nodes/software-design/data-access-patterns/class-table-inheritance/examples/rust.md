```rust run id=table-per-class
use std::collections::BTreeMap;

// Rust has no class inheritance, so the hierarchy is an enum with one variant
// for each concrete class.
enum Player {
    Footballer { name: String, club: String },
    Cricketer { name: String, runs: i64 },
    Bowler { name: String, runs: i64, wickets: i64 },
}

impl Player {
    fn describe(&self) -> String {
        match self {
            Player::Footballer { name, club } => format!("Footballer {}, club {}", name, club),
            Player::Cricketer { name, runs } => format!("Cricketer {}, runs {}", name, runs),
            Player::Bowler { name, runs, wickets } => format!("Bowler {}, runs {}, wickets {}", name, runs, wickets),
        }
    }
}

// One table for each class, all keyed by the same id.
#[derive(Default)]
struct Db {
    player: BTreeMap<i64, String>,
    footballer: BTreeMap<i64, String>,
    cricketer: BTreeMap<i64, i64>,
    bowler: BTreeMap<i64, i64>,
    last_id: i64,
}

impl Db {
    // One row in the table of each class on the path from the root, all with the same key.
    fn insert(&mut self, p: &Player) -> i64 {
        self.last_id += 1;
        let id = self.last_id;
        match p {
            Player::Footballer { name, club } => {
                self.player.insert(id, name.clone());
                self.footballer.insert(id, club.clone());
            }
            Player::Cricketer { name, runs } => {
                self.player.insert(id, name.clone());
                self.cricketer.insert(id, *runs);
            }
            Player::Bowler { name, runs, wickets } => {
                self.player.insert(id, name.clone());
                self.cricketer.insert(id, *runs);
                self.bowler.insert(id, *wickets);
            }
        }
        id
    }

    // The root row has no type, so the mapper looks for the key in the tables of the subclasses.
    fn find(&self, id: i64) -> (Player, Vec<&'static str>) {
        let name = self.player[&id].clone();
        if let Some(club) = self.footballer.get(&id) {
            return (Player::Footballer { name, club: club.clone() }, vec!["player", "footballer"]);
        }
        let runs = self.cricketer[&id];
        if let Some(wickets) = self.bowler.get(&id) {
            return (Player::Bowler { name, runs, wickets: *wickets }, vec!["player", "cricketer", "bowler"]);
        }
        (Player::Cricketer { name, runs }, vec!["player", "cricketer"])
    }

    fn delete(&mut self, id: i64) -> usize {
        // Children first.
        self.bowler.remove(&id).is_some() as usize
            + self.cricketer.remove(&id).is_some() as usize
            + self.footballer.remove(&id).is_some() as usize
            + self.player.remove(&id).is_some() as usize
    }
}

fn show<V: std::fmt::Display>(table: &BTreeMap<i64, V>) -> String {
    table.iter().map(|(k, v)| format!("{} {}", k, v)).collect::<Vec<_>>().join(" | ")
}

fn main() {
    let mut db = Db::default();
    db.insert(&Player::Footballer { name: "Ann".into(), club: "Lions".into() });
    db.insert(&Player::Cricketer { name: "Bo".into(), runs: 412 });
    let cy = db.insert(&Player::Bowler { name: "Cy".into(), runs: 95, wickets: 31 });
    println!("player: {}", show(&db.player));
    println!("footballer: {}", show(&db.footballer));
    println!("cricketer: {}", show(&db.cricketer));
    println!("bowler: {}", show(&db.bowler));
    for id in 1..=3 {
        let (p, used) = db.find(id);
        println!("loaded: {} (tables: {})", p.describe(), used.join(", "));
    }
    let names: Vec<&str> = db.cricketer.keys().map(|id| db.player[id].as_str()).collect();
    println!("cricketers: {}", names.join(", "));
    println!("delete Cy: rows deleted {}", db.delete(cy));
}
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

- Verdict: idiom. The hierarchy is an `enum`, and each table is a `BTreeMap` keyed by the shared id. `find` uses `get` and `if let` to test for a row in a subclass table.
- `delete` adds up `remove(&id).is_some()` for each table, children first, to count the deleted rows.
