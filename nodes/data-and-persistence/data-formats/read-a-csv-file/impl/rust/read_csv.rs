// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/read-a-csv-file/.

//! Reads a CSV file of RFC 4180 with a header line. The standard library has no CSV reader, so this file has a small parser.

use std::collections::HashMap;
use std::fmt;
use std::fs;
use std::io;
use std::path::Path;

/// The error for a file that cannot be read or is not valid CSV.
#[derive(Debug)]
pub enum CsvError {
    /// The file cannot be opened or is not UTF-8.
    Io(io::Error),
    /// A record has more or fewer fields than the header.
    FieldCount { record: usize, got: usize, want: usize },
    /// A quoted field has no closing quote, or has text after it.
    BadQuote(&'static str),
}

impl fmt::Display for CsvError {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        match self {
            CsvError::Io(err) => write!(f, "{err}"),
            CsvError::FieldCount { record, got, want } => {
                write!(f, "record {record} has {got} fields, want {want}")
            }
            CsvError::BadQuote(why) => write!(f, "{why}"),
        }
    }
}

impl std::error::Error for CsvError {}

impl From<io::Error> for CsvError {
    fn from(err: io::Error) -> Self {
        CsvError::Io(err)
    }
}

/// Returns the length of the line break at i: 1 for LF, 2 for CRLF, 0 for none.
fn line_break(b: &[u8], i: usize) -> usize {
    match (b.get(i), b.get(i + 1)) {
        (Some(b'\n'), _) => 1,
        (Some(b'\r'), Some(b'\n')) => 2,
        _ => 0,
    }
}

/// Splits RFC 4180 text into records of fields. An empty line gives no record.
pub fn parse_csv(text: &str) -> Result<Vec<Vec<String>>, CsvError> {
    // The parser splits only at ASCII bytes, so each field stays valid UTF-8.
    let b = text.as_bytes();
    let mut records = Vec::new();
    let mut i = 0;
    while i < b.len() {
        let empty = line_break(b, i);
        if empty > 0 {
            i += empty;
            continue;
        }
        let mut record = Vec::new();
        loop {
            let mut field = Vec::new();
            if b.get(i) == Some(&b'"') {
                i += 1;
                loop {
                    match b.get(i) {
                        None => return Err(CsvError::BadQuote("a quoted field has no closing quote")),
                        Some(b'"') if b.get(i + 1) == Some(&b'"') => {
                            field.push(b'"');
                            i += 2;
                        }
                        Some(b'"') => break,
                        Some(_) if line_break(b, i) == 2 => {
                            field.push(b'\n'); // CRLF inside quotes becomes LF.
                            i += 2;
                        }
                        Some(&c) => {
                            field.push(c);
                            i += 1;
                        }
                    }
                }
                i += 1; // the closing quote
                if i < b.len() && b[i] != b',' && line_break(b, i) == 0 {
                    return Err(CsvError::BadQuote("text after the closing quote of a field"));
                }
            } else {
                while i < b.len() && b[i] != b',' && line_break(b, i) == 0 {
                    field.push(b[i]);
                    i += 1;
                }
            }
            record.push(String::from_utf8(field).expect("a field ends at an ASCII byte"));
            if b.get(i) != Some(&b',') {
                break;
            }
            i += 1;
        }
        i += line_break(b, i);
        records.push(record);
    }
    Ok(records)
}

/// Returns one map for each record after the header, from each name to its field.
pub fn read_csv(path: impl AsRef<Path>) -> Result<Vec<HashMap<String, String>>, CsvError> {
    // read_to_string fails with InvalidData if the file is not UTF-8.
    let text = fs::read_to_string(path)?;
    let text = text.strip_prefix('\u{FEFF}').unwrap_or(&text);
    let mut rows = parse_csv(text)?.into_iter();
    let Some(header) = rows.next() else {
        return Ok(Vec::new());
    };
    rows.enumerate()
        .map(|(n, row)| {
            if row.len() != header.len() {
                return Err(CsvError::FieldCount { record: n + 1, got: row.len(), want: header.len() });
            }
            Ok(header.iter().cloned().zip(row).collect())
        })
        .collect()
}
