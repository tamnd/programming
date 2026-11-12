// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/sort-a-list-of-records/.

import assert from "node:assert/strict";
import { test } from "node:test";

import { type Employee, sortEmployees, sortNames } from "./sortEmployees.ts";

/** Makes records from [name, dept, salary] rows. */
const rows = (list: readonly (readonly [string, string, number])[]): Employee[] =>
  list.map(([name, dept, salary]) => ({ name, dept, salary }));

// The 40 records of the many-ties case: depts "b" and "a" in turn, all with salary 1.
const many: [string, string, number][] = Array.from({ length: 40 }, (_, i) => [
  `n${String(i + 1).padStart(2, "0")}`,
  i % 2 ? "a" : "b",
  1,
]);

const cases: Record<string, [[string, string, number][], [string, string, number][]]> = {
  mixed: [
    [["Ada", "eng", 120], ["Bob", "sales", 90], ["Cy", "eng", 150], ["Di", "ops", 100]],
    [["Cy", "eng", 150], ["Ada", "eng", 120], ["Di", "ops", 100], ["Bob", "sales", 90]],
  ],
  "same-salary-other-dept": [
    [["Ann", "ops", 100], ["Ben", "eng", 100], ["Cat", "hr", 100]],
    [["Ben", "eng", 100], ["Cat", "hr", 100], ["Ann", "ops", 100]],
  ],
  empty: [[], []],
  single: [[["Ada", "eng", 120]], [["Ada", "eng", 120]]],
  sorted: [
    [["Cy", "eng", 150], ["Ada", "eng", 120], ["Bob", "ops", 90]],
    [["Cy", "eng", 150], ["Ada", "eng", 120], ["Bob", "ops", 90]],
  ],
  reversed: [
    [["Bob", "ops", 90], ["Ada", "eng", 120], ["Cy", "eng", 150]],
    [["Cy", "eng", 150], ["Ada", "eng", 120], ["Bob", "ops", 90]],
  ],
  ties: [
    [["Zoe", "eng", 100], ["Xi", "hr", 80], ["Al", "eng", 100], ["Yu", "hr", 80], ["Mo", "eng", 100]],
    [["Zoe", "eng", 100], ["Al", "eng", 100], ["Mo", "eng", 100], ["Xi", "hr", 80], ["Yu", "hr", 80]],
  ],
  "many-ties": [many, [...many.filter((_, i) => i % 2), ...many.filter((_, i) => !(i % 2))]],
  case: [
    [["Ann", "eng", 100], ["Bea", "Sales", 90], ["Cal", "ops", 80]],
    [["Bea", "Sales", 90], ["Ann", "eng", 100], ["Cal", "ops", 80]],
  ],
  "non-ascii": [
    [["Eve", "équipe", 100], ["Fay", "zoo", 90], ["Gus", "eng", 80]],
    [["Gus", "eng", 80], ["Fay", "zoo", 90], ["Eve", "équipe", 100]],
  ],
  negative: [
    [["Hal", "eng", 0], ["Ida", "eng", -5], ["Jo", "eng", 5]],
    [["Jo", "eng", 5], ["Hal", "eng", 0], ["Ida", "eng", -5]],
  ],
  overflow: [
    [["Kim", "eng", -2000000000], ["Lee", "eng", 2000000000]],
    [["Lee", "eng", 2000000000], ["Kim", "eng", -2000000000]],
  ],
};

for (const [name, [given, want]] of Object.entries(cases)) {
  test(name, () => {
    const input = rows(given);
    assert.deepEqual(sortEmployees(input), rows(want));
    assert.deepEqual(input, rows(given), "the input must not change");
  });
}

// The cases of cases/locale.jsonl.
const locales: [string, string, string[], string[]][] = [
  ["en-accents", "en", ["zebra", "Émile", "apple", "Eve", "eagle"], ["apple", "eagle", "Émile", "Eve", "zebra"]],
  ["en-case", "en", ["b", "B", "a", "A"], ["a", "A", "b", "B"]],
  ["en-accent-order", "en", ["resume", "résumé", "Resume", "rèsume"], ["resume", "Resume", "résumé", "rèsume"]],
  ["sv-letters", "sv", ["ö", "z", "o", "a", "å", "ä"], ["a", "o", "z", "å", "ä", "ö"]],
  ["sv-names", "sv", ["Örjan", "Zara", "Olof", "Åsa", "Anna", "Ärla"], ["Anna", "Olof", "Zara", "Åsa", "Ärla", "Örjan"]],
  ["de-umlaut", "de", ["Äpfel", "Zebra", "Apfel", "apfel"], ["apfel", "Apfel", "Äpfel", "Zebra"]],
  ["da-aa", "da", ["Aase", "Zelda", "Ågot", "Øjvind", "Ærø"], ["Zelda", "Ærø", "Øjvind", "Ågot", "Aase"]],
  ["es-enye", "es", ["llama", "luz", "lzx", "ñu", "nube", "oso"], ["llama", "luz", "lzx", "nube", "ñu", "oso"]],
];

for (const [name, locale, names, want] of locales) {
  test(`locale ${name}`, () => {
    assert.deepEqual(sortNames(names, locale), want);
  });
}
