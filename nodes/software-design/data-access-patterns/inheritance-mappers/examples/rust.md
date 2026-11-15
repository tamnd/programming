```rust run id=mapper-for-each-class
use std::collections::BTreeMap;

#[derive(Clone)]
enum Value {
    Text(String),
    Int(i64),
}

impl Value {
    fn text(&self) -> String {
        match self {
            Value::Text(s) => s.clone(),
            Value::Int(_) => panic!("not text"),
        }
    }
    fn int(&self) -> i64 {
        match self {
            Value::Int(n) => *n,
            Value::Text(_) => panic!("not an integer"),
        }
    }
}

type Row = BTreeMap<&'static str, Value>;

// Rust has no class inheritance, so each type holds the struct of its superclass.
#[derive(Default)]
struct PlayerBase {
    id: i64,
    name: String,
}

#[derive(Default)]
struct Footballer {
    base: PlayerBase,
    club: String,
}

#[derive(Default)]
struct Cricketer {
    base: PlayerBase,
    runs: i64,
}

#[derive(Default)]
struct Bowler {
    cricketer: Cricketer,
    wickets: i64,
}

enum Player {
    Footballer(Footballer),
    Cricketer(Cricketer),
    Bowler(Bowler),
}

impl Player {
    fn base(&mut self) -> &mut PlayerBase {
        match self {
            Player::Footballer(f) => &mut f.base,
            Player::Cricketer(c) => &mut c.base,
            Player::Bowler(b) => &mut b.cricketer.base,
        }
    }
    fn cricketer(&mut self) -> &mut Cricketer {
        match self {
            Player::Cricketer(c) => c,
            Player::Bowler(b) => &mut b.cricketer,
            Player::Footballer(_) => panic!("not a cricketer"),
        }
    }
    fn describe(&self) -> String {
        match self {
            Player::Footballer(f) => format!("Footballer {}, club {}", f.base.name, f.club),
            Player::Cricketer(c) => format!("Cricketer {}, runs {}", c.base.name, c.runs),
            Player::Bowler(b) => format!("Bowler {}, runs {}, wickets {}", b.cricketer.base.name, b.cricketer.runs, b.wickets),
        }
    }
}

// Each mapper saves and loads the fields that its type declares, and calls the
// mapper of the superclass, which it holds, for the inherited fields.
trait Mapper {
    fn type_code(&self) -> &'static str;
    fn name(&self) -> &'static str;
    fn chain(&self) -> Vec<&'static str>;
    fn create(&self) -> Player;
    fn save(&self, p: &mut Player, r: &mut Row, log: &mut Vec<&'static str>);
    fn load(&self, p: &mut Player, r: &Row);
}

struct AbstractPlayerMapper;

impl AbstractPlayerMapper {
    fn chain(&self) -> Vec<&'static str> {
        vec!["AbstractPlayerMapper"]
    }
    fn save(&self, p: &mut Player, r: &mut Row, log: &mut Vec<&'static str>) {
        r.insert("name", Value::Text(p.base().name.clone()));
        log.push("name (Player)");
    }
    fn load(&self, p: &mut Player, r: &Row) {
        p.base().name = r["name"].text();
    }
}

struct FootballerMapper {
    parent: AbstractPlayerMapper,
}

impl Mapper for FootballerMapper {
    fn type_code(&self) -> &'static str {
        "footballer"
    }
    fn name(&self) -> &'static str {
        "FootballerMapper"
    }
    fn chain(&self) -> Vec<&'static str> {
        [vec![self.name()], self.parent.chain()].concat()
    }
    fn create(&self) -> Player {
        Player::Footballer(Footballer::default())
    }
    fn save(&self, p: &mut Player, r: &mut Row, log: &mut Vec<&'static str>) {
        self.parent.save(p, r, log);
        if let Player::Footballer(f) = p {
            r.insert("club", Value::Text(f.club.clone()));
        }
        log.push("club (Footballer)");
    }
    fn load(&self, p: &mut Player, r: &Row) {
        self.parent.load(p, r);
        if let Player::Footballer(f) = p {
            f.club = r["club"].text();
        }
    }
}

struct CricketerMapper {
    parent: AbstractPlayerMapper,
}

impl Mapper for CricketerMapper {
    fn type_code(&self) -> &'static str {
        "cricketer"
    }
    fn name(&self) -> &'static str {
        "CricketerMapper"
    }
    fn chain(&self) -> Vec<&'static str> {
        [vec![self.name()], self.parent.chain()].concat()
    }
    fn create(&self) -> Player {
        Player::Cricketer(Cricketer::default())
    }
    fn save(&self, p: &mut Player, r: &mut Row, log: &mut Vec<&'static str>) {
        self.parent.save(p, r, log);
        r.insert("runs", Value::Int(p.cricketer().runs));
        log.push("runs (Cricketer)");
    }
    fn load(&self, p: &mut Player, r: &Row) {
        self.parent.load(p, r);
        p.cricketer().runs = r["runs"].int();
    }
}

struct BowlerMapper {
    parent: CricketerMapper,
}

impl Mapper for BowlerMapper {
    fn type_code(&self) -> &'static str {
        "bowler"
    }
    fn name(&self) -> &'static str {
        "BowlerMapper"
    }
    fn chain(&self) -> Vec<&'static str> {
        [vec![self.name()], self.parent.chain()].concat()
    }
    fn create(&self) -> Player {
        Player::Bowler(Bowler::default())
    }
    fn save(&self, p: &mut Player, r: &mut Row, log: &mut Vec<&'static str>) {
        self.parent.save(p, r, log);
        if let Player::Bowler(b) = p {
            r.insert("wickets", Value::Int(b.wickets));
        }
        log.push("wickets (Bowler)");
    }
    fn load(&self, p: &mut Player, r: &Row) {
        self.parent.load(p, r);
        if let Player::Bowler(b) = p {
            b.wickets = r["wickets"].int();
        }
    }
}

// The mapper for the whole hierarchy: it chooses the concrete mapper and delegates to it.
struct PlayerMapper {
    mappers: Vec<Box<dyn Mapper>>,
    table: BTreeMap<i64, Row>,
    next_id: i64,
}

impl PlayerMapper {
    fn by_type(&self, t: &str) -> &dyn Mapper {
        self.mappers.iter().find(|m| m.type_code() == t).expect("mapper").as_ref()
    }

    fn insert(&mut self, p: &mut Player) -> (&'static str, Vec<&'static str>) {
        let t = match p {
            Player::Footballer(_) => "footballer",
            Player::Cricketer(_) => "cricketer",
            Player::Bowler(_) => "bowler",
        };
        let mut r = Row::new();
        r.insert("type", Value::Text(t.to_string()));
        let mut log = Vec::new();
        let m = self.by_type(t);
        m.save(p, &mut r, &mut log);
        let name = m.name();
        p.base().id = self.next_id;
        self.table.insert(self.next_id, r);
        self.next_id += 1;
        (name, log)
    }

    fn load(&self, id: i64, r: &Row) -> (&'static str, Player) {
        let m = self.by_type(&r["type"].text());
        let mut p = m.create();
        p.base().id = id;
        m.load(&mut p, r);
        (m.name(), p)
    }

    fn find(&self, id: i64) -> (&'static str, Player) {
        self.load(id, &self.table[&id])
    }

    fn find_all(&self) -> Vec<Player> {
        self.table.iter().map(|(id, r)| self.load(*id, r).1).collect()
    }
}

fn main() {
    let mut players = PlayerMapper {
        mappers: vec![
            Box::new(FootballerMapper { parent: AbstractPlayerMapper }),
            Box::new(CricketerMapper { parent: AbstractPlayerMapper }),
            Box::new(BowlerMapper { parent: CricketerMapper { parent: AbstractPlayerMapper } }),
        ],
        table: BTreeMap::new(),
        next_id: 1,
    };
    for m in &players.mappers {
        println!("mapper chain for {}: {}", m.type_code(), m.chain().join(", "));
    }
    let base = |name: &str| PlayerBase { id: 0, name: name.to_string() };
    let mut all = vec![
        Player::Footballer(Footballer { base: base("Ann"), club: "Lions".into() }),
        Player::Cricketer(Cricketer { base: base("Bo"), runs: 412 }),
        Player::Bowler(Bowler { cricketer: Cricketer { base: base("Cy"), runs: 95 }, wickets: 31 }),
    ];
    for p in all.iter_mut() {
        let (name, log) = players.insert(p);
        println!("insert {}: {} saved {}", p.base().name, name, log.join(", "));
    }
    let (name, p) = players.find(3);
    println!("find 3: {} (mapper {})", p.describe(), name);
    let found: Vec<String> = players.find_all().iter().map(|p| p.describe()).collect();
    println!("find all: {}", found.join(" | "));
}
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

- Verdict: idiom. Rust has no class inheritance, so each mapper holds the mapper of its superclass in a `parent` field and calls it first. The `Mapper` trait gives `PlayerMapper` one interface for the three concrete mappers, kept as `Box<dyn Mapper>`.
- The domain types hold the struct of their superclass, and `Player::base` and `Player::cricketer` give the shared parts.
