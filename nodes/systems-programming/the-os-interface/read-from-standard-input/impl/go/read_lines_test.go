// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/read-from-standard-input/.

package readlines

import (
	"os"
	"path/filepath"
	"slices"
	"strings"
	"testing"
)

// withStdin makes a file with the data the standard input of the process, and restores the old one at the end of the test.
func withStdin(t *testing.T, data string) {
	t.Helper()
	path := filepath.Join(t.TempDir(), "stdin.txt")
	if err := os.WriteFile(path, []byte(data), 0o644); err != nil {
		t.Fatal(err)
	}
	f, err := os.Open(path)
	if err != nil {
		t.Fatal(err)
	}
	saved := os.Stdin
	os.Stdin = f
	t.Cleanup(func() {
		os.Stdin = saved
		f.Close()
	})
}

func TestCases(t *testing.T) {
	long := strings.Repeat("x", 1<<20)
	cases := []struct {
		name string
		data string
		want []string
	}{
		{"lf", "alpha\nbeta\ngamma\n", []string{"alpha", "beta", "gamma"}},
		{"blank-lines", "first\n\n\nlast line\n", []string{"first", "", "", "last line"}},
		{"empty", "", []string{}},
		{"only-newline", "\n", []string{""}},
		{"no-final-newline", "one\ntwo\nthree", []string{"one", "two", "three"}},
		{"crlf", "one\r\ntwo\r\nthree\r\n", []string{"one", "two", "three"}},
		{"crlf-no-final-newline", "one\r\ntwo\r\nthree", []string{"one", "two", "three"}},
		{"mixed-endings", "one\r\ntwo\nthree\r\n", []string{"one", "two", "three"}},
		{"trailing-blank-line", "last\n\n", []string{"last", ""}},
		{"whitespace", "  two spaces\ttab \n", []string{"  two spaces\ttab "}},
		{"backslashes", `C:\temp\new` + "\n", []string{`C:\temp\new`}},
		{"non-ascii", "naïve café\n日本語のテキスト\n🙂🙂🙂🙂🙂🙂\n", []string{"naïve café", "日本語のテキスト", "🙂🙂🙂🙂🙂🙂"}},
		{"long-line", long + "\nend\n", []string{long, "end"}},
	}
	for _, c := range cases {
		t.Run(c.name, func(t *testing.T) {
			withStdin(t, c.data)
			got, err := ReadStdinLines()
			if err != nil || !slices.Equal(got, c.want) {
				t.Errorf("ReadStdinLines() = %q, %v; want %q, nil", got, err, c.want)
			}
		})
	}
}
