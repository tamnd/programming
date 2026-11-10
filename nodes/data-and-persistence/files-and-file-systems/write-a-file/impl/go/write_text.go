// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/write-a-file/.

// Package writetext writes a string to a file in one of three modes.
package writetext

import (
	"errors"
	"fmt"
	"os"
	"path/filepath"
)

// The modes of WriteText.
const (
	Truncate = "truncate"
	Append   = "append"
	Atomic   = "atomic"
)

// WriteText writes text to path. A Go string holds bytes, so the bytes go to
// the file with no change: no encoding step and no change of the line ends.
func WriteText(path, text, mode string) error {
	switch mode {
	case Truncate:
		// WriteFile creates or truncates the file. 0o666 is the mode before the umask.
		return os.WriteFile(path, []byte(text), 0o666)
	case Append:
		return appendText(path, text)
	case Atomic:
		return replace(path, text)
	}
	return fmt.Errorf("unknown mode %q", mode)
}

func appendText(path, text string) error {
	f, err := os.OpenFile(path, os.O_WRONLY|os.O_APPEND|os.O_CREATE, 0o666)
	if err != nil {
		return err
	}
	_, err = f.WriteString(text)
	// Close can report a write error, so its error counts too.
	return errors.Join(err, f.Close())
}

func replace(path, text string) (err error) {
	// The temporary file is in the same directory, so the rename does not
	// cross a file system.
	f, err := os.CreateTemp(filepath.Dir(path), "."+filepath.Base(path)+".*.tmp")
	if err != nil {
		return err
	}
	defer func() {
		if err != nil {
			os.Remove(f.Name())
		}
	}()
	_, err = f.WriteString(text)
	if err == nil {
		err = f.Sync()
	}
	if err = errors.Join(err, f.Close()); err != nil {
		return err
	}
	return os.Rename(f.Name(), path)
}
