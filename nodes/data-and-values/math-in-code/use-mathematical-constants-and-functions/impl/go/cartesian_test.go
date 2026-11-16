// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-mathematical-constants-and-functions/.

package cartesian

import (
	"math"
	"testing"
)

var cases = []struct {
	name       string
	r, degrees float64
	x, y       float64
}{
	{"east", 1.0, 0.0, 1.0, 0.0},
	{"north", 2.0, 90.0, 0.0, 2.0},
	{"west", 2.0, 180.0, -2.0, 0.0},
	{"diagonal", 1.0, 45.0, 0.7071067811865476, 0.7071067811865476},
	{"thirty-degrees", 2.0, 30.0, 1.7320508075688772, 1.0},
	{"zero-radius", 0.0, 123.0, 0.0, 0.0},
	{"south", 1.0, 270.0, 0.0, -1.0},
	{"negative-angle", 1.0, -90.0, 0.0, -1.0},
	{"full-turn", 1.0, 360.0, 1.0, 0.0},
	{"two-turns", 3.0, 720.0, 3.0, 0.0},
	{"negative-radius", -1.0, 0.0, -1.0, 0.0},
	{"large-radius", 1000000.0, 60.0, 500000.0, 866025.4037844386},
}

func near(got, want float64) bool {
	return math.Abs(got-want) <= math.Max(1e-9, 1e-12*math.Abs(want))
}

func TestToCartesian(t *testing.T) {
	for _, c := range cases {
		t.Run(c.name, func(t *testing.T) {
			x, y := ToCartesian(c.r, c.degrees)
			if !near(x, c.x) || !near(y, c.y) {
				t.Fatalf("got (%v, %v), want (%v, %v)", x, y, c.x, c.y)
			}
		})
	}
}

func TestConstants(t *testing.T) {
	if !near(math.Atan2(0, -1), math.Pi) || math.Sqrt(16) != 4 || math.Pow(2, 10) != 1024 {
		t.Fatal("a constant or a function is wrong")
	}
}
