// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/filter-lines-that-match-a-pattern/.

package filterlines

import (
	"errors"
	"io/fs"
	"os"
	"path/filepath"
	"reflect"
	"strconv"
	"strings"
	"testing"
)

// numbers returns 100000 lines with the numbers 0 to 99999.
func numbers() string {
	var b strings.Builder
	for i := 0; i < 100000; i++ {
		b.WriteString(strconv.Itoa(i) + "\n")
	}
	return b.String()
}

func TestFilterLines(t *testing.T) {
	cases := []struct {
		name, text, pattern string
		want                []string
	}{
		{"literal", "apple\nbanana\ncherry\n", "an", []string{"banana"}},
		{"several", "cat\ndog\ncatalog\n", "cat", []string{"cat", "catalog"}},
		{"start", "ab\nba\n", "^a", []string{"ab"}},
		{"end", "ab\nba\n", "a$", []string{"ba"}},
		{"dot", "abc\nac\naxc\n", "a.c", []string{"abc", "axc"}},
		{"star", "ac\nabc\nabbc\nadc\n", "ab*c", []string{"ac", "abc", "abbc"}},
		{"whole-line", "a\nab\nba\n", "^a$", []string{"a"}},
		{"empty", "", "a", []string{}},
		{"no-match", "abc\n", "x", []string{}},
		{"empty-lines", "a\n\nb\n\n", "^$", []string{"", ""}},
		{"match-all", "a\n\nb\n", ".*", []string{"a", "", "b"}},
		{"duplicates", "a\nb\na\n", "a", []string{"a", "a"}},
		{"no-final-newline", "a\nab", "b", []string{"ab"}},
		{"crlf-end", "ab\r\nab\n", "b$", []string{"ab"}},
		{"crlf-dot", "a\r\n", "^a.$", []string{"a\r"}},
		{"case", "Apple\napple\n", "apple", []string{"apple"}},
		{"non-ascii", "caf\u00e9\ncafe\n", "\u00e9", []string{"caf\u00e9"}},
		// Five of the numbers hold only nines.
		{"many-lines", numbers(), "^9*$", []string{"9", "99", "999", "9999", "99999"}},
	}
	for _, c := range cases {
		t.Run(c.name, func(t *testing.T) {
			path := filepath.Join(t.TempDir(), "input.txt")
			if err := os.WriteFile(path, []byte(c.text), 0o644); err != nil {
				t.Fatal(err)
			}
			got, err := FilterLines(path, c.pattern)
			if err != nil {
				t.Fatal(err)
			}
			if !reflect.DeepEqual(got, c.want) {
				t.Errorf("got %q, want %q", got, c.want)
			}
		})
	}
}

func TestNotFound(t *testing.T) {
	_, err := FilterLines(filepath.Join(t.TempDir(), "missing.txt"), "a")
	if !errors.Is(err, fs.ErrNotExist) {
		t.Errorf("got %v, want fs.ErrNotExist", err)
	}
}
