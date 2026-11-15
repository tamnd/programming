// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/fizzbuzz-task/.

package fizzbuzz

import (
	"slices"
	"testing"
)

var cases = []struct {
	name string
	n    int
	want []string
}{
	{"fifteen", 15, []string{"1", "2", "Fizz", "4", "Buzz", "Fizz", "7", "8", "Fizz", "Buzz", "11", "Fizz", "13", "14", "FizzBuzz"}},
	{"thirty", 30, []string{"1", "2", "Fizz", "4", "Buzz", "Fizz", "7", "8", "Fizz", "Buzz", "11", "Fizz", "13", "14", "FizzBuzz", "16", "17", "Fizz", "19", "Buzz", "Fizz", "22", "23", "Fizz", "Buzz", "26", "Fizz", "28", "29", "FizzBuzz"}},
	{"zero", 0, []string{}},
	{"one", 1, []string{"1"}},
	{"three", 3, []string{"1", "2", "Fizz"}},
	{"five", 5, []string{"1", "2", "Fizz", "4", "Buzz"}},
}

func TestFizzBuzz(t *testing.T) {
	for _, c := range cases {
		if got := FizzBuzz(c.n); !slices.Equal(got, c.want) {
			t.Errorf("%s: got %q, want %q", c.name, got, c.want)
		}
	}
}
