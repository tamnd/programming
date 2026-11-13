// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/lock-a-file/.

//go:build unix

// Package lockfile takes flock locks on files. The os package has no lock
// call, so it calls syscall.Flock.
package lockfile

import (
	"errors"
	"fmt"
	"os"
	"syscall"
)

// Lock is an flock lock on a whole file.
type Lock struct{ f *os.File }

// Release removes the lock and closes the file.
func (l *Lock) Release() error {
	err := syscall.Flock(int(l.f.Fd()), syscall.LOCK_UN)
	if cerr := l.f.Close(); err == nil {
		err = cerr
	}
	return err
}

// TryLockFile asks for a shared or an exclusive lock on path, without a
// wait. It returns nil and no error when another open file has a lock that
// conflicts. The file must exist.
func TryLockFile(path, mode string) (*Lock, error) {
	var op int
	switch mode {
	case "shared":
		op = syscall.LOCK_SH
	case "exclusive":
		op = syscall.LOCK_EX
	default:
		return nil, fmt.Errorf("lock mode %q", mode)
	}
	// flock locks belong to the open file, so each call opens the file again.
	f, err := os.OpenFile(path, os.O_RDWR, 0)
	if err != nil {
		return nil, err
	}
	if err := syscall.Flock(int(f.Fd()), op|syscall.LOCK_NB); err != nil {
		f.Close()
		if errors.Is(err, syscall.EWOULDBLOCK) {
			return nil, nil
		}
		return nil, &os.PathError{Op: "flock", Path: path, Err: err}
	}
	return &Lock{f}, nil
}

// TryLock is the entry of the cases: it takes held, then asks for want.
func TryLock(path, held, want string) (bool, error) {
	if held != "none" {
		first, err := TryLockFile(path, held)
		if err != nil {
			return false, err
		}
		defer first.Release()
	}
	second, err := TryLockFile(path, want)
	if err != nil || second == nil {
		return false, err
	}
	return true, second.Release()
}
