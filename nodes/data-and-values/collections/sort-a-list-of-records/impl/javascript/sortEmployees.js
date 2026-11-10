// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/sort-a-list-of-records/.

/** @typedef {{ name: string, dept: string, salary: number }} Employee */

/**
 * Returns a new array of the records by dept, then by salary from high to low.
 * The sort is stable, so records with equal keys keep their input order.
 * @param {readonly Employee[]} employees
 * @returns {Employee[]}
 */
export function sortEmployees(employees) {
  // < compares strings by UTF-16 code units, the same for every locale.
  // The salaries are 32-bit integers, so b.salary - a.salary is exact.
  return employees.toSorted(
    (a, b) => (a.dept < b.dept ? -1 : a.dept > b.dept ? 1 : 0) || b.salary - a.salary,
  );
}

/**
 * Returns a new array of the names in the order of the collation rules of a locale.
 * @param {readonly string[]} names
 * @param {string} locale a BCP 47 language tag, such as "sv"
 * @returns {string[]}
 */
export function sortNames(names, locale) {
  return names.toSorted(new Intl.Collator(locale).compare);
}
