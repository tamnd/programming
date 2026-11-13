// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/copy-a-file/.

use std::fs;
use std::io::ErrorKind;
use std::os::unix::fs::{symlink, PermissionsExt};
use std::path::PathBuf;

use algo_copy_a_file::copy_file;

/// Returns a new, empty directory for one test.
fn new_dir(name: &str) -> PathBuf {
    let dir = std::env::temp_dir().join(format!("copy-file-{}-{name}", std::process::id()));
    let _ = fs::remove_dir_all(&dir);
    fs::create_dir_all(&dir).unwrap();
    dir
}

#[test]
fn cases() {
    for (name, src, dst, src_text, dst_text) in [
        ("copy", "a.txt", "b.txt", "alpha\n", None),
        ("replace-existing", "a.txt", "b.txt", "alpha\n", Some("beta, a longer text\n")),
        ("empty-file", "a.txt", "b.txt", "", Some("beta\n")),
        ("other-directory", "a.txt", "sub/a.txt", "alpha\n", None),
        ("name-with-spaces", "my notes.txt", "copy of my notes.txt", "alpha\n", None),
        ("non-ascii-name", "a.txt", "café 日本.txt", "alpha\n", None),
        ("binary-data", "a.bin", "b.bin", "\0\r\nÿ\0", None),
    ] {
        let dir = new_dir(name);
        let (src, dst) = (dir.join(src), dir.join(dst));
        fs::create_dir_all(dst.parent().unwrap()).unwrap();
        if let Some(t) = dst_text {
            fs::write(&dst, t).unwrap();
        }
        fs::write(&src, src_text).unwrap();
        copy_file(&src, &dst).unwrap();
        assert_eq!(fs::read(&dst).unwrap(), src_text.as_bytes(), "{name}");
        assert_eq!(fs::read(&src).unwrap(), src_text.as_bytes(), "{name}");
        fs::remove_dir_all(&dir).unwrap();
    }
}

#[test]
fn missing() {
    let dir = new_dir("missing");
    let src = dir.join("a.txt");
    assert_eq!(copy_file(&src, &dir.join("b.txt")).unwrap_err().kind(), ErrorKind::NotFound);
    fs::write(&src, "alpha\n").unwrap();
    let err = copy_file(&src, &dir.join("no-such-dir/b.txt")).unwrap_err();
    assert_eq!(err.kind(), ErrorKind::NotFound);
    fs::remove_dir_all(&dir).unwrap();
}

#[test]
fn directories() {
    let dir = new_dir("directories");
    let (sub, src) = (dir.join("dir"), dir.join("a.txt"));
    fs::create_dir(&sub).unwrap();
    fs::write(&src, "alpha\n").unwrap();
    assert_eq!(copy_file(&sub, &dir.join("b.txt")).unwrap_err().kind(), ErrorKind::IsADirectory);
    assert_eq!(copy_file(&src, &sub).unwrap_err().kind(), ErrorKind::IsADirectory);
    assert_eq!(fs::read_dir(&sub).unwrap().count(), 0);
    assert!(fs::symlink_metadata(dir.join("b.txt")).is_err());
    fs::remove_dir_all(&dir).unwrap();
}

#[test]
fn same_file() {
    let dir = new_dir("same");
    let (src, link) = (dir.join("a.txt"), dir.join("b.txt"));
    fs::write(&src, "alpha\n").unwrap();
    fs::hard_link(&src, &link).unwrap();
    for dst in [&src, &link] {
        assert_eq!(copy_file(&src, dst).unwrap_err().kind(), ErrorKind::InvalidInput);
        assert_eq!(fs::read_to_string(&src).unwrap(), "alpha\n");
    }
    fs::remove_dir_all(&dir).unwrap();
}

#[test]
fn mode() {
    let dir = new_dir("mode");
    let (src, dst, other) = (dir.join("a.sh"), dir.join("b.sh"), dir.join("c.sh"));
    fs::write(&src, "echo alpha\n").unwrap();
    fs::set_permissions(&src, fs::Permissions::from_mode(0o750)).unwrap();
    fs::write(&other, "beta\n").unwrap();
    fs::set_permissions(&other, fs::Permissions::from_mode(0o640)).unwrap();
    for p in [&dst, &other] {
        copy_file(&src, p).unwrap();
        assert_eq!(fs::metadata(p).unwrap().permissions().mode() & 0o7777, 0o750);
    }
    fs::remove_dir_all(&dir).unwrap();
}

#[test]
fn large_file() {
    let dir = new_dir("large");
    let data: Vec<u8> = (0..5_000_003u64).map(|i| (i * 7 % 251) as u8).collect();
    fs::write(dir.join("big.bin"), &data).unwrap();
    copy_file(&dir.join("big.bin"), &dir.join("copy.bin")).unwrap();
    assert!(fs::read(dir.join("copy.bin")).unwrap() == data);
    fs::remove_dir_all(&dir).unwrap();
}

#[test]
fn link_at_src() {
    let dir = new_dir("link");
    fs::write(dir.join("target.txt"), "alpha\n").unwrap();
    symlink("target.txt", dir.join("link.txt")).unwrap();
    copy_file(&dir.join("link.txt"), &dir.join("b.txt")).unwrap();
    assert!(!fs::symlink_metadata(dir.join("b.txt")).unwrap().file_type().is_symlink());
    assert_eq!(fs::read_to_string(dir.join("b.txt")).unwrap(), "alpha\n");
    fs::remove_dir_all(&dir).unwrap();
}
