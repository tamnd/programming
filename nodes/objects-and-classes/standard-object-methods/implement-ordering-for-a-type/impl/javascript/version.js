// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/implement-ordering-for-a-type/.

/** A version MAJOR.MINOR.PATCH. JavaScript has no natural order for objects, so the class gives a compare function. */
export class Version {
  #major;
  #minor;
  #patch;

  constructor(major, minor, patch) {
    this.#major = major;
    this.#minor = minor;
    this.#patch = patch;
    Object.freeze(this);
  }

  static parse(text) {
    const [major, minor, patch] = text.split(".").map(Number);
    return new Version(major, minor, patch);
  }

  /** Returns a negative number, zero or a positive number, as Array.prototype.sort expects. */
  static compare(a, b) {
    return a.#major - b.#major || a.#minor - b.#minor || a.#patch - b.#patch;
  }

  compareTo(other) {
    return Version.compare(this, other);
  }

  equals(other) {
    return other instanceof Version && Version.compare(this, other) === 0;
  }

  toString() {
    return `${this.#major}.${this.#minor}.${this.#patch}`;
  }
}

export function sortVersions(versions) {
  return versions
    .map((s) => Version.parse(s))
    .sort(Version.compare)
    .map((v) => v.toString());
}
