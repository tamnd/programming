```rust run id=metadata-ext
use std::fs;
use std::os::unix::fs::MetadataExt;

fn main() -> std::io::Result<()> {
    fs::create_dir_all("tree/src")?;
    fs::write("tree/src/main.txt", "main\n")?;
    fs::write("tree/readme.txt", "read me\n")?;

    // read_dir gives the entries of a directory in no fixed order, so sort them.
    let mut names: Vec<(String, bool)> = fs::read_dir("tree")?
        .map(|e| e.map(|e| (e.file_name().to_string_lossy().into_owned(), e.path().is_dir())))
        .collect::<Result<_, _>>()?;
    names.sort();
    for (name, is_dir) in &names {
        println!("{name} {}", if *is_dir { "dir" } else { "file" });
    }

    // MetadataExt (Unix only) gives the device ID, the inode number and the link count.
    let before = fs::metadata("tree/readme.txt")?.ino();
    fs::rename("tree/readme.txt", "tree/README.txt")?;
    println!("same inode after rename: {}", fs::metadata("tree/README.txt")?.ino() == before);

    fs::hard_link("tree/src/main.txt", "tree/main-link.txt")?;
    let a = fs::metadata("tree/src/main.txt")?;
    let b = fs::metadata("tree/main-link.txt")?;
    println!("same file: {}", a.dev() == b.dev() && a.ino() == b.ino());
    println!("links: {}", a.nlink());

    // An error has a kind, so the program can test for a missing file.
    let err = fs::metadata("tree/missing.txt").unwrap_err();
    println!("missing: {:?}", err.kind());
    Ok(())
}
```

```output for=metadata-ext
readme.txt file
src dir
same inode after rename: true
same file: true
links: 2
missing: NotFound
```

- Verdict: stdlib. `fs::read_dir` gives the entries in no fixed order, so the program sorts them. The Unix extension trait `MetadataExt` gives `dev`, `ino` and `nlink`, and they show that `fs::rename` keeps the inode and that `fs::hard_link` adds a name to the same file. An error has an `ErrorKind`, such as `NotFound`.
