// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/list-files-in-a-directory/.

use std::fs;
use std::io;
use std::path::Path;

/// Returns the relative paths of the entries under `root`, sorted by code
/// point. A directory has a `/` at the end. A symbolic link is listed, but it
/// is never followed.
///
/// # Errors
///
/// An error of kind `NotFound` if `root` does not exist, `NotADirectory` if it
/// is not a directory, and `InvalidData` for a name that is not UTF-8.
pub fn list_entries(root: &Path, recursive: bool) -> io::Result<Vec<String>> {
    let mut out = Vec::new();
    walk(root, "", recursive, &mut out)?;
    // read_dir gives the entries in an order that depends on the platform.
    out.sort();
    Ok(out)
}

fn walk(dir: &Path, prefix: &str, recursive: bool, out: &mut Vec<String>) -> io::Result<()> {
    for entry in fs::read_dir(dir)? {
        let entry = entry?;
        let name = entry
            .file_name()
            .into_string()
            .map_err(|_| io::Error::new(io::ErrorKind::InvalidData, "a name is not UTF-8"))?;
        // DirEntry::file_type does not follow a symbolic link.
        if entry.file_type()?.is_dir() {
            let path = format!("{prefix}{name}/");
            out.push(path.clone());
            if recursive {
                walk(&entry.path(), &path, recursive, out)?;
            }
        } else {
            out.push(format!("{prefix}{name}"));
        }
    }
    Ok(())
}
