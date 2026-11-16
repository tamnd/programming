// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/do-complex-number-arithmetic/.

package complexarith

import (
	"math"
	"testing"
)

var cases = []struct {
	name string
	a, b []float64
	want []float64
}{
	{"textbook", []float64{3.0, 4.0}, []float64{1.0, -2.0}, []float64{4.0, 2.0, 11.0, -2.0, -1.0, 2.0, 5.0, 0.9272952180016122}},
	{"rosetta", []float64{1.0, 1.0}, []float64{3.14159, 1.25}, []float64{4.14159, 2.25, 1.8915899999999999, 4.39159, 0.38414593243590134, 0.16546321590504276, 1.4142135623730951, 0.7853981633974483}},
	{"real-only", []float64{6.0, 0.0}, []float64{3.0, 0.0}, []float64{9.0, 0.0, 18.0, 0.0, 2.0, 0.0, 6.0, 0.0}},
	{"imaginary-unit", []float64{0.0, 1.0}, []float64{0.0, 1.0}, []float64{0.0, 2.0, -1.0, 0.0, 1.0, 0.0, 1.0, 1.5707963267948966}},
	{"negative-real", []float64{-2.0, 0.0}, []float64{1.0, 0.0}, []float64{-1.0, 0.0, -2.0, 0.0, -2.0, 0.0, 2.0, 3.141592653589793}},
	{"zero", []float64{0.0, 0.0}, []float64{2.0, -3.0}, []float64{2.0, -3.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}},
	{"negative-phase", []float64{1.0, -1.0}, []float64{-1.0, -1.0}, []float64{0.0, -2.0, -2.0, 0.0, 0.0, 1.0, 1.4142135623730951, -0.7853981633974483}},
	{"imaginary-divisor", []float64{2.0, 4.0}, []float64{0.0, 2.0}, []float64{2.0, 6.0, -8.0, 4.0, 2.0, -1.0, 4.47213595499958, 1.1071487177940904}},
	{"large-divisor", []float64{1e+150, 1e+150}, []float64{1e+155, 1e+155}, []float64{1.00001e+155, 1.00001e+155, 0.0, 2e+305, 9.999999999999999e-06, 0.0, 1.414213562373095e+150, 0.7853981633974483}},
}

func close(got, want float64) bool {
	return math.Abs(got-want) <= math.Max(1e-9, 1e-12*math.Abs(want))
}

func TestComplexArithmetic(t *testing.T) {
	for _, c := range cases {
		t.Run(c.name, func(t *testing.T) {
			got := ComplexArithmetic(c.a, c.b)
			if len(got) != 8 {
				t.Fatalf("got %d numbers", len(got))
			}
			for i := range got {
				if !close(got[i], c.want[i]) {
					t.Fatalf("item %d = %v, want %v", i, got[i], c.want[i])
				}
			}
		})
	}
}
