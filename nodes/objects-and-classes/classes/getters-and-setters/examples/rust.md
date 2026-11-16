```rust run id=getter-without-get
pub struct Employee {
    name: String, // private to this module
    email: String,
}

impl Employee {
    pub fn new(name: &str, email: &str) -> Result<Employee, String> {
        let mut e = Employee { name: name.to_string(), email: String::new() };
        e.set_email(email)?;
        Ok(e)
    }

    pub fn name(&self) -> &str { // the getter has the name of the field
        &self.name
    }

    pub fn email(&self) -> &str {
        &self.email
    }

    pub fn set_email(&mut self, email: &str) -> Result<(), String> {
        let clean = email.trim().to_lowercase();
        if !clean.contains('@') {
            return Err(format!("not an email address: {clean}"));
        }
        self.email = clean;
        Ok(())
    }
}

fn main() {
    let mut e = Employee::new("Ana", "ana@example.com").unwrap();
    e.set_email("  Ana@Example.COM ").unwrap();
    println!("{} {}", e.name(), e.email());
    if let Err(msg) = e.set_email("not an address") {
        println!("{msg}");
    }
    println!("{}", e.email());
}
```

```output for=getter-without-get
Ana ana@example.com
not an email address: not an address
ana@example.com
```

The getters are `name` and `email`, not `get_name`, and they return `&str`, a borrowed view of the field, so the caller cannot change the string. The setter takes `&mut self` and returns a `Result`. A field without `pub` is private to its module, so `main` in another module could not write `e.email` directly; here `main` is in the same module, so the methods are a choice that the module makes.
