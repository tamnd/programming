// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/define-a-record-type/.

import assert from "node:assert/strict";
import { test } from "node:test";

import { birthday, makePerson } from "./birthday.js";

const cases = [
  ["first", [["Ada", 36], ["Alan", 41]], "Ada", [["Ada", 37], ["Alan", 41]]],
  ["second", [["Ada", 36], ["Alan", 41]], "Alan", [["Ada", 36], ["Alan", 42]]],
  ["empty", [], "Ada", []],
  ["not-found", [["Ada", 36]], "Grace", [["Ada", 36]]],
  ["duplicate-name", [["Ada", 36], ["Alan", 41], ["Ada", 9]], "Ada", [["Ada", 37], ["Alan", 41], ["Ada", 10]]],
  ["zero-age", [["Baby", 0]], "Baby", [["Baby", 1]]],
  ["non-ascii", [["Zoë", 20], ["Zoe", 30]], "Zoë", [["Zoë", 21], ["Zoe", 30]]],
  ["case", [["ada", 36]], "Ada", [["ada", 36]]],
];

for (const [name, pairs, who, want] of cases) {
  test(name, () => {
    const people = pairs.map(([n, a]) => makePerson(n, a));
    const before = JSON.stringify(people);
    const got = birthday(people, who);
    assert.deepEqual(got.map((p) => [p.name, p.age]), want);
    assert.equal(JSON.stringify(people), before);
  });
}

test("equality is by reference", () => {
  assert.notEqual(makePerson("Ada", 36), makePerson("Ada", 36));
  assert.deepEqual(makePerson("Ada", 36), makePerson("Ada", 36));
});
