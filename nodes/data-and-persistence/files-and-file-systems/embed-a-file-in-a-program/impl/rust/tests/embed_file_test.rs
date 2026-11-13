// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/embed-a-file-in-a-program/.

use algo_embed_a_file_in_a_program::{embedded_line, BYTES, LINES};

#[test]
fn cases() {
    let cases: [(&str, i64, Option<&str>); 8] = [
        ("first-line", 1, Some("alpha")),
        ("second-line", 2, Some("café")),
        ("last-line", 4, Some("gamma")),
        ("non-ascii-line", 2, Some("café")),
        ("empty-line", 3, Some("")),
        ("zero", 0, None),
        ("negative", -1, None),
        ("past-the-end", 5, None),
    ];
    for (name, n, want) in cases {
        assert_eq!(embedded_line(n).as_deref(), want, "{name}");
    }
}

#[test]
fn bytes_are_the_file() {
    assert_eq!(BYTES, "# SPDX-License-Identifier: MIT\nalpha\ncafé\n\ngamma\n".as_bytes());
    assert_eq!(BYTES.len(), 50);
    assert_eq!(LINES.as_bytes(), BYTES);
}

#[test]
fn working_directory_does_not_matter() {
    let dir = std::env::temp_dir().join(format!("embed-{}", std::process::id()));
    std::fs::create_dir_all(&dir).unwrap();
    std::env::set_current_dir(&dir).unwrap();
    assert_eq!(embedded_line(4).as_deref(), Some("gamma"));
    std::fs::remove_dir_all(&dir).unwrap();
}
