// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/sleep-for-a-duration/.

import { hrtime } from "node:process";
import { setTimeout as delay } from "node:timers/promises";

/** The largest delay of a Node.js timer, in milliseconds. */
const MAX_DELAY_MS = 2147483647n;

/**
 * Sleeps until clock() reaches the deadline. A sleep that ends early is
 * followed by a sleep for the rest of the time.
 * @param {bigint} durationNs
 * @param {() => bigint} clock
 * @param {(ns: bigint) => Promise<void> | void} sleep
 * @returns {Promise<void>}
 */
export async function sleepLoop(durationNs, clock, sleep) {
  let now = clock();
  const deadline = now + durationNs;
  while (now < deadline) {
    await sleep(deadline - now);
    now = clock();
  }
}

/**
 * Waits on a timer for ns nanoseconds, rounded up to whole milliseconds and
 * cut to the largest delay of a timer.
 * @param {bigint} ns
 * @returns {Promise<void>}
 */
function timerSleep(ns) {
  let ms = (ns + 999_999n) / 1_000_000n;
  if (ms > MAX_DELAY_MS) ms = MAX_DELAY_MS;
  return delay(Number(ms));
}

/**
 * Resolves after at least durationNs nanoseconds. Other code runs in the
 * meantime.
 * @param {bigint} durationNs
 * @returns {Promise<void>}
 */
export function sleepFor(durationNs) {
  return sleepLoop(durationNs, hrtime.bigint, timerSleep);
}

/**
 * Runs sleepLoop on a fake clock and returns the durations it asks for.
 * @param {bigint} durationNs
 * @param {bigint} startNs
 * @param {bigint[]} readings
 * @returns {Promise<bigint[]>}
 */
export async function sleepRequests(durationNs, startNs, readings) {
  const times = [startNs, ...readings];
  let i = 0;
  const requests = [];
  await sleepLoop(durationNs, () => times[i++], (ns) => {
    requests.push(ns);
  });
  return requests;
}
