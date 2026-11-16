// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/define-a-record-type/.

/// A struct with named fields. The derives add clone, debug printing and
/// field-by-field equality.
#[derive(Clone, Debug, PartialEq, Eq)]
pub struct Person {
    pub name: String,
    pub age: u32,
}

/// Returns a new vector where each person called name is one year older.
pub fn birthday(people: &[Person], name: &str) -> Vec<Person> {
    people
        .iter()
        .map(|p| {
            if p.name == name {
                // Struct update syntax: the fields not named here come from p.
                Person { age: p.age + 1, ..p.clone() }
            } else {
                p.clone()
            }
        })
        .collect()
}
