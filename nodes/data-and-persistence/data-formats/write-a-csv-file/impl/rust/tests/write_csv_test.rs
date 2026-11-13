// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/write-a-csv-file/.

use std::fs;

use algo_write_a_csv_file::{CsvError, format_csv, write_csv};

const CASES: &[(&str, &[&[&str]], &str)] = &[
    ("simple", &[&["name", "age"], &["Ada", "36"], &["Alan", "41"]], "name,age\r\nAda,36\r\nAlan,41\r\n"),
    ("comma", &[&["name", "born"], &["Lovelace, Ada", "1815"]], "name,born\r\n\"Lovelace, Ada\",1815\r\n"),
    ("double-quote", &[&["id", "quote"], &["1", "She said \"hi\"."]], "id,quote\r\n1,\"She said \"\"hi\"\".\"\r\n"),
    ("only-a-quote", &[&["q"], &["\""]], "q\r\n\"\"\"\"\r\n"),
    ("header-only", &[&["name", "age"]], "name,age\r\n"),
    ("newline-in-field", &[&["id", "address"], &["1", "12 High St\nLeeds"]], "id,address\r\n1,\"12 High St\nLeeds\"\r\n"),
    ("crlf-in-field", &[&["id", "address"], &["1", "12 High St\r\nLeeds"]], "id,address\r\n1,\"12 High St\r\nLeeds\"\r\n"),
    ("cr-in-field", &[&["id", "text"], &["1", "a\rb"]], "id,text\r\n1,\"a\rb\"\r\n"),
    ("empty-fields", &[&["a", "b", "c"], &["", "", ""]], "a,b,c\r\n,,\r\n"),
    ("one-empty-field", &[&["note"], &[""], &["x"]], "note\r\n\"\"\r\nx\r\n"),
    ("spaces", &[&["a", "b"], &[" x ", " y"]], "a,b\r\n x , y\r\n"),
    ("no-records", &[], ""),
    ("non-ascii", &[&["city"], &["Zürich"], &["東京"]], "city\r\nZürich\r\n東京\r\n"),
];

fn records(rows: &[&[&str]]) -> Vec<Vec<String>> {
    rows.iter().map(|r| r.iter().map(|f| f.to_string()).collect()).collect()
}

#[test]
fn format_and_write() {
    for (name, rows, want) in CASES {
        let recs = records(rows);
        assert_eq!(format_csv(&recs).unwrap(), *want, "{name}");
        let path = std::env::temp_dir().join(format!("write-csv-{}-{name}.csv", std::process::id()));
        write_csv(&path, &recs).unwrap();
        let data = fs::read(&path).unwrap();
        fs::remove_file(&path).unwrap();
        assert_eq!(data, want.as_bytes(), "{name}");
    }
}

#[test]
fn field_count() {
    for (name, rows) in [("too-many-fields", &[&["a", "b"][..], &["1", "2", "3"]]), ("too-few-fields", &[&["a", "b"][..], &["1"]])] {
        let recs = records(rows);
        assert!(matches!(format_csv(&recs), Err(CsvError::FieldCount { record: 1, .. })), "{name}");
        let path = std::env::temp_dir().join(format!("write-csv-{}-{name}.csv", std::process::id()));
        assert!(matches!(write_csv(&path, &recs), Err(CsvError::FieldCount { .. })), "{name}");
        assert!(!path.exists(), "{name}");
    }
}
