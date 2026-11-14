```rust run id=link-table
use std::collections::{BTreeMap, BTreeSet};

// A stand-in for the database: two entity tables and a link table of key pairs.
struct Db {
    employee: BTreeMap<i64, String>,
    skill: BTreeMap<i64, String>,
    employee_skill: BTreeSet<(i64, i64)>, // Sorted by both keys, like a primary key.
    last_employee: i64,
}

struct Employee {
    id: Option<i64>,
    name: String,
    skills: Vec<i64>, // A list of skill keys.
}

impl Db {
    fn insert_link(&mut self, employee_id: i64, skill_id: i64) -> Result<(), String> {
        if !self.employee_skill.insert((employee_id, skill_id)) {
            return Err("UNIQUE constraint failed: employee_skill.employee_id, employee_skill.skill_id".to_string());
        }
        Ok(())
    }

    fn insert(&mut self, e: &mut Employee) -> Result<(), String> {
        self.last_employee += 1;
        self.employee.insert(self.last_employee, e.name.clone());
        e.id = Some(self.last_employee);
        for &s in &e.skills {
            self.insert_link(self.last_employee, s)?;
        }
        Ok(())
    }

    fn update_skills(&mut self, e: &Employee) -> Result<(usize, usize), String> {
        // Compare the collection with the link rows: insert the new pairs, delete the old ones.
        let id = e.id.ok_or("not saved")?;
        let in_db: Vec<i64> = self.employee_skill.iter().filter(|(emp, _)| *emp == id).map(|(_, s)| *s).collect();
        let added: Vec<i64> = e.skills.iter().copied().filter(|s| !in_db.contains(s)).collect();
        let removed: Vec<i64> = in_db.iter().copied().filter(|s| !e.skills.contains(s)).collect();
        for &s in &added {
            self.insert_link(id, s)?;
        }
        for &s in &removed {
            self.employee_skill.remove(&(id, s));
        }
        Ok((removed.len(), added.len()))
    }

    fn skills_of(&self, employee_id: i64) -> String {
        let names: Vec<&str> = self
            .employee_skill
            .iter()
            .filter(|(emp, _)| *emp == employee_id)
            .map(|(_, s)| self.skill[s].as_str())
            .collect();
        names.join(", ")
    }

    fn employees_with(&self, skill_id: i64) -> String {
        let names: Vec<&str> = self
            .employee_skill
            .iter()
            .filter(|(_, s)| *s == skill_id)
            .map(|(emp, _)| self.employee[emp].as_str())
            .collect();
        names.join(", ")
    }

    fn links(&self) -> String {
        let parts: Vec<String> = self.employee_skill.iter().map(|(e, s)| format!("{}-{}", e, s)).collect();
        parts.join(" ")
    }
}

fn main() {
    let mut db = Db {
        employee: BTreeMap::new(),
        skill: BTreeMap::from([(1, "Python".to_string()), (2, "SQL".to_string()), (3, "Go".to_string())]),
        employee_skill: BTreeSet::new(),
        last_employee: 0,
    };
    let mut ann = Employee { id: None, name: "Ann".to_string(), skills: vec![1, 2] };
    let mut bo = Employee { id: None, name: "Bo".to_string(), skills: vec![1] };
    for e in [&mut ann, &mut bo] {
        if let Err(err) = db.insert(e) {
            println!("{}", err);
        }
    }
    println!("links: {}", db.links());
    println!("Ann: {}", db.skills_of(1));
    println!("Python: {}", db.employees_with(1));

    ann.skills = vec![2, 3];
    match db.update_skills(&ann) {
        Ok((deleted, inserted)) => println!("update Ann: deleted {}, inserted {}", deleted, inserted),
        Err(err) => println!("{}", err),
    }
    println!("links: {}", db.links());
    println!("Ann: {}", db.skills_of(1));

    if let Err(err) = db.insert_link(2, 1) {
        println!("link 2-1 again: {}", err);
    }
}
```

```output
links: 1-1 1-2 2-1
Ann: Python, SQL
Python: Ann, Bo
update Ann: deleted 1, inserted 1
links: 1-2 1-3 2-1
Ann: SQL, Go
link 2-1 again: UNIQUE constraint failed: employee_skill.employee_id, employee_skill.skill_id
```

- Verdict: idiom. The link table is a `BTreeSet<(i64, i64)>`, so the tuple is the key, the rows iterate in key order, and `insert` returns `false` for a pair that is already there.
- The methods that write rows return `Result`, and `?` passes the error of the constraint to the caller.
