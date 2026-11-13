// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/create-a-temporary-file/.

use std::fs;
use std::io::ErrorKind;
use std::os::unix::fs::PermissionsExt;
use std::path::{Path, PathBuf};

use algo_create_a_temporary_file::{make_temp_file, use_temp_file};

struct Tree(PathBuf);

impl Tree {
    fn new(name: &str) -> Tree {
        let dir = std::env::temp_dir().join(format!("temp-file-{}-{name}", std::process::id()));
        let _ = fs::remove_dir_all(&dir);
        fs::create_dir_all(dir.join("sub")).unwrap();
        Tree(dir)
    }

    fn path(&self, rel: &str) -> PathBuf {
        self.0.join(rel)
    }
}

impl Drop for Tree {
    fn drop(&mut self) {
        let _ = fs::remove_dir_all(&self.0);
    }
}

fn names(dir: &Path) -> Vec<String> {
    let mut out: Vec<String> =
        fs::read_dir(dir).unwrap().map(|e| e.unwrap().file_name().into_string().unwrap()).collect();
    out.sort();
    out
}

#[test]
fn cases() {
    let cases = [
        ("one-line", ".", "tmp-", "alpha\n"),
        ("many-lines", ".", "report-", "alpha\nbeta\ngamma\n"),
        ("in-subdirectory", "sub", "tmp-", "alpha\n"),
        ("empty-prefix", ".", "", "alpha\n"),
        ("empty-data", ".", "tmp-", ""),
        ("non-ascii-data", ".", "tmp-", "café 日本\n"),
        ("prefix-with-spaces", ".", "my temp ", "alpha\n"),
        ("non-ascii-prefix", ".", "café-", "alpha\n"),
    ];
    for (name, rel, prefix, data) in cases {
        let tree = Tree::new(name);
        assert_eq!(use_temp_file(&tree.path(rel), prefix, data).unwrap(), data, "{name}");
        assert_eq!(names(&tree.0), ["sub"], "{name}");
        assert!(names(&tree.path("sub")).is_empty(), "{name}");
    }
}

#[test]
fn make_temp_file_name_and_mode() {
    for (i, prefix) in ["tmp-", "", "my temp ", "café-"].into_iter().enumerate() {
        let tree = Tree::new(&format!("make-{i}"));
        let path = make_temp_file(&tree.0, prefix).unwrap();
        assert_eq!(path.parent().unwrap(), tree.0);
        let name = path.file_name().unwrap().to_str().unwrap();
        assert!(name.starts_with(prefix) && name.len() >= prefix.len() + 6, "{name}");
        let meta = fs::symlink_metadata(&path).unwrap();
        assert!(meta.file_type().is_file());
        assert_eq!(meta.permissions().mode() & 0o7777, 0o600);
        assert_eq!(meta.len(), 0);
    }
}

#[test]
fn two_calls_give_two_files() {
    let tree = Tree::new("two");
    let a = make_temp_file(&tree.0, "tmp-").unwrap();
    let b = make_temp_file(&tree.0, "tmp-").unwrap();
    assert_ne!(a, b);
    assert_eq!(names(&tree.0).len(), 3);
}

#[test]
fn errors() {
    let tree = Tree::new("errors");
    let err = use_temp_file(&tree.path("nodir"), "tmp-", "alpha\n").unwrap_err();
    assert_eq!(err.kind(), ErrorKind::NotFound);
    let err = use_temp_file(&tree.0, "sub/tmp-", "alpha\n").unwrap_err();
    assert_eq!(err.kind(), ErrorKind::InvalidInput);
    assert!(names(&tree.path("sub")).is_empty());
}
