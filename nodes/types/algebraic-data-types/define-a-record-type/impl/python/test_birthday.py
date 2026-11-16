# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/define-a-record-type/.

import dataclasses

import pytest

from birthday import Person, birthday

CASES = {
    "first": ([("Ada", 36), ("Alan", 41)], "Ada", [("Ada", 37), ("Alan", 41)]),
    "second": ([("Ada", 36), ("Alan", 41)], "Alan", [("Ada", 36), ("Alan", 42)]),
    "empty": ([], "Ada", []),
    "not-found": ([("Ada", 36)], "Grace", [("Ada", 36)]),
    "duplicate-name": (
        [("Ada", 36), ("Alan", 41), ("Ada", 9)],
        "Ada",
        [("Ada", 37), ("Alan", 41), ("Ada", 10)],
    ),
    "zero-age": ([("Baby", 0)], "Baby", [("Baby", 1)]),
    "non-ascii": ([("Zoë", 20), ("Zoe", 30)], "Zoë", [("Zoë", 21), ("Zoe", 30)]),
    "case": ([("ada", 36)], "Ada", [("ada", 36)]),
}


@pytest.mark.parametrize("case", CASES)
def test_birthday(case):
    pairs, name, want = CASES[case]
    people = [Person(n, a) for n, a in pairs]
    before = list(people)
    got = birthday(people, name)
    assert [(p.name, p.age) for p in got] == want
    assert people == before


def test_equality_and_printing():
    assert Person("Ada", 36) == Person("Ada", 36)
    assert Person("Ada", 36) != Person("Ada", 37)
    assert repr(Person("Ada", 36)) == "Person(name='Ada', age=36)"


def test_fields_are_read_only():
    with pytest.raises(dataclasses.FrozenInstanceError):
        Person("Ada", 36).age = 37
