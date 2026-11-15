```rust run id=sessions-on-two-servers
use std::collections::BTreeMap;

const TIMEOUT: u32 = 30; // minutes with no request, then the session ends

type Cart = Vec<(String, u32)>;

// One server's sessions, kept in its memory as serialized text, with the time of their last use.
struct SessionStore {
    name: String,
    sessions: BTreeMap<String, (String, u32)>,
    count: u32,
}

impl SessionStore {
    fn new(name: &str) -> Self {
        SessionStore { name: name.to_string(), sessions: BTreeMap::new(), count: 0 }
    }

    fn new_id(&mut self) -> String {
        // A real server makes a random ID with a secure generator, such as the getrandom crate.
        self.count += 1;
        format!("{}{}", self.name, self.count)
    }

    fn load(&self, sid: &str, now: u32) -> Option<Cart> {
        let (text, last) = self.sessions.get(sid)?;
        if now - last > TIMEOUT {
            return None;
        }
        Some(deserialize(text))
    }

    fn save(&mut self, sid: &str, cart: &Cart, now: u32) {
        self.sessions.insert(sid.to_string(), (serialize(cart), now));
    }

    fn sweep(&mut self, now: u32) -> Vec<String> {
        let idle: Vec<String> = self.sessions.iter().filter(|(_, (_, last))| now - last > TIMEOUT).map(|(sid, _)| sid.clone()).collect();
        for sid in &idle {
            self.sessions.remove(sid);
        }
        idle
    }
}

fn serialize(cart: &Cart) -> String {
    cart.iter().map(|(item, qty)| format!("{}:{}", item, qty)).collect::<Vec<_>>().join(",")
}

fn deserialize(text: &str) -> Cart {
    text.split(',')
        .filter_map(|part| part.split_once(':'))
        .map(|(item, qty)| (item.to_string(), qty.parse().unwrap_or(0)))
        .collect()
}

fn show(cart: &Cart) -> String {
    cart.iter().map(|(item, qty)| format!("{} {}", item, qty)).collect::<Vec<_>>().join(", ")
}

// One request. The client sends only its session ID; the state is on the server.
fn add(server: &mut SessionStore, sid: Option<&str>, who: &str, item: &str, now: u32) -> String {
    let (sid, mut cart, status) = match sid.and_then(|s| server.load(s, now).map(|c| (s, c))) {
        Some((s, cart)) => (s.to_string(), cart, format!("session {}", s)),
        None => {
            let mut status = match sid {
                Some(s) => format!("session {} is unknown here, ", s),
                None => String::new(),
            };
            let id = server.new_id();
            status += &format!("new session {}", id);
            (id, Vec::new(), status)
        }
    };
    match cart.iter_mut().find(|(i, _)| i == item) {
        Some(line) => line.1 += 1,
        None => cart.push((item.to_string(), 1)),
    }
    server.save(&sid, &cart, now);
    println!("10:{:02} {} adds {} on server {}: {}, cart {}", now, who, item, server.name, status, show(&cart));
    sid
}

fn main() {
    let mut a = SessionStore::new("a");
    let mut b = SessionStore::new("b");
    let ann = add(&mut a, None, "ann", "pen", 0);
    add(&mut a, None, "bob", "ink", 1);
    add(&mut a, Some(&ann), "ann", "pen", 2);
    add(&mut b, Some(&ann), "ann", "ink", 3); // a load balancer sends this request to the other server
    add(&mut a, Some(&ann), "ann", "ink", 30);
    println!("10:40 server a removes idle sessions: {}", a.sweep(40).join(", "));
    for (sid, (text, last)) in &a.sessions {
        println!("server a keeps {} = '{}', last used at 10:{:02}", sid, text, last);
    }
}
```

```output
10:00 ann adds pen on server a: new session a1, cart pen 1
10:01 bob adds ink on server a: new session a2, cart ink 1
10:02 ann adds pen on server a: session a1, cart pen 2
10:03 ann adds ink on server b: session a1 is unknown here, new session b1, cart ink 1
10:30 ann adds ink on server a: session a1, cart pen 2, ink 1
10:40 server a removes idle sessions: a2
server a keeps a1 = 'pen:2,ink:1', last used at 10:30
```

- Verdict: idiom. The store is a `BTreeMap`, so the IDs come out in order. `load` uses the `?` operator on the `Option` from `get`, so an unknown ID gives `None`.
- A server with many threads would put the store in an `Arc<Mutex<...>>`.
