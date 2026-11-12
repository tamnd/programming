```rust run id=length-prefixed-binary
#[derive(Debug, PartialEq)]
struct Person {
    name: String,
    born: i32,
    tags: Vec<String>,
}

const VERSION: u8 = 1;

fn put_str(out: &mut Vec<u8>, s: &str) {
    out.extend_from_slice(&(s.len() as u32).to_le_bytes()); // length prefix
    out.extend_from_slice(s.as_bytes());
}

struct Reader<'a> {
    data: &'a [u8],
    pos: usize,
}

impl<'a> Reader<'a> {
    fn take(&mut self, n: usize) -> Result<&'a [u8], String> {
        if n > self.data.len() - self.pos {
            return Err(format!("needs {n} bytes at byte {}, has {}", self.pos, self.data.len() - self.pos));
        }
        self.pos += n;
        Ok(&self.data[self.pos - n..self.pos])
    }
    fn u32(&mut self) -> Result<u32, String> {
        Ok(u32::from_le_bytes(self.take(4)?.try_into().unwrap()))
    }
    fn string(&mut self) -> Result<String, String> {
        let n = self.u32()? as usize; // checked by take before any allocation
        String::from_utf8(self.take(n)?.to_vec()).map_err(|e| e.to_string())
    }
}

impl Person {
    fn encode(&self) -> Vec<u8> {
        let mut out = vec![VERSION];
        put_str(&mut out, &self.name);
        out.extend_from_slice(&self.born.to_le_bytes());
        out.extend_from_slice(&(self.tags.len() as u32).to_le_bytes());
        for t in &self.tags {
            put_str(&mut out, t);
        }
        out
    }

    fn decode(data: &[u8]) -> Result<Person, String> {
        let mut r = Reader { data, pos: 0 };
        match r.take(1)?[0] {
            VERSION => {}
            v => return Err(format!("unknown version {v}")),
        }
        let name = r.string()?;
        let born = i32::from_le_bytes(r.take(4)?.try_into().unwrap());
        let count = r.u32()?;
        let tags = (0..count).map(|_| r.string()).collect::<Result<_, _>>()?;
        Ok(Person { name, born, tags })
    }
}

fn main() {
    let ada = Person { name: "Ada".into(), born: 1815, tags: vec!["math".into()] };
    let bytes = ada.encode();
    println!("{} bytes: {:?}", bytes.len(), &bytes[..12]);
    println!("round trip: {}", Person::decode(&bytes) == Ok(ada));
    println!("cut short: {:?}", Person::decode(&bytes[..10]));
    let mut newer = bytes.clone();
    newer[0] = 2;
    println!("newer file: {:?}", Person::decode(&newer));
    let evil = [1, 0xff, 0xff, 0xff, 0xff, b'A'];
    println!("huge length: {:?}", Person::decode(&evil));
}
```

```output for=length-prefixed-binary
24 bytes: [1, 3, 0, 0, 0, 65, 100, 97, 23, 7, 0, 0]
round trip: true
cut short: Err("needs 4 bytes at byte 8, has 2")
newer file: Err("unknown version 2")
huge length: Err("needs 4294967295 bytes at byte 5, has 1")
```

- Verdict: library. The standard library has no serialization framework; the serde crate is the common choice, with `#[derive(Serialize, Deserialize)]` and a separate crate for each format, such as serde_json. The example writes a binary format by hand with a version byte and length prefixes, and the decoder returns an error, not a panic, for short input, an unknown version or a length larger than the input.
