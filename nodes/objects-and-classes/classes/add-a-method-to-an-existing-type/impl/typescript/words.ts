// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/add-a-method-to-an-existing-type/.

// Tell the compiler that every string has wordCount. Interface declarations
// merge, so this adds a member to the global String interface.
declare global {
  interface String {
    wordCount(): number;
  }
}

// Then add the method at run time, as in JavaScript.
if ("wordCount" in String.prototype) {
  throw new Error("String.prototype.wordCount is already defined");
}
Object.defineProperty(String.prototype, "wordCount", {
  value: function wordCount(this: string): number {
    return this.split(/[ \t\n\r]+/).filter((w) => w !== "").length;
  },
  writable: true,
  configurable: true,
  enumerable: false,
});

export function wordCount(text: string): number {
  return text.wordCount();
}
