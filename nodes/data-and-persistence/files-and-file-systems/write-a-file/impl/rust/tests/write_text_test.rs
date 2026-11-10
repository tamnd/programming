// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/write-a-file/.

use algo_write_a_file::{Mode, write_text};
use std::fs;
use std::io::ErrorKind;
use std::path::PathBuf;
use std::sync::atomic::{AtomicU64, Ordering};

// A new empty directory for one test, removed when it goes out of scope.
struct TempDir(PathBuf);

impl TempDir {
    fn new() -> TempDir {
        static COUNT: AtomicU64 = AtomicU64::new(0);
        let n = COUNT.fetch_add(1, Ordering::Relaxed);
        let dir = std::env::temp_dir().join(format!("write-text-{}-{n}", std::process::id()));
        fs::create_dir(&dir).unwrap();
        TempDir(dir)
    }

    fn names(&self) -> Vec<String> {
        let mut out: Vec<String> = fs::read_dir(&self.0)
            .unwrap()
            .map(|e| e.unwrap().file_name().into_string().unwrap())
            .collect();
        out.sort();
        out
    }
}

impl Drop for TempDir {
    fn drop(&mut self) {
        let _ = fs::remove_dir_all(&self.0);
    }
}

const NON_ASCII: &str = "naïve café\n日本語\n🙂\n";

const CASES: &[(&str, &str, Mode, Option<&str>, &str)] = &[
    ("new-file", "alpha\nbeta\n", Mode::Truncate, None, "alpha\nbeta\n"),
    ("append-existing", "two\n", Mode::Append, Some("one\n"), "one\ntwo\n"),
    ("atomic-replace", "new\n", Mode::Atomic, Some("old\n"), "new\n"),
    ("truncate-longer", "short\n", Mode::Truncate, Some("a much longer old line\n"), "short\n"),
    ("empty-truncate", "", Mode::Truncate, Some("old\n"), ""),
    ("empty-new", "", Mode::Truncate, None, ""),
    ("append-missing", "first\n", Mode::Append, None, "first\n"),
    ("append-no-newline", "two", Mode::Append, Some("one"), "onetwo"),
    ("non-ascii", NON_ASCII, Mode::Truncate, None, NON_ASCII),
    ("crlf", "one\r\ntwo\r\n", Mode::Truncate, None, "one\r\ntwo\r\n"),
    ("mixed-endings", "lf\ncrlf\r\ncr\rend", Mode::Truncate, None, "lf\ncrlf\r\ncr\rend"),
    ("backslashes", "C:\\temp\\new\n", Mode::Truncate, None, "C:\\temp\\new\n"),
    ("format-chars", "-n 100% %s\n", Mode::Truncate, None, "-n 100% %s\n"),
    ("atomic-new", "new\n", Mode::Atomic, None, "new\n"),
    ("atomic-empty", "", Mode::Atomic, Some("old\n"), ""),
    ("atomic-non-ascii", NON_ASCII, Mode::Atomic, Some("old\n"), NON_ASCII),
];

#[test]
fn cases() {
    for &(name, text, mode, before, want) in CASES {
        let dir = TempDir::new();
        let path = dir.0.join("out.txt");
        if let Some(before) = before {
            fs::write(&path, before).unwrap();
        }
        write_text(&path, text, mode).unwrap_or_else(|e| panic!("{name}: {e}"));
        assert_eq!(fs::read(&path).unwrap(), want.as_bytes(), "{name}");
        assert_eq!(dir.names(), ["out.txt"], "{name}");
    }
}

#[test]
fn missing_dir() {
    for mode in [Mode::Truncate, Mode::Append, Mode::Atomic] {
        let dir = TempDir::new();
        let err = write_text(&dir.0.join("missing/out.txt"), "x\n", mode).unwrap_err();
        assert_eq!(err.kind(), ErrorKind::NotFound, "missing-dir-{mode:?}");
        assert!(dir.names().is_empty(), "missing-dir-{mode:?}");
    }
}

#[test]
fn atomic_onto_directory() {
    let dir = TempDir::new();
    let path = dir.0.join("out.txt");
    fs::create_dir(&path).unwrap();
    assert!(write_text(&path, "x\n", Mode::Atomic).is_err());
    assert!(path.is_dir());
    assert_eq!(dir.names(), ["out.txt"]);
}
