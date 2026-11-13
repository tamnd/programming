// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/lock-a-file/.

use std::fs;
use std::io::ErrorKind;
use std::path::PathBuf;

use algo_lock_a_file::{try_lock, try_lock_file};

struct Tree(PathBuf);

impl Tree {
    fn new(name: &str) -> Tree {
        let dir = std::env::temp_dir().join(format!("lock-file-{}-{name}", std::process::id()));
        let _ = fs::remove_dir_all(&dir);
        fs::create_dir_all(&dir).unwrap();
        fs::write(dir.join("data.txt"), "alpha\n").unwrap();
        Tree(dir)
    }

    fn data(&self) -> PathBuf {
        self.0.join("data.txt")
    }
}

impl Drop for Tree {
    fn drop(&mut self) {
        let _ = fs::remove_dir_all(&self.0);
    }
}

const CASES: &[(&str, &str, &str, bool)] = &[
    ("no-lock-exclusive", "none", "exclusive", true),
    ("no-lock-shared", "none", "shared", true),
    ("exclusive-with-exclusive", "exclusive", "exclusive", false),
    ("shared-with-shared", "shared", "shared", true),
    ("shared-with-exclusive", "shared", "exclusive", false),
    ("exclusive-with-shared", "exclusive", "shared", false),
];

#[test]
fn cases() {
    for (name, held, want, expect) in CASES {
        let t = Tree::new(name);
        assert_eq!(try_lock(&t.data(), held, want).unwrap(), *expect, "{name}");
        assert_eq!(fs::read_to_string(t.data()).unwrap(), "alpha\n", "{name}");
    }
}

#[test]
fn drop_frees_the_lock() {
    let t = Tree::new("drop");
    let first = try_lock_file(&t.data(), true).unwrap();
    assert!(first.is_some());
    assert!(try_lock_file(&t.data(), true).unwrap().is_none());
    drop(first);
    assert!(try_lock_file(&t.data(), true).unwrap().is_some());
}

#[test]
fn missing_file() {
    let t = Tree::new("missing");
    let err = try_lock_file(&t.0.join("missing.txt"), true).unwrap_err();
    assert_eq!(err.kind(), ErrorKind::NotFound);
    assert!(!t.0.join("missing.txt").exists());
}
