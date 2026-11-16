// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/define-an-enumeration/.

// JavaScript has no enum. A frozen object maps each name to its value, and
// Object.values gives the values in the order of the definition.
export const Day = Object.freeze({
  MONDAY: "monday",
  TUESDAY: "tuesday",
  WEDNESDAY: "wednesday",
  THURSDAY: "thursday",
  FRIDAY: "friday",
  SATURDAY: "saturday",
  SUNDAY: "sunday",
});

const DAYS = Object.values(Day);

export function cycle(start, steps) {
  const i = DAYS.indexOf(start);
  if (i < 0) return [];
  return Array.from({ length: steps }, (_, k) => DAYS[(i + k) % DAYS.length]);
}
