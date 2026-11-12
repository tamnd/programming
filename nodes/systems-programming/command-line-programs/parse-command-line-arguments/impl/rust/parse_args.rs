// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/parse-command-line-arguments/.

//! Parses the arguments of the tool "count [-v] [-n N] [--] [FILE...]".

use std::fmt;

/// The result of [`parse_args`].
#[derive(Debug, PartialEq, Eq)]
pub struct Args {
    pub verbose: bool,
    pub limit: u32,
    pub files: Vec<String>,
}

/// A usage error of the command line.
#[derive(Debug, PartialEq, Eq)]
pub enum UsageError {
    UnknownOption(String),
    MissingValue(String),
    BadValue(String),
}

impl fmt::Display for UsageError {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        match self {
            UsageError::UnknownOption(o) => write!(f, "unknown option {o}"),
            UsageError::MissingValue(o) => write!(f, "option {o} needs a value"),
            UsageError::BadValue(v) => write!(f, "limit {v:?} is not a decimal number"),
        }
    }
}

impl std::error::Error for UsageError {}

/// Parses the arguments without the program name, for example
/// `parse_args(std::env::args().skip(1))`.
pub fn parse_args<I>(args: I) -> Result<Args, UsageError>
where
    I: IntoIterator<Item = String>,
{
    let mut args = args.into_iter();
    let mut out = Args { verbose: false, limit: 10, files: Vec::new() };
    while let Some(arg) = args.next() {
        match arg.as_str() {
            "--" => break,
            "-v" | "--verbose" => out.verbose = true,
            "-n" | "--limit" => {
                let value = args.next().ok_or_else(|| UsageError::MissingValue(arg.clone()))?;
                out.limit = decimal(&value)?;
            }
            _ => {
                if let Some(value) = arg.strip_prefix("--limit=") {
                    out.limit = decimal(value)?;
                } else if arg.starts_with('-') && arg != "-" {
                    return Err(UsageError::UnknownOption(arg));
                } else {
                    // The first operand ends the options.
                    out.files.push(arg);
                    break;
                }
            }
        }
    }
    out.files.extend(args);
    Ok(out)
}

/// Converts a string of ASCII digits. `str::parse` alone also accepts a `+`.
fn decimal(s: &str) -> Result<u32, UsageError> {
    if s.is_empty() || !s.bytes().all(|b| b.is_ascii_digit()) {
        return Err(UsageError::BadValue(s.to_string()));
    }
    s.parse().map_err(|_| UsageError::BadValue(s.to_string()))
}
