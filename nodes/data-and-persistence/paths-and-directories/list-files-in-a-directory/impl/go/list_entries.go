// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/list-files-in-a-directory/.

// Package listentries lists the entries of a directory, or of a whole tree.
package listentries

import (
	"os"
	"path/filepath"
	"slices"
)

// ListEntries returns the relative paths of the entries under root, sorted by
// code point. A directory has a "/" at the end. A symbolic link is listed, but
// it is never followed. The error wraps fs.ErrNotExist for a missing root and
// syscall.ENOTDIR for a root that is not a directory.
func ListEntries(root string, recursive bool) ([]string, error) {
	var out []string
	var walk func(dir, prefix string) error
	walk = func(dir, prefix string) error {
		// os.ReadDir sorts the entries by name. The type of an entry comes
		// from the directory itself, so a symbolic link is not a directory.
		entries, err := os.ReadDir(dir)
		if err != nil {
			return err
		}
		for _, e := range entries {
			if !e.IsDir() {
				out = append(out, prefix+e.Name())
				continue
			}
			out = append(out, prefix+e.Name()+"/")
			if recursive {
				if err := walk(filepath.Join(dir, e.Name()), prefix+e.Name()+"/"); err != nil {
					return err
				}
			}
		}
		return nil
	}
	if err := walk(root, ""); err != nil {
		return nil, err
	}
	// Each directory is in order, but "a.txt" must come before "a/x".
	slices.Sort(out)
	return out, nil
}
