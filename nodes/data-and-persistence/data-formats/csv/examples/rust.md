```rust run id=deserialize deps=deps/rust/Cargo.lock
use serde::Deserialize;

#[derive(Deserialize)]
struct Payment {
    id: u32,
    name: String,
    amount: f64,
}

fn main() {
    let text = "id,name,amount\r\n7,\"Smith, Ann\",12.50\r\n8,Bo,3\r\n9,Cy,ten\r\n";
    let mut reader = csv::Reader::from_reader(text.as_bytes());
    for row in reader.deserialize::<Payment>() {
        match row {
            Ok(p) => println!("{} {:?} {}", p.id, p.name, p.amount),
            Err(e) => println!("error: {}", e),
        }
    }

    let bom = "\u{feff}zip,city\n02134,Boston\n";
    let mut reader = csv::Reader::from_reader(bom.as_bytes());
    println!("{:?}", reader.headers().unwrap());
}
```

```output for=deserialize
7 "Smith, Ann" 12.5
8 "Bo" 3
error: CSV deserialize error: record 3 (line: 3, byte: 45): field 2: invalid float literal
StringRecord(["zip", "city"])
```

```rust run id=ragged-write deps=deps/rust/Cargo.lock
fn main() {
    let ragged = "a,b\n1,2,3\n";
    let mut reader = csv::Reader::from_reader(ragged.as_bytes());
    for row in reader.records() {
        match row {
            Ok(r) => println!("{:?}", r),
            Err(e) => println!("error: {}", e),
        }
    }
    let mut reader = csv::ReaderBuilder::new()
        .flexible(true)
        .from_reader(ragged.as_bytes());
    for row in reader.records() {
        println!("{:?}", row.unwrap());
    }

    let mut writer = csv::Writer::from_writer(vec![]);
    writer.write_record(["7", "Smith, Ann", "said \"hi\""]).unwrap();
    writer.write_record(["8", " Bo", ""]).unwrap();
    println!("{:?}", String::from_utf8(writer.into_inner().unwrap()).unwrap());

    let mut writer = csv::WriterBuilder::new()
        .terminator(csv::Terminator::CRLF)
        .from_writer(vec![]);
    writer.write_record(["a", "b"]).unwrap();
    println!("{:?}", String::from_utf8(writer.into_inner().unwrap()).unwrap());
}
```

```output for=ragged-write
error: CSV error: record 1 (line: 2, byte: 4): found record with 3 fields, but the previous record has 2 fields
StringRecord(["1", "2", "3"])
"7,\"Smith, Ann\",\"said \"\"hi\"\"\"\n8, Bo,\n"
"a,b\r\n"
```

- Verdict: library. The Rust standard library has no CSV. The examples use the `csv` crate 1.4.0 with `serde` 1.0.229 and the `derive` feature.
- `Reader` treats the first row as a header by default, and `deserialize` maps each row to a struct by the header names. A field that does not parse as its type gives an error for that row only.
- A row with a different field count is an error (`UnequalLengths`) unless the reader is `flexible`.
- `Writer` ends lines with `\n` by default. `Terminator::CRLF` gives the line end of RFC 4180. In this run, the reader removed the BOM from the header.
