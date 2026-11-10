#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/sort-a-list-of-records/.

# sort_employees
# Reads records from stdin, one per line: NAME, DEPT and SALARY with a tab between them.
# Prints the records by DEPT, then by SALARY from high to low.
# Records with equal keys keep their input order.
sort_employees() {
  # LC_ALL=C compares bytes, so UTF-8 text sorts by code point, on every machine.
  # -k2,2 is field 2 only. -k3,3nr is field 3 as a number, from high to low.
  # -s turns off the last comparison of whole lines, so equal keys keep their order.
  LC_ALL=C sort -s -t $'\t' -k2,2 -k3,3nr
}
