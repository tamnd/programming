// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/run-a-subprocess/.

package run

import (
	"errors"
	"os/exec"
	"strings"
	"testing"
)

func TestCases(t *testing.T) {
	cases := []struct {
		name           string
		argv           []string
		status         int
		stdout, stderr string
	}{
		{"hello", []string{"printf", "hello"}, 0, "hello", ""},
		{"two-arguments", []string{"echo", "one", "two"}, 0, "one two\n", ""},
		{"exit-status", []string{"sh", "-c", "exit 3"}, 3, "", ""},
		{"both-streams", []string{"sh", "-c", "echo out; echo err >&2; exit 2"}, 2, "out\n", "err\n"},
		{"metacharacters", []string{"printf", "%s|", "a b", "$HOME", "*", "x;y", `"q"`, "'s'"}, 0, `a b|$HOME|*|x;y|"q"|'s'|`, ""},
		{"empty-argument", []string{"printf", "[%s]", ""}, 0, "[]", ""},
		{"newline-argument", []string{"printf", "%s", "line one\nline two"}, 0, "line one\nline two", ""},
		{"trailing-newlines", []string{"printf", `a\n\n`}, 0, "a\n\n", ""},
		{"non-ascii", []string{"printf", `%s\n`, "xin chào, 日本語, 🙂"}, 0, "xin chào, 日本語, 🙂\n", ""},
		{"stderr-only", []string{"sh", "-c", "echo warning >&2"}, 0, "", "warning\n"},
		{"no-output", []string{"true"}, 0, "", ""},
		{"false", []string{"false"}, 1, "", ""},
		{"status-255", []string{"sh", "-c", "exit 255"}, 255, "", ""},
		{"empty-stdin", []string{"sh", "-c", "if read -r line; then echo got; else echo eof; fi"}, 0, "eof\n", ""},
	}
	for _, c := range cases {
		t.Run(c.name, func(t *testing.T) {
			status, stdout, stderr, err := Run(c.argv)
			if err != nil {
				t.Fatalf("Run(%q): %v", c.argv, err)
			}
			if status != c.status || stdout != c.stdout || stderr != c.stderr {
				t.Errorf("Run(%q) = %d, %q, %q; want %d, %q, %q", c.argv, status, stdout, stderr, c.status, c.stdout, c.stderr)
			}
		})
	}
}

func TestLargeOutputOnBothStreams(t *testing.T) {
	script := `head -c 1048576 /dev/zero | tr '\0' x; head -c 1048576 /dev/zero | tr '\0' y >&2`
	status, stdout, stderr, err := Run([]string{"sh", "-c", script})
	if err != nil || status != 0 {
		t.Fatalf("Run: %d, %v", status, err)
	}
	if stdout != strings.Repeat("x", 1048576) || stderr != strings.Repeat("y", 1048576) {
		t.Errorf("Run: got %d and %d bytes", len(stdout), len(stderr))
	}
}

func TestMissingProgram(t *testing.T) {
	_, _, _, err := Run([]string{"no-such-program-7f3a"})
	if !errors.Is(err, exec.ErrNotFound) {
		t.Errorf("Run: err = %v; want exec.ErrNotFound", err)
	}
}

func TestSignal(t *testing.T) {
	_, _, _, err := Run([]string{"sh", "-c", "kill -9 $$"})
	if err == nil {
		t.Errorf("Run: no error for a child that a signal stopped")
	}
}
