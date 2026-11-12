```rust run id=act
use std::collections::HashMap;

fn service(requests: &[(u32, String)]) -> Vec<(u32, String)> {
    // replies in the reverse order; the token comes back unchanged
    requests.iter().rev().map(|(t, n)| (*t, format!("{} loaded", n))).collect()
}

fn main() {
    let mut pending: HashMap<u32, String> = HashMap::new();
    let mut requests = Vec::new();
    for (i, name) in ["image-7", "image-9"].iter().enumerate() {
        let token = i as u32 + 1;
        pending.insert(token, name.to_string());
        requests.push((token, name.to_string()));
        println!("send {}: load {}", token, name);
    }
    for (token, result) in service(&requests) {
        pending.remove(&token); // the token finds the state at once
        println!("done {}: {}", token, result);
    }
}
```

```output
send 1: load image-7
send 2: load image-9
done 2: image-9 loaded
done 1: image-7 loaded
```

- Verdict: idiom. The token is a `u32` key.
