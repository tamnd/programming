```rust run id=modules-and-local-items
mod playlist {
    pub struct Playlist {
        name: String,
        songs: Vec<String>,
    }

    // A struct in the same module: it can read the private fields of Playlist.
    pub struct Cursor<'a> {
        owner: &'a Playlist,
        i: usize,
    }

    impl Playlist {
        pub fn new(name: &str, songs: &[&str]) -> Playlist {
            Playlist { name: name.to_string(), songs: songs.iter().map(|s| s.to_string()).collect() }
        }
        pub fn cursor(&self) -> Cursor<'_> {
            Cursor { owner: self, i: 0 }
        }
    }

    impl<'a> Iterator for Cursor<'a> {
        type Item = String;
        fn next(&mut self) -> Option<String> {
            let song = self.owner.songs.get(self.i)?;
            self.i += 1;
            Some(format!("{}: {}", self.owner.name, song))
        }
    }
}

fn main() {
    let p = playlist::Playlist::new("road", &["one", "two"]);
    for line in p.cursor() {
        println!("{line}");
    }

    struct Pair(u32, u32); // a local item: visible only in main
    let scale = 10;
    let add = |p: Pair| (p.0 + p.1) * scale; // a closure captures scale; an item could not
    println!("{}", add(Pair(1, 2)));
}
```

```output for=modules-and-local-items
road: one
road: two
30
```

- Verdict: idiom. Rust has no nested types inside a struct or an `impl`. A module groups the two types, and privacy is per module, so `Cursor` reads the private fields of `Playlist`.
- The lifetime `'a` ties the cursor to the playlist: the compiler refuses code that drops `p` while a cursor is in use. A struct declared in a function is local to it but cannot use the function's variables; a closure can.
