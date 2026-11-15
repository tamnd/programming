```rust run id=table-per-concrete-class
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

// One table for each concrete class, with all its fields, the inherited ones too.
struct Db {
    footballer: BTreeMap<i64, (String, String)>,
    cricketer: BTreeMap<i64, (String, i64)>,
    bowler: BTreeMap<i64, (String, i64, i64)>,
    next_id: i64, // One key source for all the tables.
}

impl Db {
    fn insert(&mut self, p: &Player) {
        let id = self.next_id;
        self.next_id += 1;
        match p {
            Player::Footballer { name, club } => {
                self.footballer.insert(id, (name.clone(), club.clone()));
            }
            Player::Cricketer { name, runs } => {
                self.cricketer.insert(id, (name.clone(), *runs));
            }
            Player::Bowler { name, runs, wickets } => {
                self.bowler.insert(id, (name.clone(), *runs, *wickets));
            }
        }
    }

    // A key alone does not say which table holds the row, so the mapper tries each table.
    fn find(&self, id: i64) -> (Option<Player>, Vec<&'static str>) {
        let mut searched = vec!["footballer"];
        if let Some((name, club)) = self.footballer.get(&id) {
            return (Some(Player::Footballer { name: name.clone(), club: club.clone() }), searched);
        }
        searched.push("cricketer");
        if let Some((name, runs)) = self.cricketer.get(&id) {
            return (Some(Player::Cricketer { name: name.clone(), runs: *runs }), searched);
        }
        searched.push("bowler");
        if let Some((name, runs, wickets)) = self.bowler.get(&id) {
            return (Some(Player::Bowler { name: name.clone(), runs: *runs, wickets: *wickets }), searched);
        }
        (None, searched)
    }
}

fn main() {
    let mut db = Db { footballer: BTreeMap::new(), cricketer: BTreeMap::new(), bowler: BTreeMap::new(), next_id: 1 };
    db.insert(&Player::Footballer { name: "Ann".into(), club: "Lions".into() });
    db.insert(&Player::Cricketer { name: "Bo".into(), runs: 412 });
    db.insert(&Player::Bowler { name: "Cy".into(), runs: 95, wickets: 31 });
    let f: Vec<String> = db.footballer.iter().map(|(id, (n, c))| format!("{} {} {}", id, n, c)).collect();
    println!("footballer: {}", f.join(" | "));
    let c: Vec<String> = db.cricketer.iter().map(|(id, (n, r))| format!("{} {} {}", id, n, r)).collect();
    println!("cricketer: {}", c.join(" | "));
    let b: Vec<String> = db.bowler.iter().map(|(id, (n, r, w))| format!("{} {} {} {}", id, n, r, w)).collect();
    println!("bowler: {}", b.join(" | "));
    let (p, searched) = db.find(3);
    println!("find 3: {} (tables searched: {})", p.expect("player 3").describe(), searched.join(", "));
    // The union of all the tables, as a polymorphic query does it.
    let mut everyone: Vec<(i64, &str, &str)> = Vec::new();
    everyone.extend(db.footballer.iter().map(|(id, (n, _))| (*id, n.as_str(), "footballer")));
    everyone.extend(db.cricketer.iter().map(|(id, (n, _))| (*id, n.as_str(), "cricketer")));
    everyone.extend(db.bowler.iter().map(|(id, (n, _, _))| (*id, n.as_str(), "bowler")));
    everyone.sort();
    let all: Vec<String> = everyone.iter().map(|(id, n, t)| format!("{} {} {}", id, n, t)).collect();
    println!("all players: {}", all.join(" | "));
    let mut cricketers: Vec<&str> = db.cricketer.values().map(|(n, _)| n.as_str()).collect();
    cricketers.extend(db.bowler.values().map(|(n, _, _)| n.as_str()));
    cricketers.sort();
    println!("cricketers: {}", cricketers.join(", "));
    println!("next key: {}", db.next_id);
}
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

- Verdict: idiom. The hierarchy is an `enum` with one variant for each concrete class, and each table is a `BTreeMap` of tuples with all the fields of the variant.
- `find` returns an `Option<Player>` and the list of tables that it searched, and `if let` tests each table in turn.
