// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/sort-a-list-of-records/.

//! Sorts records of employees.

/// A record of an employee.
#[derive(Debug, Clone, PartialEq, Eq)]
pub struct Employee {
    pub name: String,
    pub dept: String,
    pub salary: i32,
}

/// Sorts the records in place by `dept`, then by `salary` from high to low.
/// The sort is stable, so records with equal keys keep their input order.
pub fn sort_employees(employees: &mut [Employee]) {
    // sort_by is stable. sort_unstable_by is not.
    // str::cmp compares bytes, so UTF-8 text sorts by code point.
    employees.sort_by(|a, b| {
        a.dept.cmp(&b.dept).then_with(|| b.salary.cmp(&a.salary)) // b before a: high to low
    });
}
