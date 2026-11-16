// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-mathematical-constants-and-functions/.

/** Returns the point [x, y] at the distance r and the angle in degrees. */
export function toCartesian(r: number, degrees: number): [number, number] {
  const t = (degrees * Math.PI) / 180;
  return [r * Math.cos(t), r * Math.sin(t)];
}
