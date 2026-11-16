// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/do-matrix-arithmetic/.

package matrix

import (
	"reflect"
	"testing"
)

var cases = []struct {
	name       string
	a, b, want [][]int64
}{
	{"two-by-two", [][]int64{[]int64{1, 2}, []int64{3, 4}}, [][]int64{[]int64{5, 6}, []int64{7, 8}}, [][]int64{[]int64{19, 22}, []int64{43, 50}}},
	{"two-by-three-times-three-by-two", [][]int64{[]int64{1, 2, 3}, []int64{4, 5, 6}}, [][]int64{[]int64{7, 8}, []int64{9, 10}, []int64{11, 12}}, [][]int64{[]int64{58, 64}, []int64{139, 154}}},
	{"one-by-one", [][]int64{[]int64{3}}, [][]int64{[]int64{4}}, [][]int64{[]int64{12}}},
	{"row-times-column", [][]int64{[]int64{1, 2, 3}}, [][]int64{[]int64{4}, []int64{5}, []int64{6}}, [][]int64{[]int64{32}}},
	{"column-times-row", [][]int64{[]int64{1}, []int64{2}, []int64{3}}, [][]int64{[]int64{4, 5, 6}}, [][]int64{[]int64{4, 5, 6}, []int64{8, 10, 12}, []int64{12, 15, 18}}},
	{"identity", [][]int64{[]int64{1, 0}, []int64{0, 1}}, [][]int64{[]int64{5, 6}, []int64{7, 8}}, [][]int64{[]int64{5, 6}, []int64{7, 8}}},
	{"other-order", [][]int64{[]int64{5, 6}, []int64{7, 8}}, [][]int64{[]int64{1, 2}, []int64{3, 4}}, [][]int64{[]int64{23, 34}, []int64{31, 46}}},
	{"zero-matrix", [][]int64{[]int64{0, 0}, []int64{0, 0}}, [][]int64{[]int64{1, 2}, []int64{3, 4}}, [][]int64{[]int64{0, 0}, []int64{0, 0}}},
	{"negative-numbers", [][]int64{[]int64{-1, 2}, []int64{3, -4}}, [][]int64{[]int64{2, 0}, []int64{1, -1}}, [][]int64{[]int64{0, -2}, []int64{2, 4}}},
	{"beyond-32-bit", [][]int64{[]int64{100000, 0}, []int64{0, 100000}}, [][]int64{[]int64{100000}, []int64{3}}, [][]int64{[]int64{10000000000}, []int64{300000}}},
}

func TestMatrixMultiply(t *testing.T) {
	for _, c := range cases {
		t.Run(c.name, func(t *testing.T) {
			got, err := MatrixMultiply(c.a, c.b)
			if err != nil || !reflect.DeepEqual(got, c.want) {
				t.Fatalf("got %v, %v, want %v", got, err, c.want)
			}
		})
	}
}

func TestTranspose(t *testing.T) {
	got := Transpose([][]int64{{1, 2, 3}, {4, 5, 6}})
	if want := [][]int64{{1, 4}, {2, 5}, {3, 6}}; !reflect.DeepEqual(got, want) {
		t.Fatalf("got %v", got)
	}
}

func TestSizesMustMatch(t *testing.T) {
	if _, err := MatrixMultiply([][]int64{{1, 2}}, [][]int64{{1, 2}}); err == nil {
		t.Fatal("no error")
	}
}
