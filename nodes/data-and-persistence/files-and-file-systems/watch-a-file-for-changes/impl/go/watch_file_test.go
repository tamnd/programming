// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/watch-a-file-for-changes/.

package watchfile

import (
	"errors"
	"os"
	"path/filepath"
	"reflect"
	"strings"
	"sync"
	"syscall"
	"testing"
	"time"
)

var cases = []struct {
	name   string
	steps  []string
	expect []string
}{
	{"create", []string{"create:hello\n"}, []string{"created"}},
	{"create-then-write", []string{"create:a\n", "write:a much longer line\n"}, []string{"created", "modified"}},
	{"create-then-delete", []string{"create:a\n", "delete"}, []string{"created", "deleted"}},
	{"append", []string{"create:a\n", "append:b\n", "append:c\n"}, []string{"created", "modified", "modified"}},
	{"no-change", []string{"create:a\n", "none", "none"}, []string{"created", "none", "none"}},
	{"same-size", []string{"create:abc\n", "write:xyz\n"}, []string{"created", "modified"}},
	{"touch-only", []string{"create:a\n", "touch"}, []string{"created", "modified"}},
	{"replace-by-rename", []string{"create:old\n", "replace:new\n"}, []string{"created", "modified"}},
	{"delete-and-create", []string{"create:a\n", "recreate:b\n"}, []string{"created", "modified"}},
	{"never-created", []string{"none"}, []string{"none"}},
}

func TestCases(t *testing.T) {
	for _, c := range cases {
		t.Run(c.name, func(t *testing.T) {
			got, err := WatchChanges(filepath.Join(t.TempDir(), "watched.txt"), c.steps)
			if err != nil {
				t.Fatal(err)
			}
			if !reflect.DeepEqual(got, c.expect) {
				t.Fatalf("got %v, want %v", got, c.expect)
			}
		})
	}
}

func TestDirectoryIsAnError(t *testing.T) {
	_, err := WatchChanges(filepath.Join(t.TempDir(), "watched.txt"), []string{"mkdir"})
	if !errors.Is(err, syscall.EISDIR) {
		t.Fatalf("got %v, want EISDIR", err)
	}
}

func TestSameSizeAndSameTimeHideAChange(t *testing.T) {
	p := filepath.Join(t.TempDir(), "watched.txt")
	at := time.Unix(5, 0)
	must(t, os.WriteFile(p, []byte("abc\n"), 0o644))
	must(t, os.Chtimes(p, at, at))
	w, err := New(p)
	must(t, err)
	must(t, os.WriteFile(p, []byte("xyz\n"), 0o644))
	must(t, os.Chtimes(p, at, at))
	if got, _ := w.Poll(); got != "none" {
		t.Fatalf("got %s, want none", got)
	}
	must(t, os.WriteFile(p, []byte("xyz!\n"), 0o644))
	if got, _ := w.Poll(); got != "modified" {
		t.Fatalf("got %s, want modified", got)
	}
}

func TestWatchCallsOnceForABurst(t *testing.T) {
	p := filepath.Join(t.TempDir(), "watched.txt")
	var mu sync.Mutex
	var calls []string
	stop := make(chan struct{})
	done := make(chan error)
	go func() {
		done <- Watch(p, func(s string) { mu.Lock(); calls = append(calls, s); mu.Unlock() },
			10*time.Millisecond, 500*time.Millisecond, stop)
	}()
	time.Sleep(30 * time.Millisecond) // let Watch take its first signature
	for i := 1; i <= 5; i++ {
		must(t, os.WriteFile(p, []byte(strings.Repeat("x", i)), 0o644))
		time.Sleep(20 * time.Millisecond)
	}
	deadline := time.Now().Add(5 * time.Second)
	for time.Now().Before(deadline) {
		mu.Lock()
		n := len(calls)
		mu.Unlock()
		if n > 0 {
			break
		}
		time.Sleep(10 * time.Millisecond)
	}
	time.Sleep(600 * time.Millisecond)
	close(stop)
	must(t, <-done)
	if !reflect.DeepEqual(calls, []string{p}) {
		t.Fatalf("got %v, want one call", calls)
	}
}

func must(t *testing.T, err error) {
	t.Helper()
	if err != nil {
		t.Fatal(err)
	}
}
