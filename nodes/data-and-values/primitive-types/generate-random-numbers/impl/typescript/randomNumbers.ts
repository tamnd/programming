// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/generate-random-numbers/.

import { getRandomValues, randomBytes } from "node:crypto";

/** A function that returns a random integer from 0 to 2^32 - 1. */
export type WordSource = () => number;

const WORDS = 1n << 32n;
const MASK64 = (1n << 64n) - 1n;

/**
 * Returns an integer from lo to hi, each with equal probability. A word below
 * 2^32 % n is rejected, so that each remainder comes from the same number of
 * words.
 */
export function intInRange(nextWord: WordSource, lo: bigint, hi: bigint): bigint {
  const n = hi - lo + 1n;
  if (n < 1n || n > WORDS) {
    throw new RangeError("need lo <= hi and at most 2^32 values");
  }
  const least = WORDS % n;
  for (;;) {
    const word = BigInt(nextWord());
    if (word >= least) {
      return lo + (word % n);
    }
  }
}

/** Shuffles items in place with the Fisher-Yates method. */
export function shuffle<T>(nextWord: WordSource, items: T[]): void {
  for (let i = items.length - 1; i > 0; i--) {
    const j = Number(intInRange(nextWord, 0n, BigInt(i)));
    [items[i], items[j]] = [items[j] as T, items[i] as T];
  }
}

/**
 * Returns a word source that gives the same words for the same seed. Node.js
 * has no seeded generator, so this is SplitMix64 (Steele, Lea and Flood),
 * as in the reference code of Sebastiano Vigna, with the top 32 bits of each
 * output.
 */
export function seededWords(seed: bigint): WordSource {
  let state = BigInt.asUintN(64, seed);
  return () => {
    state = (state + 0x9e3779b97f4a7c15n) & MASK64;
    let z = state;
    z = ((z ^ (z >> 30n)) * 0xbf58476d1ce4e5b9n) & MASK64;
    z = ((z ^ (z >> 27n)) * 0x94d049bb133111ebn) & MASK64;
    z ^= z >> 31n;
    return Number(z >> 32n);
  };
}

/** Returns a word source from the secure generator. */
export function secureWords(): WordSource {
  return () => getRandomValues(new Uint32Array(1))[0] as number;
}

/** Returns nbytes secure random bytes as 2 * nbytes hexadecimal digits. */
export function tokenHex(nbytes: number): string {
  return randomBytes(nbytes).toString("hex");
}
