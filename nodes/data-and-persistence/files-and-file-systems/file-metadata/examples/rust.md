```rust run id=metadata-and-ext
use std::fs;
use std::os::unix::fs::{MetadataExt, PermissionsExt};
use std::time::{Duration, SystemTime};

fn main() -> std::io::Result<()> {
    fs::write("data.txt", "hello")?;
    fs::set_permissions("data.txt", fs::Permissions::from_mode(0o640))?;
    let t = SystemTime::UNIX_EPOCH + Duration::from_secs(1_700_000_000);
    fs::File::options().write(true).open("data.txt")?.set_modified(t)?;
    fs::hard_link("data.txt", "second-name.txt")?;
    std::os::unix::fs::symlink("data.txt", "link")?;

    // metadata follows the link, symlink_metadata does not.
    let st = fs::metadata("link")?;
    let lst = fs::symlink_metadata("link")?;
    println!("metadata:         file {}, len {}, mode {:o}", st.is_file(), st.len(),
        st.permissions().mode() & 0o777);
    println!("symlink_metadata: symlink {}, len {}", lst.file_type().is_symlink(), lst.len());
    let secs = st.modified()?.duration_since(SystemTime::UNIX_EPOCH).unwrap().as_secs();
    println!("modified: {secs}");

    // MetadataExt gives the fields of the Unix stat structure.
    let other = fs::metadata("second-name.txt")?;
    println!("nlink {}, same inode {}", st.nlink(), (st.dev(), st.ino()) == (other.dev(), other.ino()));
    println!("readonly: {}", st.permissions().readonly());
    Ok(())
}
```

```output for=metadata-and-ext
metadata:         file true, len 5, mode 640
symlink_metadata: symlink true, len 8
modified: 1700000000
nlink 2, same inode true
readonly: false
```

- Verdict: stdlib. `fs::metadata` follows a link, and `fs::symlink_metadata` describes the link. The portable `Metadata` has the file type, `len`, `permissions` and three times, and `created` returns an error where the system has no creation time. The trait `std::os::unix::fs::MetadataExt` adds the fields of the Unix stat structure, such as `dev`, `ino`, `mode`, `nlink`, `uid` and `gid`. `Permissions::readonly` looks only at the write bits, not at the user who runs the program.
