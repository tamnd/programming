// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/take-the-first-n-items/.

/** Returns a copy of the first n items; a shorter array gives all its items. */
export function take(items: readonly number[], n: number): number[] {
  // slice stops at the end of the array, so n can be larger than the length.
  return items.slice(0, n);
}

/** The infinite sequence start, start + 1, ... It runs only when it is read. */
function* naturals(start: number): Generator<number> {
  for (let i = start; ; i++) {
    yield i;
  }
}

/** Returns the first n items of the infinite sequence start, start + 1, ... */
export function takeNaturals(start: number, n: number): number[] {
  const out: number[] = [];
  if (n === 0) {
    return out;
  }
  // The loop stops the generator after n items, so it never reads the rest.
  for (const i of naturals(start)) {
    out.push(i);
    if (out.length === n) {
      break;
    }
  }
  return out;
}
