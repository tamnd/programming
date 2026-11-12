// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/sort-a-list-of-records/.

use algo_sort_a_list_of_records::{Employee, sort_employees};

type Row = (&'static str, &'static str, i32);

fn records(rows: &[Row]) -> Vec<Employee> {
    rows.iter()
        .map(|&(name, dept, salary)| Employee {
            name: name.to_string(),
            dept: dept.to_string(),
            salary,
        })
        .collect()
}

/// The 40 records of the many-ties case: depts "b" and "a" in turn, all with salary 1.
fn many() -> Vec<Employee> {
    (0..40)
        .map(|i| Employee {
            name: format!("n{:02}", i + 1),
            dept: if i % 2 == 1 { "a" } else { "b" }.to_string(),
            salary: 1,
        })
        .collect()
}

fn check(name: &str, mut got: Vec<Employee>, want: Vec<Employee>) {
    sort_employees(&mut got);
    assert_eq!(got, want, "case {name}");
}

#[test]
fn cases() {
    let cases: &[(&str, &[Row], &[Row])] = &[
        (
            "mixed",
            &[
                ("Ada", "eng", 120),
                ("Bob", "sales", 90),
                ("Cy", "eng", 150),
                ("Di", "ops", 100),
            ],
            &[
                ("Cy", "eng", 150),
                ("Ada", "eng", 120),
                ("Di", "ops", 100),
                ("Bob", "sales", 90),
            ],
        ),
        (
            "same-salary-other-dept",
            &[("Ann", "ops", 100), ("Ben", "eng", 100), ("Cat", "hr", 100)],
            &[("Ben", "eng", 100), ("Cat", "hr", 100), ("Ann", "ops", 100)],
        ),
        ("empty", &[], &[]),
        ("single", &[("Ada", "eng", 120)], &[("Ada", "eng", 120)]),
        (
            "sorted",
            &[("Cy", "eng", 150), ("Ada", "eng", 120), ("Bob", "ops", 90)],
            &[("Cy", "eng", 150), ("Ada", "eng", 120), ("Bob", "ops", 90)],
        ),
        (
            "reversed",
            &[("Bob", "ops", 90), ("Ada", "eng", 120), ("Cy", "eng", 150)],
            &[("Cy", "eng", 150), ("Ada", "eng", 120), ("Bob", "ops", 90)],
        ),
        (
            "ties",
            &[
                ("Zoe", "eng", 100),
                ("Xi", "hr", 80),
                ("Al", "eng", 100),
                ("Yu", "hr", 80),
                ("Mo", "eng", 100),
            ],
            &[
                ("Zoe", "eng", 100),
                ("Al", "eng", 100),
                ("Mo", "eng", 100),
                ("Xi", "hr", 80),
                ("Yu", "hr", 80),
            ],
        ),
        (
            "case",
            &[
                ("Ann", "eng", 100),
                ("Bea", "Sales", 90),
                ("Cal", "ops", 80),
            ],
            &[
                ("Bea", "Sales", 90),
                ("Ann", "eng", 100),
                ("Cal", "ops", 80),
            ],
        ),
        (
            "non-ascii",
            &[
                ("Eve", "équipe", 100),
                ("Fay", "zoo", 90),
                ("Gus", "eng", 80),
            ],
            &[
                ("Gus", "eng", 80),
                ("Fay", "zoo", 90),
                ("Eve", "équipe", 100),
            ],
        ),
        (
            "negative",
            &[("Hal", "eng", 0), ("Ida", "eng", -5), ("Jo", "eng", 5)],
            &[("Jo", "eng", 5), ("Hal", "eng", 0), ("Ida", "eng", -5)],
        ),
        (
            "overflow",
            &[
                ("Kim", "eng", -2_000_000_000),
                ("Lee", "eng", 2_000_000_000),
            ],
            &[
                ("Lee", "eng", 2_000_000_000),
                ("Kim", "eng", -2_000_000_000),
            ],
        ),
    ];
    for &(name, given, want) in cases {
        check(name, records(given), records(want));
    }
}

#[test]
fn many_ties() {
    let given = many();
    let (odd, even): (Vec<_>, Vec<_>) = given
        .iter()
        .cloned()
        .enumerate()
        .partition(|(i, _)| i % 2 == 1);
    let want = odd.into_iter().chain(even).map(|(_, e)| e).collect();
    check("many-ties", given, want);
}
