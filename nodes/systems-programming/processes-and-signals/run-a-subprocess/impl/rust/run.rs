// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/run-a-subprocess/.

use std::io;
use std::process::Command;

/// Runs `argv` with no shell and returns the exit status and the text of the
/// two output streams. A missing program gives an error of the kind
/// `NotFound`. A status that is not 0 is a result, not an error.
pub fn run(argv: &[&str]) -> io::Result<(i32, String, String)> {
    let (program, args) = argv
        .split_first()
        .ok_or_else(|| io::Error::new(io::ErrorKind::InvalidInput, "argv is empty"))?;
    // output() reads both pipes to the end and waits for the child. The
    // standard input of the child is closed at once.
    let output = Command::new(program).args(args).output()?;
    // On Unix, code() is None when a signal stopped the child.
    let status = output
        .status
        .code()
        .ok_or_else(|| io::Error::other(format!("{program}: {}", output.status)))?;
    let text = |bytes: Vec<u8>| {
        String::from_utf8(bytes).map_err(|e| io::Error::new(io::ErrorKind::InvalidData, e))
    };
    Ok((status, text(output.stdout)?, text(output.stderr)?))
}
