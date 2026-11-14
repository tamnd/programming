// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-map-with-a-default-value/.

/**
 * @param {Map<string, number>} table
 * @param {string[]} keys
 * @param {number} fallback
 * @returns {number[]}
 */
export function lookup(table, keys, fallback) {
  // ?? uses the fallback only for undefined, so a stored 0 stays 0.
  // || would also replace 0.
  return keys.map((key) => table.get(key) ?? fallback);
}
