// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/log-an-error/.

use std::io::{self, Write};

/// Returns the port number in text, or the message of the error.
pub fn parse_port(text: &str) -> Result<u16, String> {
    if text.is_empty() || !text.bytes().all(|b| b.is_ascii_digit()) {
        return Err(format!("not a number: '{text}'"));
    }
    let mut port: u32 = 0;
    for b in text.bytes() {
        port = port * 10 + u32::from(b - b'0');
        if port > 65535 {
            return Err(format!("out of range: {text}"));
        }
    }
    if port == 0 {
        return Err(format!("out of range: {text}"));
    }
    Ok(port as u16)
}

/// Puts value in double quotes and escapes the characters that could end
/// the line or the value.
pub fn quote(value: &str) -> String {
    let mut s = String::from("\"");
    for c in value.chars() {
        match c {
            '\\' => s.push_str("\\\\"),
            '"' => s.push_str("\\\""),
            '\n' => s.push_str("\\n"),
            '\r' => s.push_str("\\r"),
            '\t' => s.push_str("\\t"),
            _ => s.push(c),
        }
    }
    s.push('"');
    s
}

/// Returns the good ports. Writes a log line to log for each bad text, then
/// a summary. An error of the writer goes to the caller.
pub fn read_ports(texts: &[&str], log: &mut dyn Write) -> io::Result<Vec<u16>> {
    let mut ports = Vec::new();
    for (i, text) in texts.iter().enumerate() {
        match parse_port(text) {
            Ok(port) => ports.push(port),
            Err(e) => writeln!(log, "level=ERROR msg=\"read port failed\" line={} err={}", i + 1, quote(&e))?,
        }
    }
    writeln!(log, "level=INFO msg=\"ports read\" ok={} total={}", ports.len(), texts.len())?;
    Ok(ports)
}
