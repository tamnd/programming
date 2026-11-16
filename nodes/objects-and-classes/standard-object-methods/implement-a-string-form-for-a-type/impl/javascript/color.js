// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/implement-a-string-form-for-a-type/.

const inspect = Symbol.for("nodejs.util.inspect.custom");

const hex = (v) => v.toString(16).toUpperCase().padStart(2, "0");

export class Color {
  #r;
  #g;
  #b;

  constructor(r, g, b) {
    for (const v of [r, g, b]) {
      if (!Number.isInteger(v) || v < 0 || v > 255) {
        throw new RangeError(`not a channel value: ${v}`);
      }
    }
    this.#r = r;
    this.#g = g;
    this.#b = b;
  }

  // The display form: String(c), template literals and + call it.
  toString() {
    return `#${hex(this.#r)}${hex(this.#g)}${hex(this.#b)}`;
  }

  // The debug form for console.log and util.inspect in Node.js.
  [inspect]() {
    return `Color(r=${this.#r}, g=${this.#g}, b=${this.#b})`;
  }
}

export function formatColors(reds, greens, blues) {
  if (reds.length !== greens.length || reds.length !== blues.length) {
    throw new RangeError("the lists must have the same length");
  }
  return reds.map((r, i) => `${new Color(r, greens[i], blues[i])}`);
}
