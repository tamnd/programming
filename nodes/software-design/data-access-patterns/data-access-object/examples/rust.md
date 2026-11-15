```rust run id=two-daos-one-interface
use std::collections::BTreeMap;

// The transfer object: plain data that goes in and out of the DAO.
#[derive(Clone)]
struct Player {
    id: u32,
    name: String,
    country: String,
}

fn player(id: u32, name: &str, country: &str) -> Player {
    Player { id, name: name.to_string(), country: country.to_string() }
}

// The interface that the business code knows. It names no storage.
trait PlayerDao {
    fn name(&self) -> &'static str;
    fn insert(&mut self, p: &Player);
    fn find(&self, id: u32) -> Player;
    fn find_all(&self) -> Vec<Player>;
    fn update(&mut self, p: &Player);
    fn delete(&mut self, id: u32);
    fn find_by_country(&self, country: &str) -> Vec<Player> {
        self.find_all().into_iter().filter(|p| p.country == country).collect()
    }
}

// The data source is a map from key to record.
struct MemoryPlayerDao {
    rows: BTreeMap<u32, (String, String)>,
}

impl PlayerDao for MemoryPlayerDao {
    fn name(&self) -> &'static str {
        "MemoryPlayerDao"
    }
    fn insert(&mut self, p: &Player) {
        self.rows.insert(p.id, (p.name.clone(), p.country.clone()));
    }
    fn find(&self, id: u32) -> Player {
        let (name, country) = &self.rows[&id];
        player(id, name, country)
    }
    fn find_all(&self) -> Vec<Player> {
        self.rows.keys().map(|id| self.find(*id)).collect()
    }
    fn update(&mut self, p: &Player) {
        self.insert(p);
    }
    fn delete(&mut self, id: u32) {
        self.rows.remove(&id);
    }
}

// The data source is a flat file: one line "id;name;country" for each player.
struct TextPlayerDao {
    lines: Vec<String>,
}

fn parse(line: &str) -> Player {
    let f: Vec<&str> = line.split(';').collect();
    player(f[0].parse().unwrap(), f[1], f[2])
}

fn format(p: &Player) -> String {
    format!("{};{};{}", p.id, p.name, p.country)
}

impl TextPlayerDao {
    fn index(&self, id: u32) -> usize {
        self.lines.iter().position(|line| parse(line).id == id).expect("no such player")
    }
}

impl PlayerDao for TextPlayerDao {
    fn name(&self) -> &'static str {
        "TextPlayerDao"
    }
    fn insert(&mut self, p: &Player) {
        self.lines.push(format(p));
    }
    fn find(&self, id: u32) -> Player {
        parse(&self.lines[self.index(id)])
    }
    fn find_all(&self) -> Vec<Player> {
        let mut all: Vec<Player> = self.lines.iter().map(|line| parse(line)).collect();
        all.sort_by_key(|p| p.id);
        all
    }
    fn update(&mut self, p: &Player) {
        let i = self.index(p.id);
        self.lines[i] = format(p);
    }
    fn delete(&mut self, id: u32) {
        let i = self.index(id);
        self.lines.remove(i);
    }
}

// The business object. It uses only the DAO interface.
struct PlayerService<'a> {
    dao: &'a mut dyn PlayerDao,
}

impl PlayerService<'_> {
    fn transfer(&mut self, id: u32, country: &str) {
        let mut p = self.dao.find(id);
        p.country = country.to_string();
        self.dao.update(&p);
    }
}

fn names(players: &[Player]) -> String {
    players.iter().map(|p| format!("{} {}", p.name, p.country)).collect::<Vec<_>>().join(" | ")
}

fn run(dao: &mut dyn PlayerDao) {
    println!("dao: {}", dao.name());
    for p in [player(1, "Ann", "NZ"), player(2, "Bo", "NZ"), player(3, "Cy", "GB")] {
        dao.insert(&p);
    }
    println!("all: {}", names(&dao.find_all()));
    println!("find 1: {}", names(&[dao.find(1)]));
    PlayerService { dao: &mut *dao }.transfer(1, "AU");
    println!("from NZ: {}", names(&dao.find_by_country("NZ")));
    println!("from AU: {}", names(&dao.find_by_country("AU")));
    dao.delete(3);
    println!("all: {}", names(&dao.find_all()));
}

fn main() {
    // The choice of storage is made here, in one place.
    let mut memory = MemoryPlayerDao { rows: BTreeMap::new() };
    let mut text = TextPlayerDao { lines: Vec::new() };
    run(&mut memory);
    run(&mut text);

    // The business code never saw this format; only TextPlayerDao knows it.
    println!("lines in the text store: {}", text.lines.join(" | "));
}
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

- Verdict: idiom. `PlayerDao` is a trait, and `find_by_country` is a default method that both DAOs share. The business code takes `&mut dyn PlayerDao`, so it does not know the type of the DAO.
- A DAO for a database would return `Result` values; the example leaves out the errors because the data is in memory.
