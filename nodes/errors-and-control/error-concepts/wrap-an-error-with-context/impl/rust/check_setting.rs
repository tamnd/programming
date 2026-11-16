// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/wrap-an-error-with-context/.

use std::error::Error;
use std::fmt;

/// The error of parse_port.
#[derive(Debug, Clone, PartialEq, Eq)]
pub enum ParseError {
    NotANumber(String),
    OutOfRange(String),
}

impl fmt::Display for ParseError {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        match self {
            ParseError::NotANumber(text) => write!(f, "not a number: '{text}'"),
            ParseError::OutOfRange(text) => write!(f, "out of range: {text}"),
        }
    }
}

impl Error for ParseError {}

/// A setting is bad. The cause is the error of parse_port.
#[derive(Debug)]
pub struct ConfigError {
    pub context: String,
    pub cause: ParseError,
}

// Display shows only the context. The cause comes from source(), so that a
// program that prints the chain does not print the cause two times.
impl fmt::Display for ConfigError {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        f.write_str(&self.context)
    }
}

impl Error for ConfigError {
    fn source(&self) -> Option<&(dyn Error + 'static)> {
        Some(&self.cause)
    }
}

/// Returns the port number in text.
pub fn parse_port(text: &str) -> Result<u16, ParseError> {
    if text.is_empty() || !text.bytes().all(|b| b.is_ascii_digit()) {
        return Err(ParseError::NotANumber(text.to_string()));
    }
    let mut port: u32 = 0;
    for b in text.bytes() {
        port = port * 10 + u32::from(b - b'0');
        if port > 65535 {
            return Err(ParseError::OutOfRange(text.to_string()));
        }
    }
    if port == 0 {
        return Err(ParseError::OutOfRange(text.to_string()));
    }
    Ok(port as u16)
}

/// Returns the port number, or a ConfigError that keeps the ParseError.
pub fn read_port(place: &str, text: &str) -> Result<u16, ConfigError> {
    parse_port(text).map_err(|cause| ConfigError {
        context: format!("{place}: read port"),
        cause,
    })
}

/// Joins the message of e and the messages of its sources.
fn chain(e: &dyn Error) -> String {
    let mut message = e.to_string();
    let mut cur = e.source();
    while let Some(s) = cur {
        message.push_str(": ");
        message.push_str(&s.to_string());
        cur = s.source();
    }
    message
}

/// Returns "port N", or "error: " and the chain of messages.
pub fn check_setting(place: &str, text: &str) -> String {
    match read_port(place, text) {
        Ok(port) => format!("port {port}"),
        Err(e) => format!("error: {}", chain(&e)),
    }
}
