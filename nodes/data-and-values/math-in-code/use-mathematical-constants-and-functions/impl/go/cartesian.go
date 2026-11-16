// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-mathematical-constants-and-functions/.

// Package cartesian turns a distance and an angle in degrees into the
// coordinates x and y.
package cartesian

import "math"

// ToCartesian returns the point x, y at the distance r and the angle in
// degrees.
func ToCartesian(r, degrees float64) (x, y float64) {
	t := degrees * math.Pi / 180
	return r * math.Cos(t), r * math.Sin(t)
}
