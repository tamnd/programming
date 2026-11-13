// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/write-a-csv-file/.

//! Writes records as a CSV file of RFC 4180. The standard library has no CSV writer, so this file has a small one.

use std::fmt;
use std::fs;
use std::io;
use std::path::Path;

/// The error for records that cannot be written.
#[derive(Debug)]
pub enum CsvError {
    /// The file cannot be written.
    Io(io::Error),
    /// A record has more or fewer fields than the first record (the header).
    FieldCount { record: usize, got: usize, want: usize },
}

impl fmt::Display for CsvError {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        match self {
            CsvError::Io(e) => write!(f, "{e}"),
            CsvError::FieldCount { record, got, want } => {
                write!(f, "record {record} has {got} fields, the header has {want}")
            }
        }
    }
}

impl std::error::Error for CsvError {}

fn needs_quotes(field: &str, alone: bool) -> bool {
    // A record with one empty field needs quotes, or the line is empty and a reader skips it.
    (alone && field.is_empty()) || field.contains([',', '"', '\r', '\n'])
}

/// Returns the CSV text of the records. The first record is the header.
pub fn format_csv<S: AsRef<str>>(records: &[Vec<S>]) -> Result<String, CsvError> {
    let mut out = String::new();
    for (n, record) in records.iter().enumerate() {
        if record.len() != records[0].len() {
            return Err(CsvError::FieldCount { record: n, got: record.len(), want: records[0].len() });
        }
        for (i, field) in record.iter().enumerate() {
            let field = field.as_ref();
            if i > 0 {
                out.push(',');
            }
            if needs_quotes(field, record.len() == 1) {
                out.push('"');
                out.push_str(&field.replace('"', "\"\""));
                out.push('"');
            } else {
                out.push_str(field);
            }
        }
        out.push_str("\r\n");
    }
    Ok(out)
}

/// Writes the records to the file at path as UTF-8, with no byte order mark.
pub fn write_csv<S: AsRef<str>>(path: &Path, records: &[Vec<S>]) -> Result<(), CsvError> {
    let text = format_csv(records)?;
    fs::write(path, text).map_err(CsvError::Io)
}
