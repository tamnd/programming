// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/define-an-enumeration/.

// An object with "as const" in place of a TypeScript enum. Its values have
// literal types, and the type Day is the union of them.
export const Day = {
  Monday: "monday",
  Tuesday: "tuesday",
  Wednesday: "wednesday",
  Thursday: "thursday",
  Friday: "friday",
  Saturday: "saturday",
  Sunday: "sunday",
} as const;

export type Day = (typeof Day)[keyof typeof Day];

const DAYS: readonly Day[] = Object.values(Day);

// isDay converts a string to a Day: after the check, TypeScript knows that s
// is a Day.
function isDay(s: string): s is Day {
  return (DAYS as readonly string[]).includes(s);
}

export function cycle(start: string, steps: number): Day[] {
  if (!isDay(start)) return [];
  const i = DAYS.indexOf(start);
  return Array.from({ length: steps }, (_, k) => DAYS[(i + k) % DAYS.length]);
}
