// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/define-a-custom-error-type/.

import assert from "node:assert/strict";
import { test } from "node:test";

import { checkPort, parsePort, PortError } from "./checkPort.ts";

const cases: [string, string, string][] = [
  ["valid", "8080", "port 8080"],
  ["not-a-number", "x80", "invalid port 'x80': not a number"],
  ["too-large", "65536", "invalid port '65536': out of range"],
  ["empty", "", "invalid port '': empty"],
  ["zero", "0", "invalid port '0': out of range"],
  ["one", "1", "port 1"],
  ["largest", "65535", "port 65535"],
  ["leading-zeros", "0080", "port 80"],
  ["negative", "-1", "invalid port '-1': not a number"],
  ["leading-space", " 80", "invalid port ' 80': not a number"],
  ["overflow", "99999999999999999999", "invalid port '99999999999999999999': out of range"],
];

for (const [name, text, want] of cases) {
  test(name, () => {
    assert.equal(checkPort(text), want);
  });
}

test("fields", () => {
  assert.throws(
    () => parsePort("x80"),
    (e: unknown) => e instanceof PortError && e.name === "PortError" && e.text === "x80" && e.reason === "not a number",
  );
});
