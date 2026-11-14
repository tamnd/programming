// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-linked-list/.

import assert from "node:assert/strict";
import { test } from "node:test";

import { runOps } from "./linked_list.js";

const cases = [
  ["deque", ["push_back 1", "push_back 2", "push_front 0", "pop_back", "push_back 3"], [0, 1, 3]],
  ["remove-middle", ["push_back 1", "push_back 2", "push_back 3", "remove 2"], [1, 3]],
  ["empty", [], []],
  ["push-pop", ["push_back 1", "pop_front"], []],
  ["single", ["push_front 7"], [7]],
  ["remove-only", ["push_back 5", "remove 5", "push_back 6"], [6]],
  ["remove-head", ["push_back 1", "push_back 2", "remove 1", "push_front 0"], [0, 2]],
  ["remove-tail", ["push_back 1", "push_back 2", "remove 2", "push_back 3"], [1, 3]],
  ["pop-both-ends", ["push_back 1", "push_back 2", "pop_front", "pop_back", "push_front 3"], [3]],
  ["remove-absent", ["push_back 1", "remove 9"], [1]],
  ["duplicates", ["push_back 4", "push_back 5", "push_back 4", "remove 4"], [5, 4]],
  ["push-front-order", ["push_front 1", "push_front 2", "push_front 3"], [3, 2, 1]],
  ["negative", ["push_front -1", "push_back -2", "remove -1"], [-2]],
  ["limit", ["push_back 2147483647", "push_front -2147483648", "remove 2147483647", "push_back 2147483647"], [-2147483648, 2147483647]],
];

for (const [name, ops, want] of cases) {
  test(name, () => {
    assert.deepEqual(runOps(ops), want);
  });
}
