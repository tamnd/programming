// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/define-an-enumeration/.

/// The days of the week. Each variant has no data, so `as usize` gives its
/// position.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub enum Day {
    Monday,
    Tuesday,
    Wednesday,
    Thursday,
    Friday,
    Saturday,
    Sunday,
}

impl Day {
    /// Rust cannot loop over the variants of an enum, so a constant array
    /// lists them.
    pub const ALL: [Day; 7] = [
        Day::Monday,
        Day::Tuesday,
        Day::Wednesday,
        Day::Thursday,
        Day::Friday,
        Day::Saturday,
        Day::Sunday,
    ];

    pub fn name(self) -> &'static str {
        match self {
            Day::Monday => "monday",
            Day::Tuesday => "tuesday",
            Day::Wednesday => "wednesday",
            Day::Thursday => "thursday",
            Day::Friday => "friday",
            Day::Saturday => "saturday",
            Day::Sunday => "sunday",
        }
    }

    pub fn parse(s: &str) -> Option<Day> {
        Day::ALL.into_iter().find(|d| d.name() == s)
    }
}

/// Returns steps day names from start on, with monday after sunday.
pub fn cycle(start: &str, steps: usize) -> Vec<&'static str> {
    let Some(day) = Day::parse(start) else {
        return Vec::new();
    };
    let i = day as usize;
    (0..steps).map(|k| Day::ALL[(i + k) % Day::ALL.len()].name()).collect()
}
