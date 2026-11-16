// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/define-a-record-type/.

// readonly fields cannot be assigned after the object is built. The check
// is at compile time only.
export type Person = { readonly name: string; readonly age: number };

export function birthday(people: readonly Person[], name: string): Person[] {
  return people.map((p) => (p.name === name ? { ...p, age: p.age + 1 } : p));
}
