// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/implement-ordering-for-a-type/.

/** A version MAJOR.MINOR.PATCH. JavaScript has no natural order for objects, so the class gives a compare function. */
export class Version {
  #major: number;
  #minor: number;
  #patch: number;

  constructor(major: number, minor: number, patch: number) {
    this.#major = major;
    this.#minor = minor;
    this.#patch = patch;
    Object.freeze(this);
  }

  static parse(text: string): Version {
    const [major, minor, patch] = text.split(".").map(Number);
    return new Version(major, minor, patch);
  }

  /** Returns a negative number, zero or a positive number, as Array.prototype.sort expects. */
  static compare(a: Version, b: Version): number {
    return a.#major - b.#major || a.#minor - b.#minor || a.#patch - b.#patch;
  }

  compareTo(other: Version): number {
    return Version.compare(this, other);
  }

  equals(other: unknown): boolean {
    return other instanceof Version && Version.compare(this, other) === 0;
  }

  toString(): string {
    return `${this.#major}.${this.#minor}.${this.#patch}`;
  }
}

export function sortVersions(versions: string[]): string[] {
  return versions
    .map((s) => Version.parse(s))
    .sort(Version.compare)
    .map((v) => v.toString());
}
