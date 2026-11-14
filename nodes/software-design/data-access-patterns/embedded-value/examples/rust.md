```rust run id=value-in-owner-columns
use std::collections::BTreeMap;
use std::fmt;

// Value objects: no key, compared by their fields with the derived PartialEq.
#[derive(Clone, Debug, PartialEq)]
struct DateRange {
    start: String,
    end: Option<String>, // None: the employment has not ended.
}

impl fmt::Display for DateRange {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        write!(f, "{} to {}", self.start, self.end.as_deref().unwrap_or("open"))
    }
}

#[derive(Clone, Debug, PartialEq)]
struct Money {
    amount: i64,
    currency: String,
}

impl fmt::Display for Money {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        write!(f, "{} {}", self.amount, self.currency)
    }
}

fn money(amount: i64, currency: &str) -> Money {
    Money { amount, currency: currency.to_string() }
}

// The row: the columns of the employment table.
struct Row {
    person: String,
    start_date: String,
    end_date: Option<String>,
    salary_amount: i64,
    salary_currency: String,
}

struct Employment {
    id: Option<i64>,
    person: String,
    period: DateRange,
    salary: Money,
}

// A stand-in for the database: one table keyed by id.
#[derive(Default)]
struct Db {
    employment: BTreeMap<i64, Row>,
    last_id: i64,
}

impl Db {
    fn to_row(e: &Employment) -> Row {
        Row {
            person: e.person.clone(),
            start_date: e.period.start.clone(),
            end_date: e.period.end.clone(),
            salary_amount: e.salary.amount,
            salary_currency: e.salary.currency.clone(),
        }
    }

    fn insert(&mut self, e: &mut Employment) {
        self.last_id += 1;
        self.employment.insert(self.last_id, Db::to_row(e));
        e.id = Some(self.last_id);
    }

    fn update(&mut self, e: &Employment) {
        if let Some(id) = e.id {
            self.employment.insert(id, Db::to_row(e));
        }
    }

    fn find(&self, id: i64) -> Employment {
        let r = &self.employment[&id];
        Employment {
            id: Some(id),
            person: r.person.clone(),
            period: DateRange { start: r.start_date.clone(), end: r.end_date.clone() },
            salary: money(r.salary_amount, &r.salary_currency),
        }
    }
}

fn main() {
    let mut db = Db::default();
    let mut ann = Employment {
        id: None,
        person: "Ann".to_string(),
        period: DateRange { start: "2024-01-01".to_string(), end: Some("2024-12-31".to_string()) },
        salary: money(5000, "EUR"),
    };
    let mut bo = Employment {
        id: None,
        person: "Bo".to_string(),
        period: DateRange { start: "2025-03-01".to_string(), end: None },
        salary: money(4000, "EUR"),
    };
    db.insert(&mut ann);
    db.insert(&mut bo);
    for (id, r) in &db.employment {
        let end = r.end_date.as_deref().unwrap_or("null");
        println!("row: {} {} {} {} {} {}", id, r.person, r.start_date, end, r.salary_amount, r.salary_currency);
    }
    let mut ann = db.find(1);
    let bo = db.find(2);
    for e in [&ann, &bo] {
        println!("loaded: {} | {} | {}", e.person, e.period, e.salary);
    }
    let same = if ann.salary == money(5000, "EUR") { "yes" } else { "no" };
    println!("salary of Ann equals 5000 EUR: {}", same);

    ann.salary = money(5500, "EUR"); // A new value replaces the old one.
    db.update(&ann);
    println!("after raise: Ann {} | Bo {}", db.find(1).salary, db.find(2).salary);

    let above: Vec<&str> = db
        .employment
        .values()
        .filter(|r| r.salary_currency == "EUR" && r.salary_amount > 4500)
        .map(|r| r.person.as_str())
        .collect();
    println!("EUR salaries above 4500: {}", above.join(", "));
}
```

```output
row: 1 Ann 2024-01-01 2024-12-31 5000 EUR
row: 2 Bo 2025-03-01 null 4000 EUR
loaded: Ann | 2024-01-01 to 2024-12-31 | 5000 EUR
loaded: Bo | 2025-03-01 to open | 4000 EUR
salary of Ann equals 5000 EUR: yes
after raise: Ann 5500 EUR | Bo 4000 EUR
EUR salaries above 4500: Ann
```

- Verdict: idiom. `DateRange` and `Money` derive `PartialEq`, so `==` compares their fields. The end date is an `Option<String>`, so `None` stands for a null column.
- `to_row` copies the values into a flat `Row`, and `find` clones the columns into new values. The owner holds its values, so no other owner can change them.
