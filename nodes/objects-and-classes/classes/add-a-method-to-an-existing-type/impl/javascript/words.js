// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/add-a-method-to-an-existing-type/.

// Add wordCount to String.prototype, so that every string has it. The
// property is not enumerable, like the built-in methods, and the code refuses
// to replace a method that is already there.
if ("wordCount" in String.prototype) {
  throw new Error("String.prototype.wordCount is already defined");
}
Object.defineProperty(String.prototype, "wordCount", {
  value: function wordCount() {
    return this.split(/[ \t\n\r]+/).filter((w) => w !== "").length;
  },
  writable: true,
  configurable: true,
  enumerable: false,
});

export function wordCount(text) {
  return text.wordCount();
}
