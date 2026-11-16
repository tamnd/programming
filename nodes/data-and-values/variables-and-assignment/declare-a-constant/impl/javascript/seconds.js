// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/declare-a-constant/.

// const makes a binding that gets no new value. The names in upper case
// are a convention for constants that are known before the program runs.
const SECONDS_PER_MINUTE = 60;
const MINUTES_PER_HOUR = 60;
const HOURS_PER_DAY = 24;
export const SECONDS_PER_DAY = SECONDS_PER_MINUTE * MINUTES_PER_HOUR * HOURS_PER_DAY;

/** Returns the number of seconds in days days of 24 hours. */
export function secondsIn(days) {
  return days * SECONDS_PER_DAY;
}
