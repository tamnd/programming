// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/watch-a-file-for-changes/.

// Package watchfile watches a file for changes: it polls the size and the
// modification time of the file.
package watchfile

import (
	"errors"
	"fmt"
	"io/fs"
	"os"
	"strings"
	"syscall"
	"time"
)

// BaseMtime is in seconds; the cases set the mtime to BaseMtime + step.
const BaseMtime = 1_700_000_000

type signature struct {
	size  int64
	mtime time.Time
}

// stat gives the signature of the file, or ok false when it does not exist.
func stat(path string) (sig signature, ok bool, err error) {
	info, err := os.Stat(path)
	if errors.Is(err, fs.ErrNotExist) {
		return signature{}, false, nil
	}
	if err != nil {
		return signature{}, false, err
	}
	if info.IsDir() {
		return signature{}, false, &fs.PathError{Op: "watch", Path: path, Err: syscall.EISDIR}
	}
	return signature{info.Size(), info.ModTime()}, true, nil
}

// Watcher remembers the last signature of a file and tells what changed since then.
type Watcher struct {
	path   string
	last   signature
	exists bool
}

// New makes a watcher of path, and takes the first signature.
func New(path string) (*Watcher, error) {
	sig, ok, err := stat(path)
	if err != nil {
		return nil, err
	}
	return &Watcher{path, sig, ok}, nil
}

// Poll returns created, deleted, modified or none, for the time since the last poll.
func (w *Watcher) Poll() (string, error) {
	sig, ok, err := stat(w.path)
	if err != nil {
		return "", err
	}
	before, existed := w.last, w.exists
	w.last, w.exists = sig, ok
	switch {
	case !existed && !ok:
		return "none", nil
	case !existed:
		return "created", nil
	case !ok:
		return "deleted", nil
	case sig.size != before.size || !sig.mtime.Equal(before.mtime):
		return "modified", nil
	}
	return "none", nil
}

// Watch calls onChange once for each burst of changes, after the file is
// quiet, until stop is closed.
func Watch(path string, onChange func(string), interval, quiet time.Duration, stop <-chan struct{}) error {
	w, err := New(path)
	if err != nil {
		return err
	}
	var pending time.Time // the time of the last change that onChange has not seen
	tick := time.NewTicker(interval)
	defer tick.Stop()
	for {
		select {
		case <-stop:
			return nil
		case <-tick.C:
		}
		change, err := w.Poll()
		if err != nil {
			return err
		}
		if change != "none" {
			pending = time.Now()
		} else if !pending.IsZero() && time.Since(pending) >= quiet {
			pending = time.Time{}
			onChange(path)
		}
	}
}

// applyStep makes the change of one step of a case (the entry of the cases uses it).
func applyStep(path, step string, number int) error {
	kind, text, _ := strings.Cut(step, ":")
	var err error
	switch kind {
	case "create", "write":
		err = os.WriteFile(path, []byte(text), 0o644)
	case "append":
		var f *os.File
		if f, err = os.OpenFile(path, os.O_WRONLY|os.O_APPEND, 0); err == nil {
			_, err = f.WriteString(text)
			if cerr := f.Close(); err == nil {
				err = cerr
			}
		}
	case "replace":
		if err = os.WriteFile(path+".tmp", []byte(text), 0o644); err == nil {
			err = os.Rename(path+".tmp", path)
		}
	case "recreate":
		if err = os.Remove(path); err == nil {
			err = os.WriteFile(path, []byte(text), 0o644)
		}
	case "delete":
		err = os.Remove(path)
	case "mkdir":
		err = os.Mkdir(path, 0o755)
	case "touch", "none":
	default:
		err = fmt.Errorf("unknown step %q", step)
	}
	if err != nil {
		return err
	}
	if info, serr := os.Stat(path); kind != "none" && serr == nil && info.Mode().IsRegular() {
		t := time.Unix(int64(BaseMtime+number), 0)
		return os.Chtimes(path, t, t)
	}
	return nil
}

// WatchChanges is the entry of the cases: it polls after each step and returns the results.
func WatchChanges(path string, steps []string) ([]string, error) {
	w, err := New(path)
	if err != nil {
		return nil, err
	}
	results := []string{}
	for i, step := range steps {
		if err := applyStep(path, step, i+1); err != nil {
			return nil, err
		}
		change, err := w.Poll()
		if err != nil {
			return nil, err
		}
		results = append(results, change)
	}
	return results, nil
}
