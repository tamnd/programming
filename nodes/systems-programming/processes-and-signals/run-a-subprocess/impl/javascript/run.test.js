// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/run-a-subprocess/.

import assert from "node:assert/strict";
import { test } from "node:test";

import { run } from "./run.js";

// [case, argv, expected [status, stdout, stderr]]
const cases = [
  ["hello", ["printf", "hello"], [0, "hello", ""]],
  ["two-arguments", ["echo", "one", "two"], [0, "one two\n", ""]],
  ["exit-status", ["sh", "-c", "exit 3"], [3, "", ""]],
  ["both-streams", ["sh", "-c", "echo out; echo err >&2; exit 2"], [2, "out\n", "err\n"]],
  ["metacharacters", ["printf", "%s|", "a b", "$HOME", "*", "x;y", '"q"', "'s'"], [0, "a b|$HOME|*|x;y|\"q\"|'s'|", ""]],
  ["empty-argument", ["printf", "[%s]", ""], [0, "[]", ""]],
  ["newline-argument", ["printf", "%s", "line one\nline two"], [0, "line one\nline two", ""]],
  ["trailing-newlines", ["printf", "a\\n\\n"], [0, "a\n\n", ""]],
  ["non-ascii", ["printf", "%s\\n", "xin chào, 日本語, 🙂"], [0, "xin chào, 日本語, 🙂\n", ""]],
  ["stderr-only", ["sh", "-c", "echo warning >&2"], [0, "", "warning\n"]],
  ["no-output", ["true"], [0, "", ""]],
  ["false", ["false"], [1, "", ""]],
  ["status-255", ["sh", "-c", "exit 255"], [255, "", ""]],
  ["empty-stdin", ["sh", "-c", "if read -r line; then echo got; else echo eof; fi"], [0, "eof\n", ""]],
];

for (const [name, argv, want] of cases) {
  test(name, () => {
    assert.deepEqual(run(argv), want);
  });
}

test("large output on both streams", () => {
  const script = "head -c 1048576 /dev/zero | tr '\\0' x; head -c 1048576 /dev/zero | tr '\\0' y >&2";
  const [status, out, err] = run(["sh", "-c", script]);
  assert.equal(status, 0);
  assert.equal(out, "x".repeat(1048576));
  assert.equal(err, "y".repeat(1048576));
});

test("missing program", () => {
  assert.throws(() => run(["no-such-program-7f3a"]), { code: "ENOENT" });
});

test("signal", () => {
  assert.throws(() => run(["sh", "-c", "kill -9 $$"]), /signal SIGKILL/);
});
