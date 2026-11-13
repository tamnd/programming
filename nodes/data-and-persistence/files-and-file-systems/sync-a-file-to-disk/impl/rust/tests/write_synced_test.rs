// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/sync-a-file-to-disk/.

use std::fs;
use std::io::ErrorKind;
use std::path::PathBuf;

use algo_sync_a_file_to_disk::{sync_directory, write_synced};

struct Tree(PathBuf);

impl Tree {
    fn new(name: &str) -> Tree {
        let dir = std::env::temp_dir().join(format!("sync-file-{}-{name}", std::process::id()));
        let _ = fs::remove_dir_all(&dir);
        fs::create_dir_all(&dir).unwrap();
        Tree(dir)
    }

    fn names(&self) -> Vec<String> {
        let mut v: Vec<String> =
            fs::read_dir(&self.0).unwrap().map(|e| e.unwrap().file_name().into_string().unwrap()).collect();
        v.sort();
        v
    }
}

impl Drop for Tree {
    fn drop(&mut self) {
        let _ = fs::remove_dir_all(&self.0);
    }
}

const CASES: &[(&str, &str, Option<&str>)] = &[
    ("new-file", "alpha\nbeta\n", None),
    ("replace-longer", "short\n", Some("a much longer old line\n")),
    ("many-lines", "one\ntwo\nthree\nfour\n", None),
    ("empty-replace", "", Some("old\n")),
    ("empty-new", "", None),
    ("no-final-newline", "last line", None),
    ("non-ascii", "naïve café\n日本語\n🙂\n", None),
    ("crlf", "one\r\ntwo\r\n", None),
];

#[test]
fn cases() {
    for (name, text, before) in CASES {
        let t = Tree::new(name);
        let path = t.0.join("out.txt");
        if let Some(b) = before {
            fs::write(&path, b).unwrap();
        }
        write_synced(&path, text).unwrap();
        assert_eq!(fs::read(&path).unwrap(), text.as_bytes(), "{name}");
        assert_eq!(t.names(), vec!["out.txt"], "{name}");
    }
}

#[test]
fn syncs_a_directory() {
    let t = Tree::new("dir");
    sync_directory(&t.0).unwrap();
}

#[test]
fn missing_directory() {
    let t = Tree::new("missing");
    let err = write_synced(&t.0.join("missing").join("out.txt"), "x\n").unwrap_err();
    assert_eq!(err.kind(), ErrorKind::NotFound);
    assert!(t.names().is_empty());
}

#[test]
fn onto_directory() {
    let t = Tree::new("onto");
    let path = t.0.join("out.txt");
    fs::create_dir(&path).unwrap();
    let err = write_synced(&path, "x\n").unwrap_err();
    assert_eq!(err.kind(), ErrorKind::IsADirectory);
    assert!(path.is_dir());
}
