// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/apply-a-patch/.

package applypatch

import (
	"errors"
	"fmt"
	"io/fs"
	"os"
	"path/filepath"
	"strings"
	"testing"
)

// name: the old text, the patch, the new text
var cases = []struct{ name, old, patch, want string }{
	{"change-a-line", "a\nb\nc\n", "--- old.txt\n+++ new.txt\n@@ -1,3 +1,3 @@\n a\n-b\n+B\n c\n", "a\nB\nc\n"},
	{"two-hunks", "apple\nbanana\ncherry\ndate\nelder\nfig\ngrape\n", "--- old.txt\n+++ new.txt\n@@ -1,3 +1,3 @@\n apple\n-banana\n+blueberry\n cherry\n@@ -6,2 +6,2 @@\n fig\n-grape\n+guava\n", "apple\nblueberry\ncherry\ndate\nelder\nfig\nguava\n"},
	{"git-header", "def main():\n    print(1)\n", "diff --git a/main.py b/main.py\nindex 1234567..89abcde 100644\n--- a/main.py\n+++ b/main.py\n@@ -1,2 +1,2 @@ def main():\n def main():\n-    print(1)\n+    print(2)\n", "def main():\n    print(2)\n"},
	{"add-at-end", "a\n", "@@ -1 +1,2 @@\n a\n+b\n", "a\nb\n"},
	{"old-empty", "", "--- /dev/null\n+++ b.txt\n@@ -0,0 +1,2 @@\n+a\n+b\n", "a\nb\n"},
	{"delete-all", "a\nb\n", "@@ -1,2 +0,0 @@\n-a\n-b\n", ""},
	{"no-count", "a\nb\nc\n", "@@ -2 +2 @@\n-b\n+x\n", "a\nx\nc\n"},
	{"zero-context", "apple\nbanana\ncherry\ndate\nelder\nfig\ngrape\n", "@@ -3 +3 @@\n-cherry\n+coconut\n@@ -6 +5,0 @@\n-fig\n@@ -7,0 +7 @@\n+honeydew\n", "apple\nbanana\ncoconut\ndate\nelder\ngrape\nhoneydew\n"},
	{"insert-at-start", "b\n", "@@ -0,0 +1 @@\n+a\n", "a\nb\n"},
	{"old-no-final-newline", "a\nb", "@@ -1,2 +1,2 @@\n a\n-b\n\\ No newline at end of file\n+b\n", "a\nb\n"},
	{"new-no-final-newline", "a\nb\n", "@@ -1,2 +1,2 @@\n a\n-b\n+b\n\\ No newline at end of file\n", "a\nb"},
	{"empty-context-line", "a\n\nb\n", "@@ -1,3 +1,3 @@\n a\n\n-b\n+c\n", "a\n\nc\n"},
	{"space-context-line", "a\n\nb\n", "@@ -1,3 +1,3 @@\n a\n \n-b\n+c\n", "a\n\nc\n"},
	{"crlf", "a\r\nb\r\n", "@@ -1,2 +1,2 @@\n a\r\n-b\r\n+c\r\n", "a\r\nc\r\n"},
	{"header-like-line", "x\n-- y\n", "--- old.txt\n+++ new.txt\n@@ -1,2 +1 @@\n x\n--- y\n", "x\n"},
	{"non-ascii", "caf\u00e9\n", "@@ -1 +1 @@\n-caf\u00e9\n+na\u00efve\n", "na\u00efve\n"},
}

// name: the old text, the patch, the kind of the error
var errorCases = []struct {
	name, old, patch string
	kind             error
}{
	{"mismatch", "a\nb\nc\n", "@@ -1,3 +1,3 @@\n a\n-x\n+y\n c\n", ErrMismatch},
	{"malformed", "a\nb\nc\n", "@@ -1,3 +1,3 @@\n a\n-b\n", ErrMalformed},
	{"no-hunk", "a\n", "no hunk here\n", ErrMalformed},
	{"bad-mark", "a\n", "@@ -1 +1 @@\n*a\n", ErrMalformed},
	{"past-the-end", "a\n", "@@ -2 +2 @@\n-a\n+b\n", ErrMismatch},
}

func TestApplyPatch(t *testing.T) {
	for _, c := range cases {
		t.Run(c.name, func(t *testing.T) {
			got, err := ApplyPatch(c.old, c.patch)
			if err != nil || got != c.want {
				t.Errorf("ApplyPatch = %q, %v; want %q", got, err, c.want)
			}
		})
	}
}

func TestErrors(t *testing.T) {
	for _, c := range errorCases {
		t.Run(c.name, func(t *testing.T) {
			_, err := ApplyPatch(c.old, c.patch)
			var pe *PatchError
			if !errors.As(err, &pe) || !errors.Is(err, c.kind) {
				t.Errorf("ApplyPatch error = %v; want %v", err, c.kind)
			}
		})
	}
}

func TestManyHunks(t *testing.T) {
	// 1000 lines, and 10 hunks with no context change 10 of them.
	var oldText, patch, want strings.Builder
	for i := range 1000 {
		fmt.Fprintf(&oldText, "%d\n", i)
		if i%100 == 50 {
			fmt.Fprintf(&patch, "@@ -%d +%d @@\n-%d\n+x%d\n", i+1, i+1, i, i)
			fmt.Fprintf(&want, "x%d\n", i)
		} else {
			fmt.Fprintf(&want, "%d\n", i)
		}
	}
	got, err := ApplyPatch(oldText.String(), patch.String())
	if err != nil || got != want.String() {
		t.Errorf("ApplyPatch: %v, or the text is wrong", err)
	}
}

func TestApplyPatchFile(t *testing.T) {
	dir := t.TempDir()
	c := cases[13] // crlf
	if c.name != "crlf" {
		t.Fatalf("cases[13] is %s", c.name)
	}
	oldPath, patchPath := filepath.Join(dir, "old.txt"), filepath.Join(dir, "a.patch")
	if err := os.WriteFile(oldPath, []byte(c.old), 0o600); err != nil {
		t.Fatal(err)
	}
	if err := os.WriteFile(patchPath, []byte(c.patch), 0o600); err != nil {
		t.Fatal(err)
	}
	got, err := ApplyPatchFile(oldPath, patchPath)
	if err != nil || got != c.want {
		t.Errorf("ApplyPatchFile = %q, %v; want %q", got, err, c.want)
	}
}

func TestNotFound(t *testing.T) {
	dir := t.TempDir()
	patchPath := filepath.Join(dir, "a.patch")
	if err := os.WriteFile(patchPath, []byte("@@ -1 +1 @@\n-a\n+b\n"), 0o600); err != nil {
		t.Fatal(err)
	}
	if _, err := ApplyPatchFile(filepath.Join(dir, "missing.txt"), patchPath); !errors.Is(err, fs.ErrNotExist) {
		t.Errorf("ApplyPatchFile error = %v; want fs.ErrNotExist", err)
	}
}
