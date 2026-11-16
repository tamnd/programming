// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/start-a-thread/.

import assert from "node:assert/strict";
import { test } from "node:test";

import { threadGreetings } from "./greetings.ts";

const many = Array.from({ length: 32 }, (_, i) => `t${i}`);

const cases: [string, string[], string[]][] = [
  ["two", ["Ada", "Linus"], ["hello from Ada", "hello from Linus"]],
  ["three", ["main", "worker", "logger"], ["hello from main", "hello from worker", "hello from logger"]],
  ["empty", [], []],
  ["single", ["solo"], ["hello from solo"]],
  ["empty-name", [""], ["hello from "]],
  ["duplicate", ["twin", "twin"], ["hello from twin", "hello from twin"]],
  ["unicode", ["Zoë", "李雷"], ["hello from Zoë", "hello from 李雷"]],
  ["many", many, many.map((n) => `hello from ${n}`)],
];

for (const [name, names, want] of cases) {
  test(name, async () => {
    assert.deepEqual(await threadGreetings(names), want);
  });
}
