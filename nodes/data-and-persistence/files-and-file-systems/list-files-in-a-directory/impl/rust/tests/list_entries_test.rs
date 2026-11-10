// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/list-files-in-a-directory/.

use std::fs;
use std::io::ErrorKind;
use std::os::unix::fs::symlink;
use std::path::PathBuf;

use algo_list_files_in_a_directory::list_entries;

/// A new directory that is removed when the value is dropped.
struct Tree(PathBuf);

impl Drop for Tree {
    fn drop(&mut self) {
        let _ = fs::remove_dir_all(&self.0);
    }
}

/// Makes each entry of the tree in a new directory.
fn make_tree(name: &str, tree: &[&str]) -> Tree {
    let root = std::env::temp_dir().join(format!("list-entries-{}-{name}", std::process::id()));
    let _ = fs::remove_dir_all(&root);
    fs::create_dir(&root).unwrap();
    for entry in tree {
        if let Some((link, target)) = entry.split_once(" -> ") {
            symlink(target, root.join(link)).unwrap();
        } else if entry.ends_with('/') {
            fs::create_dir(root.join(entry)).unwrap();
        } else {
            fs::write(root.join(entry), b"").unwrap();
        }
    }
    Tree(root)
}

#[test]
fn cases() {
    let tree = ["docs/", "docs/guide.md", "docs/img/", "docs/img/logo.png", "readme.md"];
    let cases: [(&str, &[&str], bool, &[&str]); 14] = [
        ("flat", &["b.txt", "a.txt", "c.txt"], false, &["a.txt", "b.txt", "c.txt"]),
        ("nested", &tree, true, &tree),
        ("nested-shallow", &tree, false, &["docs/", "readme.md"]),
        ("empty", &[], true, &[]),
        ("empty-subdir", &["empty/"], true, &["empty/"]),
        (
            "hidden",
            &[".env", ".git/", ".git/config", "main.go"],
            true,
            &[".env", ".git/", ".git/config", "main.go"],
        ),
        ("case-order", &["a.txt", "B.txt", "c.txt"], false, &["B.txt", "a.txt", "c.txt"]),
        (
            "dot-before-slash",
            &["a/", "a/x", "a.txt", "a-b"],
            true,
            &["a-b", "a.txt", "a/", "a/x"],
        ),
        (
            "symlink-to-dir",
            &["real/", "real/f", "link -> real"],
            true,
            &["link", "real/", "real/f"],
        ),
        ("symlink-to-file", &["f", "g -> f"], true, &["f", "g"]),
        ("broken-symlink", &["dangling -> nowhere"], true, &["dangling"]),
        ("symlink-loop", &["d/", "d/up -> .."], true, &["d/", "d/up"]),
        (
            "spaces",
            &["my file.txt", "two  spaces/", "two  spaces/x"],
            true,
            &["my file.txt", "two  spaces/", "two  spaces/x"],
        ),
        (
            "non-ascii",
            &["été.txt", "zebra.txt", "apple.txt"],
            false,
            &["apple.txt", "zebra.txt", "été.txt"],
        ),
    ];
    for (name, tree, recursive, want) in cases {
        let got = list_entries(&make_tree(name, tree).0, recursive).unwrap();
        assert_eq!(got, want, "case {name}");
    }
}

#[test]
fn missing() {
    let root = make_tree("missing", &[]);
    let err = list_entries(&root.0.join("missing"), true).unwrap_err();
    assert_eq!(err.kind(), ErrorKind::NotFound);
}

#[test]
fn not_a_directory() {
    let root = make_tree("not-a-directory", &["notes.txt"]);
    let err = list_entries(&root.0.join("notes.txt"), true).unwrap_err();
    assert_eq!(err.kind(), ErrorKind::NotADirectory);
}
