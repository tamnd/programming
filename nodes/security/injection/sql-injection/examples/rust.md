```rust wrong id=format
fn find_orders_sql(customer: &str) -> String {
    format!("SELECT id FROM orders WHERE customer = '{customer}'")
}

fn main() {
    println!("{}", find_orders_sql("alice"));
    println!("{}", find_orders_sql("x' OR '1'='1"));
}
```

```output for=format
SELECT id FROM orders WHERE customer = 'alice'
SELECT id FROM orders WHERE customer = 'x' OR '1'='1'
```

```rust run id=static-sql
// The SQL text must be a &'static str: a literal in the program.
// A String made with format! at run time does not have this type.
struct Query {
    sql: &'static str,
    params: Vec<String>,
}

impl Query {
    fn new(sql: &'static str) -> Self {
        Query { sql, params: Vec::new() }
    }

    fn bind(mut self, value: &str) -> Self {
        self.params.push(value.to_string());
        self
    }
}

fn main() {
    let customer = "x' OR '1'='1";
    let query = Query::new("SELECT id FROM orders WHERE customer = ?1").bind(customer);
    println!("query: {}", query.sql);
    println!("args:  {:?}", query.params);
}
```

```output for=static-sql
query: SELECT id FROM orders WHERE customer = ?1
args:  ["x' OR '1'='1"]
```

```rust fails expect=compile id=runtime-sql
struct Query {
    sql: &'static str,
}

fn main() {
    let customer = String::from("alice");
    let text = format!("SELECT id FROM orders WHERE customer = '{customer}'");
    let query = Query { sql: &text };
    println!("{}", query.sql);
}
```

- Verdict: library. The Rust standard library has no database API. The examples use only the standard library, and the bullets below describe the common crates.
- `format!` builds SQL text from input in the same way as concatenation in other languages.
- The type `&'static str` accepts a string literal and rejects a `String` made at run time, unless the program leaks the string on purpose. So the compiler stops SQL text that was built from input. The third example fails to compile for this reason. sqlx goes further: its `query!` macro takes a literal and checks it against the database at compile time.
- With rusqlite, pass values in `params!` and use `?1`, `?2` or `:name` in the text: `conn.query_row("SELECT id FROM orders WHERE customer = ?1", params![customer], |row| row.get(0))`.
- With sqlx, chain `.bind(value)` for each placeholder: `sqlx::query("SELECT id FROM orders WHERE customer = $1").bind(customer)`.
- No example here uses a real database: rusqlite and sqlx were not in the local cargo cache when the examples were recorded, and the run has no network.
