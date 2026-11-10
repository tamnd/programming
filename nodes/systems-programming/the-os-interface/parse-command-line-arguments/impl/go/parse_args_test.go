// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/parse-command-line-arguments/.

package parseargs

import (
	"reflect"
	"testing"
)

func TestCases(t *testing.T) {
	cases := []struct {
		name string
		args []string
		want Args
	}{
		{"files-only", []string{"a.txt", "b.txt"}, Args{false, 10, []string{"a.txt", "b.txt"}}},
		{"short-options", []string{"-v", "-n", "3", "a.txt"}, Args{true, 3, []string{"a.txt"}}},
		{"long-options", []string{"--verbose", "--limit", "5", "a.txt", "b.txt"}, Args{true, 5, []string{"a.txt", "b.txt"}}},
		{"no-args", []string{}, Args{false, 10, []string{}}},
		{"long-equals", []string{"--limit=7", "notes.md"}, Args{false, 7, []string{"notes.md"}}},
		{"repeated", []string{"-n", "3", "--limit", "5", "a.txt"}, Args{false, 5, []string{"a.txt"}}},
		{"zero", []string{"-n", "0", "a.txt"}, Args{false, 0, []string{"a.txt"}}},
		{"end-of-options", []string{"-v", "--", "-n", "--limit=4"}, Args{true, 10, []string{"-n", "--limit=4"}}},
		{"only-terminator", []string{"--"}, Args{false, 10, []string{}}},
		{"second-terminator", []string{"--", "--"}, Args{false, 10, []string{"--"}}},
		{"dash", []string{"-"}, Args{false, 10, []string{"-"}}},
		{"dash-after-option", []string{"-n", "2", "-", "b.txt"}, Args{false, 2, []string{"-", "b.txt"}}},
		{"empty-arg", []string{""}, Args{false, 10, []string{""}}},
		{"space", []string{"-v", "my file.txt"}, Args{true, 10, []string{"my file.txt"}}},
		{"non-ascii", []string{"--limit=4", "日本語.txt", "café.md"}, Args{false, 4, []string{"日本語.txt", "café.md"}}},
	}
	for _, c := range cases {
		t.Run(c.name, func(t *testing.T) {
			got, err := ParseArgs(c.args)
			if err != nil {
				t.Fatalf("ParseArgs(%q): %v", c.args, err)
			}
			if !reflect.DeepEqual(got, c.want) {
				t.Errorf("ParseArgs(%q) = %#v, want %#v", c.args, got, c.want)
			}
		})
	}
}

func TestErrors(t *testing.T) {
	errs := []struct {
		name string
		args []string
	}{
		{"unknown-short", []string{"-x", "a.txt"}},
		{"unknown-long", []string{"--color", "a.txt"}},
		{"missing-value", []string{"-n"}},
		{"missing-value-long", []string{"--limit"}},
		{"not-a-number", []string{"-n", "ten", "a.txt"}},
		{"not-a-number-long", []string{"--limit=ten"}},
		{"empty-value", []string{"--limit=", "a.txt"}},
		{"verbose-with-value", []string{"--verbose=yes"}},
	}
	for _, c := range errs {
		t.Run(c.name, func(t *testing.T) {
			if got, err := ParseArgs(c.args); err == nil {
				t.Errorf("ParseArgs(%q) = %#v, want an error", c.args, got)
			}
		})
	}
}
