// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-fibonacci-numbers/.

package fibonacci

import "testing"

var cases = []struct {
	name string
	n    int64
	want int64
}{
	{"ten", 10, 55},
	{"twenty", 20, 6765},
	{"thirty", 30, 832040},
	{"zero", 0, 0},
	{"one", 1, 1},
	{"two", 2, 1},
	{"largest-32-bit", 46, 1836311903},
	{"beyond-32-bit", 47, 2971215073},
	{"binet-fails", 71, 308061521170129},
	{"largest-64-bit", 92, 7540113804746346429},
}

func TestFibonacci(t *testing.T) {
	for _, c := range cases {
		t.Run(c.name, func(t *testing.T) {
			if got := Fibonacci(c.n); got != c.want {
				t.Fatalf("Fibonacci(%d) = %d, want %d", c.n, got, c.want)
			}
			if got := Big(c.n); !got.IsInt64() || got.Int64() != c.want {
				t.Fatalf("Big(%d) = %v", c.n, got)
			}
		})
	}
}

func TestBig(t *testing.T) {
	if got := Big(100).String(); got != "354224848179261915075" {
		t.Fatalf("Big(100) = %s", got)
	}
}
