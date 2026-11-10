// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/read-an-environment-variable/.

//! Read an environment variable with a default.

use std::env::{self, VarError};

/// Returns the value of the environment variable `name`, or `default` when it is not set.
///
/// An empty value is a value, so it does not give `default`.
/// A value that is not valid Unicode gives `VarError::NotUnicode`.
pub fn get_env(name: &str, default: &str) -> Result<String, VarError> {
    match env::var(name) {
        Err(VarError::NotPresent) => Ok(default.to_owned()),
        other => other,
    }
}
