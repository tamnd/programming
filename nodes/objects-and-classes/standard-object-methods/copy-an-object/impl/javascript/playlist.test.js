// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/copy-an-object/.

import assert from "node:assert/strict";
import { test } from "node:test";

import { Playlist, copyAndChange } from "./playlist.js";

const cases = [
  ["middle", [["Intro", "Song", "Outro"], 1, "Hit"], [["Intro", "Hit", "Outro"], ["Intro", "Hit", "Outro"], ["Intro", "Song", "Outro"]]],
  ["two", [["A", "B"], 0, "C"], [["C", "B"], ["C", "B"], ["A", "B"]]],
  ["single", [["Solo"], 0, "Duet"], [["Duet"], ["Duet"], ["Solo"]]],
  ["first", [["a", "b", "c"], 0, "z"], [["z", "b", "c"], ["z", "b", "c"], ["a", "b", "c"]]],
  ["last", [["a", "b", "c"], 2, "z"], [["a", "b", "z"], ["a", "b", "z"], ["a", "b", "c"]]],
  ["same-title", [["a", "b"], 1, "b"], [["a", "b"], ["a", "b"], ["a", "b"]]],
  ["duplicates", [["x", "x", "x"], 1, "y"], [["x", "y", "x"], ["x", "y", "x"], ["x", "x", "x"]]],
];

for (const [name, args, want] of cases) {
  test(name, () => {
    assert.deepEqual(copyAndChange(...args), want);
  });
}

test("copies are new objects", () => {
  const original = new Playlist("mix", ["a"]);
  const shallow = original.shallowCopy();
  const deep = original.deepCopy();
  assert.ok(shallow !== original && deep !== original);
  assert.ok(shallow.songs === original.songs && deep.songs !== original.songs);
  shallow.name = "other";
  assert.equal(original.name, "mix");
});

test("structuredClone drops the class", () => {
  const copy = structuredClone(new Playlist("mix", ["a"]));
  assert.ok(!(copy instanceof Playlist));
  assert.deepEqual(copy.songs, ["a"]);
});

test("bad index", () => {
  assert.throws(() => copyAndChange(["a"], 1, "b"), RangeError);
});
