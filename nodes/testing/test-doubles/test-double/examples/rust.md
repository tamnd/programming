```rust test id=register
use std::cell::RefCell;
use std::collections::HashSet;

pub trait Users {
    fn exists(&self, email: &str) -> bool;
    fn add(&mut self, email: &str);
}

pub trait Mailer {
    fn send(&self, to: &str, body: &str);
}

pub fn register(email: &str, users: &mut impl Users, mailer: &impl Mailer) -> &'static str {
    if users.exists(email) {
        return "taken";
    }
    users.add(email);
    mailer.send(email, "Welcome!");
    "created"
}

#[cfg(test)]
mod tests {
    use super::*;

    #[derive(Default)]
    struct FakeUsers(HashSet<String>);

    impl Users for FakeUsers {
        fn exists(&self, email: &str) -> bool {
            self.0.contains(email)
        }
        fn add(&mut self, email: &str) {
            self.0.insert(email.to_string());
        }
    }

    // send takes &self, so the spy needs RefCell to record calls.
    #[derive(Default)]
    struct SpyMailer {
        sent: RefCell<Vec<(String, String)>>,
    }

    impl Mailer for SpyMailer {
        fn send(&self, to: &str, body: &str) {
            self.sent.borrow_mut().push((to.to_string(), body.to_string()));
        }
    }

    #[test]
    fn unused_email_is_stored_and_welcomed() {
        let mut users = FakeUsers::default();
        let mailer = SpyMailer::default();
        assert_eq!(register("ada@example.com", &mut users, &mailer), "created");
        assert!(users.exists("ada@example.com"));
        assert_eq!(
            *mailer.sent.borrow(),
            vec![("ada@example.com".to_string(), "Welcome!".to_string())]
        );
    }

    #[test]
    fn taken_email_gets_no_mail() {
        let mut users = FakeUsers::default();
        users.add("ada@example.com");
        let mailer = SpyMailer::default();
        assert_eq!(register("ada@example.com", &mut users, &mailer), "taken");
        assert!(mailer.sent.borrow().is_empty());
    }
}
```

```output

running 2 tests
test tests::taken_email_gets_no_mail ... ok
test tests::unused_email_is_stored_and_welcomed ... ok

test result: ok. 2 passed; 0 failed; 0 ignored; 0 measured; 0 filtered out; finished in $TIME
```

- Verdict: idiom. The standard library has no mock functionality. A double is a type that implements the trait that the code takes, here through `impl Users` and `impl Mailer`.
- `send` takes `&self`, so a spy cannot change its fields in the normal way. `RefCell` moves the borrow check to run time, which lets the spy push a record through a shared reference. The Rust book uses the same pattern for its mock object.
- The doubles live in `#[cfg(test)] mod tests`, so they are compiled only into the test build.
- `register` is generic over the traits, so the compiler makes one copy for the doubles and one for the real types. A `&dyn Mailer` parameter would give one copy and a call through a vtable.

Without `RefCell`, the spy tries to change a field through `&self`, and the compiler rejects it with error E0596.

```rust fails expect=compile id=no-refcell
trait Mailer {
    fn send(&self, to: &str, body: &str);
}

struct SpyMailer {
    sent: Vec<(String, String)>,
}

impl Mailer for SpyMailer {
    fn send(&self, to: &str, body: &str) {
        self.sent.push((to.to_string(), body.to_string()));
    }
}

fn main() {
    let spy = SpyMailer { sent: Vec::new() };
    spy.send("ada@example.com", "Welcome!");
    println!("{}", spy.sent.len());
}
```
