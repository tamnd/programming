```rust run id=account-record-set
use std::fmt;

// A cell of a record set: the types that the columns of this example use.
#[derive(Clone, Debug, PartialEq)]
enum Cell {
    Int(i64),
    Text(String),
}

impl fmt::Display for Cell {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        match self {
            Cell::Int(n) => write!(f, "{}", n),
            Cell::Text(s) => write!(f, "{}", s),
        }
    }
}

// Rows and named columns in memory, in the shape of the result of a query.
struct RecordSet {
    columns: Vec<String>,
    rows: Vec<Vec<Cell>>,
    states: Vec<&'static str>,
}

impl RecordSet {
    fn new(columns: &[&str], rows: Vec<Vec<Cell>>) -> RecordSet {
        let states = vec!["unchanged"; rows.len()];
        RecordSet { columns: columns.iter().map(|c| c.to_string()).collect(), rows, states }
    }

    fn index(&self, column: &str) -> usize {
        self.columns.iter().position(|c| c == column).expect("no such column")
    }

    fn get(&self, i: usize, column: &str) -> &Cell {
        &self.rows[i][self.index(column)]
    }

    fn int(&self, i: usize, column: &str) -> i64 {
        match self.get(i, column) {
            Cell::Int(n) => *n,
            other => panic!("not an integer: {}", other),
        }
    }

    fn set(&mut self, i: usize, column: &str, v: Cell) {
        let k = self.index(column);
        self.rows[i][k] = v;
        if self.states[i] == "unchanged" {
            self.states[i] = "modified";
        }
    }

    fn add(&mut self, values: Vec<Cell>) {
        self.rows.push(values);
        self.states.push("added");
    }
}

fn money(c: i64) -> String {
    format!("{}.{:02}", c / 100, c % 100)
}

fn text(s: &str) -> Cell {
    Cell::Text(s.to_string())
}

fn main() {
    // The columns and rows as a database driver returns them for:
    // select id, name, balance from account order by id
    let mut rs = RecordSet::new(
        &["id", "name", "balance"],
        vec![vec![Cell::Int(1), text("Ann"), Cell::Int(12000)], vec![Cell::Int(2), text("Bob"), Cell::Int(1550)]],
    );

    println!("columns: {}", rs.columns.join(", "));
    let b = rs.int(1, "balance");
    rs.set(1, "balance", Cell::Int(b + 1000));
    rs.add(vec![Cell::Int(3), text("Cy"), Cell::Int(0)]);
    for i in 0..rs.rows.len() {
        println!("{} | {} | {} | {}", rs.get(i, "id"), rs.get(i, "name"), money(rs.int(i, "balance")), rs.states[i]);
    }
    let rich: Vec<String> = (0..rs.rows.len()).filter(|&i| rs.int(i, "balance") > 2000).map(|i| rs.get(i, "name").to_string()).collect();
    println!("balance over 20.00: {}", rich.join(", "));
    println!("rows to save: {}", rs.states.iter().filter(|s| **s != "unchanged").count());
}
```

```output
columns: id, name, balance
1 | Ann | 120.00 | unchanged
2 | Bob | 25.50 | modified
3 | Cy | 0.00 | added
balance over 20.00: Ann, Bob
rows to save: 2
```

- Verdict: idiom. The cells are an enum with one variant for each type of column, so a cell can hold an integer or a text. `int` reads a cell as an integer and panics if it is not one.
- The standard library has no database access; crates such as `rusqlite` give rows with named columns.
