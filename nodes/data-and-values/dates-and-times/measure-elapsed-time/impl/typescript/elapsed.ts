// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/measure-elapsed-time/.

import { hrtime } from "node:process";

/**
 * Runs work once and returns the time it took, in nanoseconds of clock.
 * hrtime.bigint is relative to an arbitrary time in the past and is not
 * related to the time of day. A bigint has no rounding.
 */
export function measure(work: () => unknown, clock: () => bigint = hrtime.bigint): bigint {
  const start = clock();
  work();
  return clock() - start;
}

/** Returns endNs - startNs as "M.UUU ms", with the rest under one microsecond dropped. */
export function elapsedMs(startNs: bigint, endNs: bigint): string {
  const ns = endNs - startNs;
  // Division of bigint values truncates, and ns is not negative.
  const micros = String((ns / 1_000n) % 1_000n).padStart(3, "0");
  return `${ns / 1_000_000n}.${micros} ms`;
}
