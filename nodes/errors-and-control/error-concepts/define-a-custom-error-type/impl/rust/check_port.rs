// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/define-a-custom-error-type/.

use std::error::Error;
use std::fmt;

/// Why a text is not a valid TCP port number.
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum Reason {
    Empty,
    NotANumber,
    OutOfRange,
}

/// Reports that a text is not a valid TCP port number.
#[derive(Debug, Clone, PartialEq, Eq)]
pub struct PortError {
    pub text: String,
    pub reason: Reason,
}

impl fmt::Display for Reason {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        f.write_str(match self {
            Reason::Empty => "empty",
            Reason::NotANumber => "not a number",
            Reason::OutOfRange => "out of range",
        })
    }
}

impl fmt::Display for PortError {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(f, "invalid port '{}': {}", self.text, self.reason)
    }
}

// Debug and Display are enough; source() keeps its default, None.
impl Error for PortError {}

/// Returns the port number in text, or a PortError.
pub fn parse_port(text: &str) -> Result<u16, PortError> {
    let fail = |reason| PortError { text: text.to_string(), reason };
    if text.is_empty() {
        return Err(fail(Reason::Empty));
    }
    if !text.bytes().all(|b| b.is_ascii_digit()) {
        return Err(fail(Reason::NotANumber));
    }
    let mut port: u32 = 0;
    for b in text.bytes() {
        port = port * 10 + u32::from(b - b'0');
        if port > 65535 {
            return Err(fail(Reason::OutOfRange));
        }
    }
    if port == 0 {
        return Err(fail(Reason::OutOfRange));
    }
    Ok(port as u16)
}

/// Returns "port N", or the message of the PortError.
pub fn check_port(text: &str) -> String {
    match parse_port(text) {
        Ok(port) => format!("port {port}"),
        Err(e) => e.to_string(),
    }
}
