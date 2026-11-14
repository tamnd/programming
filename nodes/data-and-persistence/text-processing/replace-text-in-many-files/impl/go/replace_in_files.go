// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/replace-text-in-many-files/.

// Package replaceinfiles replaces a text with another text in each file of a
// list.
package replaceinfiles

import (
	"bytes"
	"errors"
	"os"
	"path/filepath"
)

// ErrEmptyOld is the error when the text to find is empty.
var ErrEmptyOld = errors.New("replaceinfiles: the text to find is empty")

// ReplaceInFiles replaces each instance of old with new in each file, and
// returns the number of instances in each file. old and new are plain text,
// not patterns, and an instance can contain LF bytes. Instances do not
// overlap. A file with no instance is not written. A changed file goes to a
// new file in the same directory, and os.Rename puts it in the place of the
// old one. The files are done in order, and the first error stops the
// function: the files before have the new text, and the files after do not
// change. A missing file gives an error for which errors.Is(err,
// fs.ErrNotExist) is true.
func ReplaceInFiles(paths []string, old, new string) ([]int, error) {
	if old == "" {
		return nil, ErrEmptyOld
	}
	counts := make([]int, 0, len(paths))
	for _, path := range paths {
		data, err := os.ReadFile(path)
		if err != nil {
			return nil, err
		}
		// Count and ReplaceAll both find instances that do not overlap, from
		// left to right, so the count is the number of replacements.
		n := bytes.Count(data, []byte(old))
		if n > 0 {
			if err := writeAtomically(path, bytes.ReplaceAll(data, []byte(old), []byte(new))); err != nil {
				return nil, err
			}
		}
		counts = append(counts, n)
	}
	return counts, nil
}

// writeAtomically writes data to a new file next to path, and renames it over
// path.
func writeAtomically(path string, data []byte) (err error) {
	info, err := os.Stat(path)
	if err != nil {
		return err
	}
	tmp, err := os.CreateTemp(filepath.Dir(path), "."+filepath.Base(path)+".*.tmp")
	if err != nil {
		return err
	}
	// On each return before the rename, close and remove the new file.
	renamed := false
	defer func() {
		if !renamed {
			tmp.Close()
			os.Remove(tmp.Name())
		}
	}()
	if _, err := tmp.Write(data); err != nil {
		return err
	}
	// CreateTemp makes the file with mode 0600.
	if err := tmp.Chmod(info.Mode().Perm()); err != nil {
		return err
	}
	if err := tmp.Sync(); err != nil {
		return err
	}
	if err := tmp.Close(); err != nil {
		return err
	}
	if err := os.Rename(tmp.Name(), path); err != nil {
		return err
	}
	renamed = true
	return nil
}
