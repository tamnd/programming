// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/tail-a-growing-file/.

package followfile

import (
	"errors"
	"fmt"
	"io/fs"
	"os"
	"path/filepath"
	"reflect"
	"strings"
	"testing"
	"time"
)

var cases = []struct {
	name, initial string
	steps         []string
	want          [][]string
}{
	{"append-lines", "old\n", []string{"append:a\nb\n", "poll", "append:c\n", "poll"}, [][]string{{"a", "b"}, {"c"}}},
	{"partial-line", "", []string{"append:ab", "poll", "append:c\nd", "poll", "append:\n", "poll"}, [][]string{{}, {"abc"}, {"d"}}},
	{"rotate", "1\n", []string{"append:2\n", "poll", "rotate", "append:3\n", "poll"}, [][]string{{"2"}, {"3"}}},
	{"truncate", "", []string{"append:a\nb\n", "poll", "truncate:", "poll", "append:c\n", "poll"}, [][]string{{"a", "b"}, {}, {"c"}}},
	{"no-change", "x\n", []string{"poll", "poll"}, [][]string{{}, {}}},
	{"empty-line", "", []string{"append:\n\na\n", "poll"}, [][]string{{"", "", "a"}}},
	{"crlf", "", []string{"append:a\r\nb\r\n", "poll"}, [][]string{{"a\r", "b\r"}}},
	{"truncate-and-write", "", []string{"append:aaaa\n", "poll", "truncate:b\n", "poll"}, [][]string{{"aaaa"}, {"b"}}},
	{"truncate-partial", "", []string{"append:abc", "poll", "truncate:d\n", "poll"}, [][]string{{}, {"abc", "d"}}},
	{"rotate-late-writes", "", []string{"append:1\n", "poll", "rotate", "append-old:2\n", "append:3\n", "poll"}, [][]string{{"1"}, {"2", "3"}}},
	{"rotate-partial", "", []string{"append:a", "poll", "rotate", "append:b\n", "poll"}, [][]string{{}, {"a", "b"}}},
	{"rotate-twice", "", []string{"append:1\n", "poll", "rotate", "append:2\n", "poll", "rotate", "append:3\n", "poll"}, [][]string{{"1"}, {"2"}, {"3"}}},
	{"delete", "", []string{"append:1\n", "poll", "delete", "poll", "poll"}, [][]string{{"1"}, {}, {}}},
	{"delete-partial", "", []string{"append:x", "poll", "delete", "poll"}, [][]string{{}, {"x"}}},
	{"delete-and-create", "", []string{"append:1\n", "poll", "delete", "poll", "create:2\n", "poll"}, [][]string{{"1"}, {}, {"2"}}},
	{"delete-and-create-between-polls", "", []string{"append:1\n", "poll", "delete", "create:2\n", "poll"}, [][]string{{"1"}, {"2"}}},
	{"start-mid-line", "abc", []string{"append:d\ne\n", "poll"}, [][]string{{"d", "e"}}},
	{"non-ascii", "", []string{"append:caf\u00e9\nna\u00efve\n", "poll"}, [][]string{{"caf\u00e9", "na\u00efve"}}},
}

func TestFollowFile(t *testing.T) {
	for _, c := range cases {
		got, err := FollowFile(filepath.Join(t.TempDir(), "followed.txt"), c.initial, c.steps)
		if err != nil {
			t.Errorf("%s: %v", c.name, err)
			continue
		}
		// An empty poll is nil in got and empty in want.
		for i := range got {
			if got[i] == nil {
				got[i] = []string{}
			}
		}
		if !reflect.DeepEqual(got, c.want) {
			t.Errorf("%s: got %q, want %q", c.name, got, c.want)
		}
	}
}

func TestNotFound(t *testing.T) {
	_, err := FollowFile(filepath.Join(t.TempDir(), "missing", "followed.txt"), "", []string{"poll"})
	if !errors.Is(err, fs.ErrNotExist) {
		t.Errorf("got %v, want fs.ErrNotExist", err)
	}
	if _, err := NewFollower(filepath.Join(t.TempDir(), "missing.txt")); !errors.Is(err, fs.ErrNotExist) {
		t.Errorf("got %v, want fs.ErrNotExist", err)
	}
}

func TestManyLines(t *testing.T) {
	var b strings.Builder
	for i := range 100000 {
		fmt.Fprintf(&b, "%d\n", i)
	}
	got, err := FollowFile(filepath.Join(t.TempDir(), "followed.txt"), "", []string{"append:" + b.String(), "poll"})
	if err != nil {
		t.Fatal(err)
	}
	lines := got[0]
	if len(lines) != 100000 || lines[0] != "0" || lines[99999] != "99999" {
		t.Errorf("got %d lines", len(lines))
	}
}

func TestCharacterAcrossPolls(t *testing.T) {
	path := filepath.Join(t.TempDir(), "followed.txt")
	got, err := FollowFile(path, "", []string{"append:caf\xc3", "poll", "append:\xa9\n", "poll"})
	if err != nil {
		t.Fatal(err)
	}
	if len(got[0]) != 0 || !reflect.DeepEqual(got[1], []string{"café"}) {
		t.Errorf("got %q", got)
	}
}

func TestFollow(t *testing.T) {
	path := filepath.Join(t.TempDir(), "followed.txt")
	if err := os.WriteFile(path, []byte("old\n"), 0o644); err != nil {
		t.Fatal(err)
	}
	fl, err := NewFollower(path)
	if err != nil {
		t.Fatal(err)
	}
	defer fl.Close()
	stop := make(chan struct{})
	done := make(chan error)
	var lines []string
	go func() {
		done <- fl.Follow(10*time.Millisecond, stop, func(line string) {
			lines = append(lines, line)
			if len(lines) == 2 {
				close(stop)
			}
		})
	}()
	if err := appendFile(path, "a\nb\n"); err != nil {
		t.Fatal(err)
	}
	select {
	case err := <-done:
		if err != nil || !reflect.DeepEqual(lines, []string{"a", "b"}) {
			t.Errorf("got %q, %v", lines, err)
		}
	case <-time.After(5 * time.Second):
		t.Fatal("no lines after 5 seconds")
	}
}
