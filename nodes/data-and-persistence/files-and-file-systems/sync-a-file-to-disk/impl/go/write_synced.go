// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/sync-a-file-to-disk/.

// Package syncfile writes a file and returns when its data is on the disk.
package syncfile

import (
	"os"
	"path/filepath"
)

// SyncDirectory syncs the entries of dir, so that a new name in it is on disk.
func SyncDirectory(dir string) error {
	d, err := os.Open(dir)
	if err != nil {
		return err
	}
	if err := d.Sync(); err != nil {
		d.Close()
		return err
	}
	return d.Close()
}

// WriteSynced writes text to path, syncs the file, closes it, and syncs the
// directory of path. Each step returns its error.
func WriteSynced(path, text string) error {
	f, err := os.OpenFile(path, os.O_WRONLY|os.O_CREATE|os.O_TRUNC, 0o644)
	if err != nil {
		return err
	}
	// An *os.File has no buffer: Write gives the bytes to the kernel.
	if _, err := f.WriteString(text); err != nil {
		f.Close()
		return err
	}
	if err := f.Sync(); err != nil {
		f.Close()
		return err
	}
	if err := f.Close(); err != nil {
		return err
	}
	abs, err := filepath.Abs(path)
	if err != nil {
		return err
	}
	return SyncDirectory(filepath.Dir(abs))
}
