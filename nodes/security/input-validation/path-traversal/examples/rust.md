```rust wrong id=join
use std::path::Path;

fn main() {
    let public = Path::new("/srv/app/public");
    for name in ["report.txt", "../secret.txt", "/etc/hosts"] {
        println!("{}", public.join(name).display());
    }
}
```

```output for=join
/srv/app/public/report.txt
/srv/app/public/../secret.txt
/etc/hosts
```

`Path::join` keeps `..` as a component, and the operating system climbs out when it opens the path. An absolute name replaces the whole path.

```rust run id=components
use std::path::{Component, Path, PathBuf};

// Accept only plain names: no root, no drive prefix, no "..".
fn local_path(root: &Path, name: &str) -> Result<PathBuf, String> {
    let mut path = root.to_path_buf();
    for part in Path::new(name).components() {
        match part {
            Component::Normal(p) => path.push(p),
            Component::CurDir => {}
            _ => return Err(format!("refused {name:?}")),
        }
    }
    Ok(path)
}

fn main() {
    let public = Path::new("/srv/app/public");
    for name in ["report.txt", "docs/./guide.txt", "../secret.txt", "/etc/hosts"] {
        match local_path(public, name) {
            Ok(path) => println!("{}", path.display()),
            Err(error) => println!("{error}"),
        }
    }
}
```

```output for=components
/srv/app/public/report.txt
/srv/app/public/docs/guide.txt
refused "../secret.txt"
refused "/etc/hosts"
```

- Verdict: stdlib. `Path::components` splits a path into typed parts: `RootDir`, `Prefix` (a Windows drive), `ParentDir` for `..`, `CurDir` for `.` and `Normal` for a name. A function that accepts only `Normal` and `CurDir` cannot leave the root by its text.
- `PathBuf::push` and `Path::join` replace the path when the new part is absolute.
- The check is lexical. It does not see a symbolic link inside the root that points out. Where other users can create links, call `fs::canonicalize` on the result and check that it starts with the canonical root. `Path::starts_with` compares whole components.
- The standard library has no `openat`-style API that keeps every open beneath a directory.
