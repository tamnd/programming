// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/read-an-environment-variable/.

import assert from "node:assert/strict";
import { test } from "node:test";

import { getEnv } from "./getEnv.js";

// [case, variables to set, name to read, default, expected value]
const cases = [
  ["present", { APP_PORT: "8080" }, "APP_PORT", "80", "8080"],
  ["path-value", { APP_HOME: "/srv/app" }, "APP_HOME", "/opt/app", "/srv/app"],
  ["missing", {}, "APP_PORT", "80", "80"],
  ["empty-value", { APP_MODE: "" }, "APP_MODE", "production", ""],
  ["empty-default", {}, "APP_MODE", "", ""],
  ["spaces", { APP_TITLE: "  two  spaces  " }, "APP_TITLE", "none", "  two  spaces  "],
  ["equals", { APP_QUERY: "key=value=more" }, "APP_QUERY", "none", "key=value=more"],
  ["newline", { APP_TEXT: "first\nsecond" }, "APP_TEXT", "none", "first\nsecond"],
  ["non-ascii", { APP_GREETING: "xin chào, 日本語, 🙂" }, "APP_GREETING", "none", "xin chào, 日本語, 🙂"],
  ["case", { APP_PORT: "8080" }, "app_port", "80", "80"],
  ["inherited-name", {}, "toString", "none", "none"],
];

for (const [name, env, key, defaultValue, want] of cases) {
  test(name, () => {
    const saved = { ...process.env };
    try {
      delete process.env[key];
      Object.assign(process.env, env);
      assert.equal(getEnv(key, defaultValue), want);
    } finally {
      for (const k of Object.keys(process.env)) if (!Object.hasOwn(saved, k)) delete process.env[k];
      Object.assign(process.env, saved);
    }
  });
}
