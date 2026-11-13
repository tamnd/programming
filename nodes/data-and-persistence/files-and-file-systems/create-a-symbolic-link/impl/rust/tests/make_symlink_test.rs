// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/create-a-symbolic-link/.

use std::fs;
use std::io::ErrorKind;
use std::os::unix::fs::symlink;
use std::path::{Path, PathBuf};

use algo_create_a_symbolic_link::make_symlink;

struct Tree(PathBuf);

impl Tree {
    fn new(name: &str) -> Tree {
        let dir = std::env::temp_dir().join(format!("make-symlink-{}-{name}", std::process::id()));
        let _ = fs::remove_dir_all(&dir);
        fs::create_dir_all(dir.join("sub")).unwrap();
        fs::write(dir.join("a.txt"), "alpha\n").unwrap();
        fs::write(dir.join("sub/b.txt"), "beta\n").unwrap();
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

#[test]
fn cases() {
    let cases = [
        ("file-target", "a.txt", "link"),
        ("directory-target", "sub", "link"),
        ("link-in-subdirectory", "b.txt", "sub/link"),
        ("dangling-target", "missing.txt", "link"),
        ("relative-target", "a.txt", "sub/link"),
        ("parent-target", "../a.txt", "sub/link"),
        ("name-with-spaces", "my notes.txt", "my link"),
        ("non-ascii-name", "café 日本.txt", "lien 日本"),
    ];
    for (name, target, rel) in cases {
        let tree = Tree::new(name);
        let link = tree.path(rel);
        assert_eq!(make_symlink(target, &link).unwrap(), Path::new(target), "{name}");
        assert!(fs::symlink_metadata(&link).unwrap().file_type().is_symlink(), "{name}");
    }
}

#[test]
fn exists() {
    let tree = Tree::new("exists");
    for rel in ["a.txt", "sub"] {
        let err = make_symlink("b.txt", tree.path(rel)).unwrap_err();
        assert_eq!(err.kind(), ErrorKind::AlreadyExists, "{rel}");
    }
}

#[test]
fn dangling_link_exists() {
    let tree = Tree::new("dangling");
    symlink("missing.txt", tree.path("link")).unwrap();
    let err = make_symlink("a.txt", tree.path("link")).unwrap_err();
    assert_eq!(err.kind(), ErrorKind::AlreadyExists);
    assert_eq!(fs::read_link(tree.path("link")).unwrap(), Path::new("missing.txt"));
}

#[test]
fn missing_directory() {
    let tree = Tree::new("missing");
    let err = make_symlink("a.txt", tree.path("nodir/link")).unwrap_err();
    assert_eq!(err.kind(), ErrorKind::NotFound);
}

#[test]
fn link_reads_target() {
    let tree = Tree::new("reads");
    make_symlink("../a.txt", tree.path("sub/up")).unwrap();
    assert_eq!(fs::read_to_string(tree.path("sub/up")).unwrap(), "alpha\n");
}

#[test]
fn absolute_target() {
    let tree = Tree::new("absolute");
    let target = tree.path("a.txt");
    assert_eq!(make_symlink(&target, tree.path("link")).unwrap(), target);
}
