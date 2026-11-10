# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/sort-a-list-of-records/.

import pytest

from sort_employees import Employee, sort_employees

# The 40 records of the many-ties case: depts "b" and "a" in turn, all with salary 1.
MANY = [(f"n{i + 1:02d}", "a" if i % 2 else "b", 1) for i in range(40)]

CASES = {
    "mixed": (
        [
            ("Ada", "eng", 120),
            ("Bob", "sales", 90),
            ("Cy", "eng", 150),
            ("Di", "ops", 100),
        ],
        [
            ("Cy", "eng", 150),
            ("Ada", "eng", 120),
            ("Di", "ops", 100),
            ("Bob", "sales", 90),
        ],
    ),
    "same-salary-other-dept": (
        [("Ann", "ops", 100), ("Ben", "eng", 100), ("Cat", "hr", 100)],
        [("Ben", "eng", 100), ("Cat", "hr", 100), ("Ann", "ops", 100)],
    ),
    "empty": ([], []),
    "single": ([("Ada", "eng", 120)], [("Ada", "eng", 120)]),
    "sorted": (
        [("Cy", "eng", 150), ("Ada", "eng", 120), ("Bob", "ops", 90)],
        [("Cy", "eng", 150), ("Ada", "eng", 120), ("Bob", "ops", 90)],
    ),
    "reversed": (
        [("Bob", "ops", 90), ("Ada", "eng", 120), ("Cy", "eng", 150)],
        [("Cy", "eng", 150), ("Ada", "eng", 120), ("Bob", "ops", 90)],
    ),
    "ties": (
        [
            ("Zoe", "eng", 100),
            ("Xi", "hr", 80),
            ("Al", "eng", 100),
            ("Yu", "hr", 80),
            ("Mo", "eng", 100),
        ],
        [
            ("Zoe", "eng", 100),
            ("Al", "eng", 100),
            ("Mo", "eng", 100),
            ("Xi", "hr", 80),
            ("Yu", "hr", 80),
        ],
    ),
    "many-ties": (MANY, MANY[1::2] + MANY[0::2]),
    "case": (
        [("Ann", "eng", 100), ("Bea", "Sales", 90), ("Cal", "ops", 80)],
        [("Bea", "Sales", 90), ("Ann", "eng", 100), ("Cal", "ops", 80)],
    ),
    "non-ascii": (
        [("Eve", "équipe", 100), ("Fay", "zoo", 90), ("Gus", "eng", 80)],
        [("Gus", "eng", 80), ("Fay", "zoo", 90), ("Eve", "équipe", 100)],
    ),
    "negative": (
        [("Hal", "eng", 0), ("Ida", "eng", -5), ("Jo", "eng", 5)],
        [("Jo", "eng", 5), ("Hal", "eng", 0), ("Ida", "eng", -5)],
    ),
    "overflow": (
        [("Kim", "eng", -2000000000), ("Lee", "eng", 2000000000)],
        [("Lee", "eng", 2000000000), ("Kim", "eng", -2000000000)],
    ),
}


@pytest.mark.parametrize("name", CASES)
def test_cases(name: str) -> None:
    given, want = CASES[name]
    employees = [Employee(*e) for e in given]
    got = sort_employees(employees)
    assert got == [Employee(*e) for e in want]
    assert employees == [Employee(*e) for e in given], "the input must not change"
