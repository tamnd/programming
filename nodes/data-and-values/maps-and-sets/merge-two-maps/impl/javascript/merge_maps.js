// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/merge-two-maps/.

/**
 * @param {Map<string, number>} base
 * @param {Map<string, number>} override
 * @returns {[string, number][]}
 */
export function mergeMaps(base, override) {
  // The entries of override come later, so their values win.
  const merged = new Map([...base, ...override]);
  return [...merged].sort((p, q) => (p[0] < q[0] ? -1 : p[0] > q[0] ? 1 : 0));
}
