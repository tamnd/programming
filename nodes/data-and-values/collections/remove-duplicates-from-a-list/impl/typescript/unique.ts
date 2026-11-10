// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/remove-duplicates-from-a-list/.

export interface User {
  readonly name: string;
  readonly email: string;
}

/** Returns a new array of the items without later copies. The first copies keep their order. */
export function unique<T>(items: Iterable<T>): T[] {
  // A Set visits its values in insertion order. It compares them with SameValueZero.
  return [...new Set(items)];
}

/** Returns a new array of the first item for each key, in input order. */
export function uniqueBy<T, K>(items: Iterable<T>, key: (item: T) => K): T[] {
  const seen = new Set<K>();
  const out: T[] = [];
  for (const item of items) {
    const k = key(item);
    if (!seen.has(k)) {
      seen.add(k);
      out.push(item);
    }
  }
  return out;
}

/** Returns the first user for each email. Case does not matter in the emails. */
export function uniqueByEmail(users: Iterable<User>): User[] {
  return uniqueBy(users, (u) => u.email.toLowerCase());
}
