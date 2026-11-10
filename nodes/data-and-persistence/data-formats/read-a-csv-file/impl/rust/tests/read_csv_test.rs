// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/read-a-csv-file/.

use std::collections::HashMap;
use std::fs;
use std::io::ErrorKind;
use std::path::PathBuf;

use algo_read_a_csv_file::{CsvError, read_csv};

/// A file in the temporary directory that is removed when the value is dropped.
struct Input(PathBuf);

impl Input {
    fn new(name: &str, data: &str) -> Input {
        let path = std::env::temp_dir().join(format!("read-csv-{}-{name}.csv", std::process::id()));
        fs::write(&path, data).unwrap();
        Input(path)
    }
}

impl Drop for Input {
    fn drop(&mut self) {
        let _ = fs::remove_file(&self.0);
    }
}

fn recs(rows: &[&[(&str, &str)]]) -> Vec<HashMap<String, String>> {
    rows.iter()
        .map(|row| row.iter().map(|&(k, v)| (k.to_string(), v.to_string())).collect())
        .collect()
}

#[test]
fn cases() {
    let ada_alan: &[&[(&str, &str)]] = &[&[("name", "Ada"), ("age", "36")], &[("name", "Alan"), ("age", "41")]];
    let cases: &[(&str, &str, &[&[(&str, &str)]])] = &[
        ("simple", "name,age\nAda,36\nAlan,41\n", ada_alan),
        ("crlf", "name,age\r\nAda,36\r\nAlan,41\r\n", ada_alan),
        ("no-final-newline", "name,age\nAda,36\nAlan,41", ada_alan),
        ("quoted-comma", "name,born\n\"Lovelace, Ada\",1815\n", &[&[("name", "Lovelace, Ada"), ("born", "1815")]]),
        ("escaped-quote", "id,quote\n1,\"She said \"\"hi\"\".\"\n", &[&[("id", "1"), ("quote", "She said \"hi\".")]]),
        ("quoted-header", "\"first name\",\"last name\"\nAda,Lovelace\n", &[&[("first name", "Ada"), ("last name", "Lovelace")]]),
        ("newline-in-field", "id,address\n1,\"12 High St\nLeeds\"\n", &[&[("id", "1"), ("address", "12 High St\nLeeds")]]),
        ("crlf-in-field", "id,address\r\n1,\"12 High St\r\nLeeds\"\r\n", &[&[("id", "1"), ("address", "12 High St\nLeeds")]]),
        ("blank-line-in-field", "id,note\n1,\"one\n\ntwo\"\n", &[&[("id", "1"), ("note", "one\n\ntwo")]]),
        ("empty-last-line", "a,b\n1,2\n\n", &[&[("a", "1"), ("b", "2")]]),
        ("blank-lines", "a,b\n\n1,2\n\n\n3,4\n", &[&[("a", "1"), ("b", "2")], &[("a", "3"), ("b", "4")]]),
        ("empty-fields", "a,b,c\n,,\n", &[&[("a", ""), ("b", ""), ("c", "")]]),
        ("quoted-empty", "name\n\"\"\n", &[&[("name", "")]]),
        ("spaces", "a,b\n x , y \n", &[&[("a", " x "), ("b", " y ")]]),
        ("header-only", "a,b\n", &[]),
        ("empty-file", "", &[]),
        ("bom", "\u{FEFF}name,age\nAda,36\n", &[&[("name", "Ada"), ("age", "36")]]),
        (
            "non-ascii",
            "city,country\nZürich,Schweiz\n東京,日本\n",
            &[&[("city", "Zürich"), ("country", "Schweiz")], &[("city", "東京"), ("country", "日本")]],
        ),
    ];
    for &(name, data, want) in cases {
        let input = Input::new(name, data);
        assert_eq!(read_csv(&input.0).unwrap(), recs(want), "{name}");
    }
}

#[test]
fn invalid() {
    let cases = [
        ("too-few-fields", "a,b\n1\n", "field-count"),
        ("too-many-fields", "a,b\n1,2,3\n", "field-count"),
        ("unclosed-quote", "a,b\n1,\"two\n", "bad-quote"),
        ("text-after-quote", "a,b\n1,\"two\"x\n", "bad-quote"),
    ];
    for (name, data, kind) in cases {
        let input = Input::new(name, data);
        let got = match read_csv(&input.0) {
            Err(CsvError::FieldCount { .. }) => "field-count",
            Err(CsvError::BadQuote(_)) => "bad-quote",
            other => panic!("{name}: {other:?}"),
        };
        assert_eq!(got, kind, "{name}");
    }
}

#[test]
fn missing() {
    let path = std::env::temp_dir().join(format!("read-csv-{}-missing.csv", std::process::id()));
    match read_csv(path) {
        Err(CsvError::Io(err)) => assert_eq!(err.kind(), ErrorKind::NotFound),
        other => panic!("{other:?}"),
    }
}
