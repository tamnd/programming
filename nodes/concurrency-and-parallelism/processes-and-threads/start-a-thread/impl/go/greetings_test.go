// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/start-a-thread/.

package greetings

import (
	"fmt"
	"slices"
	"testing"
)

func TestThreadGreetings(t *testing.T) {
	var many, manyWant []string
	for i := range 32 {
		many = append(many, fmt.Sprintf("t%d", i))
		manyWant = append(manyWant, fmt.Sprintf("hello from t%d", i))
	}
	cases := []struct {
		name        string
		names, want []string
	}{
		{"two", []string{"Ada", "Linus"}, []string{"hello from Ada", "hello from Linus"}},
		{"three", []string{"main", "worker", "logger"}, []string{"hello from main", "hello from worker", "hello from logger"}},
		{"empty", []string{}, []string{}},
		{"single", []string{"solo"}, []string{"hello from solo"}},
		{"empty-name", []string{""}, []string{"hello from "}},
		{"duplicate", []string{"twin", "twin"}, []string{"hello from twin", "hello from twin"}},
		{"unicode", []string{"Zoë", "李雷"}, []string{"hello from Zoë", "hello from 李雷"}},
		{"many", many, manyWant},
	}
	for _, c := range cases {
		if got := ThreadGreetings(c.names); !slices.Equal(got, c.want) {
			t.Errorf("case %s: got %q, want %q", c.name, got, c.want)
		}
	}
}
