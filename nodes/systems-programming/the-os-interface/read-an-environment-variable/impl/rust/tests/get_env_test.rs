// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/read-an-environment-variable/.

use std::env::{self, VarError};

use algo_read_an_environment_variable::get_env;

// set_var and remove_var are unsafe, because another thread can read the
// environment at the same time. The test harness runs the tests of a file on
// many threads, so this file has one test, and it changes the environment
// only from that test.
#[test]
fn cases() {
    let cases: [(&str, &[(&str, &str)], &str, &str, &str); 11] = [
        ("present", &[("APP_PORT", "8080")], "APP_PORT", "80", "8080"),
        (
            "path-value",
            &[("APP_HOME", "/srv/app")],
            "APP_HOME",
            "/opt/app",
            "/srv/app",
        ),
        ("missing", &[], "APP_PORT", "80", "80"),
        (
            "empty-value",
            &[("APP_MODE", "")],
            "APP_MODE",
            "production",
            "",
        ),
        ("empty-default", &[], "APP_MODE", "", ""),
        (
            "spaces",
            &[("APP_TITLE", "  two  spaces  ")],
            "APP_TITLE",
            "none",
            "  two  spaces  ",
        ),
        (
            "equals",
            &[("APP_QUERY", "key=value=more")],
            "APP_QUERY",
            "none",
            "key=value=more",
        ),
        (
            "newline",
            &[("APP_TEXT", "first\nsecond")],
            "APP_TEXT",
            "none",
            "first\nsecond",
        ),
        (
            "non-ascii",
            &[("APP_GREETING", "xin chào, 日本語, 🙂")],
            "APP_GREETING",
            "none",
            "xin chào, 日本語, 🙂",
        ),
        ("case", &[("APP_PORT", "8080")], "app_port", "80", "80"),
        ("inherited-name", &[], "toString", "none", "none"),
    ];
    for (case, vars, name, default, want) in cases {
        // SAFETY: no other thread reads or writes the environment now.
        unsafe { env::remove_var(name) };
        for &(key, value) in vars {
            unsafe { env::set_var(key, value) };
        }
        assert_eq!(get_env(name, default), Ok(want.to_owned()), "case {case}");
        for &(key, _) in vars {
            unsafe { env::remove_var(key) };
        }
    }

    // A value that is not UTF-8 is an error, not the default.
    #[cfg(unix)]
    {
        use std::ffi::OsStr;
        use std::os::unix::ffi::OsStrExt;
        let bytes = OsStr::from_bytes(b"caf\xe9");
        unsafe { env::set_var("APP_NAME", bytes) };
        assert_eq!(
            get_env("APP_NAME", "none"),
            Err(VarError::NotUnicode(bytes.to_owned()))
        );
        unsafe { env::remove_var("APP_NAME") };
    }
}
