// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/apply-a-patch/.

use std::fs;
use std::io::ErrorKind;
use std::path::PathBuf;

use algo_apply_a_patch::{apply_patch, apply_patch_file, PatchError};

/// (name, the old text, the patch, the new text)
const CASES: &[(&str, &str, &str, &str)] = &[
    ("change-a-line", "a\nb\nc\n", "--- old.txt\n+++ new.txt\n@@ -1,3 +1,3 @@\n a\n-b\n+B\n c\n", "a\nB\nc\n"),
    ("two-hunks", "apple\nbanana\ncherry\ndate\nelder\nfig\ngrape\n", "--- old.txt\n+++ new.txt\n@@ -1,3 +1,3 @@\n apple\n-banana\n+blueberry\n cherry\n@@ -6,2 +6,2 @@\n fig\n-grape\n+guava\n", "apple\nblueberry\ncherry\ndate\nelder\nfig\nguava\n"),
    ("git-header", "def main():\n    print(1)\n", "diff --git a/main.py b/main.py\nindex 1234567..89abcde 100644\n--- a/main.py\n+++ b/main.py\n@@ -1,2 +1,2 @@ def main():\n def main():\n-    print(1)\n+    print(2)\n", "def main():\n    print(2)\n"),
    ("add-at-end", "a\n", "@@ -1 +1,2 @@\n a\n+b\n", "a\nb\n"),
    ("old-empty", "", "--- /dev/null\n+++ b.txt\n@@ -0,0 +1,2 @@\n+a\n+b\n", "a\nb\n"),
    ("delete-all", "a\nb\n", "@@ -1,2 +0,0 @@\n-a\n-b\n", ""),
    ("no-count", "a\nb\nc\n", "@@ -2 +2 @@\n-b\n+x\n", "a\nx\nc\n"),
    ("zero-context", "apple\nbanana\ncherry\ndate\nelder\nfig\ngrape\n", "@@ -3 +3 @@\n-cherry\n+coconut\n@@ -6 +5,0 @@\n-fig\n@@ -7,0 +7 @@\n+honeydew\n", "apple\nbanana\ncoconut\ndate\nelder\ngrape\nhoneydew\n"),
    ("insert-at-start", "b\n", "@@ -0,0 +1 @@\n+a\n", "a\nb\n"),
    ("old-no-final-newline", "a\nb", "@@ -1,2 +1,2 @@\n a\n-b\n\\ No newline at end of file\n+b\n", "a\nb\n"),
    ("new-no-final-newline", "a\nb\n", "@@ -1,2 +1,2 @@\n a\n-b\n+b\n\\ No newline at end of file\n", "a\nb"),
    ("empty-context-line", "a\n\nb\n", "@@ -1,3 +1,3 @@\n a\n\n-b\n+c\n", "a\n\nc\n"),
    ("space-context-line", "a\n\nb\n", "@@ -1,3 +1,3 @@\n a\n \n-b\n+c\n", "a\n\nc\n"),
    ("crlf", "a\r\nb\r\n", "@@ -1,2 +1,2 @@\n a\r\n-b\r\n+c\r\n", "a\r\nc\r\n"),
    ("header-like-line", "x\n-- y\n", "--- old.txt\n+++ new.txt\n@@ -1,2 +1 @@\n x\n--- y\n", "x\n"),
    ("non-ascii", "caf\u{e9}\n", "@@ -1 +1 @@\n-caf\u{e9}\n+na\u{ef}ve\n", "na\u{ef}ve\n"),
];

/// (name, the old text, the patch, the kind of the error)
const ERRORS: &[(&str, &str, &str, &str)] = &[
    (
        "mismatch",
        "a\nb\nc\n",
        "@@ -1,3 +1,3 @@\n a\n-x\n+y\n c\n",
        "mismatch",
    ),
    (
        "malformed",
        "a\nb\nc\n",
        "@@ -1,3 +1,3 @@\n a\n-b\n",
        "malformed",
    ),
    ("no-hunk", "a\n", "no hunk here\n", "malformed"),
    ("bad-mark", "a\n", "@@ -1 +1 @@\n*a\n", "malformed"),
    ("past-the-end", "a\n", "@@ -2 +2 @@\n-a\n+b\n", "mismatch"),
];

#[test]
fn apply_patch_cases() {
    for &(name, old, patch, want) in CASES {
        assert_eq!(
            apply_patch(old, patch).ok().as_deref(),
            Some(want),
            "{name}"
        );
    }
}

#[test]
fn error_cases() {
    for &(name, old, patch, kind) in ERRORS {
        let got = match apply_patch(old, patch) {
            Err(PatchError::Malformed { .. }) => "malformed",
            Err(PatchError::Mismatch { .. }) => "mismatch",
            _ => "other",
        };
        assert_eq!(got, kind, "{name}");
    }
}

#[test]
fn many_hunks() {
    // 1000 lines, and 10 hunks with no context change 10 of them.
    let old: String = (0..1000).map(|i| format!("{i}\n")).collect();
    let patch: String = (50..1000)
        .step_by(100)
        .map(|i| format!("@@ -{} +{} @@\n-{i}\n+x{i}\n", i + 1, i + 1))
        .collect();
    let want: String = (0..1000)
        .map(|i| {
            if i % 100 == 50 {
                format!("x{i}\n")
            } else {
                format!("{i}\n")
            }
        })
        .collect();
    assert_eq!(apply_patch(&old, &patch).unwrap(), want);
}

fn temp_dir(name: &str) -> PathBuf {
    let dir = std::env::temp_dir().join(format!("apply-a-patch-{name}-{}", std::process::id()));
    fs::create_dir_all(&dir).unwrap();
    dir
}

#[test]
fn apply_patch_file_crlf() {
    let dir = temp_dir("file");
    let &(_, old, patch, want) = CASES.iter().find(|c| c.0 == "crlf").unwrap();
    fs::write(dir.join("old.txt"), old).unwrap();
    fs::write(dir.join("a.patch"), patch).unwrap();
    let got = apply_patch_file(dir.join("old.txt"), dir.join("a.patch"));
    fs::remove_dir_all(&dir).unwrap();
    assert_eq!(got.unwrap(), want);
}

#[test]
fn not_found() {
    let dir = temp_dir("missing");
    fs::write(dir.join("a.patch"), "@@ -1 +1 @@\n-a\n+b\n").unwrap();
    let got = apply_patch_file(dir.join("missing.txt"), dir.join("a.patch"));
    fs::remove_dir_all(&dir).unwrap();
    assert!(matches!(got, Err(PatchError::Io(e)) if e.kind() == ErrorKind::NotFound));
}
