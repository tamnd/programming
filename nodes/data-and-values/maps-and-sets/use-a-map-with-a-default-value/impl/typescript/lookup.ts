// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-map-with-a-default-value/.

export function lookup(table: ReadonlyMap<string, number>, keys: readonly string[], fallback: number): number[] {
  // ?? uses the fallback only for undefined, so a stored 0 stays 0.
  // || would also replace 0.
  return keys.map((key) => table.get(key) ?? fallback);
}
