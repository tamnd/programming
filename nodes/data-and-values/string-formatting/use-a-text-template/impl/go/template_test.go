// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-text-template/.

package template

import "testing"

var cases = []struct {
	name     string
	template string
	names    []string
	values   []string
	want     string
}{
	{"greeting", "Hello, $name!", []string{"name"}, []string{"Ada"}, "Hello, Ada!"},
	{"braces", "${item}s cost $$${price}", []string{"item", "price"}, []string{"Apple", "3"}, "Apples cost $3"},
	{"two-uses", "$a and $a again", []string{"a"}, []string{"x"}, "x and x again"},
	{"no-placeholders", "plain text", []string{}, []string{}, "plain text"},
	{"empty-template", "", []string{"x"}, []string{"1"}, ""},
	{"dollar-escape", "$$name", []string{"name"}, []string{"Ada"}, "$name"},
	{"adjacent-text", "${n}th", []string{"n"}, []string{"4"}, "4th"},
	{"value-with-dollar", "$v", []string{"v"}, []string{"$x and ${y}"}, "$x and ${y}"},
	{"empty-value", "[$v]", []string{"v"}, []string{""}, "[]"},
	{"underscore-name", "$first_name $last_name", []string{"first_name", "last_name"}, []string{"Grace", "Hopper"}, "Grace Hopper"},
	{"name-ends-at-punct", "$a.$b", []string{"a", "b"}, []string{"1", "2"}, "1.2"},
	{"unused-value", "$a", []string{"a", "b"}, []string{"1", "2"}, "1"},
}

func TestSubstitute(t *testing.T) {
	for _, c := range cases {
		t.Run(c.name, func(t *testing.T) {
			got, err := Substitute(c.template, c.names, c.values)
			if err != nil || got != c.want {
				t.Fatalf("Substitute(%q) = %q, %v, want %q", c.template, got, err, c.want)
			}
		})
	}
}

func TestUnknownName(t *testing.T) {
	if _, err := Substitute("Hi $who", []string{"name"}, []string{"Ada"}); err == nil {
		t.Fatal("want an error for an unknown name")
	}
}
