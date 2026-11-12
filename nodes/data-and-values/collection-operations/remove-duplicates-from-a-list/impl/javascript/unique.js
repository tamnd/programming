// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/remove-duplicates-from-a-list/.

/** @typedef {{ name: string, email: string }} User */

/**
 * Returns a new array of the items without later copies. The first copies keep their order.
 * @template T
 * @param {Iterable<T>} items
 * @returns {T[]}
 */
export function unique(items) {
  // A Set visits its values in insertion order. It compares them with SameValueZero.
  return [...new Set(items)];
}

/**
 * Returns a new array of the first item for each key, in input order.
 * @template T, K
 * @param {Iterable<T>} items
 * @param {(item: T) => K} key
 * @returns {T[]}
 */
export function uniqueBy(items, key) {
  const seen = new Set();
  const out = [];
  for (const item of items) {
    const k = key(item);
    if (!seen.has(k)) {
      seen.add(k);
      out.push(item);
    }
  }
  return out;
}

/**
 * Returns the first user for each email. Case does not matter in the emails.
 * @param {Iterable<User>} users
 * @returns {User[]}
 */
export function uniqueByEmail(users) {
  return uniqueBy(users, (u) => u.email.toLowerCase());
}
