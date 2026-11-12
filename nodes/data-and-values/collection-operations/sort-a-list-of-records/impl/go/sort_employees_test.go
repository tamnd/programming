// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/sort-a-list-of-records/.

package records

import (
	"fmt"
	"slices"
	"testing"
)

type e = Employee

// many returns the 40 records of the many-ties case: depts "b" and "a" in turn,
// all with salary 1. With parity 0 or 1 it returns only the records at even or
// odd positions.
func many(parity int) []Employee {
	var out []Employee
	for i := range 40 {
		if parity >= 0 && i%2 != parity {
			continue
		}
		dept := "b"
		if i%2 == 1 {
			dept = "a"
		}
		out = append(out, e{fmt.Sprintf("n%02d", i+1), dept, 1})
	}
	return out
}

func TestSortEmployees(t *testing.T) {
	cases := []struct {
		name        string
		given, want []Employee
	}{
		{"mixed",
			[]e{{"Ada", "eng", 120}, {"Bob", "sales", 90}, {"Cy", "eng", 150}, {"Di", "ops", 100}},
			[]e{{"Cy", "eng", 150}, {"Ada", "eng", 120}, {"Di", "ops", 100}, {"Bob", "sales", 90}}},
		{"same-salary-other-dept",
			[]e{{"Ann", "ops", 100}, {"Ben", "eng", 100}, {"Cat", "hr", 100}},
			[]e{{"Ben", "eng", 100}, {"Cat", "hr", 100}, {"Ann", "ops", 100}}},
		{"empty", []e{}, []e{}},
		{"single", []e{{"Ada", "eng", 120}}, []e{{"Ada", "eng", 120}}},
		{"sorted",
			[]e{{"Cy", "eng", 150}, {"Ada", "eng", 120}, {"Bob", "ops", 90}},
			[]e{{"Cy", "eng", 150}, {"Ada", "eng", 120}, {"Bob", "ops", 90}}},
		{"reversed",
			[]e{{"Bob", "ops", 90}, {"Ada", "eng", 120}, {"Cy", "eng", 150}},
			[]e{{"Cy", "eng", 150}, {"Ada", "eng", 120}, {"Bob", "ops", 90}}},
		{"ties",
			[]e{{"Zoe", "eng", 100}, {"Xi", "hr", 80}, {"Al", "eng", 100}, {"Yu", "hr", 80}, {"Mo", "eng", 100}},
			[]e{{"Zoe", "eng", 100}, {"Al", "eng", 100}, {"Mo", "eng", 100}, {"Xi", "hr", 80}, {"Yu", "hr", 80}}},
		{"many-ties", many(-1), append(many(1), many(0)...)},
		{"case",
			[]e{{"Ann", "eng", 100}, {"Bea", "Sales", 90}, {"Cal", "ops", 80}},
			[]e{{"Bea", "Sales", 90}, {"Ann", "eng", 100}, {"Cal", "ops", 80}}},
		{"non-ascii",
			[]e{{"Eve", "équipe", 100}, {"Fay", "zoo", 90}, {"Gus", "eng", 80}},
			[]e{{"Gus", "eng", 80}, {"Fay", "zoo", 90}, {"Eve", "équipe", 100}}},
		{"negative",
			[]e{{"Hal", "eng", 0}, {"Ida", "eng", -5}, {"Jo", "eng", 5}},
			[]e{{"Jo", "eng", 5}, {"Hal", "eng", 0}, {"Ida", "eng", -5}}},
		{"overflow",
			[]e{{"Kim", "eng", -2000000000}, {"Lee", "eng", 2000000000}},
			[]e{{"Lee", "eng", 2000000000}, {"Kim", "eng", -2000000000}}},
	}
	for _, c := range cases {
		t.Run(c.name, func(t *testing.T) {
			input := slices.Clone(c.given)
			if got := SortEmployees(input); !slices.Equal(got, c.want) {
				t.Errorf("got %v, want %v", got, c.want)
			}
			if !slices.Equal(input, c.given) {
				t.Errorf("the input changed to %v", input)
			}
		})
	}
}
