```rust run id=shop-on-two-servers
use std::collections::BTreeMap;

// The "database" is a set of tables that every server shares.
// A real system keeps these tables in a database server.

const TIMEOUT: u32 = 30; // minutes with no request before a session ends

struct Session {
    user: String,
    last_used: u32,
}

struct Line {
    key: String, // the session ID for a cart line, the order ID for an order line
    item: String,
    qty: u32,
}

#[derive(Default)]
struct Db {
    sessions: BTreeMap<String, Session>,
    cart_lines: Vec<Line>, // the pending data
    orders: Vec<String>,
    order_lines: Vec<Line>, // the committed business data
}

fn hhmm(t: u32) -> String {
    format!("{}:{:02}", t / 60, t % 60)
}

impl Db {
    fn start(&mut self, sid: &str, user: &str, now: u32) {
        let s = Session { user: user.to_string(), last_used: now };
        self.sessions.insert(sid.to_string(), s);
    }

    // Return the user of the session, or None if it is unknown or idle.
    fn touch(&mut self, sid: &str, now: u32) -> Option<String> {
        let s = self.sessions.get_mut(sid)?;
        if now - s.last_used > TIMEOUT {
            return None;
        }
        s.last_used = now;
        Some(s.user.clone())
    }

    fn cart(&self, sid: &str) -> String {
        let parts: Vec<String> = self
            .cart_lines
            .iter()
            .filter(|l| l.key == sid)
            .map(|l| format!("{} {}", l.item, l.qty))
            .collect();
        parts.join(", ")
    }

    fn add(&mut self, server: &str, sid: &str, item: &str, now: u32) {
        let user = self.touch(sid, now).unwrap_or_default();
        match self.cart_lines.iter_mut().find(|l| l.key == sid && l.item == item) {
            Some(l) => l.qty += 1,
            None => self.cart_lines.push(Line { key: sid.to_string(), item: item.to_string(), qty: 1 }),
        }
        println!("{} {} adds {} on server {}: cart {}", hhmm(now), user, item, server, self.cart(sid));
    }

    // The pending cart lines become an order. A database does this in one transaction.
    fn checkout(&mut self, server: &str, sid: &str, now: u32) {
        let user = self.touch(sid, now).unwrap_or_default();
        let lines = self.cart(sid);
        self.orders.push(user.clone());
        let oid = self.orders.len().to_string();
        let (mine, rest): (Vec<Line>, Vec<Line>) =
            std::mem::take(&mut self.cart_lines).into_iter().partition(|l| l.key == sid);
        for l in mine {
            self.order_lines.push(Line { key: oid.clone(), item: l.item, qty: l.qty });
        }
        self.cart_lines = rest;
        println!("{} {} checks out on server {}: order {} with {}", hhmm(now), user, server, oid, lines);
    }

    fn report(&self) {
        let mut sums: BTreeMap<&str, u32> = BTreeMap::new();
        for l in &self.order_lines {
            *sums.entry(l.item.as_str()).or_insert(0) += l.qty;
        }
        let parts: Vec<String> = sums.iter().map(|(i, q)| format!("{} {}", i, q)).collect();
        println!("sales: {}", parts.join(", "));
    }

    fn purge(&mut self, now: u32) {
        let ids: Vec<String> = self
            .sessions
            .iter()
            .filter(|(_, s)| now - s.last_used > TIMEOUT)
            .map(|(id, _)| id.clone())
            .collect();
        self.cart_lines.retain(|l| !ids.contains(&l.key));
        for id in &ids {
            self.sessions.remove(id);
        }
        println!("{} purge sessions idle for more than {} minutes: {}", hhmm(now), TIMEOUT, ids.join(", "));
    }
}

fn main() {
    let mut db = Db::default();
    db.start("s1", "ann", 600);
    db.start("s2", "bob", 601);
    db.add("a", "s1", "pen", 600);
    db.add("a", "s2", "ink", 601);
    db.add("b", "s1", "pen", 603); // server b reads the rows that server a wrote
    db.add("a", "s1", "ink", 604);
    db.checkout("b", "s1", 625);
    db.report();
    db.purge(640);
    println!(
        "rows left: sessions {}, cart lines {}, orders {}, order lines {}",
        db.sessions.len(),
        db.cart_lines.len(),
        db.orders.len(),
        db.order_lines.len()
    );
}
```

```output
10:00 ann adds pen on server a: cart pen 1
10:01 bob adds ink on server a: cart ink 1
10:03 ann adds pen on server b: cart pen 2
10:04 ann adds ink on server a: cart pen 2, ink 1
10:25 ann checks out on server b: order 1 with pen 2, ink 1
sales: ink 1, pen 2
10:40 purge sessions idle for more than 30 minutes: s2
rows left: sessions 1, cart lines 0, orders 1, order lines 2
```

- Verdict: library. The Rust standard library has no database access, so a real server uses a crate for its database. The example keeps the tables in a `Db` value, so it runs with no crate.
- `touch` returns `None` for a session that is unknown or idle. `partition` splits the cart lines into the lines of the session and the rest.
