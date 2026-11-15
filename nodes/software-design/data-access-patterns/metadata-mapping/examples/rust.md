```rust run id=generic-mapper-from-metadata
use std::collections::BTreeMap;

#[derive(Clone, Debug)]
enum Value {
    Int(i64),
    Text(String),
}

impl std::fmt::Display for Value {
    fn fmt(&self, f: &mut std::fmt::Formatter) -> std::fmt::Result {
        match self {
            Value::Int(n) => write!(f, "{}", n),
            Value::Text(s) => write!(f, "{}", s),
        }
    }
}

impl From<&i64> for Value {
    fn from(n: &i64) -> Self {
        Value::Int(*n)
    }
}

impl From<&String> for Value {
    fn from(s: &String) -> Self {
        Value::Text(s.clone())
    }
}

trait FromValue {
    fn from_value(v: &Value) -> Self;
}

impl FromValue for i64 {
    fn from_value(v: &Value) -> Self {
        match v {
            Value::Int(n) => *n,
            Value::Text(_) => panic!("not an integer"),
        }
    }
}

impl FromValue for String {
    fn from_value(v: &Value) -> Self {
        match v {
            Value::Text(s) => s.clone(),
            Value::Int(_) => panic!("not text"),
        }
    }
}

// What the generic mapper needs from each mapped type.
trait Mapped: Sized {
    const NAME: &'static str;
    const TABLE: &'static str;
    const COLUMNS: &'static [(&'static str, &'static str)];
    fn id(&self) -> i64;
    fn values(&self) -> Vec<Value>;
    fn from_values(values: &[Value]) -> Self;
}

// Rust has no runtime reflection, so this macro reads the metadata at compile time
// and generates the struct and the code that moves each field to and from its column.
macro_rules! data_map {
    ($name:ident, $table:literal, { $($field:ident : $ty:ty => $column:literal),+ $(,)? }) => {
        #[derive(Debug)]
        struct $name {
            $($field: $ty),+
        }

        impl Mapped for $name {
            const NAME: &'static str = stringify!($name);
            const TABLE: &'static str = $table;
            const COLUMNS: &'static [(&'static str, &'static str)] = &[$((stringify!($field), $column)),+];
            fn id(&self) -> i64 {
                self.id
            }
            fn values(&self) -> Vec<Value> {
                vec![$(Value::from(&self.$field)),+]
            }
            fn from_values(values: &[Value]) -> Self {
                let mut it = values.iter();
                $name { $($field: <$ty>::from_value(it.next().expect("value"))),+ }
            }
        }
    };
}

data_map!(Player, "player", { id: i64 => "id", name: String => "name", country: String => "nation" });
data_map!(Club, "club", { id: i64 => "id", name: String => "title", city: String => "city" });

impl std::fmt::Display for Player {
    fn fmt(&self, f: &mut std::fmt::Formatter) -> std::fmt::Result {
        write!(f, "Player(id={}, name={}, country={})", self.id, self.name, self.country)
    }
}

impl std::fmt::Display for Club {
    fn fmt(&self, f: &mut std::fmt::Formatter) -> std::fmt::Result {
        write!(f, "Club(id={}, name={}, city={})", self.id, self.name, self.city)
    }
}

fn describe<T: Mapped>() -> String {
    let pairs: Vec<String> = T::COLUMNS.iter().map(|(f, c)| format!("{} -> {}", f, c)).collect();
    format!("{} -> table {}: {}", T::NAME, T::TABLE, pairs.join(", "))
}

// A stand-in for the database: each table maps an id to the values of its columns.
struct Mapper {
    db: BTreeMap<&'static str, BTreeMap<i64, Vec<Value>>>,
}

impl Mapper {
    fn insert<T: Mapped>(&mut self, obj: &T) -> String {
        self.db.entry(T::TABLE).or_default().insert(obj.id(), obj.values());
        let cols: Vec<&str> = T::COLUMNS.iter().map(|(_, c)| *c).collect();
        let marks = vec!["?"; T::COLUMNS.len()];
        format!("insert into {} ({}) values ({})", T::TABLE, cols.join(", "), marks.join(", "))
    }

    fn find<T: Mapped>(&self, id: i64) -> T {
        T::from_values(&self.db[T::TABLE][&id])
    }

    fn update<T: Mapped>(&mut self, obj: &T) -> String {
        self.db.get_mut(T::TABLE).expect("table").insert(obj.id(), obj.values());
        let sets: Vec<String> = T::COLUMNS.iter().filter(|(f, _)| *f != "id").map(|(_, c)| format!("{} = ?", c)).collect();
        format!("update {} set {} where id = ?", T::TABLE, sets.join(", "))
    }
}

fn main() {
    let mut mapper = Mapper { db: BTreeMap::new() };
    println!("map {}", describe::<Player>());
    println!("map {}", describe::<Club>());
    println!("insert: {}", mapper.insert(&Player { id: 1, name: "Ann".into(), country: "NZ".into() }));
    println!("insert: {}", mapper.insert(&Club { id: 1, name: "Lions".into(), city: "Leeds".into() }));
    let mut p: Player = mapper.find(1);
    println!("find player 1: {}", p);
    let c: Club = mapper.find(1);
    println!("find club 1: {}", c);
    p.country = "AU".into();
    println!("update: {}", mapper.update(&p));
    for table in ["player", "club"] {
        for values in mapper.db[table].values() {
            let vs: Vec<String> = values.iter().map(|v| v.to_string()).collect();
            println!("row in {}: {}", table, vs.join(" "));
        }
    }
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

- Verdict: idiom. Rust has no run-time reflection, so the `data_map!` macro reads the metadata at compile time and generates each struct with its `Mapped` impl: the table, the column list, `values` and `from_values`. This is the code generation form of the pattern.
- The generic mapper uses the `Mapped` trait, so `find::<Player>` and `find::<Club>` share one implementation.
