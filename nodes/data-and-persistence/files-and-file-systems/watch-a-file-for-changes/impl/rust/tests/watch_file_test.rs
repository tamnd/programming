// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/watch-a-file-for-changes/.

use std::fs::{self, File, FileTimes};
use std::io::ErrorKind;
use std::path::PathBuf;
use std::sync::atomic::{AtomicBool, Ordering};
use std::sync::{Arc, Mutex};
use std::thread;
use std::time::{Duration, Instant, SystemTime, UNIX_EPOCH};

use algo_watch_a_file_for_changes::{watch, watch_changes, FileWatcher};

struct Tree(PathBuf);

impl Tree {
    fn new(name: &str) -> Tree {
        let nanos = SystemTime::now().duration_since(UNIX_EPOCH).unwrap().as_nanos();
        let dir = std::env::temp_dir().join(format!("watch-{name}-{}-{nanos}", std::process::id()));
        fs::create_dir_all(&dir).unwrap();
        Tree(dir)
    }
}

impl Drop for Tree {
    fn drop(&mut self) {
        let _ = fs::remove_dir_all(&self.0);
    }
}

const CASES: &[(&str, &[&str], &[&str])] = &[
    ("create", &["create:hello\n"], &["created"]),
    ("create-then-write", &["create:a\n", "write:a much longer line\n"], &["created", "modified"]),
    ("create-then-delete", &["create:a\n", "delete"], &["created", "deleted"]),
    ("append", &["create:a\n", "append:b\n", "append:c\n"], &["created", "modified", "modified"]),
    ("no-change", &["create:a\n", "none", "none"], &["created", "none", "none"]),
    ("same-size", &["create:abc\n", "write:xyz\n"], &["created", "modified"]),
    ("touch-only", &["create:a\n", "touch"], &["created", "modified"]),
    ("replace-by-rename", &["create:old\n", "replace:new\n"], &["created", "modified"]),
    ("delete-and-create", &["create:a\n", "recreate:b\n"], &["created", "modified"]),
    ("never-created", &["none"], &["none"]),
];

#[test]
fn cases() {
    for (name, steps, expect) in CASES {
        let tree = Tree::new(name);
        let got = watch_changes(tree.0.join("watched.txt"), steps).unwrap();
        assert_eq!(got, *expect, "case {name}");
    }
}

#[test]
fn directory_is_an_error() {
    let tree = Tree::new("dir");
    let err = watch_changes(tree.0.join("watched.txt"), &["mkdir"]).unwrap_err();
    assert_eq!(err.kind(), ErrorKind::IsADirectory);
}

fn set_mtime(path: &PathBuf, secs: u64) {
    let t = UNIX_EPOCH + Duration::from_secs(secs);
    File::options().write(true).open(path).unwrap().set_times(FileTimes::new().set_modified(t)).unwrap();
}

#[test]
fn same_size_and_same_time_hide_a_change() {
    let tree = Tree::new("same");
    let p = tree.0.join("watched.txt");
    fs::write(&p, "abc\n").unwrap();
    set_mtime(&p, 5);
    let mut watcher = FileWatcher::new(&p).unwrap();
    fs::write(&p, "xyz\n").unwrap();
    set_mtime(&p, 5);
    assert_eq!(watcher.poll().unwrap(), "none");
    fs::write(&p, "xyz!\n").unwrap();
    assert_eq!(watcher.poll().unwrap(), "modified");
}

#[test]
fn watch_calls_once_for_a_burst() {
    let tree = Tree::new("burst");
    let p = tree.0.join("watched.txt");
    let calls = Arc::new(Mutex::new(Vec::<PathBuf>::new()));
    let done = Arc::new(AtomicBool::new(false));
    let handle = {
        let (p, calls, done) = (p.clone(), calls.clone(), done.clone());
        thread::spawn(move || {
            watch(
                &p,
                |f| calls.lock().unwrap().push(f.to_path_buf()),
                Duration::from_millis(10),
                Duration::from_millis(500),
                || done.load(Ordering::SeqCst),
            )
        })
    };
    for i in 1..=5 {
        fs::write(&p, "x".repeat(i)).unwrap();
        thread::sleep(Duration::from_millis(20));
    }
    let deadline = Instant::now() + Duration::from_secs(5);
    while calls.lock().unwrap().is_empty() && Instant::now() < deadline {
        thread::sleep(Duration::from_millis(10));
    }
    thread::sleep(Duration::from_millis(600));
    done.store(true, Ordering::SeqCst);
    handle.join().unwrap().unwrap();
    assert_eq!(*calls.lock().unwrap(), vec![p]);
}
