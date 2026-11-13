// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/embed-a-file-in-a-program/.

package embedfile

import (
	"os"
	"testing"
)

func TestCases(t *testing.T) {
	cases := []struct {
		name string
		n    int
		want string
		ok   bool
	}{
		{"first-line", 1, "alpha", true},
		{"second-line", 2, "café", true},
		{"last-line", 4, "gamma", true},
		{"non-ascii-line", 2, "café", true},
		{"empty-line", 3, "", true},
		{"zero", 0, "", false},
		{"negative", -1, "", false},
		{"past-the-end", 5, "", false},
	}
	for _, c := range cases {
		t.Run(c.name, func(t *testing.T) {
			got, ok := EmbeddedLine(c.n)
			if got != c.want || ok != c.ok {
				t.Fatalf("EmbeddedLine(%d) = %q, %v; want %q, %v", c.n, got, ok, c.want, c.ok)
			}
		})
	}
}

func TestBytesAreTheFile(t *testing.T) {
	data, err := os.ReadFile("lines.txt")
	if err != nil {
		t.Fatal(err)
	}
	if lines != string(data) || len(lines) != 50 {
		t.Fatalf("embedded %q (%d bytes), file %q", lines, len(lines), data)
	}
}

func TestWorkingDirectoryDoesNotMatter(t *testing.T) {
	old, err := os.Getwd()
	if err != nil {
		t.Fatal(err)
	}
	if err := os.Chdir(t.TempDir()); err != nil {
		t.Fatal(err)
	}
	defer os.Chdir(old)
	if got, ok := EmbeddedLine(4); got != "gamma" || !ok {
		t.Fatalf("got %q, %v", got, ok)
	}
}
