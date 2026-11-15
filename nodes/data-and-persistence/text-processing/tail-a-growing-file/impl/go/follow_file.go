// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/tail-a-growing-file/.

// Package followfile follows a file that grows, as tail -F does.
package followfile

import (
	"bytes"
	"errors"
	"fmt"
	"io"
	"io/fs"
	"os"
	"strings"
	"time"
)

// A Follower reads the lines that are added to a file, one poll at a time.
// It keeps the file open, so it can read the rest of the file after a rename
// or a delete.
type Follower struct {
	path    string
	f       *os.File // nil if no file is open
	offset  int64
	pending []byte // the bytes after the last LF
}

// NewFollower opens the file at path and starts at its end. The error is
// fs.ErrNotExist if no file is at path.
func NewFollower(path string) (*Follower, error) {
	f, err := os.Open(path)
	if err != nil {
		return nil, err
	}
	info, err := f.Stat()
	if err != nil {
		f.Close()
		return nil, err
	}
	return &Follower{path: path, f: f, offset: info.Size()}, nil
}

// Poll returns the complete lines that were added since the last poll.
func (fl *Follower) Poll() ([]string, error) {
	var lines []string
	if fl.f != nil {
		// Look at the path before the read. Then the read gets each byte that
		// was written before a rename or a delete.
		now, err := os.Stat(fl.path)
		if err != nil && !errors.Is(err, fs.ErrNotExist) {
			return nil, err
		}
		open, err := fl.f.Stat()
		if err != nil {
			return nil, err
		}
		if open.Size() < fl.offset {
			// The file is shorter than the offset, so it was truncated.
			lines = fl.flush(lines)
			fl.offset = 0
		}
		if lines, err = fl.read(lines); err != nil {
			return nil, err
		}
		if now == nil || !os.SameFile(now, open) {
			// The file was deleted or renamed. Its rest is read.
			lines = fl.flush(lines)
			fl.Close()
		}
	}
	if fl.f == nil {
		f, err := os.Open(fl.path)
		if errors.Is(err, fs.ErrNotExist) {
			return lines, nil
		}
		if err != nil {
			return nil, err
		}
		fl.f, fl.offset = f, 0
		return fl.read(lines)
	}
	return lines, nil
}

// Close closes the file.
func (fl *Follower) Close() error {
	if fl.f == nil {
		return nil
	}
	err := fl.f.Close()
	fl.f = nil
	return err
}

func (fl *Follower) read(lines []string) ([]string, error) {
	buf := make([]byte, 65536)
	for {
		n, err := fl.f.ReadAt(buf, fl.offset)
		fl.pending = append(fl.pending, buf[:n]...)
		fl.offset += int64(n)
		if err == io.EOF {
			break
		}
		if err != nil {
			return nil, err
		}
	}
	// Split the bytes at each LF.
	start := 0
	for {
		lf := bytes.IndexByte(fl.pending[start:], '\n')
		if lf < 0 {
			break
		}
		lines = append(lines, string(fl.pending[start:start+lf]))
		start += lf + 1
	}
	fl.pending = append([]byte(nil), fl.pending[start:]...)
	return lines, nil
}

func (fl *Follower) flush(lines []string) []string {
	if len(fl.pending) > 0 {
		lines = append(lines, string(fl.pending))
		fl.pending = nil
	}
	return lines
}

// Follow calls emit with each new line. It polls the file each interval, and
// returns when stop is closed. Make the follower before the goroutine that
// calls Follow: the follower starts at the end of the file when it is made.
func (fl *Follower) Follow(interval time.Duration, stop <-chan struct{}, emit func(string)) error {
	ticker := time.NewTicker(interval)
	defer ticker.Stop()
	for {
		lines, err := fl.Poll()
		if err != nil {
			return err
		}
		for _, line := range lines {
			emit(line)
		}
		select {
		case <-stop:
			return nil
		case <-ticker.C:
		}
	}
}

// FollowFile writes initial to path, makes a follower, does the steps and
// returns the lines of each poll.
func FollowFile(path, initial string, steps []string) ([][]string, error) {
	if err := os.WriteFile(path, []byte(initial), 0o644); err != nil {
		return nil, err
	}
	fl, err := NewFollower(path)
	if err != nil {
		return nil, err
	}
	defer fl.Close()
	old := path + ".1"
	polls := [][]string{}
	for _, step := range steps {
		op, text, _ := strings.Cut(step, ":")
		switch op {
		case "poll":
			lines, err := fl.Poll()
			if err != nil {
				return nil, err
			}
			polls = append(polls, lines)
		case "append", "append-old":
			name := path
			if op == "append-old" {
				name = old
			}
			err = appendFile(name, text)
		case "truncate":
			err = os.WriteFile(path, []byte(text), 0o644)
		case "rotate":
			if err = os.Rename(path, old); err == nil {
				err = createFile(path, "")
			}
		case "delete":
			err = os.Remove(path)
		case "create":
			err = createFile(path, text)
		default:
			err = fmt.Errorf("bad step: %q", step)
		}
		if err != nil {
			return nil, err
		}
	}
	return polls, nil
}

func appendFile(name, text string) error {
	f, err := os.OpenFile(name, os.O_WRONLY|os.O_APPEND|os.O_CREATE, 0o644)
	if err != nil {
		return err
	}
	_, err = f.WriteString(text)
	return errors.Join(err, f.Close())
}

func createFile(name, text string) error {
	f, err := os.OpenFile(name, os.O_WRONLY|os.O_CREATE|os.O_EXCL, 0o644)
	if err != nil {
		return err
	}
	_, err = f.WriteString(text)
	return errors.Join(err, f.Close())
}
