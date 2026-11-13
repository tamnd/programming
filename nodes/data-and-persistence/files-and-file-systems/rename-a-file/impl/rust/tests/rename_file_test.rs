// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/rename-a-file/.

use std::fs;
use std::io::{ErrorKind, Read};
use std::os::unix::fs::{symlink, MetadataExt};
use std::path::{Path, PathBuf};

use algo_rename_a_file::rename_file;

/// Returns a new, empty directory for one test.
fn new_dir(name: &str) -> PathBuf {
    let dir = std::env::temp_dir().join(format!("rename-file-{}-{name}", std::process::id()));
    let _ = fs::remove_dir_all(&dir);
    fs::create_dir_all(&dir).unwrap();
    dir
}

fn lexists(path: &Path) -> bool {
    fs::symlink_metadata(path).is_ok()
}

#[test]
fn cases() {
    for (name, src, dst, src_text, dst_text, want) in [
        ("rename", "a.txt", "b.txt", "alpha\n", None, "alpha\n"),
        ("replace-existing", "a.txt", "b.txt", "alpha\n", Some("beta\n"), "alpha\n"),
        ("same-path", "a.txt", "a.txt", "alpha\n", Some("alpha\n"), "alpha\n"),
        ("empty-file", "a.txt", "b.txt", "", Some("beta\n"), ""),
        ("other-directory", "a.txt", "sub/a.txt", "alpha\n", None, "alpha\n"),
        ("name-with-spaces", "my notes.txt", "old notes.txt", "alpha\n", None, "alpha\n"),
        ("non-ascii-name", "a.txt", "café 日本.txt", "alpha\n", None, "alpha\n"),
    ] {
        let dir = new_dir(name);
        let (src, dst) = (dir.join(src), dir.join(dst));
        fs::create_dir_all(dst.parent().unwrap()).unwrap();
        if let Some(t) = dst_text {
            fs::write(&dst, t).unwrap();
        }
        fs::write(&src, src_text).unwrap();
        rename_file(&src, &dst).unwrap();
        assert_eq!(fs::read_to_string(&dst).unwrap(), want, "{name}");
        assert!(src == dst || !lexists(&src), "{name}");
        fs::remove_dir_all(&dir).unwrap();
    }
}

#[test]
fn errors() {
    let dir = new_dir("errors");
    let src = dir.join("a.txt");
    let err = rename_file(&src, dir.join("b.txt")).unwrap_err();
    assert_eq!(err.kind(), ErrorKind::NotFound);
    fs::write(&src, "alpha\n").unwrap();
    let err = rename_file(&src, dir.join("no-such-dir/b.txt")).unwrap_err();
    assert_eq!(err.kind(), ErrorKind::NotFound);
    fs::create_dir(dir.join("dir")).unwrap();
    assert!(rename_file(&src, dir.join("dir")).is_err());
    assert!(dir.join("dir").is_dir());
    assert_eq!(fs::read_to_string(&src).unwrap(), "alpha\n");
    fs::remove_dir_all(&dir).unwrap();
}

#[test]
fn link() {
    let dir = new_dir("link");
    fs::write(dir.join("a.txt"), "alpha\n").unwrap();
    symlink("a.txt", dir.join("link")).unwrap();
    rename_file(dir.join("link"), dir.join("new-link")).unwrap();
    assert_eq!(fs::read_link(dir.join("new-link")).unwrap(), Path::new("a.txt"));
    assert!(!lexists(&dir.join("link")));
    fs::remove_dir_all(&dir).unwrap();
}

#[test]
fn open_dst_keeps_old_data() {
    let dir = new_dir("open");
    let (src, dst) = (dir.join("a.txt"), dir.join("b.txt"));
    fs::write(&src, "alpha\n").unwrap();
    fs::write(&dst, "beta\n").unwrap();
    let mut old = fs::File::open(&dst).unwrap();
    rename_file(&src, &dst).unwrap();
    let mut s = String::new();
    old.read_to_string(&mut s).unwrap();
    assert_eq!(s, "beta\n");
    assert_eq!(fs::read_to_string(&dst).unwrap(), "alpha\n");
    fs::remove_dir_all(&dir).unwrap();
}

#[test]
fn cross_device() {
    let dir = new_dir("cross");
    let shm = Path::new("/dev/shm");
    let other = match (fs::metadata(shm), fs::metadata(&dir)) {
        (Ok(a), Ok(b)) => a.is_dir() && a.dev() != b.dev(),
        _ => false,
    };
    if other {
        let src = dir.join("a.txt");
        fs::write(&src, "alpha\n").unwrap();
        let dst = shm.join(format!("rename-file-{}.txt", std::process::id()));
        let err = rename_file(&src, &dst).unwrap_err();
        assert_eq!(err.kind(), ErrorKind::CrossesDevices);
        assert_eq!(fs::read_to_string(&src).unwrap(), "alpha\n");
        assert!(!lexists(&dst));
    }
    fs::remove_dir_all(&dir).unwrap();
}
