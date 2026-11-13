// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/set-file-permissions/.

use std::fs;
use std::io::ErrorKind;
use std::os::unix::fs::PermissionsExt;
use std::path::PathBuf;

use algo_set_file_permissions::set_mode;

/// Returns a new, empty directory for one test.
fn new_dir(name: &str) -> PathBuf {
    let dir = std::env::temp_dir().join(format!("set-mode-{}-{name}", std::process::id()));
    let _ = fs::remove_dir_all(&dir);
    fs::create_dir_all(&dir).unwrap();
    dir
}

fn new_file(dir: &PathBuf, name: &str) -> PathBuf {
    let p = dir.join(name);
    fs::write(&p, "alpha\n").unwrap();
    fs::set_permissions(&p, fs::Permissions::from_mode(0o644)).unwrap();
    p
}

fn mode(p: &PathBuf) -> u32 {
    fs::metadata(p).unwrap().permissions().mode() & 0o7777
}

#[test]
fn cases() {
    for (name, rel, m, result) in [
        ("owner-read-write-group-read", "a.txt", "640", 0o640),
        ("owner-only", "a.txt", "600", 0o600),
        ("executable", "a.txt", "755", 0o755),
        ("same-mode", "a.txt", "644", 0o644),
        ("no-permissions", "a.txt", "000", 0),
        ("leading-zero", "a.txt", "0640", 0o640),
        ("set-user-id", "a.txt", "4755", 0o4755),
        ("name-with-spaces", "my notes.txt", "640", 0o640),
        ("non-ascii-name", "café 日本.txt", "640", 0o640),
    ] {
        let dir = new_dir(name);
        let p = new_file(&dir, rel);
        assert_eq!(set_mode(&p, m).unwrap(), result, "{name}");
        assert_eq!(mode(&p), result, "{name}");
        fs::remove_dir_all(&dir).unwrap();
    }
}

#[test]
fn invalid_modes() {
    let dir = new_dir("invalid");
    let p = new_file(&dir, "a.txt");
    for m in ["648", "64", "10640", "u+x", "", "640\n", "+640"] {
        assert_eq!(set_mode(&p, m).unwrap_err().kind(), ErrorKind::InvalidInput, "{m:?}");
    }
    assert_eq!(mode(&p), 0o644);
    fs::remove_dir_all(&dir).unwrap();
}

#[test]
fn missing_file() {
    let dir = new_dir("missing");
    assert_eq!(set_mode(dir.join("missing.txt"), "640").unwrap_err().kind(), ErrorKind::NotFound);
    fs::remove_dir_all(&dir).unwrap();
}

#[test]
fn symbolic_link_changes_target() {
    let dir = new_dir("link");
    let target = new_file(&dir, "a.txt");
    std::os::unix::fs::symlink("a.txt", dir.join("link")).unwrap();
    assert_eq!(set_mode(dir.join("link"), "600").unwrap(), 0o600);
    assert_eq!(mode(&target), 0o600);
    fs::remove_dir_all(&dir).unwrap();
}

#[test]
fn directory() {
    let dir = new_dir("dir");
    let d = dir.join("dir");
    fs::create_dir(&d).unwrap();
    assert_eq!(set_mode(&d, "700").unwrap(), 0o700);
    fs::remove_dir_all(&dir).unwrap();
}
