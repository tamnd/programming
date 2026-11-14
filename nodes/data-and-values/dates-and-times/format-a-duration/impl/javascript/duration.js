// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/format-a-duration/.

const pad = (n) => String(n).padStart(2, "0");

// Returns the duration as HH:MM:SS. The hours do not wrap at 24. The seconds
// must be a safe integer, so the arithmetic on numbers is exact.
export function formatDuration(seconds) {
  const sign = seconds < 0 ? "-" : "";
  const a = Math.abs(seconds);
  const hours = Math.floor(a / 3600);
  const minutes = Math.floor((a % 3600) / 60);
  return `${sign}${pad(hours)}:${pad(minutes)}:${pad(a % 60)}`;
}
