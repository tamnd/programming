# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/sort-a-list-of-records/.

from collections.abc import Iterable
from dataclasses import dataclass


@dataclass(frozen=True)
class Employee:
    name: str
    dept: str
    salary: int


def sort_employees(employees: Iterable[Employee]) -> list[Employee]:
    """Return the records by dept, then by salary from high to low.

    The sort is stable, so records with equal keys keep their input order.
    """
    # The key is a tuple: tuples compare item by item. -salary puts high first.
    return sorted(employees, key=lambda e: (e.dept, -e.salary))
